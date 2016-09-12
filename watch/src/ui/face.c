#include <Elementary.h>
#include <watch_app_efl.h>

#include "face.h"
#include "layout.h"

#define ID_HOUR          0
#define ID_MINUTE        1
#define ID_SECOND        2

#define ANGLE_PER_HOUR   30.0
#define ANGLE_PER_MINUTE 6.0
#define ANGLE_PER_SECOND 6.0

#define MINUTES_PER_HOUR 60.0
#define SECONDS_PER_MIN  60.0

static Evas_Object *win         = NULL;
static Evas_Object *layout_main = NULL;

static void
_conformant_create()
{
   Evas_Object *conformant = elm_conformant_add(win);

   // 4. Create main layout from main.edj file
   //    which will be created after compiling main.edc source file.
   layout_main = layout_add(conformant, "edje/layouts/main.edj", "main");

   elm_object_content_set(conformant, layout_main);

   evas_object_show(conformant);
   elm_win_resize_object_add(win, conformant);
}

void
face_create(int width, int height)
{
   // 1. Set to use opengl rendering, for using more smooth drawing of arrows when
   //    it will be rotated.
   elm_config_accel_preference_set("opengl");

   // 2. In watch application window not created but received from library, because
   //    watch application is running in "watch manager application" so this application
   //    gives widget to use it as our watch application window.
   watch_app_get_elm_win(&win);

   // 3. Create conformant widget and resize it by our window.
   //    Conformant widget is the base container of our watch face.
   //    We will put all other ui components to it.
   _conformant_create();

   // 5. Resize window and conformant to necessary size.
   evas_object_resize(win, width, height);

   // 6. Show window after all UI is created to optimize all drawing procedures.
   evas_object_show(win);
}

void
face_destroy()
{
   // 12. This function will delete main window,
   //     and before to do this all children of window will be deleted too.
   evas_object_del(win);
}

void
face_time_ui_update(int hour24, int minute, int second)
{
   // 7. Get edje object which represent all objects(parts) of edc file for our layout.
   Evas_Object *edje      = elm_layout_edje_get(layout_main);
   Edje_Message_Float msg = {0,};

   // 8. Calculation of actual angles for arrows
   double hour_angle   = ((hour24 % 12) + minute / MINUTES_PER_HOUR) * ANGLE_PER_HOUR;
   double minute_angle = (minute + second / SECONDS_PER_MIN) * ANGLE_PER_MINUTE;
   double angle_second = second * ANGLE_PER_SECOND;

   // 9. Send message to edje to rotate hour arrow for given angle
   msg.val = hour_angle;
   edje_object_message_send(edje, EDJE_MESSAGE_FLOAT, ID_HOUR, &msg);

   // 10. Send message to edje to rotate minute arrow for given angle
   msg.val = minute_angle;
   edje_object_message_send(edje, EDJE_MESSAGE_FLOAT, ID_MINUTE, &msg);

   // 11. Send message to edje to rotate second arrow for given angle
   msg.val = angle_second;
   edje_object_message_send(edje, EDJE_MESSAGE_FLOAT, ID_SECOND, &msg);
}
