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

#ifndef __MDNS_PRIVATE_H__
#define __MDNS_PRIVATE_H__

#include <string.h>
#include "mdns.h"
#include "mdns_message.h"

enum mdns_status_t {
	INIT,
	FIRST_PROBE_SENT,	/* wait a random amount of time and probe */
	SECOND_PROBE_SENT,	/* wait 250ms and probe */
	THIRD_PROBE_SENT,	/* wait 250ms and probe */
	READY_TO_RESPOND,	/* we have claimed our name */
	READY_TO_SEND,		/* response prepared, waiting to send */
};

enum mdns_event_t {
	EVENT_RX,		/* recieved a DNS packet */
	EVENT_CTRL,		/* recieved a control message */
	EVENT_TIMEOUT,	/* timed out waiting for DNS packet */
};

/* internal control message types. */
enum mdns_commands_t {
	MDNS_CTRL_HALT = 0,
	MDNS_CTRL_MONITOR,
	MDNS_CTRL_UNMONITOR,
	MDNS_CTRL_IFACE_UP,
	MDNS_CTRL_IFACE_DOWN,
	MDNS_CTRL_DEANNOUNE_ALL,
	MDNS_CTRL_ANNOUNCE_SERVICE,
	MDNS_CTRL_DEANNOUNCE_SERVICE,
	MDNS_CTRL_ANNOUNCE_SERVICE_ARR,
	MDNS_CTRL_DEANNOUNCE_SERVICE_ARR,
	MDNS_CTRL_REANNOUNCE,
};

/* mDNS responder statistics
 *
 * rx_queries: Number of Rx packets containing mDNS queries
 * rx_answers: Number of Rx packets containing mDNS answers
 *
 * NOTE: A single mDNS packet can contain both queries and
 * answers, hence it is expected that (rx_queries + rx_answers)
 * can be greater than total_rx
 *
 * rx_errors: Number of received mDNS packets which have some kind of
 * error, which is found while parsing the packet
 *
 * rx_known_ans: Number of Rx packets containing answers which match
 * device's resource record(s) and have TTL greater than a specific
 * threshold (MDNS_TTL_THRESHOLD). No response is sent by the device
 * when such packets are received
 *
 */
typedef struct {
	int total_rx, rx_queries, rx_answers;
	int rx_errors, rx_known_ans;
	int total_tx, tx_probes, tx_bye;
	int tx_reannounce, tx_response;
} mdns_responder_stats;

/* mdns service config structure
 *
 * config structure contains interface handle and service list to be published
 * on that interface. config->iface_handle holds the interface handle and
 * config->services holds pointers to list of service. config->num_services
 * holds no. of services announced per interface.
 */
struct mdns_service_config {
	/** interface handle */
	void *iface_handle;
	/** list of services announced on given interface */
	struct mdns_service *services[MAX_MDNS_LST + 1];
	/** No. of services registered */
	int num_services;
};

/* Return values from mdns_prepare_response */
#define RS_ERROR		-1		/* error in querry or response prep */
#define RS_NO_SEND		0x01		/* nothing to send */
#define RS_SEND			0x02	/* send */
#define RS_SEND_DELAY	0x04	/* this is a service query, delay 20-120ms */
/* No record corresponding to the service(s) published by device is found
 * in answer section of incoming query */
#define RS_ANS_NOT_FOUND	0x08

#ifdef MDNS_DBG
extern char *statenames[];
extern char *eventnames[];
#endif

/* logging helpers */
#if defined(MDNS_LOG) && !defined(MDNS_DBG)
#define LOG mdns_log
#define debug_print_message(m) do {} while (0)
#define debug_print_name(m, n) do {} while (0)
#define DBG(...) do {} while (0)

#elif defined(MDNS_LOG) && defined(MDNS_DBG)
#undef DBG
#undef LOG
#define LOG mdns_log
void debug_print_message(struct mdns_message *m);
void debug_print_name(struct mdns_message *m, uint8_t *name);
#define DBG mdns_log

#elif !defined(MDNS_LOG) && !defined(MDNS_DBG)
#define debug_print_message(m) do {} while (0)
#define debug_print_name(m, n) do {} while (0)
#ifdef DBG
#undef DBG
#define DBG(...) do {} while (0)
#endif
#define LOG(...) do {} while (0)

#else
#error "MDNS_DBG is only supported if MDNS_LOG is enabled"
#endif

/* helpers for accessing elements.  value arguments are in host byte order */
#define get_uint16_t(p) ((*(uint8_t *)(p) << 8) | *((uint8_t *)(p) + 1))
#define get_uint32_t(p) ((*(uint8_t *)(p) << 24) | \
					   (*((uint8_t *)(p) + 1) << 16) | \
					   (*((uint8_t *)(p) + 2) << 8) |  \
					   (*((uint8_t *)(p) + 3)))
#define set_uint16_t(p, v) do { \
	*(uint8_t *)(p) = ((v) >> 8) & 0xff;	 \
	*((uint8_t *)(p) + 1) = (v) & 0xff;		 \
	} while (0)
#define set_uint32_t(p, v)  do { \
	*(uint8_t *)(p) = ((v) >> 24) & 0xff;	 \
	*((uint8_t *)(p) + 1) = ((v) >> 16) & 0xff;		\
	*((uint8_t *)(p) + 2) = ((v) >> 8) & 0xff;		\
	*((uint8_t *)(p) + 3) = (v) & 0xff;				\
	} while (0)

/* helpers for handling dns names */
uint8_t *dname_put_label(uint8_t *dst, const char *label);
int dname_size(uint8_t *dname);
int dname_increment(uint8_t *name);
int dname_cmp(uint8_t *p1, uint8_t *n1, uint8_t *p2, uint8_t *n2);
int dnameify(char *name, uint16_t kvlen, uint8_t sep, uint8_t *dest);
int dname_copy(uint8_t *dst, uint8_t *p, uint8_t *src);
uint8_t *dname_label_to_c(char *dst, uint8_t *p, uint8_t *src,
						  int keepuscores);
