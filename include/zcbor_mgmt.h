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

typedef uint_fast8_t (*zcbor_mgmt_decoder_func)(const uint8_t *payload, size_t payload_len,
						void *result, size_t *payload_len_out);

int zcbor_mgmt_decode(struct mgmt_ctxt *ctxt, zcbor_mgmt_decoder_func decoder, void *result,
		      bool client);

#endif /* ZCBOR_MGMT_H__ */