--- src/config_directives.c.orig	2019-02-24 14:27:57 UTC
+++ src/config_directives.c
@@ -318,6 +318,10 @@ CFGFUN(focus_on_window_activation, const char *mode) {
     DLOG("Set new focus_on_window_activation mode = %i.\n", config.focus_on_window_activation);
 }
 
+CFGFUN(window_icon_size, const long size) {
+	config.window_icon_size = size;
+}
+
 CFGFUN(title_align, const char *alignment) {
     if (strcmp(alignment, "left") == 0) {
         config.title_align = ALIGN_LEFT;
