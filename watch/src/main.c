#include <watch_app.h>

#include "face.h"

/**
 * This function called once when user choose in watch faces menu current watch.
 * Also it called when watch is created after rebooting.
 */
static bool
_create_cb(int width, int height, void *data)
{
   // 4. Create our UI here.
   face_create(width, height);

   return true;
}

/**
 * This function called once when user change watch faces to another from watch faces menu.
 * Also it called when watch reloaded or switched off.
 *
 * Here we need to release resources of application.
 */
static void
_terminate_cb(void *data)
{
   face_destroy();
}

/**
 * This function called each second from this point we will update watch time ui.
 * Input parameter to function watch_time will be used to set ui according to it.
 */
static void
_time_tick_cb(watch_time_h watch_time, void *data)
{
   int hour24 = 10;
   int minute = 10;
   int second = 36;

   // 5. Retrieving separate time values.
   watch_time_get_hour24(watch_time, &hour24);
   watch_time_get_minute(watch_time, &minute);
   watch_time_get_second(watch_time, &second);

   // 6. Update ui for given time values.
   face_time_ui_update(hour24, minute, second);
}

int
main(int argc, char *argv[])
{
   // 1. This is struct with lifecycle callbacks.
   watch_app_lifecycle_callback_s callbacks = {0,};

   // 2. Registering special callbacks for application life events.
   callbacks.create    = _create_cb;
   callbacks.terminate = _terminate_cb;
   callbacks.time_tick = _time_tick_cb;

   // 3. Starting specific application efl loop: watch application in this case.
   return watch_app_main(argc, argv, &callbacks, NULL);
}

