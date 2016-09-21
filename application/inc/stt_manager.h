#ifndef _STT_MANAGER_H_
#define _STT_MANAGER_H_

void create_stt_handle();

void destroy_stt_handle();

void prepare_for_stt();

void unprepared_for_stt();

void start_stt(const char *language, const char *type);

void stop_stt();

#endif /* _STT_MANAGER_H_ */
