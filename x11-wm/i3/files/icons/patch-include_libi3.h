--- include/libi3.h.orig	2019-01-27 15:45:19 UTC
+++ include/libi3.h
@@ -615,6 +615,11 @@ color_t draw_util_hex_to_color(const char *color);
 void draw_util_text(i3String *text, surface_t *surface, color_t fg_color, color_t bg_color, int x, int y, int max_width);
 
 /**
+ * Draw the given image using libi3.
+ */
+void draw_util_image(unsigned char *src, int src_width, int src_height, surface_t *surface, int x, int y, int width, int height);
+
+/**
  * Draws a filled rectangle.
  * This function is a convenience wrapper and takes care of flushing the
  * surface as well as restoring the cairo state.
