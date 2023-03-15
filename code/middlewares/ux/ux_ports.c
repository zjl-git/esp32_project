#include "ux_ports.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// static portMUX_TYPE g_mutex = portMUX_INITIALIZER_UNLOCKED;

void *ux_ports_malloc(uint32_t num)
{
    void *buf = pvPortMalloc(num);
    return buf;
}

void ux_ports_free(void *memory)
{
    vPortFree(memory);
}

void ux_ports_synchronized(uint32_t *mask)
{
    // taskENTER_CRITICAL(&g_mutex);
}

void ux_ports_synchronized_end(uint32_t mask)
{
    // taskEXIT_CRITICAL(&g_mutex);
}

void *ux_ports_create_mute(void)
{
    return xSemaphoreCreateMutex();
}

void *ux_ports_create_semaphore(void)
{
    return xSemaphoreCreateBinary();
}

uint32_t ux_ports_semaphore_give(void *semaphore, uint8_t from_isr)
{
    int woken = false;
    if (from_isr) {
        return xSemaphoreGiveFromISR(semaphore, &woken);
    } else {
        return xSemaphoreGive(semaphore);
    }
}

uint32_t ux_ports_semaphore_take(void *semaphore, uint32_t block_time)
{
    if (block_time == UX_PORT_MAX_DELAY) {
        return xSemaphoreTake(semaphore, portMAX_DELAY);
    } else {
        return xSemaphoreTake(semaphore, block_time);
    }
}


uint32_t ux_ports_get_currents_ms(void)
{
    return xTaskGetTickCount();
}

void ux_ports_create_task(ux_ports_task_handler task_func, char *task_name, 
                          uint16_t task_depth, void *task_parameters, uint32_t task_priority)
{
    // xTaskCreate((TaskFunction_t)task_func, task_name, task_depth, task_parameters, task_priority, NULL);
    xTaskCreatePinnedToCore((TaskFunction_t)task_func, task_name, task_depth, task_parameters, task_priority, NULL, 1);
}
