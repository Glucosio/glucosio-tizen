#ifndef LAYOUT_H_
#define LAYOUT_H_

#include <Evas.h>

/**
 * Util function to create layout object from given edje file and group name.
 */
Evas_Object *layout_add(Evas_Object *parent, const char *edje_file, const char *group_name);

#endif /* LAYOUT_H_ */
