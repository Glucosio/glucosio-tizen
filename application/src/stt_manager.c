#include <stt.h>
#include <dlog.h>
#include <Eina.h>
#include <Ecore.h>

#include "stt_manager.h"

static stt_h stt;

bool
result_time_cb(stt_h stt, int index, stt_result_time_event_e event, const char* text, long start_time, long end_time, void* user_data)
{
   dlog_print(DLOG_INFO, "READING", "results: %s", text);
   return true;
}

void
_recognition_result_cb(stt_h stt,
                       stt_result_event_e event,
                       const char** data,
                       int data_count,
                       const char* msg,
                       void *user_data)
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
    ret = stt_set_recognition_result_cb(stt, _recognition_result_cb, NULL);
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

void
create_stt_handle() {
   dlog_print(DLOG_INFO, "TAG", "create_stt_handle called");
   stt_h stt;
   int ret;
   ret = stt_create(&stt);
   if (STT_ERROR_NONE != ret)
      dlog_print(DLOG_DEBUG, "ERROR", "in create stt handle");
}

void
destroy_stt_handle() {
   dlog_print(DLOG_INFO, "TAG", "destroy stt handle called");
   int ret;
   ret = stt_destroy(stt); /* stt is the STT handle */
   if (STT_ERROR_NONE != ret)
      dlog_print(DLOG_DEBUG, "ERROR", "error in destroy stt handle");

}

void
prepare_for_stt() {
   dlog_print(DLOG_INFO, "TAG", "prepare_for_stt called");
   int ret;
   ret = stt_prepare(stt);
   if (STT_ERROR_NONE != ret)
      dlog_print(DLOG_DEBUG, "ERROR", "error in prepare_for_stt");
}

void
unprepared_for_stt() {
   dlog_print(DLOG_INFO, "TAG", "unprepared_for_stt called");
   int ret;
   ret = stt_unprepare(stt);
   if (STT_ERROR_NONE != ret)
      dlog_print(DLOG_DEBUG, "ERROR", "error in unprepared_for_stt");
}

void
start_stt(const char* language, const char* type) {
   dlog_print(DLOG_INFO, "TAG", "start_stt called");
   int ret;
   ret = stt_start(stt, language, STT_RECOGNITION_TYPE_FREE); /* The default language is NULL */
   if (STT_ERROR_NONE != ret)
      dlog_print(DLOG_DEBUG, "ERROR", "error in start_stt");
}

void
stop_stt() {
   dlog_print(DLOG_INFO, "TAG", "stop_stt called");
   int ret;
   ret = stt_stop(stt);
   if (STT_ERROR_NONE != ret)
      dlog_print(DLOG_DEBUG, "ERROR", "error in stop_stt");
}
