--- src/render.c.orig	2019-01-27 15:45:19 UTC
+++ src/render.c
@@ -26,7 +26,9 @@ int render_deco_height(void) {
     int deco_height = config.font.height + 4;
     if (config.font.height & 0x01)
         ++deco_height;
-    return deco_height;
+    /* minimum decoration height to allow icon to fit
+     * not actually required, icon would be cropped otherwise */
+    return deco_height < 16 ? 16 : deco_height;
 }
 
 /*
