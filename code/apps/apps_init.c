#include "apps_init.h"
#include "app_preproc_activity.h"
#include "app_home_screen_idle_activity.h"

#include "ux_manager.h"


#define LOG_TAG        "apps"

static void zlog_assert_callback(const char* expr, const char* func, uint32_t line)
{
    zlog_assert("ASSERT","(%s) has assert failed at %s:%ld.\n", expr, func, line);
}

extern void ux_test(void);
static void apps_configs_module_init(void)
{
    ux_start();
    ux_send_event(false, UX_ACTIVITY_EVENT_PRIORITY_LOWSET, 0, 1, NULL, 0, NULL, 0);
    ux_send_event(false, UX_ACTIVITY_EVENT_PRIORITY_LOW, 1, 1, NULL, 0, NULL, 0);
    ux_send_event(false, UX_ACTIVITY_EVENT_PRIORITY_MIDDLE, 2, 1, NULL, 0, NULL, 0);
    ux_send_event(false, UX_ACTIVITY_EVENT_PRIORITY_HIGH, 3, 1, NULL, 0, NULL, 0);
    ux_send_event(false, UX_ACTIVITY_EVENT_PRIORITY_HIGHEST, 4, 1, NULL, 0, NULL, 0);
    ux_send_event(false, UX_ACTIVITY_EVENT_PRIORITY_SYSTEM, 5, 1, NULL, 0, NULL, 0);

    ux_send_event(false, UX_ACTIVITY_EVENT_PRIORITY_LOWSET, 0, 2, NULL, 0, NULL, 0);
    ux_send_event(false, UX_ACTIVITY_EVENT_PRIORITY_LOW, 1, 2, NULL, 0, NULL, 0);
    ux_send_event(false, UX_ACTIVITY_EVENT_PRIORITY_MIDDLE, 2, 2, NULL, 0, NULL, 0);
    ux_send_event(false, UX_ACTIVITY_EVENT_PRIORITY_HIGH, 3, 2, NULL, 0, NULL, 0);
    ux_send_event(false, UX_ACTIVITY_EVENT_PRIORITY_HIGHEST, 4, 2, NULL, 0, NULL, 0);

    ux_remove_event(4, 1);
    ux_remove_event(4, 2);

    ux_remove_event(0, 2);
    ux_remove_event(1, 2);
    ux_remove_event(2, 2);
    ux_remove_event(3, 2);
    ux_remove_event(5, 1);

    // ux_test();
}

static void task_def_create(void)
{

}


static void log_init(void)
{
    zlog_init();

    zlog_set_format(ZLOG_LVL_ASSERT, ZLOG_FORMAT_ALL & ~(ZLOG_FORMAT_P_INFO));
    zlog_set_format(ZLOG_LVL_ERROR, ZLOG_FORMAT_ALL & ~(ZLOG_FORMAT_P_INFO|ZLOG_FORMAT_I_INFO));
    zlog_set_format(ZLOG_LVL_WARN, ZLOG_FORMAT_ALL & ~(ZLOG_FORMAT_P_INFO|ZLOG_FORMAT_I_INFO|ZLOG_FORMAT_DIR));
    zlog_set_format(ZLOG_LVL_INFO, ZLOG_FORMAT_LEVEL|ZLOG_FORMAT_TAG|ZLOG_FORMAT_TIME|ZLOG_FORMAT_FUNC|ZLOG_FORMAT_LINE);
    zlog_set_format(ZLOG_LVL_DEBUG, ZLOG_FORMAT_LEVEL|ZLOG_FORMAT_TAG|ZLOG_FORMAT_TIME|ZLOG_FORMAT_FUNC);
    zlog_set_format(ZLOG_LVL_VERBOSE, ZLOG_FORMAT_LEVEL|ZLOG_FORMAT_TAG|ZLOG_FORMAT_TIME);

    zlog_assert_set_hook(zlog_assert_callback);

    zlog_start();
}

static void system_init(void)
{

}

void apps_init(void)
{
    system_init();

    log_init();

    apps_configs_module_init();

    task_def_create();

    APPS_LOG_D(LOG_TAG, "apps init complete");
}