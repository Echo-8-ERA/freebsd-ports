--- include/config_directives.h.orig	2019-02-24 13:31:45 UTC
+++ include/config_directives.h
@@ -56,6 +56,7 @@ CFGFUN(disable_randr15, const char *value);
 CFGFUN(fake_outputs, const char *outputs);
 CFGFUN(force_display_urgency_hint, const long duration_ms);
 CFGFUN(focus_on_window_activation, const char *mode);
+CFGFUN(window_icon_size, const long size);
 CFGFUN(title_align, const char *alignment);
 CFGFUN(show_marks, const char *value);
 CFGFUN(hide_edge_borders, const char *borders);
