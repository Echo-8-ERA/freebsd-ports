--- src/x.c.orig	2019-01-27 15:45:19 UTC
+++ src/x.c
@@ -578,6 +578,7 @@ void x_draw_decoration(Con *con) {
 
     /* 6: draw the title */
     int text_offset_y = (con->deco_rect.height - config.font.height) / 2;
+    int text_offset_x = 0;
 
     const int title_padding = logical_px(2);
     const int deco_width = (int)con->deco_rect.width;
@@ -634,6 +635,8 @@ void x_draw_decoration(Con *con) {
         }
     } else {
         title = con->title_format == NULL ? win->name : con_parse_title_format(con);
+        if ((config.window_icon_size > 0) && (win->icon))
+			text_offset_x = config.window_icon_size + 2;
     }
     if (title == NULL) {
         goto copy_pixmaps;
@@ -643,7 +646,7 @@ void x_draw_decoration(Con *con) {
     switch (config.title_align) {
         case ALIGN_LEFT:
             /* (pad)[text    ](pad)[mark + its pad) */
-            title_offset_x = title_padding;
+            title_offset_x = title_padding + text_offset_x;
             break;
         case ALIGN_CENTER:
             /* (pad)[  text  ](pad)[mark + its pad)
@@ -669,6 +672,24 @@ void x_draw_decoration(Con *con) {
 
     if (win == NULL || con->title_format != NULL) {
         I3STRING_FREE(title);
+    }
+    
+    /* Draw the icon */
+    if (win->icon) {
+        uint16_t width = config.window_icon_size;
+        uint16_t height = config.window_icon_size;
+
+        int icon_offset_y = (con->deco_rect.height - height) / 2;
+
+        draw_util_image(
+                (unsigned char *)win->icon,
+                win->icon_width,
+                win->icon_height,
+                &(parent->frame_buffer),
+                con->deco_rect.x + logical_px(2),
+                con->deco_rect.y + icon_offset_y,
+                width,
+                height);
     }
 
     x_draw_decoration_after_title(con, p);
