/*
 * Generated using zcbor version 0.3.99
 * https://github.com/NordicSemiconductor/zcbor
 * Generated with a --default-max-qty of 0
 */

#ifndef ERROR_RSP_H__
#define ERROR_RSP_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "zcbor_decode.h"
#include "error_rsp_types.h"

uint_fast8_t cbor_decode_error_rsp(
		const uint8_t *payload, size_t payload_len,
		struct error_rsp *result,
		size_t *payload_len_out);


#endif /* ERROR_RSP_H__ */
