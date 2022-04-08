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

/**
 * @brief Decode response from server by client (generate client notification)
 */
#define zcbor_mgmt_decode_client(_c, _d, _r)                                                       \
	zcbor_mgmt_decode((_c), (zcbor_mgmt_func)(_d), (_r), true)

/**
 * @brief Decode command from client by server
 */
#define zcbor_mgmt_decode_server(_c, _d, _r)                                                       \
	zcbor_mgmt_decode((_c), (zcbor_mgmt_func)(_d), (_r), false)

/**
 * @brief Encode message using ZCBOR
 *
 * @param ctxt management context
 * @param encoder zcbor encoder function (cast)
 * @param input encoder input
 * @return int 0 on success, MGMT_ERR_[...] code on failure.
 */
int zcbor_mgmt_encode(struct mgmt_ctxt *ctxt, zcbor_mgmt_func encoder, void *input);

/**
 * @brief Decode error response
 *
 * @param ctxt management context
 * @return int MGMT_ERR_DECODE if error response not found, otherwise error
 * code from message
 */
int zcbor_mgmt_decode_err(struct mgmt_ctxt *ctxt);

#endif /* ZCBOR_MGMT_H__ */