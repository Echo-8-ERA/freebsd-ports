--- include/data.h.orig	2019-01-27 15:45:19 UTC
+++ include/data.h
@@ -483,6 +483,11 @@ struct Window {
 
     /* aspect ratio from WM_NORMAL_HINTS (MPlayer uses this for example) */
     double aspect_ratio;
+    
+    /** Window icon, as array of ARGB pixels */
+	uint32_t* icon;
+	int icon_width;
+	int icon_height;
 };
 
 /**
