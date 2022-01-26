/*
 * Copyright (C) 2007-2011 cozybit Inc.
 *
 * This file is part of libmdns.
 *
 * libmdns is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * libmdns is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libmdns. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MDNS_MESSAGE_H
#define MDNS_MESSAGE_H

#include "mdns_port.h"
#include "mdns_queue.h"

/* settings, updated by HF to reduce memory size */
#define MDNS_MAX_QUESTIONS		20//48
#define MDNS_MAX_ANSWERS		12//32
#define MDNS_MAX_AUTHORITIES	12//32
#define MDNS_INADDRARPA_LEN	30 /* like .45.1.168.192.in-addr.arpa */
#ifdef CONFIG_IPV6
#define MDNS_AAAA_RR_LEN       16
#endif	/*	CONFIG_IPV6	*/

/* class */
#define C_IN	0x0001	/* Internet */
#define C_FLUSH	0x8001	/* FLUSH */

/* types (that are used in mDNS).  See RFC 1035 for format details */
#define T_A		1	/* host address */
#define T_NS	2	/* authoritative name server */
#define T_CNAME	5	/* canonical name for an alias */
#define T_PTR	12	/* domain name pointer */
#define T_TXT	16	/* text strings */
#ifdef CONFIG_IPV6
#define T_AAAA	28	/* IPv6 host address */
#endif	/*	CONFIG_IPV6	*/
#define T_SRV	33	/* service */
#define T_ANY	255	/* all records */

/* mDNS message header */
struct mdns_header {
	uint16_t id;
	union {
		struct { uint16_t
#ifdef MDNS_ENDIAN_LITTLE
			rcode:4,	/* response code */
			cd:1,		/* checking disabled  RFC-2535*/
			ad:1,		/* authentic data RFC-2535 */
			z:1,		/* zero */
			ra:1,		/* recursion available */
			rd:1,		/* recursion desired */
			tc:1,		/* truncated */
			aa:1,		/* authoritative answer */
			opcode:4,	/* should be 0 for mDNS messages */
			qr:1;		/* query/response */
#else
			qr:1,
			opcode:4,
			aa:1,
			tc:1,
			rd:1,
			ra:1,
			z:1,
			ad:1,
			cd:1,
			rcode:4;
#endif
		} fields;
		uint16_t num;
	} flags;
	uint16_t qdcount; /* number of entries in questions section */
	uint16_t ancount; /* number of resource records in answer section */
	uint16_t nscount;
	uint16_t arcount;
};

/* DNS Q/R values */
#define QUERY 0
#define RESPONSE 1

/* DNS opcodes (see RFC 2929) */
#define DNS_OPCODE_QUERY 0
#define DNS_OPCODE_IQUERY 1
#define DNS_OPCODE_STATUS 2
#define DNS_OPCODE_NOTIFY 3
#define DNS_OPCODE_UPDATE 5

/* internal section numbers */
#define MDNS_SECTION_QUESTIONS 0
#define MDNS_SECTION_ANSWERS 1
#define MDNS_SECTION_AUTHORITIES 2
#define MDNS_SECTION_ADDITIONALS 3

/* mDNS question (query) representation */
struct mdns_question {
	uint8_t *qname;
	uint16_t qtype; /* question type */
	uint16_t qclass; /* question class */
};

/* mDNS resource record (RR) format */
struct mdns_resource {
	uint8_t *name;
	uint16_t type; /* resource type for the RDATA field */
	uint16_t class; /* resource class for the RDATA field */
	uint32_t ttl; /* how long the record may be cached */
	uint16_t rdlength; /* length of RDATA field */
	void *rdata;
#ifdef MDNS_QUERY_API
	SLIST_ENTRY(mdns_resource) list_item;
#endif
};

/* structured rdata for SRV record */
struct rr_srv {
	uint16_t priority;
	uint16_t weight;
	uint16_t port;
	uint8_t target[0];
};

/* This defines the maximum size in bytes for the mDNS data.
 * The maximum mDNS message is actually 9000 bytes including all lower-level 
 * headers (Multicast DNS draft sec 18).  However, the same section advises 
 * implementors to keep it under 1500 bytes, which is the Ethernet MTU.
 * We subtract out 28 bytes for the IP and UDP headers, leaving 1472 bytes
 * for the DNS portion of the packet.
 */
#define MDNS_DATA_MAX	1472

#ifdef MDNS_QUERY_API
SLIST_HEAD(rr_list, mdns_resource);
#endif

/* mDNS message representation */
struct mdns_message {
	uint8_t data[MDNS_DATA_MAX]; /* raw data for packet. */
	struct mdns_header *header;
	uint8_t *cur; /* next byte to read or write */
	uint8_t *end; /* end of message buffer */
	int len; /* length of message buffer */
	struct mdns_question questions[MDNS_MAX_QUESTIONS];
	uint16_t num_questions;
	struct mdns_resource answers[MDNS_MAX_ANSWERS];
	uint16_t num_answers;
	struct mdns_resource authorities[MDNS_MAX_AUTHORITIES];
	uint16_t num_authorities;
#ifdef MDNS_QUERY_API
	struct rr_list as, srvs, txts, ptrs;
#endif
};

#endif
