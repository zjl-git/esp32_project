#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "demo.h"

TaskHandle_t g_task1_handle;

static void task1_handle(void *arg)
{
    // demo_ble_advertise_init();
    // demo_ble_ext_advertise_init();
    demo_ble_gatt_server_init();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(5000));
        // printf("this is task1\r\n");
    }
    
}

void app_main(void)
{
    xTaskCreatePinnedToCore(task1_handle, "task1_handle", 4096, NULL, 3, &g_task1_handle, tskNO_AFFINITY);
}