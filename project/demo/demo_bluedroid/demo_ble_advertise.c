#include <string.h>

#include "hl_nvs.h"
#include "hl_ble.h"
#include "hl_ble_gap.h"

#include "esp_log.h"
#define BLE_TAG                         " [demo_ble_advertise] "

#define DEVICE_NAME                     "demo_ble_advertise"

static uint8_t g_adv_data[] = {
        /* flags */
        0x02, 0x01, 0x06,
        /* tx power*/
        0x02, 0x0a, 0xeb,
        /* service uuid */
        // 0x03, 0x03, 0x11, 0x22,
        /* device name */
        0x05, 0x09, 'd', 'e', 'm', 'o',
        /*swift pair*/
        0x0F, 0xFF, 0x06, 0x00, 0x03, 0x01, 0x80, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x18, 0x04, 0x20,
};

static uint8_t g_scan_rsp_data[] = {
        /* flags */
        0x02, 0x01, 0x06,
        /* tx power */
        0x02, 0x0a, 0xeb,
        /* service uuid */
        // 0x03, 0x03, 0x22, 0x11,
};

static esp_ble_adv_params_t g_adv_params = {
    .adv_int_min        = 0x20,
    .adv_int_max        = 0x40,
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

static char *key_type_to_str(esp_ble_key_type_t key_type)
{
   char *key_str = NULL;
   switch(key_type) {
    case ESP_LE_KEY_NONE:
        key_str = "ESP_LE_KEY_NONE";
        break;
    case ESP_LE_KEY_PENC:
        key_str = "ESP_LE_KEY_PENC";
        break;
    case ESP_LE_KEY_PID:
        key_str = "ESP_LE_KEY_PID";
        break;
    case ESP_LE_KEY_PCSRK:
        key_str = "ESP_LE_KEY_PCSRK";
        break;
    case ESP_LE_KEY_PLK:
        key_str = "ESP_LE_KEY_PLK";
        break;
    case ESP_LE_KEY_LLK:
        key_str = "ESP_LE_KEY_LLK";
        break;
    case ESP_LE_KEY_LENC:
        key_str = "ESP_LE_KEY_LENC";
        break;
    case ESP_LE_KEY_LID:
        key_str = "ESP_LE_KEY_LID";
        break;
    case ESP_LE_KEY_LCSRK:
        key_str = "ESP_LE_KEY_LCSRK";
        break;
    default:
        key_str = "INVALID BLE KEY TYPE";
        break;
   }
   return key_str;
}

static char *auth_req_to_str(esp_ble_auth_req_t auth_req)
{
   char *auth_str = NULL;
   switch(auth_req) {
    case ESP_LE_AUTH_NO_BOND:
        auth_str = "ESP_LE_AUTH_NO_BOND";
        break;
    case ESP_LE_AUTH_BOND:
        auth_str = "ESP_LE_AUTH_BOND";
        break;
    case ESP_LE_AUTH_REQ_MITM:
        auth_str = "ESP_LE_AUTH_REQ_MITM";
        break;
    case ESP_LE_AUTH_REQ_BOND_MITM:
        auth_str = "ESP_LE_AUTH_REQ_BOND_MITM";
        break;
    case ESP_LE_AUTH_REQ_SC_ONLY:
        auth_str = "ESP_LE_AUTH_REQ_SC_ONLY";
        break;
    case ESP_LE_AUTH_REQ_SC_BOND:
        auth_str = "ESP_LE_AUTH_REQ_SC_BOND";
        break;
    case ESP_LE_AUTH_REQ_SC_MITM:
        auth_str = "ESP_LE_AUTH_REQ_SC_MITM";
        break;
    case ESP_LE_AUTH_REQ_SC_MITM_BOND:
        auth_str = "ESP_LE_AUTH_REQ_SC_MITM_BOND";
        break;
    default:
        auth_str = "INVALID BLE AUTH REQ";
        break;
   }
   return auth_str;
}

static void show_bonded_devices(void)
{
    int dev_num = hl_ble_gap_get_bond_device_num();

    esp_ble_bond_dev_t *dev_list = (esp_ble_bond_dev_t *)malloc(sizeof(esp_ble_bond_dev_t) * dev_num);
    hl_ble_gap_get_bond_device_list(&dev_num, dev_list);
    ESP_LOGI(BLE_TAG, "(%s) Bonded devices number : %d\n", __func__, dev_num);

    ESP_LOGI(BLE_TAG, "(%s) Bonded devices list : %d\n", __func__, dev_num);
    for (int i = 0; i < dev_num; i++) {
        esp_log_buffer_hex(BLE_TAG, (void *)dev_list[i].bd_addr, sizeof(esp_bd_addr_t));
    }
    free(dev_list);
}

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
        case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "(%s) set adv data complete evt", __func__);
            break;

        case ESP_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "(%s) set scan rsp data complete evt", __func__);
            break;

        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "(%s) start adv complete evt", __func__);
            if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE(BLE_TAG, "(%s) Advertising start failed", __func__);
            }
            break;

        case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "(%s) stop adv complete evt", __func__);
            if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE(BLE_TAG, "(%s) Advertising stop failed", __func__);
            }
            break;

        case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
            ESP_LOGI(BLE_TAG, "(%s) update connection params status = %d, min_int = %d, max_int = %d,conn_int = %d,latency = %d, timeout = %d",
                                __func__,
                                param->update_conn_params.status,
                                param->update_conn_params.min_int,
                                param->update_conn_params.max_int,
                                param->update_conn_params.conn_int,
                                param->update_conn_params.latency,
                                param->update_conn_params.timeout);
            break;

        case ESP_GAP_BLE_PASSKEY_REQ_EVT:
            ESP_LOGI(BLE_TAG, "(%s) passkey request event", __func__);
            // esp_ble_passkey_reply(heart_rate_profile_tab[HEART_PROFILE_APP_IDX].remote_bda, true, 0x00);
            break;

        case ESP_GAP_BLE_OOB_REQ_EVT:
            uint8_t tk[16] = {1};
            ESP_LOGI(BLE_TAG, "(%s) OOB request event", __func__);
            hl_ble_gap_oob_req_reply(param->ble_security.ble_req.bd_addr, tk, sizeof(tk));
            break;

        case ESP_GAP_BLE_LOCAL_IR_EVT:
            ESP_LOGI(BLE_TAG, "(%s) local IR event", __func__);
            break;

        case ESP_GAP_BLE_LOCAL_ER_EVT:
            ESP_LOGI(BLE_TAG, "(%s) local ER event", __func__);
            break;

        case ESP_GAP_BLE_NC_REQ_EVT:
            /*when the IO has DisplayYesNO capability and the peer device IO also has DisplayYesNo capability*/
            ESP_LOGI(BLE_TAG, "(%s) Numeric Comparison request event", __func__);
            hl_ble_confirm_reply(param->ble_security.ble_req.bd_addr, true);
            ESP_LOGI(BLE_TAG, "the passkey Notify number:%" PRIu32, param->ble_security.key_notif.passkey);
        
            break;

        case ESP_GAP_BLE_SEC_REQ_EVT:
            ESP_LOGI(BLE_TAG, "(%s) security request event", __func__);
            hl_ble_gap_security_rsp(param->ble_security.ble_req.bd_addr, true);
            break;

        case ESP_GAP_BLE_PASSKEY_NOTIF_EVT:
            /*when the IO  has Output capability and the peer device IO has Input capability, show the passkey number to the user to input it in the peer device*/
            ESP_LOGI(BLE_TAG, "(%s) passkey notification event", __func__);
            ESP_LOGI(BLE_TAG, "The passkey Notify number:%06" PRIu32, param->ble_security.key_notif.passkey);
            break;

        case ESP_GAP_BLE_KEY_EVT:
            /*shows the ble key info share with peer device to the user*/
            ESP_LOGI(BLE_TAG, "(%s) key event for peer device keys", __func__);
            ESP_LOGI(BLE_TAG, "key type = %s", key_type_to_str(param->ble_security.ble_key.key_type));
            break;

        case ESP_GAP_BLE_AUTH_CMPL_EVT:
            ESP_LOGI(BLE_TAG, "(%s) authentication complete indication", __func__);
            esp_bd_addr_t bd_addr;
            memcpy(bd_addr, param->ble_security.auth_cmpl.bd_addr, sizeof(esp_bd_addr_t));

            ESP_LOGI(BLE_TAG, "remote BD_ADDR:");
            esp_log_buffer_hex(BLE_TAG, bd_addr, 6);

            ESP_LOGI(BLE_TAG, "address type = %d", param->ble_security.auth_cmpl.addr_type);
            ESP_LOGI(BLE_TAG, "pair status = %s",param->ble_security.auth_cmpl.success ? "success" : "fail");
        
            if(!param->ble_security.auth_cmpl.success) {
                ESP_LOGI(BLE_TAG, "fail reason = 0x%x",param->ble_security.auth_cmpl.fail_reason);
            } else {
                ESP_LOGI(BLE_TAG, "auth mode = %s",auth_req_to_str(param->ble_security.auth_cmpl.auth_mode));
            }
            show_bonded_devices();
            break;

        case ESP_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "(%s) remove the bond device complete, status:%d", __func__, param->remove_bond_dev_cmpl.status);
            esp_log_buffer_hex(BLE_TAG, (void *)param->remove_bond_dev_cmpl.bd_addr, sizeof(esp_bd_addr_t));
            ESP_LOGI(BLE_TAG, "------------------------------------");
            break;

        case ESP_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "(%s) Enable/disable privacy on the local device complete", __func__);
            break;

        default:
            ESP_LOGE(BLE_TAG, "(%s) understanding event:%d", __func__, event);
            break;
    }
}

