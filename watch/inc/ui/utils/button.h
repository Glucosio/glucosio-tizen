#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Evas.h>

/**
 * Util function to create button object with given text and style.
 */
Evas_Object *button_add(Evas_Object *parent, const char *text, const char *style);

#endif /* _BUTTON_H_ */
