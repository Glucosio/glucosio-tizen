#include <watch_app.h>
#include <Elementary.h>
#include <watch_app_efl.h>

#include "face.h"
#include "button.h"
#include "layout.h"
#include "path_utils.h"

/**
 * For adding new style necessary to add new images of bg, arrow_hour, arrow_minute
 * and arrow_second into res/images/ folder and change next number to new styles
 * number.
 */
#define STYLE_COUNT         4

#define ID_HOUR             0
#define ID_MINUTE           1
#define ID_SECOND           2

#define ANGLE_PER_HOUR      30.0
#define ANGLE_PER_MINUTE    6.0
#define ANGLE_PER_SECOND    6.0

#define MINUTES_PER_HOUR    60.0
#define SECONDS_PER_MIN     60.0

#define DOUBLE_CLICK_TIME   250 /* msec */

#define BG_FORMAT           "images/bg_%d.png"
#define HOUR_ARROW_FORMAT   "images/arrow_hour_%d.png"
#define MINUTE_ARROW_FORMAT "images/arrow_minute_%d.png"
#define SECOND_ARROW_FORMAT "images/arrow_second_%d.png"

static Evas_Object *win          = NULL;
static Evas_Object *layout_main  = NULL;
static Evas_Object *bg           = NULL;
static Evas_Object *arrow_hour   = NULL;
static Evas_Object *arrow_minute = NULL;
static Evas_Object *arrow_second = NULL;

static int style_number          = 0;

static void
_image_file_change(Evas_Object *image, const char *rel_format_path)
{
   char abs_path_format[PATH_MAX] = {0,};
   char abs_path[PATH_MAX]        = {0,};

   file_full_resource_path_get(rel_format_path, abs_path_format);
   snprintf(abs_path, PATH_MAX, abs_path_format, style_number);

   elm_image_file_set(image, abs_path, NULL);
}

static void
_double_click_cb()
{
   style_number = (style_number + 1) % STYLE_COUNT;

   // Change image files according to new style number.
   _image_file_change(bg, BG_FORMAT);
   _image_file_change(arrow_hour, HOUR_ARROW_FORMAT);
   _image_file_change(arrow_minute, MINUTE_ARROW_FORMAT);
   _image_file_change(arrow_second, SECOND_ARROW_FORMAT);
}

static long long
_timestamp_get()
{
   watch_time_h current_time = {0,};
   time_t timestamp          = 0;
   int    msec               = 0;

   watch_time_get_current_time(&current_time);
   watch_time_get_utc_timestamp(current_time, &timestamp);
   watch_time_get_millisecond(current_time, &msec);
   watch_time_delete(current_time);

   return timestamp * 1000 + msec;
}

static void
_click_cb(void *data, Evas_Object *button, void *event_info)
{
   static long long prev_click_time = 0;
   long long current_click_time     = _timestamp_get();

   // Checking time difference betwen current and previous click
   // If two click performed in DOUBLE_CLICK_TIME range _double_click_cb() called
   // to change style to next by changing images for bg and arrows.
   if (current_click_time - prev_click_time <= DOUBLE_CLICK_TIME)
     {
        _double_click_cb();
        prev_click_time = 0;
     }
   else
     {
        prev_click_time = current_click_time;
     }
}

static void
_style_images_create()
{
   // 1.6 Create image objects
   bg           = elm_image_add(layout_main);
   arrow_hour   = elm_image_add(layout_main);
   arrow_minute = elm_image_add(layout_main);
   arrow_second = elm_image_add(layout_main);

   // 1.7 Set image path to show using style number.
   _image_file_change(bg, BG_FORMAT);
   _image_file_change(arrow_hour, HOUR_ARROW_FORMAT);
   _image_file_change(arrow_minute, MINUTE_ARROW_FORMAT);
   _image_file_change(arrow_second, SECOND_ARROW_FORMAT);

   // 1.8 Show images.
   evas_object_show(bg);
   evas_object_show(arrow_hour);
   evas_object_show(arrow_minute);
   evas_object_show(arrow_second);
}

static void
_conformant_create()
{
   Evas_Object *conformant = elm_conformant_add(win);

   evas_object_size_hint_weight_set(conformant, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   // 1.4 Create main layout from main.edj file
   //    which will be created after compiling main.edc source file.
   layout_main = layout_add(conformant, "edje/layouts/main.edj", "main");

   // 1.5 Create image objects for bg, hour, second and minute arrows.
   _style_images_create();
   elm_object_part_content_set(layout_main, "swallow.bg", bg);
   elm_object_part_content_set(layout_main, "swallow.minute", arrow_minute);
   elm_object_part_content_set(layout_main, "swallow.hour", arrow_hour);
   elm_object_part_content_set(layout_main, "swallow.second", arrow_second);

   // 1.9 Create transparent button to change style by double-click on this button.
   Evas_Object *button = button_add(layout_main, NULL, "transparent");

   // 1.10 Register click callback to handle double click there and change style.
   evas_object_smart_callback_add(button, "clicked", _click_cb, NULL);

   // 1.11 Push change style button into swallow part in layout edc.
   elm_object_part_content_set(layout_main, "swallow.button.style_change", button);

   elm_object_content_set(conformant, layout_main);

   evas_object_show(conformant);
   elm_win_resize_object_add(win, conformant);
}

void
face_create(int width, int height)
{
   // 1.1 Set to use opengl rendering, for using more smooth drawing of arrows when
   //     it will be rotated.
   elm_config_accel_preference_set("opengl");

   // 1.2 In watch application window not created but received from library, because
   //     watch application is running in "watch manager application" so this application
   //     gives widget to use it as our watch application window.
   watch_app_get_elm_win(&win);

   // 1.3 Create conformant widget and resize it by our window.
   //     Conformant widget is the base container of our watch face.
   //     We will put all other ui components to it.
   _conformant_create();

   // 1.12 Resize window and conformant to necessary size.
   evas_object_resize(win, width, height);

   // 1.13 Show window after all UI is created to optimize all drawing procedures.
   evas_object_show(win);
}

void
face_destroy()
{
   // 2.1 This function will delete main window,
   //     and before to do this all children of window will be deleted too.
   evas_object_del(win);
}

void
face_time_ui_update(int hour24, int minute, int second)
{
   // 3.1 Get edje object which represent all objects(parts) of edc file for our layout.
   Evas_Object *edje      = elm_layout_edje_get(layout_main);
   Edje_Message_Float msg = {0,};

   // 3.2 Calculation of actual angles for arrows
   double hour_angle   = ((hour24 % 12) + minute / MINUTES_PER_HOUR) * ANGLE_PER_HOUR;
   double minute_angle = (minute + second / SECONDS_PER_MIN) * ANGLE_PER_MINUTE;
   double angle_second = second * ANGLE_PER_SECOND;

   // 3.3 Send message to edje to rotate hour arrow for given angle
   msg.val = hour_angle;
   edje_object_message_send(edje, EDJE_MESSAGE_FLOAT, ID_HOUR, &msg);

   // 3.4 Send message to edje to rotate minute arrow for given angle
   msg.val = minute_angle;
   edje_object_message_send(edje, EDJE_MESSAGE_FLOAT, ID_MINUTE, &msg);

   // 3.5 Send message to edje to rotate second arrow for given angle
   msg.val = angle_second;
   edje_object_message_send(edje, EDJE_MESSAGE_FLOAT, ID_SECOND, &msg);
}
