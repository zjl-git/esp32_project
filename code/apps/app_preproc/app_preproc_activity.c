#include "app_preproc_activity.h"

#define LOG_TAG        "app_preproc"

bool app_preproc_activity(ux_activity_context *self, uint32_t event_group, uint32_t event_id, void *extra_data, uint32_t data_len)
{
    APPS_LOG_D(LOG_TAG, "pre-proc receive event_group=%d event_id=0x%d", event_group, event_id);
    return false;
}