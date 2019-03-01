--- src/handlers.c.orig	2019-01-27 15:45:19 UTC
+++ src/handlers.c
@@ -1392,6 +1392,19 @@ static bool handle_strut_partial_change(void *data, xc
     return true;
 }
 
+static bool handle_windowicon_change(void *data, xcb_connection_t *conn, uint8_t state,
+                                     xcb_window_t window, xcb_atom_t atom, xcb_get_property_reply_t *prop) {
+    Con *con;
+    if ((con = con_by_window_id(window)) == NULL || con->window == NULL)
+        return false;
+
+    window_update_icon(con->window, prop);
+
+    x_push_changes(croot);
+
+    return true;
+}
+
 /* Returns false if the event could not be processed (e.g. the window could not
  * be found), true otherwise */
 typedef bool (*cb_property_handler_t)(void *data, xcb_connection_t *c, uint8_t state, xcb_window_t window, xcb_atom_t atom, xcb_get_property_reply_t *property);
@@ -1413,7 +1426,8 @@ static struct property_handler_t property_handlers[] =
     {0, 128, handle_class_change},
     {0, UINT_MAX, handle_strut_partial_change},
     {0, UINT_MAX, handle_window_type},
-    {0, 5 * sizeof(uint64_t), handle_motif_hints_change}};
+    {0, 5 * sizeof(uint64_t), handle_motif_hints_change},
+    {0, UINT_MAX, handle_windowicon_change}};
 #define NUM_HANDLERS (sizeof(property_handlers) / sizeof(struct property_handler_t))
 
 /*
@@ -1435,6 +1449,7 @@ void property_handlers_init(void) {
     property_handlers[8].atom = A__NET_WM_STRUT_PARTIAL;
     property_handlers[9].atom = A__NET_WM_WINDOW_TYPE;
     property_handlers[10].atom = A__MOTIF_WM_HINTS;
+    property_handlers[11].atom = A__NET_WM_ICON;
 }
 
 static void property_notify(uint8_t state, xcb_window_t window, xcb_atom_t atom) {
