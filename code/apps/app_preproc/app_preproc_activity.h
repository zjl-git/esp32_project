#ifndef _APPS_PREPROC__
#define _APPS_PREPROC__

#ifdef __cplusplus
extern "C"
{
#endif

#include "apps_init.h"
#include "ux_manager.h"

bool app_preproc_activity(ux_activity_context *self, uint32_t event_group, uint32_t event_id, void *extra_data, uint32_t data_len);

#ifdef __cplusplus
}
#endif

#endif
