#include "apps_init.h"
#include "app_preproc_activity.h"
#include "app_home_screen_idle_activity.h"

#include "ux_manager.h"
#define LOG_TAG        "apps"

/******************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
TaskHandle_t g_test_task_handle;
static void test_task_handle(void *arg)
{
    uint8_t a = 2;
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(5000));
        ux_send_event(false, UX_ACTIVITY_EVENT_PRIORITY_LOWSET, a, a+1, NULL, 0, NULL, 0);
        a += 2;
        ux_send_event(false, UX_ACTIVITY_EVENT_PRIORITY_LOWSET, a, a+1, NULL, 0, NULL, 500);
        a += 2;
    }
}
/******************************************************************************/

static void zlog_assert_callback(const char* expr, const char* func, uint32_t line)
{
    zlog_assert("ASSERT","(%s) has assert failed at %s:%ld.\n", expr, func, line);
}

static void apps_configs_module_init(void)
{
    ux_set_pre_proc_func(app_preproc_activity);
    ux_start_activity(NULL, app_home_screen_idle_activity, UX_ACTIVITY_PRIORITY_IDLE_BACKGROUND, NULL, 0);
    ux_start();
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

    xTaskCreatePinnedToCore(test_task_handle, "test_task_handle", 4096, NULL, 3, &g_test_task_handle, 1);
}