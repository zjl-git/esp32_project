#ifndef __UX_PORTS_H__
#define __UX_PORTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>

#include "zlog.h"
#define UX_LOG_TAG                  "UX"

#define UX_LOG_E(msg, ...)          zlog_error(UX_LOG_TAG, msg, ##__VA_ARGS__)
#define UX_LOG_W(msg, ...)          zlog_warn(UX_LOG_TAG, msg, ##__VA_ARGS__)
#define UX_LOG_I(msg, ...)          zlog_info(UX_LOG_TAG, msg, ##__VA_ARGS__)
#define UX_LOG_D(msg, ...)          zlog_debug(UX_LOG_TAG, msg, ##__VA_ARGS__)

#define UX_PORTS_TASK_PRIO          1

#define UX_PORT_MAX_DELAY           0xFFFFFFFF

typedef void (*ux_ports_task_handler)(void *);

void *ux_ports_malloc(uint32_t num);

void ux_ports_free(void *memory);

void ux_ports_synchronized(uint32_t *mask);

void ux_ports_synchronized_end(uint32_t mask);

void *ux_ports_create_mute(void);

void *ux_ports_create_semaphore(void);

uint32_t ux_ports_semaphore_give(void *semaphore, uint8_t from_isr);

uint32_t ux_ports_semaphore_take(void *semaphore, uint32_t block_time);

void ux_ports_create_task(ux_ports_task_handler task_func, char *task_name, 
                          uint16_t task_depth, void *task_parameters, uint32_t task_priority);

uint32_t ux_ports_get_currents_ms(void);



#ifdef __cplusplus
}
#endif

#endif 
