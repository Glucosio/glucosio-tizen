#include <app.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>

#include "path_utils.h"

void
file_full_resource_path_get(const char *relative_path, char *absolute_path)
{
   if (!relative_path)
     return;

   // 1. something like /opt/usr/apps/org.example.watch/res/ will be here in res path.
   char *res_path = app_get_resource_path();
   if (res_path)
     {
        // 2. Concatenate ...res/ directory path and file path.
        snprintf(absolute_path, PATH_MAX, "%s%s", res_path, relative_path);
        free(res_path);
     }
}
