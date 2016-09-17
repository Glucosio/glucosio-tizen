#include <app.h>
#include <app_info.h>
#include <app_manager.h>

#include "application_manager.h"

#define APP_ID "org.example.glucosioedc"

Eina_Bool
glucosio_glucose_value_get(int *value)
{
   Eina_Bool result_is_ok = EINA_FALSE;
   int result = 0;

   if (glucosio_ui_application_is_exist())
     {
        // TODO: implement value getting from ui application.

        *value = result;
     }

   return result_is_ok;
}

Eina_Bool
glucosio_ui_application_is_exist()
{
   app_info_h app_info;

   if (app_manager_get_app_info(APP_ID, &app_info) == APP_MANAGER_ERROR_NO_SUCH_APP)
     return EINA_FALSE;

   app_info_destroy(app_info);

   return EINA_TRUE;
}

Eina_Bool
glucosio_ui_application_run()
{
   Eina_Bool is_run = EINA_FALSE;
   app_control_h control;

   if (_application_is_exist(app_id))
     {
        app_control_create(&control);

        app_control_set_app_id(control, APP_ID);
        app_control_set_operation(control, APP_CONTROL_OPERATION_DEFAULT);

        if (app_control_send_launch_request(control, NULL, NULL) == APP_CONTROL_ERROR_NONE)
          is_run = EINA_TRUE;

        app_control_destroy(control);
     }

   return is_run;
}
