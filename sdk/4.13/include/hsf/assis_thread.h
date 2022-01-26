
#ifndef ASSIS_THREAD_H
#define ASSIS_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif


#define ASSIS_PORT 	48899
#define ASSIS_PBUF_SIZE			200

void start_assis(uint16_t port);
int smtlk_send_log();

#ifdef __cplusplus
}
#endif

#endif

