#include "hl_nvs.h"
#include "hl_ble.h"
#include "hl_ble_gap.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"

#include "esp_log.h"
#define BLE_TAG                         " [demo_ble_ext_advertise] "

#define DEVICE_NAME                     "demo_ble_ext_advertise"

#define EXT_ADV_HANDLE                  0
#define NUM_EXT_ADV                     1

static SemaphoreHandle_t g_adv_sem = NULL;
static uint8_t g_addr_2m[6] = {0xc0, 0xde, 0x52, 0x00, 0x00, 0x02};

static esp_ble_gap_ext_adv_params_t g_ext_adv_params_2m = {
    // .type = ESP_BLE_GAP_SET_EXT_ADV_PROP_CONNECTABLE | ESP_BLE_GAP_SET_EXT_ADV_PROP_INCLUDE_TX_PWR,
    .type = ESP_BLE_GAP_SET_EXT_ADV_PROP_NONCONN_NONSCANNABLE_UNDIRECTED,
    .interval_min = 100/0.625,
    .interval_max = 100/0.625,
    .channel_map = ADV_CHNL_ALL,
    .filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
    .primary_phy = ESP_BLE_GAP_PRI_PHY_1M,
    .max_skip = 0,
    .secondary_phy = ESP_BLE_GAP_PHY_2M,
    .sid = 0,
    .scan_req_notif = false,
    .own_addr_type = BLE_ADDR_TYPE_RANDOM,
    .tx_power = 9,
};


static uint8_t g_ext_adv_data[] = {
        /* flags */
        0x02, 0x01, 0x06,
        /* tx power*/
        0x02, 0x0a, 0xeb,
        /* service uuid */
        0x03, 0x03, 0x11, 0x22,
        /* device name */
        0x0D, 0x09, 'd', 'e', 'm', 'o', '-', 'e', 'x', 't',' ', 'a', 'd', 'v',
        /*swift pair*/
        0x0F, 0xFF, 0x06, 0x00, 0x03, 0x01, 0x80, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x18, 0x04, 0x20,
};

static uint8_t g_per_adv_data[] = {
        /* flags */
        0x02, 0x01, 0x06,
        /* tx power*/
        0x02, 0x0a, 0xeb,
        /* service uuid */
        0x03, 0x03, 0x11, 0x22,
        /* device name */
        0x0D, 0x09, 'd', 'e', 'm', 'o', '-', 'p', 'e', 'r',' ', 'a', 'd', 'v',
};

static esp_ble_gap_ext_adv_t g_ext_adv[1] = {
    // instance, duration, peroid
    [0] = {EXT_ADV_HANDLE, 0, 0},
};

static esp_ble_gap_periodic_adv_params_t g_periodic_adv_params = {
    .interval_min = 0x40,                           /*80 ms interval*/
    .interval_max = 0x40,
    .properties = 0,                                /*Do not include TX power*/
};

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
        case ESP_GAP_BLE_EXT_ADV_SET_PARAMS_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "%s set ext adv params complete evt", __func__);
            if (param->ext_adv_set_params.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE(BLE_TAG, "%s set ext adv params failed", __func__);
            } else {
                xSemaphoreGive(g_adv_sem);
            }
            break;

        case ESP_GAP_BLE_EXT_ADV_SET_RAND_ADDR_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "%s set ext adv rand addr complete evt", __func__);
            if (param->ext_adv_set_rand_addr.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE(BLE_TAG, "%s set ext adv rand addr failed", __func__);
            } else {
                xSemaphoreGive(g_adv_sem);
            }
            break;

        case ESP_GAP_BLE_EXT_ADV_DATA_SET_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "%s set ext adv data complete evt", __func__);
            if (param->ext_adv_data_set.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE(BLE_TAG, "%s set ext adv data failed", __func__);
            } else {
                xSemaphoreGive(g_adv_sem);
            }
            break;

        case ESP_GAP_BLE_EXT_ADV_START_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "%s start ext adv complete evt", __func__);
            if (param->ext_adv_start.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE(BLE_TAG, "%s start ext adv failed", __func__);
            } else {
                xSemaphoreGive(g_adv_sem);
            }
            break;

        case ESP_GAP_BLE_PERIODIC_ADV_SET_PARAMS_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "%s set periodic adv params complete evt", __func__);
            if (param->peroid_adv_set_params.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE(BLE_TAG, "%s set periodic adv params failed", __func__);
            } else {
                xSemaphoreGive(g_adv_sem);
            }
            break;

        case ESP_GAP_BLE_PERIODIC_ADV_DATA_SET_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "%s set periodic adv data complete evt", __func__);
            if (param->period_adv_data_set.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE(BLE_TAG, "%s set periodic adv data failed", __func__);
            } else {
                xSemaphoreGive(g_adv_sem);
            }
            break;

        case ESP_GAP_BLE_PERIODIC_ADV_START_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "%s start periodic adv complete evt", __func__);
            if (param->period_adv_start.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE(BLE_TAG, "%s start periodic adv failed", __func__);
            } else {
                xSemaphoreGive(g_adv_sem);
            }
            break;

        default:
            ESP_LOGE(BLE_TAG, "%s understanding event:%d", __func__, event);
            break;
    }
}

