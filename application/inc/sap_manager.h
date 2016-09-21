#ifndef _SAP_MANAGER_H_
#define _SAP_MANAGER_H_

#include <glib.h>

typedef void (*peer_found_cb)(void *data);

void     sap_initialize();

gboolean sap_find_peers(peer_found_cb callback, void *data);

gboolean sap_request_service_connection(void);

gboolean sap_terminate_service_connection(void);

gboolean sap_send_data(const char *message);

#endif /* _SAP_MANAGER_H_ */
