/*
 * Copyright (c) 2018-2022 mcumgr authors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZCBOR_MGMT_H__
#define ZCBOR_MGMT_H__

#include <stdint.h>
#include "mgmt/mgmt.h"
#include "error_rsp.h"

/**
 * @brief Wrap the decoder/encoder function to handle encoding/decoding using context.
 * @note Loss of compile time type check of result type.
 */
typedef uint_fast8_t (*zcbor_mgmt_func)(const uint8_t *payload, size_t payload_len,
					void *result_or_input, size_t *payload_len_out);

/**
 * @brief Decode message using ZCBOR
 *
 * @param ctxt management context
 * @param decoder zcbor decoder function (cast)
 * @param result void pointer to decoder result
 * @param client true if MGMT_EVT_OP_CLIENT_RECV should be sent on
 * successful decode, false otherwise
 * @note void *result is passed as callback argument
 * @return int 0 on success, MGMT_ERR_[...] code on failure.
 */
int zcbor_mgmt_decode(struct mgmt_ctxt *ctxt, zcbor_mgmt_func decoder, void *result,
		      bool client);

int zcbor_mgmt_encode(struct mgmt_ctxt *ctxt, zcbor_mgmt_func encoder, void *input);

#endif /* ZCBOR_MGMT_H__ */