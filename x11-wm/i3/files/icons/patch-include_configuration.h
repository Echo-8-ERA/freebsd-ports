--- include/configuration.h.orig	2019-02-24 13:27:00 UTC
+++ include/configuration.h
@@ -207,6 +207,9 @@ struct Config {
         ALIGN_CENTER,
         ALIGN_RIGHT
     } title_align;
+    
+    /** Window icon size */
+    int32_t window_icon_size;
 
     /** The default border style for new windows. */
     border_style_t default_border;
