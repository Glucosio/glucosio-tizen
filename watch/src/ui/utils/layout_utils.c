#include <Elementary.h>

#include "path_utils.h"
#include "layout.h"

Evas_Object *
layout_add(Evas_Object *parent, const char *edje_file, const char *group_name)
{
   if (!parent || !edje_file || !group_name)
     return NULL;

   Evas_Object *layout = elm_layout_add(parent);
   char edj_path[PATH_MAX] = {0,};

   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);

   // 1. Convert relative path to res/ dir for given file to absolute path.
   file_full_resource_path_get(edje_file, edj_path);
   elm_layout_file_set(layout, edj_path, group_name);

   evas_object_show(layout);

   return layout;
}

