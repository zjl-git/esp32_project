#include "oled_ssd1306_port.h"
#include "oled_ssd1306_config.h"
#include "oled_ssd1306.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "hl_gpio.h"
#include "hl_spi.h"

static void oled_ssd1306_port_dc_set(void)
{
    hl_gpio_set_level(OLCD_DC_PIN, true);
}

static void oled_ssd1306_port_dc_clr(void)
{
    hl_gpio_set_level(OLCD_DC_PIN, false);
}

static void oled_ssd1306_port_cs_set(void)
{
    hl_gpio_set_level(OLCD_CS_PIN, true);
}

static void oled_ssd1306_port_cs_clr(void)
{
    hl_gpio_set_level(OLCD_CS_PIN, false);
}


static void oled_ssd1306_port_rest_set(void)
{
    hl_gpio_set_level(OLCD_REST_PIN, true);
}

static void oled_ssd1306_port_rest_clr(void)
{
    hl_gpio_set_level(OLCD_REST_PIN, false);
}

static void oled_ssd1306_port_delay(unsigned int ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void oled_ssd1306_port_init(void)
{
    hl_gpio_init(OLCD_DC_PIN, HL_GPIO_MODE_OUTPUT, HL_GPIO_INTR_DISABLE, false, true);
    hl_gpio_init(OLCD_CS_PIN, HL_GPIO_MODE_OUTPUT, HL_GPIO_INTR_DISABLE, false, true);
    hl_gpio_init(OLCD_REST_PIN, HL_GPIO_MODE_OUTPUT, HL_GPIO_INTR_DISABLE, false, true);

    hl_spi_master_add_device(HL_SPI2_DEVICE1, -1);

    oled_ssd1306_port_delay(10);

    oled_ssd1306_port_rest_clr();
    oled_ssd1306_port_delay(10);
    oled_ssd1306_port_rest_set();

    oled_ssd1306_port_delay(200);
}

void oled_ssd1306_port_write_spi_data(unsigned char data, unsigned char mode)
{
    unsigned char buf = data;
    oled_ssd1306_port_cs_clr();

    if (mode == OLED_WRITE_CMD) {
        oled_ssd1306_port_dc_clr();
    } else if (mode == OLED_WRITE_DAT) {
        oled_ssd1306_port_dc_set();
    } else {
        return;
    }

    hl_spi_master_device_write(HL_SPI2_DEVICE1, &buf, 1);

    oled_ssd1306_port_cs_set();
    oled_ssd1306_port_dc_set();
}