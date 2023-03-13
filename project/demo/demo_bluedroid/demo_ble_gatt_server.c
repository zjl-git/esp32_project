#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "hl_ble_gatt.h"
#include "hl_ble_gap.h"
#include "demo.h"

#include "esp_log.h"
#define BLE_TAG                         " [demo_ble_gatt_server] "

static void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

#define GATTS_SERVICE_UUID_TEST_A   0x00FF
#define GATTS_CHAR_UUID_TEST_A      0xFF01
#define GATTS_DESCR_UUID_TEST_A     0x3333
#define GATTS_NUM_HANDLE_TEST_A     4

#define PROFILE_NUM                 1
#define PROFILE_A_APP_ID            0

#define PREPARE_BUF_MAX_SIZE        1024
struct prepare_type_env{
    uint8_t prepare_buf[1024];
    uint16_t prepare_len;
};

static struct prepare_type_env g_a_prepare_write_env;


static uint8_t g_characteristic_value[] = {0x99,0x88,0x77};
static uint8_t g_notify_data[] = {0xA1,0xA2,0xA3, 0xA4,0xA5,0xA6};
static uint8_t g_indicate_data[] = {0xB1,0xB2,0xB3, 0xB4,0xB5,0xB6};


static esp_gatt_char_prop_t g_profile_a_property = 0;

static esp_attr_value_t g_gatts_demo_char_val =
{
    .attr_max_len = 40,
    .attr_len     = sizeof(g_characteristic_value),
    .attr_value   = g_characteristic_value,
};

struct gatts_profile_inst {
    esp_gatts_cb_t gatts_cb;                    /*gatt 回调函数*/
    uint16_t gatts_if;                          /*gatt 接口*/
    uint16_t app_id;                            /*应用的id*/
    uint16_t conn_id;                           /*连接的id*/
    uint16_t service_handle;                    /*服务service句柄*/
    esp_gatt_srvc_id_t service_id;              /*服务service id*/
    uint16_t char_handle;                       /*特征值characteristic 句柄*/
    esp_bt_uuid_t char_uuid;                    /*特征值characteristic 的uuid*/
    esp_gatt_perm_t perm;                       /*特征属性attribute 授权*/
    esp_gatt_char_prop_t property;              /*特征characteristic的特性*/
    uint16_t descr_handle;                      /*描述descriptor句柄*/
    esp_bt_uuid_t descr_uuid;                   /*描述descriptor 的uuid*/
};


static struct gatts_profile_inst g_profile_tab[PROFILE_NUM] = {
    [PROFILE_A_APP_ID] = {
        .gatts_cb = gatts_profile_a_event_handler,
        .gatts_if = ESP_GATT_IF_NONE,
    },
};

static void gatts_profile_write_event(esp_gatt_if_t gatts_if, struct prepare_type_env *prepare_write_env, esp_ble_gatts_cb_param_t *param)
{
    esp_gatt_status_t status = ESP_GATT_OK;
    if (!param->write.need_rsp) {
        return ;
    }

    if (!param->write.is_prep) {
        hl_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, status, NULL);
        return ;
    }

    if (param->write.offset > PREPARE_BUF_MAX_SIZE) {
        status = ESP_GATT_INVALID_OFFSET;
    } else if ((param->write.offset + param->write.len) > PREPARE_BUF_MAX_SIZE) {
        status = ESP_GATT_INVALID_ATTR_LEN;
    }

    esp_gatt_rsp_t *gatt_rsp = (esp_gatt_rsp_t *)malloc(sizeof(esp_gatt_rsp_t));
    gatt_rsp->attr_value.len = param->write.len;
    gatt_rsp->attr_value.handle = param->write.handle;
    gatt_rsp->attr_value.offset = param->write.offset;
    gatt_rsp->attr_value.auth_req = ESP_GATT_AUTH_REQ_NONE;
    memcpy(gatt_rsp->attr_value.value, param->write.value, param->write.len);
    if (hl_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, status, gatt_rsp) != ESP_OK ) {
        ESP_LOGE(BLE_TAG, "(%s) gatt app A send write response error!", __func__);
    }
    free(gatt_rsp);

    if (status != ESP_GATT_OK) {
        return ;
    } else {
        memcpy(prepare_write_env->prepare_buf + param->write.offset, param->write.value, param->write.len);
        prepare_write_env->prepare_len += param->write.len;
    }
}