void demo_ble_ext_advertise_init(void)
{
    esp_err_t ret;

    hl_nvs_init();

    hl_ble_init();

    g_adv_sem = xSemaphoreCreateBinary();
    if (g_adv_sem == NULL) {
        ESP_LOGE(BLE_TAG, "%s create semaphore failed", __func__);
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);

    ret = hl_ble_gap_set_device_name(DEVICE_NAME);
    if (ret) {
        ESP_LOGE(BLE_TAG, "%s set device name failed, error code = %x", __func__, ret);
        return;
    }

    ret = hl_ble_gap_register_callback(gap_event_handler);
    if (ret) {
        ESP_LOGE(BLE_TAG, "%s register gap callback failed, error code = %x", __func__, ret);
        return;
    }

    /*extend adv*/
    ret = hl_ble_gap_set_ext_adv_params(EXT_ADV_HANDLE, &g_ext_adv_params_2m);
    if (ret) {
        ESP_LOGE(BLE_TAG, "%s set ext adv params failed, error code = %x", __func__, ret);
        return;
    } else {
        xSemaphoreTake(g_adv_sem, portMAX_DELAY);
    }

    ret = hl_ble_gap_set_ext_adv_rand_addr(EXT_ADV_HANDLE, g_addr_2m);
    if (ret) {
        ESP_LOGE(BLE_TAG, "%s set ext adv rand addr failed, error code = %x", __func__, ret);
        return;
    } else {
        xSemaphoreTake(g_adv_sem, portMAX_DELAY);
    }

    ret = hl_ble_gap_set_ext_adv_data_raw(EXT_ADV_HANDLE, g_ext_adv_data, sizeof(g_ext_adv_data));
    if (ret) {
        ESP_LOGE(BLE_TAG, "%s set ext adv data failed, error code = %x", __func__, ret);
        return;
    } else {
        xSemaphoreTake(g_adv_sem, portMAX_DELAY);
    }

    ret = hl_ble_gap_start_ext_adv(NUM_EXT_ADV, &g_ext_adv[0]);
    if (ret) {
        ESP_LOGE(BLE_TAG, "%s start ext adv failed, error code = %x", __func__, ret);
        return;
    } else {
        xSemaphoreTake(g_adv_sem, portMAX_DELAY);
    }

    /*periodic adv*/
    ret = hl_ble_gap_set_periodic_adv_params(EXT_ADV_HANDLE, &g_periodic_adv_params);
    if (ret) {
        ESP_LOGE(BLE_TAG, "%s set periodic adv params failed, error code = %x", __func__, ret);
        return;
    } else {
        xSemaphoreTake(g_adv_sem, portMAX_DELAY);
    }

    ret = hl_ble_gap_set_periodic_adv_data_raw(EXT_ADV_HANDLE, g_per_adv_data, sizeof(g_per_adv_data));
    if (ret) {
        ESP_LOGE(BLE_TAG, "%s set periodic adv data failed, error code = %x", __func__, ret);
        return;
    } else {
        xSemaphoreTake(g_adv_sem, portMAX_DELAY);
    }

    ret = hl_ble_gap_start_periodic_adv(EXT_ADV_HANDLE);
    if (ret) {
        ESP_LOGE(BLE_TAG, "%s start periodic adv failed, error code = %x", __func__, ret);
        return;
    } else {
        xSemaphoreTake(g_adv_sem, portMAX_DELAY);
    }


    ESP_LOGI(BLE_TAG, "%s BLE init ok!", __func__);
}