--- libi3/draw_util.c.orig	2019-01-27 15:45:19 UTC
+++ libi3/draw_util.c
@@ -140,6 +140,41 @@ void draw_util_text(i3String *text, surface_t *surface
     cairo_surface_mark_dirty(surface->surface);
 }
 
+/**
+ * Draw the given image using libi3.
+ * This function is a convenience wrapper and takes care of flushing the
+ * surface as well as restoring the cairo state.
+ *
+ */
+void draw_util_image(unsigned char *src, int src_width, int src_height, surface_t *surface, int x, int y, int width, int height) {
+    RETURN_UNLESS_SURFACE_INITIALIZED(surface);
+
+    double scale;
+
+    cairo_save(surface->cr);
+
+    cairo_surface_t *image;
+
+    image = cairo_image_surface_create_for_data(
+            src,
+            CAIRO_FORMAT_ARGB32,
+            src_width,
+            src_height,
+            src_width * 4);
+
+    cairo_translate(surface->cr, x, y);
+
+    scale = MIN((double)width / src_width, (double)height / src_height);
+    cairo_scale(surface->cr, scale, scale);
+
+    cairo_set_source_surface(surface->cr, image, 0, 0);
+    cairo_paint(surface->cr);
+
+    cairo_surface_destroy(image);
+
+    cairo_restore(surface->cr);
+}
+
 /*
  * Draws a filled rectangle.
  * This function is a convenience wrapper and takes care of flushing the
