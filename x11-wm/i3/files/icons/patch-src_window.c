--- src/window.c.orig	2019-01-27 15:45:19 UTC
+++ src/window.c
@@ -17,6 +17,7 @@ void window_free(i3Window *win) {
     FREE(win->class_class);
     FREE(win->class_instance);
     i3string_free(win->name);
+    FREE(win->icon);
     FREE(win->ran_assignments);
     FREE(win);
 }
@@ -370,4 +371,78 @@ void window_update_motif_hints(i3Window *win, xcb_get_
 #undef MWM_DECOR_ALL
 #undef MWM_DECOR_BORDER
 #undef MWM_DECOR_TITLE
+}
+
+void window_update_icon(i3Window *win, xcb_get_property_reply_t *prop)
+{
+	
+	if (config.window_icon_size <= 0) return;
+    uint32_t *data = NULL;
+    uint64_t len = 0;
+	uint64_t data_len = config.window_icon_size * config.window_icon_size;
+    if (!prop || prop->type != XCB_ATOM_CARDINAL || prop->format != 32) {
+        DLOG("_NET_WM_ICON is not set\n");
+        FREE(prop);
+        return;
+    }
+
+    uint32_t prop_value_len = xcb_get_property_value_length(prop);
+    uint32_t *prop_value = (uint32_t *) xcb_get_property_value(prop);
+
+    /* Find the number of icons in the reply. */
+    while (prop_value_len > (sizeof(uint32_t) * 2) && prop_value &&
+            prop_value[0] && prop_value[1])
+    {
+        /* Check that the property is as long as it should be (in bytes),
+           handling integer overflow. "+2" to handle the width and height
+           fields. */
+        const uint64_t crt_len = prop_value[0] * (uint64_t) prop_value[1];
+        const uint64_t expected_len = (crt_len + 2) * 4;
+
+        if (expected_len > prop_value_len) {
+            break;
+        }
+
+        if (len == 0 || (crt_len >= data_len && crt_len < len)) {
+            len = crt_len;
+            data = prop_value;
+        }
+        if (len == data_len) {
+            break; /* found icon of desired size */
+        }
+
+        /* Find pointer to next icon in the reply. */
+        prop_value_len -= expected_len;
+        prop_value = (uint32_t *) (((uint8_t *) prop_value) + expected_len);
+    }
+
+    if (!data) {
+        DLOG("Could not get _NET_WM_ICON\n");
+        FREE(prop);
+        return;
+    }
+
+    LOG("Got _NET_WM_ICON of size: (%d,%d)\n", data[0], data[1]);
+    win->name_x_changed = true; /* trigger a redraw */
+
+    win->icon_width = data[0];
+    win->icon_height = data[1];
+    win->icon = srealloc(win->icon, len * 4);
+
+    for (uint64_t i = 0; i < len; i++) {
+        uint8_t r, g, b, a;
+        a = (data[2 + i] >> 24) & 0xff;
+        r = (data[2 + i] >> 16) & 0xff;
+        g = (data[2 + i] >>  8) & 0xff;
+        b = (data[2 + i] >>  0) & 0xff;
+
+        /* Cairo uses premultiplied alpha */
+        r = (r * a) / 0xff;
+        g = (g * a) / 0xff;
+        b = (b * a) / 0xff;
+
+        win->icon[i] = (a << 24) | (r << 16) | (g << 8) | b;
+    }
+
+    FREE(prop);
 }
