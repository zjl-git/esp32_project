#include "apps_init.h"
#include "app_preproc_activity.h"
#include "app_home_screen_idle_activity.h"

#include "ux_manager.h"
#include "minilzo.h"

#include "hl_uart.h"
#include "hl_i2c.h"
#include "hl_simulate_i2c.h"
#include "hl_spi.h"

#define LOG_TAG        "apps"

/******************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define IN_LEN          4 * 1024
#define OUT_LEN         IN_LEN + IN_LEN / 16 + 64 + 3
static uint8_t in_buf[IN_LEN];
static uint8_t out_buf[OUT_LEN];
static uint8_t work_buf[IN_LEN + 64];

TaskHandle_t g_test_task_handle;
static void test_task_handle(void *arg)
{
    uint8_t a = 2;

    uint8_t re;
    uint32_t in_len = IN_LEN;
    uint32_t out_len = 0, new_len = 0;

    uint32_t i = 0;
    for (i = 0; i < IN_LEN; i++) {
        in_buf[i] = i + 1;
    }
    // memset(in_buf, 0x00, in_len);

    re = lzo1x_1_compress(in_buf, in_len, out_buf, &out_len, work_buf);
    if (re == LZO_E_OK) {
        APPS_LOG_D(LOG_TAG, "minilzo compressed %d btyes into %d bytes", in_len, out_len);
    } else {
        APPS_LOG_E(LOG_TAG, "minilzo internal error - compression failed:%d", re);
        return ;
    }

    new_len = in_len;
    re = lzo1x_decompress(out_buf, out_len, in_buf, &new_len, NULL);
    if (re == LZO_E_OK) {
        APPS_LOG_D(LOG_TAG, "minilzo decompressed %d btyes into %d bytes", out_len, new_len);
    } else {
        APPS_LOG_E(LOG_TAG, "minilzo internal error - decompression failed:%d", re);
        return ;
    }

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

static void apps_module_init(void)
{
    ux_set_pre_proc_func(app_preproc_activity);
    ux_start_activity(NULL, app_home_screen_idle_activity, UX_ACTIVITY_PRIORITY_IDLE_BACKGROUND, NULL, 0);
    ux_start();
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

static void board_init(void)
{
    hl_uart_init(HL_UART_PORT1, 115200);

    hl_spi_master_init(HL_SPI2);

    // hl_simulate_i2c_init();

    // hl_i2c_init();
}

void apps_init(void)
{
    board_init();

    system_init();

    log_init();

    // apps_module_init();

    if (lzo_init() != LZO_E_OK) {
        APPS_LOG_E(LOG_TAG, "minilzo init fail!!!");
    }

    APPS_LOG_D(LOG_TAG, "apps init complete");
    // xTaskCreatePinnedToCore(test_task_handle, "test_task_handle", 4096, NULL, 3, &g_test_task_handle, 1);
}