#include <stt.h>
#include <sap.h>
#include <app.h>
#include <dlog.h>
#include <Ecore.h>
#include <Elementary.h>
#include <efl_extension.h>
#include <system_settings.h>

#include "sap_manager.h"
#include "stt_manager.h"

#define TAG      "GLUCOSIO.MAIN"
#define EDJ_FILE "edje/glucosioedc.edj"
#define GRP_MAIN "main"

typedef struct appdata{
   Evas_Object* win;
   Evas_Object* layout;
   Evas_Object* conform;
   Evas_Object* entry;
} appdata_s;

static void
layout_back_cb(void *data, Evas_Object *obj, void *event_info)
{
   appdata_s *ad = data;
   /* Let window go to hide state. */
   elm_win_lower(ad->win);
}

static void
app_get_resource(const char *edj_file_in, char *edj_path_out, int edj_path_max)
{
   char *res_path = app_get_resource_path();
   if (res_path) {
      snprintf(edj_path_out, edj_path_max, "%s%s", res_path, edj_file_in);
      free(res_path);
   }
}

static Eina_Bool _timer_cb(void *data) {
   stop_stt();
   return ECORE_CALLBACK_DONE;
}

static void _click_cb(void *data, Evas_Object *button, void *event_info){
   dlog_print(DLOG_DEBUG, TAG, "clicked");
   Evas_Object *layout = data;
   elm_layout_signal_emit(layout, "microphone_show", "our_signal_source");
   create_stt_handle();
   prepare_for_stt();
   start_stt("en_US", STT_RECOGNITION_TYPE_FREE);
   ecore_timer_add(7, _timer_cb, NULL);
}


static void _add_cb(void *data, Evas_Object *button, void *event_info){
	elm_object_focus_set(data, EINA_TRUE);
}


static void
_peer_found_cb(void *data)
{
   const char *data_to_send = data;
   dlog_print(DLOG_DEBUG, TAG, "Peer found try to send: %s", data_to_send);

   // Sending data to hosting
   sap_send_data(data_to_send);
   sap_terminate_service_connection();
}

static void
_enter_click_cb(void *data, Evas_Object *entry, void *event_info)
{
   // Get entered text from entry
   const char *glucose_reading = elm_entry_entry_get(entry);
   dlog_print(DLOG_DEBUG, TAG, "Value: %s", glucose_reading);

   // Hide keyboard input panel
   elm_entry_input_panel_hide(entry);

   // Create sap connection.
   sap_find_peers(_peer_found_cb, (void *)glucose_reading);
}

static void
_entry_create(appdata_s *data)
{
   data->entry = elm_entry_add(data->layout);
   elm_object_translatable_part_text_set(data->entry, "elm.guide", "Enter...");
   Elm_Entry_Filter_Limit_Size limit_filter;
   limit_filter.max_char_count = 3;
   elm_entry_markup_filter_append(data->entry, elm_entry_filter_limit_size, &limit_filter);

   elm_entry_input_panel_layout_set(data->entry, ELM_INPUT_PANEL_LAYOUT_NUMBERONLY);

   elm_entry_single_line_set(data->entry, EINA_TRUE);
   evas_object_smart_callback_add(data->entry, "activated", _enter_click_cb, data);

   elm_object_part_content_set(data->layout, "entry_area", data->entry);
   evas_object_show(data->entry);
}


static void
create_base_gui(appdata_s *ad)
{
   char edj_path[PATH_MAX] = {0, };

   /* Window */
   ad->win = elm_win_util_standard_add(NULL, NULL);

   /* Conformant */
   ad->conform = elm_conformant_add(ad->win);
   elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
   elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
   evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(ad->win, ad->conform);
   evas_object_show(ad->conform);
   /* Base Layout */
   app_get_resource(EDJ_FILE, edj_path, (int)PATH_MAX);
   ad->layout = elm_layout_add(ad->win);
   elm_layout_file_set(ad->layout, edj_path, GRP_MAIN);
   elm_object_part_text_set(ad->layout, "txt_title", "Glucosio");

   _entry_create(ad);

   evas_object_size_hint_weight_set(ad->layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   eext_object_event_callback_add(ad->layout, EEXT_CALLBACK_BACK, layout_back_cb, ad);
   elm_object_content_set(ad->conform, ad->layout);

   Evas_Object *button = elm_button_add(ad->layout);
   evas_object_smart_callback_add(button, "clicked", _add_cb, ad->entry);
   elm_object_text_set(button, "Add");


   elm_object_part_content_set(ad->layout, "bottom_button_area", button);

   elm_object_style_set(button, "bottom");
   evas_object_show(button);
   /* Show window after base gui is set up */
   evas_object_show(ad->win);

}

static bool
_app_create_cb(void *data)
{
   appdata_s *ad = data;

   sap_initialize();
   create_base_gui(ad);

   return true;
}

static void
_app_control_cb(app_control_h app_control, void *data)
{
   /* Handle the launch request. */
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
   /*APP_EVENT_LANGUAGE_CHANGED*/
   char *locale = NULL;
   system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
   elm_language_set(locale);
   free(locale);
   return;
}

int
main(int argc, char *argv[])
{
   appdata_s ad                               = {0,};
   ui_app_lifecycle_callback_s event_callback = {0,};
   app_event_handler_h handlers[5]            = {NULL,};

   event_callback.create      = _app_create_cb;
   event_callback.app_control = _app_control_cb;

   ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);

   return ui_app_main(argc, argv, &event_callback, &ad);
}
