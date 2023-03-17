#include "app_home_screen_idle_activity.h"

#define LOG_TAG        "app_home_idle"

/******************************************************************************/
uint8_t i = 0;

bool app_test1_activity(ux_activity_context *self, uint32_t event_group, uint32_t event_id, void *extra_data, uint32_t data_len)
{
    APPS_LOG_D("app_test", "test1_activity receive event_group=%d event_id=0x%d", event_group, event_id);
    
    if ( i == 10) {
        APPS_LOG_D("app_test", "set result");
        ux_set_result(self, NULL, 0);
    } else if (i == 15) {
        APPS_LOG_D("app_test", "finsh test1 activity");
        ux_finish_activity(self, self);
    }
    if (event_group == 0) {
        return true;
    } else {
        return false;
    }
}   

static void _test(ux_activity_context *self) 
{
    i += 1;

    if (i == 5) {
        APPS_LOG_D(LOG_TAG, "start test1 activity");
        ux_start_activity(self, app_test1_activity, UX_ACTIVITY_PRIORITY_LOW, NULL, 0);
    }
}


/******************************************************************************/

bool app_home_screen_idle_activity(ux_activity_context *self, uint32_t event_group, uint32_t event_id, void *extra_data, uint32_t data_len)
{
    APPS_LOG_D(LOG_TAG, "idle receive event_group=%d event_id=0x%d", event_group, event_id);
    _test(self);
    
    if (event_group == 0) {
        if (event_id == UX_ACTIVITY_SYSTEM_EVENT_ID_SET_RESULT) {
            APPS_LOG_D(LOG_TAG, "recv result");
        }
        return true;
    } else {
        return false;
    }
}