void demo_ble_advertise_init(void)
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

    ret = hl_ble_gap_set_adv_data(g_adv_data, sizeof(g_adv_data));
    if (ret) {
        ESP_LOGE(BLE_TAG, "(%s) set adv data failed, error code = %x", __func__, ret);
        return;
    }

    ret = hl_ble_gap_set_scan_rsp_data(g_scan_rsp_data, sizeof(g_scan_rsp_data));
    if (ret) {
        ESP_LOGE(BLE_TAG, "(%s) set adv data failed, error code = %x", __func__, ret);
        return;
    }

    ret = hl_ble_gap_start_advertising(&g_adv_params);
    if (ret) {
        ESP_LOGE(BLE_TAG, "(%s) start adv failed, error code = %x", __func__, ret);
        return;
    }

    
    /*SMP*/
    /*static Passkey*/
    uint32_t static_passkey = 123456;
    hl_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, &static_passkey, sizeof(uint32_t));

    /*Authentication requirements*/
    esp_ble_auth_req_t auth_req = ESP_LE_AUTH_REQ_SC_MITM_BOND;         /*bonding with peer device after authentication*/
    hl_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));

    /*IO capability*/
    esp_ble_io_cap_t io_cap = ESP_IO_CAP_NONE;
    hl_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &io_cap, sizeof(uint8_t));

    /*Maximum Encryption key size*/
    uint8_t key_size = 16;                                              /*the key size should be 7~16 bytes*/
    hl_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size, sizeof(uint8_t));

    /*specified SMP Authentication requirement*/
    uint8_t auth_option = ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_DISABLE;
    hl_ble_gap_set_security_param(ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH, &auth_option, sizeof(uint8_t));

    /*OOB support*/
    uint8_t oob_support = ESP_BLE_OOB_DISABLE;
    hl_ble_gap_set_security_param(ESP_BLE_SM_OOB_SUPPORT, &oob_support, sizeof(uint8_t));
    
    /*Initiator Key Distribution/Generation*/
    uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    hl_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key, sizeof(uint8_t));

    /*Responder Key Distribution/Generation*/
    uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    hl_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));
}

void demo_ble_advertist_stop(void)
{
    esp_err_t ret = hl_ble_gap_stop_advertising();
    if (ret) {
        ESP_LOGE(BLE_TAG, "(%s) stop adv failed, error code = %x", __func__, ret);
    }

}

void demo_ble_advertist_start(void)
{
    esp_err_t ret = hl_ble_gap_start_advertising(&g_adv_params);
    if (ret) {
        ESP_LOGE(BLE_TAG, "(%s) start adv failed, error code = %x", __func__, ret);
    }
}