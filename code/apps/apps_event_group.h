#ifndef _APPS_EVENT_GROUP__
#define _APPS_EVENT_GROUP__

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    APPS_EVENT_GROUP_APP_INTERACTION,
    APPS_EVENT_GROUP_KEY,
    APPS_EVENT_GROUP_BT,
    APPS_EVENT_GROUP_LED,
    APPS_EVENT_GROUP_LCD,
} apps_event_group;

#ifdef __cplusplus
}
#endif

#endif
