#ifndef _APPS_INIT__
#define _APPS_INIT__

#ifdef __cplusplus
extern "C"
{
#endif

#include "zlog.h"

#include "apps_event_group.h"
#include "apps_event_id.h"

#define APPS_LOG_A(tag, ...)        ZLOG_A(tag, __VA_ARGS__)
#define APPS_LOG_E(tag, ...)        ZLOG_E(tag, __VA_ARGS__)
#define APPS_LOG_W(tag, ...)        ZLOG_W(tag, __VA_ARGS__)
#define APPS_LOG_I(tag, ...)        ZLOG_I(tag, __VA_ARGS__)
#define APPS_LOG_D(tag, ...)        ZLOG_D(tag, __VA_ARGS__)
#define APPS_LOG_V(tag, ...)        ZLOG_V(tag, __VA_ARGS__)

void apps_init(void);

#ifdef __cplusplus
}
#endif

#endif
