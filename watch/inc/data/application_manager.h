#ifndef _APPLICATION_MANAGER_H_
#define _APPLICATION_MANAGER_H_

#include <Eina.h>

/*
 * Util function to get last measured value from ui application.
 */
Eina_Bool glucosio_glucose_value_get(int *value);

/*
 * Util function to check that ui application is exist.
 */
Eina_Bool glucosio_ui_application_is_exist();

/*
 * Util function to run ui application to add new measurment.
 */
Eina_Bool glucosio_ui_application_run();

#endif /* _APPLICATION_MANAGER_H_ */
