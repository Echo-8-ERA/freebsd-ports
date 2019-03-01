--- parser-specs/config.spec.orig	2019-02-24 13:32:42 UTC
+++ parser-specs/config.spec
@@ -46,6 +46,7 @@ state INITIAL:
   'force_display_urgency_hint'             -> FORCE_DISPLAY_URGENCY_HINT
   'focus_on_window_activation'             -> FOCUS_ON_WINDOW_ACTIVATION
   'title_align'                            -> TITLE_ALIGN
+  'window_icon_size'                       -> WINDOW_ICON_SIZE
   'show_marks'                             -> SHOW_MARKS
   'workspace'                              -> WORKSPACE
   'ipc_socket', 'ipc-socket'               -> IPC_SOCKET
@@ -253,6 +254,11 @@ state FORCE_DISPLAY_URGENCY_HINT:
 state TITLE_ALIGN:
   alignment = 'left', 'center', 'right'
       -> call cfg_title_align($alignment)
+      
+# window_icon_size <size>
+state WINDOW_ICON_SIZE:
+  window_icon_size = number
+      -> call cfg_window_icon_size(&window_icon_size)
 
 # show_marks
 state SHOW_MARKS:
