#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "hl_ble_gatt.h"
#include "hl_ble_gap.h"
#include "demo.h"

#include "esp_log.h"
#define BLE_TAG                         " [demo_ble_gatt_client] "

extern void demo_ble_scan_start(void);

static uint8_t g_write_test_data[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

static void gattc_profile_a_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);

#define GATTC_SERVICE_UUID_TEST_A   0x00FF
#define GATTC_CHAR_UUID_TEST_A      0xFF01

#define PROFILE_NUM                 1
#define PROFILE_A_APP_ID            0
#define INVALID_HANDLE              0

static uint8_t g_connect = false;
static uint8_t g_get_server = false;

static esp_gattc_char_elem_t *g_characteristic_elem_result   = NULL;
static esp_gattc_descr_elem_t *g_descriptor_elem_result = NULL;

static esp_bt_uuid_t g_remote_filter_service_uuid = {
    .len = ESP_UUID_LEN_16,
    .uuid = {.uuid16 = GATTC_SERVICE_UUID_TEST_A,},
};

static esp_bt_uuid_t g_remote_filter_char_uuid = {
    .len = ESP_UUID_LEN_16,
    .uuid = {.uuid16 = GATTC_CHAR_UUID_TEST_A,},
};

static esp_bt_uuid_t g_notify_descr_uuid = {
    .len = ESP_UUID_LEN_16,
    .uuid = {.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG,},
};


struct gattc_profile_inst {
    esp_gattc_cb_t gattc_cb;
    uint16_t gattc_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_start_handle;
    uint16_t service_end_handle;
    uint16_t char_handle;
    esp_bd_addr_t remote_bda;
};

static struct gattc_profile_inst g_profile_tab[PROFILE_NUM] = {
    [PROFILE_A_APP_ID] = {
        .gattc_cb = gattc_profile_a_event_handler,
        .gattc_if = ESP_GATT_IF_NONE,
    },
};

