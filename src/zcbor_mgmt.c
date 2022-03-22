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

#include <mgmt/mcumgr/buf.h>
#include <mgmt/mcumgr/smp.h>
#include <mgmt/mgmt.h>

#include "zcbor_decode.h"
#include "zcbor_encode.h"
#include "zcbor_mgmt.h"

int zcbor_mgmt_decode(struct mgmt_ctxt *ctxt, zcbor_mgmt_decoder_func decoder, void *result,
		      bool client)
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
	LOG_DBG("decode err: %u len: %u size: %u", decode_err, decode_len, cbor_size);

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
