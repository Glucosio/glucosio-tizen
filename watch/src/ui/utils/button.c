#include <Elementary.h>

#include "button.h"

Evas_Object *
button_add(Evas_Object *parent, const char *text, const char *style)
{
   Evas_Object *button = elm_button_add(parent);

   if (style)
     elm_object_style_set(button, style);

   if (text)
     elm_object_text_set(button, text);

   evas_object_show(button);

   return button;
}
