/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "RRC"
 * 	found in "description.asn1"
 * 	`asn1c -D src -no-gen-OER -no-gen-example -fcompound-names -fno-include-deps`
 */

#ifndef	_RRCSetupRequest_H_
#define	_RRCSetupRequest_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RRCSetupRequest-IEs.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RRCSetupRequest */
typedef struct RRCSetupRequest {
	RRCSetupRequest_IEs_t	 rrcSetupRequest;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RRCSetupRequest_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RRCSetupRequest;

#ifdef __cplusplus
}
#endif

#endif	/* _RRCSetupRequest_H_ */
#include <asn_internal.h>
