#include <string.h>

#include "hl_nvs.h"
#include "hl_ble.h"
#include "hl_ble_gap.h"

#include "esp_log.h"
#define BLE_TAG                         " [demo_ble_scan] "

#define DEVICE_NAME                     "demo_ble_scan"

static const char g_remote_device_name[] = "demo";

static esp_ble_scan_params_t g_ble_scan_params = {
    .scan_type              = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type          = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy     = BLE_SCAN_FILTER_ALLOW_ALL,
    .scan_interval          = 0x50,
    .scan_window            = 0x30,
    .scan_duplicate         = BLE_SCAN_DUPLICATE_DISABLE
};

extern void demo_ble_gatt_client_open(esp_bd_addr_t remote_bda, esp_ble_addr_type_t remote_addr_type);

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    uint8_t *adv_name = NULL;
    uint8_t adv_name_len = 0;
    switch (event) {
        case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "(%s) set scan params complete evt", __func__);
            // uint32_t duration = 30;
            // hl_ble_gap_start_scanning(duration);
            break;

        case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "(%s) start scan complete evt", __func__);
            if (param->scan_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE(BLE_TAG, "scan start failed, error status = %x", param->scan_start_cmpl.status);
            }
            break;

        case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "(%s) stop scan complete evt", __func__);
            if (param->scan_stop_cmpl.status != ESP_BT_STATUS_SUCCESS){
                ESP_LOGE(BLE_TAG, "scan stop failed, error status = %x", param->scan_stop_cmpl.status);
            }
            break;

        case ESP_GAP_BLE_SCAN_RESULT_EVT:
            esp_ble_gap_cb_param_t *scan_result = (esp_ble_gap_cb_param_t *)param;
            // ESP_LOGI(BLE_TAG, "%s scan result complete evt", __func__);
            switch (scan_result->scan_rst.search_evt) {
                case ESP_GAP_SEARCH_INQ_RES_EVT:
                    
                    // esp_log_buffer_hex(BLE_TAG, scan_result->scan_rst.bda, 6);
                    // ESP_LOGI(BLE_TAG, "searched Adv Data Len %d, Scan Response Len %d", scan_result->scan_rst.adv_data_len, scan_result->scan_rst.scan_rsp_len);
                    
                    adv_name = esp_ble_resolve_adv_data(scan_result->scan_rst.ble_adv, ESP_BLE_AD_TYPE_NAME_CMPL, &adv_name_len);
                    // ESP_LOGI(BLE_TAG, "searched Device Name Len %d", adv_name_len);
                    // esp_log_buffer_char(BLE_TAG, adv_name, adv_name_len);

                    /*
                    if (scan_result->scan_rst.adv_data_len > 0) {
                        ESP_LOGI(BLE_TAG, "adv data:");
                        esp_log_buffer_hex(BLE_TAG, &scan_result->scan_rst.ble_adv[0], scan_result->scan_rst.adv_data_len);
                    }
                    if (scan_result->scan_rst.scan_rsp_len > 0) {
                        ESP_LOGI(BLE_TAG, "scan resp:");
                        esp_log_buffer_hex(BLE_TAG, &scan_result->scan_rst.ble_adv[scan_result->scan_rst.adv_data_len], scan_result->scan_rst.scan_rsp_len);
                    }
                    */
                    // ESP_LOGI(BLE_TAG, "\n");

                    if (adv_name != NULL) {
                        if (strlen(g_remote_device_name) == adv_name_len && strncmp((char *)adv_name, g_remote_device_name, adv_name_len) == 0) {
                            ESP_LOGI(BLE_TAG, "searched device %s\n", g_remote_device_name);
                                ESP_LOGI(BLE_TAG, "connect to the remote device.");
                                hl_ble_gap_stop_scanning();
                                demo_ble_gatt_client_open(scan_result->scan_rst.bda, scan_result->scan_rst.ble_addr_type);
                        }
                    }

                    break;

                default:
                    break;
            }
            break;

        case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
            ESP_LOGI(BLE_TAG, "(%s) update connection params status = %d, min_int = %d, max_int = %d,conn_int = %d,latency = %d, timeout = %d",
                                __func__, param->update_conn_params.status, param->update_conn_params.min_int, param->update_conn_params.max_int,
                                param->update_conn_params.conn_int, param->update_conn_params.latency, param->update_conn_params.timeout);
            break;

        default:
            ESP_LOGE(BLE_TAG, "(%s) understanding event:%d", __func__, event);
            break;
    }
}

void demo_ble_scan_init(void)
{
    esp_err_t ret;

    hl_nvs_init();

    hl_ble_init();

    ret = hl_ble_gap_set_device_name(DEVICE_NAME);
    if (ret) {
        ESP_LOGE(BLE_TAG, "(%s) set device name failed, error code = %x", __func__, ret);
        return;
    }

    ret = hl_ble_gap_register_callback(gap_event_handler);
    if (ret) {
        ESP_LOGE(BLE_TAG, "(%s) register gap callback failed, error code = %x", __func__, ret);
        return;
    }

    ret = hl_ble_gap_set_scan_params(&g_ble_scan_params);
    if (ret) {
        ESP_LOGE(BLE_TAG, "(%s) set scan params failed, error code = %x", __func__, ret);
        return;
    }
}

void demo_ble_scan_start(void)
{
    uint32_t duration = 30;
    hl_ble_gap_start_scanning(duration);
}