int dname_label_cmp(uint8_t *p1, uint8_t *l1, uint8_t *p2, uint8_t *l2);
uint8_t *dname_label_next(uint8_t *p, uint8_t *n);
void txt_to_c_ncpy(char *dst, int dlen, uint8_t *txt, int tlen);
int dname_overrun(uint8_t *p, uint8_t *e, uint8_t *n);
/* d points to a dname pointer in a message.  return the offset in the
 * packet.
 */
#define POINTER(d) ((((*(d) & ~0xC0) << 8) | *((d) + 1)) & 0xFFFFU)
#define IS_POINTER(c) ((c) & 0xC0)

/* internal flags for service data */
#define SERVICE_CHECKED_FLAG	1
#define SERVICE_HAS_A_FLAG		2
#define SERVICE_HAS_SRV_FLAG	4
#define SRV_ADDED				8
#define TXT_ADDED				16

#define SERVICE_IS_READY(s) \
	(((s)->flags & (SERVICE_HAS_A_FLAG|SERVICE_HAS_SRV_FLAG)) ==	\
	 (SERVICE_HAS_A_FLAG|SERVICE_HAS_SRV_FLAG))

/* internal API functions for responder */
int responder_launch(const char *domain, char *hostname);
int responder_halt(void);
void responder_tests(void);

/* internal API functions for querier */
int query_launch(void);
int query_halt(void);

/* internal common functions used by responder and querier */
/* return the amount of tail room in the message m */
#define TAILROOM(m) ((m)->end - (m)->cur + 1)

/* ensure that message m has at least l bytes of room left */
#define CHECK_TAILROOM(m, l)										  \
	do {															  \
		if (TAILROOM(m) < l) {										  \
			DBG("Warning: truncated mdns message (%d).\r\n", __LINE__); \
			return -1;												  \
		}															  \
	} while (0)

/* return the number of valid bytes in message m's buffer */
#define VALID_LENGTH(m) ((m)->cur - (m)->data)

/* dumb macro to set a struct timeval to "ms" milliseconds. */
#define SET_TIMEOUT(t, ms)								\
	do {												\
		(t)->tv_sec = (ms)/1000;						\
		(t)->tv_usec = ((ms)%1000) * 1000;				\
	} while (0)

/* sock6 should be passed as -1, when message is required to be sent only
 * on IPv4 address.
 * to_addr should be passed as 0, when message is required to be sent on
 * mDNS multicast address, otherwise unicast reply would be sent on to_addr.
 */
int mdns_send_msg(struct mdns_message *m, int sock, int sock6,
		unsigned short port, void *out_interface, in_addr_t to_addr);
#ifdef CONFIG_IPV6
int mdns6_send_msg(struct mdns_message *m, int sock, unsigned short port,
		void *out_interface);
#endif	/*	CONFIG_IPV6	*/
int mdns_send_ctrl_msg(int msg, uint16_t port);
int mdns_send_ctrl_iface_msg(int msg[], uint16_t port, int size);
int mdns_add_srv_ptr_txt(struct mdns_message *m, struct mdns_service *s,
			 uint8_t *fqdn, int section, uint32_t ttl);
int mdns_add_srv(struct mdns_message *m, uint16_t priority,
		 uint16_t weight, uint16_t port, uint8_t *target);
int mdns_add_txt(struct mdns_message *m, char *txt, uint16_t len);
int mdns_add_question(struct mdns_message *m, uint8_t *qname, uint16_t qtype,
					  uint16_t qclass);
int mdns_add_answer(struct mdns_message *m, uint8_t *name, uint16_t type,
					uint16_t class, uint32_t ttl);
int mdns_add_answer_o(struct mdns_message *m, uint16_t offset, uint16_t type,
					  uint16_t class, uint32_t ttl);
int mdns_add_answer_lo(struct mdns_message *m, uint8_t *label, uint16_t offset,
		       uint16_t type, uint16_t class, uint32_t ttl);
int mdns_add_authority(struct mdns_message *m, uint8_t *name, uint16_t type,
					   uint16_t class, uint32_t ttl);
int mdns_add_uint32_t(struct mdns_message *m, uint32_t i);
#ifdef CONFIG_IPV6
int mdns_add_aaaa(struct mdns_message *m, void *i);
#endif	/*	CONFIG_IPV6	*/
int mdns_add_name(struct mdns_message *m, uint8_t *name);
int mdns_add_name_lo(struct mdns_message *m, uint8_t *label, uint16_t offset);
int mdns_query_init(struct mdns_message *m);
int mdns_parse_message(struct mdns_message *m, int mlen);
int mdns_response_init(struct mdns_message *m);
#ifdef CONFIG_DNSSD_QUERY
int dns_send_msg(struct mdns_message *m, int sock, unsigned short port,
		 void *out_interface, struct in_addr out_addr);
int dns_socket_close(int s);
int dns_socket_ucast(uint16_t port);
#endif
uint32_t interval(uint32_t start, uint32_t stop);
void recalc_timeout(struct timeval *t, uint32_t start, uint32_t stop,
					uint32_t target);

#ifndef ASSERT
#ifdef MDNS_DBG
#define ASSERT(condition) do { \
	if (!(condition)) { \
		DBG("%s: %d: ASSERTION FAILED\r\n", __FILE__, __LINE__); \
		while (1); \
	} \
} while (0)

#else
#define ASSERT(condition) do {} while (0)
#endif
#endif /* ! ASSERT */

#endif /* __MDNS_PRIVATE_H__ */
