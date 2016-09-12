#include "glucosioedc.h"
#include <stt.h>
#include <sap.h>

typedef struct appdata{
	Evas_Object* win;
	Evas_Object* layout;
	Evas_Object* conform;
} appdata_s;


/* start stt wrapper functions */

static stt_h stt;

void
create_stt_handle()
{
    dlog_print(DLOG_INFO, "TAG", "create_stt_handle called");
    stt_h stt;
    int ret;
    ret = stt_create(&stt);
     if (STT_ERROR_NONE != ret)
    	dlog_print(DLOG_DEBUG, "ERROR", "in create stt handle");
}

void
destroy_stt_handle()
{
	dlog_print(DLOG_INFO, "TAG", "destroy stt handle called");
    int ret;
    ret = stt_destroy(stt); /* stt is the STT handle */
     if (STT_ERROR_NONE != ret)
     	dlog_print(DLOG_DEBUG, "ERROR", "error in destroy stt handle");

}


void
prepare_for_stt()
{
    dlog_print(DLOG_INFO, "TAG", "prepare_for_stt called");
	int ret;
    ret = stt_prepare(stt);
    if (STT_ERROR_NONE != ret)
    	dlog_print(DLOG_DEBUG, "ERROR", "error in prepare_for_stt");
}

void
unprepared_for_stt()
{
    dlog_print(DLOG_INFO, "TAG", "unprepared_for_stt called");
    int ret;
    ret = stt_unprepare(stt);
    if (STT_ERROR_NONE != ret)
    	dlog_print(DLOG_DEBUG, "ERROR", "error in unprepared_for_stt");
}


void
start_stt(const char* language, const char* type)
{
	dlog_print(DLOG_INFO, "TAG", "start_stt called");
	int ret;
    ret = stt_start(stt, language, STT_RECOGNITION_TYPE_FREE); /* The default language is NULL */
    if (STT_ERROR_NONE != ret)
    	dlog_print(DLOG_DEBUG, "ERROR", "error in start_stt");
}

void
stop_stt()
{
	dlog_print(DLOG_INFO, "TAG", "stop_stt called");
    int ret;
    ret = stt_stop(stt);
    if (STT_ERROR_NONE != ret)
    	dlog_print(DLOG_DEBUG, "ERROR", "error in stop_stt");
}


/* handle stt results */

bool
result_time_cb(stt_h stt, int index, stt_result_time_event_e event, const char* text, long start_time, long end_time, void* user_data)
{
	dlog_print(DLOG_INFO, "READING", "results: %s", text);
}


void
recognition_result_cb( stt_result_event_e event, const char** data, int data_count, const char* msg, void* user_data)
{
    /* If you want to get time info of result */
    int ret;
    ret = stt_foreach_detailed_result(stt, result_time_cb, NULL);
    if (STT_ERROR_NONE != ret)
    	dlog_print(DLOG_DEBUG, "ERROR", "error in recognition_result_cb");
    /* Your code */
}

void
set_recognition_result_cb()
{
    int ret;
    ret = stt_set_recognition_result_cb(stt, recognition_result_cb, NULL);
    if (STT_ERROR_NONE != ret)
    	dlog_print(DLOG_DEBUG, "ERROR", "error in set_recognition_result_cb");
}

void
unset_recognition_result_cb()
{
    int ret;
    ret = stt_unset_recognition_result_cb(stt);
    if (STT_ERROR_NONE != ret)
    	dlog_print(DLOG_DEBUG, "ERROR", "error in recognition_result_cb");
}


static Eina_Bool _timer_cb(void *data) {
	stop_stt();
	return ECORE_CALLBACK_DONE;

}

/* end handle stt results */




/* end stt wrapper functions */

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

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


static void _click_cb(void *data, Evas_Object *button, void *event_info){
	dlog_print(DLOG_DEBUG, "TAG", "clicked");
	Evas_Object *layout = data;
	elm_layout_signal_emit(layout, "microphone_show", "our_signal_source");
	create_stt_handle();
	prepare_for_stt();
	set_recognition_result_cb();
	start_stt('en_US',STT_RECOGNITION_TYPE_FREE);
	ecore_timer_add(7, _timer_cb, NULL);
	set_recognition_result_cb();

}

static void
create_base_gui(appdata_s *ad)
{
	char edj_path[PATH_MAX] = {0, };

	/* Window */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_conformant_set(ad->win, EINA_TRUE);
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);

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
	evas_object_size_hint_weight_set(ad->layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	eext_object_event_callback_add(ad->layout, EEXT_CALLBACK_BACK, layout_back_cb, ad);
	elm_object_content_set(ad->conform, ad->layout);

	Evas_Object *button = elm_button_add(ad->layout);
	evas_object_smart_callback_add(button, "clicked", _click_cb, ad->layout);


	elm_object_part_content_set(ad->layout, "bottom_button_area", button);

	elm_object_style_set(button, "bottom");
	evas_object_show(button);
	/* Show window after base gui is set up */
	evas_object_show(ad->win);

}


static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
	   Initialize UI resources and application's data
	   If this function returns true, the main loop of application starts
	   If this function returns false, the application is terminated */
	appdata_s *ad = data;

	create_base_gui(ad);

	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
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

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int
main(int argc, char *argv[])
{

	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);
	ui_app_remove_event_handler(handlers[APP_EVENT_LOW_MEMORY]);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main() is failed. err = %d", ret);
	}

	return ret;
}
