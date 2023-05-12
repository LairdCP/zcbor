/*
 * Copyright (c) 2018-2022 mcumgr authors
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <logging/log.h>
LOG_MODULE_REGISTER(mgmt_zcbor, CONFIG_ZCBOR_MGMT_LOG_LEVEL);

#include <cborattr/cborattr.h>
#include <net/buf.h>
#include <tinycbor/cbor.h>
#include <tinycbor/cbor_buf_writer.h>
#include <zephyr.h>

#include <mgmt/mgmt.h>
#include <mgmt/mcumgr/buf.h>
#include <mgmt/mcumgr/smp.h>

#include "zcbor_common.h"
#include "zcbor_decode.h"
#include "zcbor_encode.h"
#include "zcbor_mgmt.h"

int zcbor_mgmt_decode(struct mgmt_ctxt *ctxt, zcbor_mgmt_func decoder, void *result, bool client)
{
	struct cbor_nb_reader *cnr = (struct cbor_nb_reader *)ctxt->parser.d;
	uint8_t *cbor_data = cnr->nb->data;
	size_t cbor_size = cnr->nb->len;
	size_t decode_len = 0;
	uint_fast8_t decode_err = ZCBOR_ERR_UNKNOWN;

	/* Use net buf because other layers have been stripped ([Base64] and SMP header) */
	if (decoder) {
		decode_err = decoder(cbor_data, cbor_size, result, &decode_len);
	}
	LOG_DBG("decode err: %s len: %u size: %u", zcbor_get_error_string(decode_err), decode_len,
		cbor_size);

#ifdef CONFIG_MGMT_ZCBOR_VERBOSE
	LOG_HEXDUMP_DBG(cbor_data, cbor_size, __func__);
#endif

	/* Receive event is only sent when decode is valid */
	if (decode_err != 0) {
		return MGMT_ERR_DECODE;
	} else {
		if (client) {
			mgmt_evt(MGMT_EVT_OP_CLIENT_RECV, &ctxt->hdr, result);
		}
		return 0;
	}
}

int zcbor_mgmt_encode(struct mgmt_ctxt *ctxt, zcbor_mgmt_func encoder, void *input)
{
	uint8_t *buf = NULL;
	size_t encode_len = 0;
	uint_fast8_t encode_err;
	struct cbor_encoder_writer *w = ctxt->encoder.writer;
	int write_status;
	int status = MGMT_ERR_EOK;

	buf = k_calloc(1, CONFIG_MCUMGR_BUF_SIZE);
	if (buf == NULL) {
		return MGMT_ERR_ENOMEM;
	}

	do {
		encode_err = encoder(buf, CONFIG_MCUMGR_BUF_SIZE, input, &encode_len);
		if (encode_err) {
			LOG_ERR("Unable to encode response %s", zcbor_get_error_string(encode_err));
			status = MGMT_ERR_ENCODE;
			break;
		} else {
			LOG_DBG("Encode length %u", encode_len);
			LOG_HEXDUMP_DBG(buf, encode_len, "cbor out");
		}

		write_status = w->write(w, buf, encode_len);
		if (write_status != 0) {
			LOG_ERR("Unable to write response %d", write_status);
			status = MGMT_ERR_WRITE;
			break;
		}

	} while (0);

	k_free(buf);

	return status;
}

int zcbor_mgmt_decode_err(struct mgmt_ctxt *ctxt)
{
	struct error_rsp rsp;

	if (zcbor_mgmt_decode(ctxt, (zcbor_mgmt_func)cbor_decode_error_rsp, &rsp, false) != 0) {
		return MGMT_ERR_DECODE;
	} else {
		return rsp.rc;
	}
}
