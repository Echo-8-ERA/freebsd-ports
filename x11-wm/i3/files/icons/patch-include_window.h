--- include/window.h.orig	2019-01-27 15:45:19 UTC
+++ include/window.h
@@ -89,3 +89,9 @@ void window_update_hints(i3Window *win, xcb_get_proper
  *
  */
 void window_update_motif_hints(i3Window *win, xcb_get_property_reply_t *prop, border_style_t *motif_border_style);
+
+/**
+ * Updates the _NET_WM_ICON
+ *
+ */
+void window_update_icon(i3Window *win, xcb_get_property_reply_t *prop);