static void gatts_profile_exec_write_event(struct prepare_type_env *prepare_write_env, esp_ble_gatts_cb_param_t *param)
{
    if (param->exec_write.exec_write_flag == ESP_GATT_PREP_WRITE_EXEC){
        esp_log_buffer_hex(BLE_TAG, prepare_write_env->prepare_buf, prepare_write_env->prepare_len);
    } else {
        ESP_LOGI(BLE_TAG, "(%s) gatt app A exec write cancel", __func__);
    }
    prepare_write_env->prepare_len = 0;
}

static void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    uint16_t length = 0;
    const uint8_t *prf_char;
    esp_ble_conn_update_params_t conn_params = {0};

    switch (event) {
        case ESP_GATTS_REG_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt reg app A evt, status:%d, app_id:%d", __func__, param->reg.status, param->reg.app_id);
            g_profile_tab[PROFILE_A_APP_ID].service_id.is_primary = true;
            g_profile_tab[PROFILE_A_APP_ID].service_id.id.inst_id = 0x00;
            g_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
            g_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID_TEST_A;
            hl_ble_gatts_create_service(gatts_if, &g_profile_tab[PROFILE_A_APP_ID].service_id, GATTS_NUM_HANDLE_TEST_A);
            break;

        case ESP_GATTS_CREATE_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt app A create service evt, status:%d, service_handle:%d", __func__, param->create.status, param->create.service_handle);
            g_profile_tab[PROFILE_A_APP_ID].service_handle = param->create.service_handle;
            g_profile_tab[PROFILE_A_APP_ID].char_uuid.len = ESP_UUID_LEN_16;
            g_profile_tab[PROFILE_A_APP_ID].char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_TEST_A;
            hl_ble_gatts_start_service(g_profile_tab[PROFILE_A_APP_ID].service_handle);

            g_profile_a_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY | ESP_GATT_CHAR_PROP_BIT_INDICATE;
            hl_ble_gatts_add_characteristic(g_profile_tab[PROFILE_A_APP_ID].service_handle, &g_profile_tab[PROFILE_A_APP_ID].char_uuid,
                                           ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, g_profile_a_property, &g_gatts_demo_char_val, NULL);
            break;

        case ESP_GATTS_ADD_CHAR_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt app A add characteristic evt, status %d,  attr_handle %d, service_handle %d", __func__, 
                                                                param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
            g_profile_tab[PROFILE_A_APP_ID].char_handle = param->add_char.attr_handle;
            g_profile_tab[PROFILE_A_APP_ID].descr_uuid.len = ESP_UUID_LEN_16;
            g_profile_tab[PROFILE_A_APP_ID].descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
            
            hl_ble_gatts_get_attribute_value(param->add_char.attr_handle,  &length, &prf_char);
            ESP_LOGI(BLE_TAG, "(%s) gatt demo char leng = %d", __func__, length);

            hl_ble_gatts_add_characteristic_descr(g_profile_tab[PROFILE_A_APP_ID].service_handle, &g_profile_tab[PROFILE_A_APP_ID].descr_uuid,
                                                 ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, NULL, NULL);
            break;

        case ESP_GATTS_ADD_CHAR_DESCR_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt app A add characteristic descriptor evt, status %d, attr_handle %d, service_handle %d", __func__, 
                                    param->add_char_descr.status, param->add_char_descr.attr_handle, param->add_char_descr.service_handle);
            g_profile_tab[PROFILE_A_APP_ID].descr_handle = param->add_char_descr.attr_handle;
            break;

        case ESP_GATTS_START_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt reg app A  start evt, status %d, service_handle %d", __func__, param->start.status, param->start.service_handle);
            break;

        case ESP_GATTS_CONNECT_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt app A connect evt, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x", __func__, param->connect.conn_id, 
                                                        param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                                                        param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5]);
            conn_params.latency = 0;
            conn_params.max_int = 0x20;                 /*max_int = 0x20*1.25ms = 40ms*/
            conn_params.min_int = 0x10;                 /*min_int = 0x10*1.25ms = 20ms*/
            conn_params.timeout = 400;                  /*timeout = 400*10ms = 4000ms*/
            memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
            g_profile_tab[PROFILE_A_APP_ID].conn_id = param->connect.conn_id;
            hl_ble_gap_update_conn_params(&conn_params);
            break;

        case ESP_GATTS_READ_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt app A read event", __func__);
            esp_gatt_rsp_t rsp;
            memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
            rsp.attr_value.handle = param->read.handle;
            rsp.attr_value.len = 4;
            rsp.attr_value.value[0] = 0x11;
            rsp.attr_value.value[1] = 0x22;
            rsp.attr_value.value[2] = 0x33;
            rsp.attr_value.value[3] = 0x44;
            hl_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id, ESP_GATT_OK, &rsp);
            break;

        case ESP_GATTS_WRITE_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt app A write event, conn_id %d, trans_id %" PRIu32 ", handle %d", __func__, param->write.conn_id, param->write.trans_id, param->write.handle);
            esp_log_buffer_hex(BLE_TAG, param->write.value, param->write.len);
            if (!param->write.is_prep && g_profile_tab[PROFILE_A_APP_ID].descr_handle == param->write.handle && param->write.len == 2) {
                uint16_t descr_value = param->write.value[1]<<8 | param->write.value[0];
                if ((descr_value == 0x0001) && (g_profile_a_property & ESP_GATT_CHAR_PROP_BIT_NOTIFY)) {
                    ESP_LOGI(BLE_TAG, "gatt app A notify enable");
                    hl_ble_gatts_send_indicate(gatts_if, param->write.conn_id, g_profile_tab[PROFILE_A_APP_ID].char_handle, sizeof(g_notify_data), g_notify_data, false);
                } else if ((descr_value == 0x0002) && (g_profile_a_property & ESP_GATT_CHAR_PROP_BIT_INDICATE)) {
                    ESP_LOGI(BLE_TAG, "gatt app A indicate enable");
                    hl_ble_gatts_send_indicate(gatts_if, param->write.conn_id, g_profile_tab[PROFILE_A_APP_ID].char_handle, sizeof(g_indicate_data), g_indicate_data, true);
                } else if (descr_value == 0x0000) {
                    ESP_LOGI(BLE_TAG, "gatt app A notify/indicate disenable");
                } else {
                    ESP_LOGI(BLE_TAG, "gatt app A unknown descr value");
                }
            }
            gatts_profile_write_event(gatts_if, &g_a_prepare_write_env, param);
            break;

        case ESP_GATTS_EXEC_WRITE_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt app A write event", __func__);
            hl_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
            gatts_profile_exec_write_event(&g_a_prepare_write_env, param);
            break;

        case ESP_GATTS_MTU_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt app A set mtu event, mtu:%d", __func__, param->mtu.mtu);
            break;

        case ESP_GATTS_DISCONNECT_EVT:
            ESP_LOGI(BLE_TAG, "(%s) gatt app A disconnect event", __func__);
            demo_ble_advertist_start();
            break;

        default:
            break;
    }
}

static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    if (event == ESP_GATTS_REG_EVT) {
        if (param->reg.status != ESP_GATT_OK) {
            ESP_LOGE(BLE_TAG, "(%s) gatt reg app failed", __func__);
        } else {
            g_profile_tab[param->reg.app_id].gatts_if = gatts_if;
        }
    }

    for (int i = 0; i < PROFILE_NUM; i++) {
        if (gatts_if == ESP_GATT_IF_NONE || gatts_if == g_profile_tab[i].gatts_if) {
            if (g_profile_tab[i].gatts_cb) {
                g_profile_tab[i].gatts_cb(event, gatts_if, param);
            }
        }
    }
}

void demo_ble_gatt_server_init(void)
{
    esp_err_t ret;

    demo_ble_advertise_init();

    ret = hl_ble_gatts_register_callback(gatts_event_handler);
    if (ret) {
        ESP_LOGE(BLE_TAG, "(%s) register gatt callback failed, error code = %x", __func__, ret);
        return;
    }

    ret = hl_ble_gatts_register_app(PROFILE_A_APP_ID);
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