static void gattc_profile_a_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
{
    uint16_t count = 0;
    esp_gatt_status_t status;

    switch (event) {
        case ESP_GATTC_REG_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt reg app A evt", __func__);
            demo_ble_scan_start();
            break;

        case ESP_GATTC_OPEN_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt open evt", __func__);
            if (param->open.status != ESP_GATT_OK) {
                ESP_LOGE(BLE_TAG, "open failed, status %d", param->open.status);
            }
            break;

        case ESP_GATTC_CONNECT_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt connect evt, conn_id:%d, if:%d", __func__, param->connect.conn_id, gattc_if);
            esp_log_buffer_hex(BLE_TAG, param->connect.remote_bda, 6);
            g_connect = true;
            g_profile_tab[PROFILE_A_APP_ID].conn_id = param->connect.conn_id;
            memcpy(g_profile_tab[PROFILE_A_APP_ID].remote_bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
            if (hl_ble_gattc_send_mtu_req(gattc_if, param->connect.conn_id)) {
                ESP_LOGE(BLE_TAG, "config MTU error");
            }
            break; 

        case ESP_GATTC_DIS_SRVC_CMPL_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt discover service evt", __func__);
            if (param->dis_srvc_cmpl.status != ESP_GATT_OK){
                ESP_LOGE(BLE_TAG, "discover service failed, status %d", param->dis_srvc_cmpl.status);
            } else {
                ESP_LOGI(BLE_TAG, "discover service complete conn_id %d", param->dis_srvc_cmpl.conn_id);
                hl_ble_gattc_search_service(gattc_if, param->cfg_mtu.conn_id, &g_remote_filter_service_uuid);
            }

            break;

        case ESP_GATTC_SEARCH_RES_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt get service discovery result evt", __func__);
            ESP_LOGI(BLE_TAG, "SEARCH RES: conn_id = %x is primary service %d", param->search_res.conn_id, param->search_res.is_primary);
            ESP_LOGI(BLE_TAG, "start handle %d end handle %d current handle value %d", param->search_res.start_handle, param->search_res.end_handle, param->search_res.srvc_id.inst_id);
            if (param->search_res.srvc_id.uuid.len == ESP_UUID_LEN_16 && param->search_res.srvc_id.uuid.uuid.uuid16 == GATTC_SERVICE_UUID_TEST_A) {
                ESP_LOGI(BLE_TAG, "service found");
                g_get_server = true;
                g_profile_tab[PROFILE_A_APP_ID].service_start_handle = param->search_res.start_handle;
                g_profile_tab[PROFILE_A_APP_ID].service_end_handle = param->search_res.end_handle;
                ESP_LOGI(BLE_TAG, "UUID16: %02x", param->search_res.srvc_id.uuid.uuid.uuid16);
            }
            break;

        case ESP_GATTC_SEARCH_CMPL_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt service discovery completed evt", __func__);
            if (param->search_cmpl.status != ESP_GATT_OK){
                ESP_LOGE(BLE_TAG, "search service failed, error status = %x", param->search_cmpl.status);
                break;
            }

            if(param->search_cmpl.searched_service_source == ESP_GATT_SERVICE_FROM_REMOTE_DEVICE) {
                ESP_LOGI(BLE_TAG, "Get service information from remote device");
            } else if (param->search_cmpl.searched_service_source == ESP_GATT_SERVICE_FROM_NVS_FLASH) {
                ESP_LOGI(BLE_TAG, "Get service information from flash");
            } else {
                ESP_LOGI(BLE_TAG, "unknown service source");
            }

            if (!g_get_server) {
                break;
            }

            status = hl_ble_gattc_get_attr_count(gattc_if, param->search_cmpl.conn_id, ESP_GATT_DB_CHARACTERISTIC, g_profile_tab[PROFILE_A_APP_ID].service_start_handle, 
                                                 g_profile_tab[PROFILE_A_APP_ID].service_end_handle, INVALID_HANDLE, &count);
            if (status != ESP_GATT_OK){
                ESP_LOGE(BLE_TAG, "esp_ble_gattc_get_attr_count error");
            }

            if (count == 0) {
                ESP_LOGE(BLE_TAG, "no char found");
                break;
            }

            g_characteristic_elem_result = (esp_gattc_char_elem_t *)malloc(sizeof(esp_gattc_char_elem_t) * count);
            if (!g_characteristic_elem_result) {
                ESP_LOGE(BLE_TAG, "gattc no mem");
                break;
            }

            status = hl_ble_gattc_get_characteristic_by_uuid(gattc_if, param->search_cmpl.conn_id, g_profile_tab[PROFILE_A_APP_ID].service_start_handle,
                                                             g_profile_tab[PROFILE_A_APP_ID].service_end_handle, g_remote_filter_char_uuid, g_characteristic_elem_result, &count);
            if (status != ESP_GATT_OK){
                ESP_LOGE(BLE_TAG, "esp_ble_gattc_get_char_by_uuid error");
            }

            if (count > 0 && (g_characteristic_elem_result[0].properties & ESP_GATT_CHAR_PROP_BIT_NOTIFY)){
                    g_profile_tab[PROFILE_A_APP_ID].char_handle = g_characteristic_elem_result[0].char_handle;
                    hl_ble_gattc_register_for_notify(gattc_if, g_profile_tab[PROFILE_A_APP_ID].remote_bda, g_characteristic_elem_result[0].char_handle);
            }
            free(g_characteristic_elem_result);
            break;

        case ESP_GATTC_REG_FOR_NOTIFY_EVT:
            uint16_t notify_en = 1;
            ESP_LOGI(BLE_TAG, "(%s) gatt register notify of service completed evt", __func__);
            if (param->reg_for_notify.status != ESP_GATT_OK){
                ESP_LOGE(BLE_TAG, "REG FOR NOTIFY failed: error status = %d", param->reg_for_notify.status);
                break;
            }

            esp_gatt_status_t ret_status = hl_ble_gattc_get_attr_count(gattc_if, g_profile_tab[PROFILE_A_APP_ID].conn_id, ESP_GATT_DB_DESCRIPTOR, g_profile_tab[PROFILE_A_APP_ID].service_start_handle,
                                                                       g_profile_tab[PROFILE_A_APP_ID].service_end_handle, g_profile_tab[PROFILE_A_APP_ID].char_handle, &count);
            if (ret_status != ESP_GATT_OK){
                ESP_LOGE(BLE_TAG, "esp_ble_gattc_get_attr_count error");
            }

            if (count == 0) {
                ESP_LOGE(BLE_TAG, "no char found");
                break;
            }

            g_descriptor_elem_result = (esp_gattc_descr_elem_t *)malloc(sizeof(esp_gattc_descr_elem_t) * count);
            if (!g_descriptor_elem_result) {
                ESP_LOGE(BLE_TAG, "gattc no mem");
                break;
            }

            status = hl_ble_gattc_get_descriptor_by_char_handle(gattc_if, g_profile_tab[PROFILE_A_APP_ID].conn_id, param->reg_for_notify.handle,
                                                                g_notify_descr_uuid, g_descriptor_elem_result, &count);
            if (status != ESP_GATT_OK){
                ESP_LOGE(BLE_TAG, "hl_ble_gattc_get_descriptor_by_char_handle error");
            }

            if (count > 0 && g_descriptor_elem_result[0].uuid.len == ESP_UUID_LEN_16 && g_descriptor_elem_result[0].uuid.uuid.uuid16 == ESP_GATT_UUID_CHAR_CLIENT_CONFIG){
                status = hl_ble_gattc_write_characteristic_descriptor(gattc_if, g_profile_tab[PROFILE_A_APP_ID].conn_id, g_descriptor_elem_result[0].handle,
                                                                      sizeof(notify_en), (uint8_t *)&notify_en, ESP_GATT_WRITE_TYPE_RSP, ESP_GATT_AUTH_REQ_NONE);
                if (status != ESP_GATT_OK){
                    ESP_LOGE(BLE_TAG, "hl_ble_gattc_write_characteristic_descriptor error");
                }
            }
            break;

        case ESP_GATTC_NOTIFY_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt notification or indication arrives evt", __func__);
            if (param->notify.is_notify){
                ESP_LOGI(BLE_TAG, "receive notify value:");
            }else{
                ESP_LOGI(BLE_TAG, "receive indicate value:");
            }
            esp_log_buffer_hex(BLE_TAG, param->notify.value, param->notify.value_len);
            break;

        case ESP_GATTC_WRITE_DESCR_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt characteristic descriptor write completes evt", __func__);
            if (param->write.status != ESP_GATT_OK){
                ESP_LOGE(BLE_TAG, "write descr failed, error status = %x", param->write.status);
                break;
            }

            hl_ble_gattc_write_characteristic(gattc_if, g_profile_tab[PROFILE_A_APP_ID].conn_id, g_profile_tab[PROFILE_A_APP_ID].char_handle,
                                              sizeof(g_write_test_data), g_write_test_data, ESP_GATT_WRITE_TYPE_RSP, ESP_GATT_AUTH_REQ_NONE);
            break;

        case ESP_GATTC_SRVC_CHG_EVT:
            esp_bd_addr_t bda;
            ESP_LOGI(BLE_TAG, "(%s) gatt service changed occurs evt", __func__);
            memcpy(bda, param->srvc_chg.remote_bda, sizeof(esp_bd_addr_t));
            ESP_LOGI(BLE_TAG, "bd_addr:");
            esp_log_buffer_hex(BLE_TAG, bda, sizeof(esp_bd_addr_t));
            break;

        case ESP_GATTC_WRITE_CHAR_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt characteristic write operation completes evt", __func__);
            if (param->write.status != ESP_GATT_OK){
                ESP_LOGE(BLE_TAG, "write char failed, error status = %x", param->write.status);
                break;
            }
            break;

        case ESP_GATTC_DISCONNECT_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt disconnect evt", __func__);
            g_connect = false;
            g_get_server = false;
            ESP_LOGI(BLE_TAG, "reason = %d", param->disconnect.reason);
            break;

        case ESP_GATTC_CFG_MTU_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt config mtu evt", __func__);
            if (param->cfg_mtu.status != ESP_GATT_OK){
                ESP_LOGE(BLE_TAG,"config mtu failed, error status = %x", param->cfg_mtu.status);
            }
            ESP_LOGI(BLE_TAG, "status %d, MTU %d, conn_id %d", param->cfg_mtu.status, param->cfg_mtu.mtu, param->cfg_mtu.conn_id);
            break;

        default:
            break;
    }
}


static void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
{
    if (event == ESP_GATTC_REG_EVT) {
        if (param->reg.status != ESP_GATT_OK) {
            ESP_LOGE(BLE_TAG, "(%s) gatt reg app failed", __func__);
        } else {
            g_profile_tab[param->reg.app_id].gattc_if = gattc_if;
        }
    }

    for (int i = 0; i < PROFILE_NUM; i++) {
        if (gattc_if == ESP_GATT_IF_NONE || gattc_if == g_profile_tab[i].gattc_if) {
            if (g_profile_tab[i].gattc_cb) {
                g_profile_tab[i].gattc_cb(event, gattc_if, param);
            }
        }
    }
}

void demo_ble_gatt_client_init(void)
{
    esp_err_t ret;

    demo_ble_scan_init();

    ret = hl_ble_gattc_register_callback(gattc_event_handler);
    if (ret) {
        ESP_LOGE(BLE_TAG, "(%s) register gatt callback failed, error code = %x", __func__, ret);
        return;
    }

    ret = hl_ble_gattc_register_app(PROFILE_A_APP_ID);
    if (ret) {
        ESP_LOGE(BLE_TAG, "(%s) register app A failed, error code = %x", __func__, ret);
        return;
    }

    ret = hl_ble_gatt_set_mtu(500);
    if (ret) {
        ESP_LOGE(BLE_TAG, "(%s) set mtu failed, error code = %x", __func__, ret);
        return;
    }
}

void demo_ble_gatt_client_open(esp_bd_addr_t remote_bda, esp_ble_addr_type_t remote_addr_type)
{
    hl_ble_gattc_open(g_profile_tab[PROFILE_A_APP_ID].gattc_if, remote_bda, remote_addr_type, true);
}