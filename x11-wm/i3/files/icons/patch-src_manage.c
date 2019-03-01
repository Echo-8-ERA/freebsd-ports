--- src/manage.c.orig	2019-01-27 15:45:19 UTC
+++ src/manage.c
@@ -91,6 +91,8 @@ void manage_window(xcb_window_t window, xcb_get_window
         role_cookie, startup_id_cookie, wm_hints_cookie,
         wm_normal_hints_cookie, motif_wm_hints_cookie, wm_user_time_cookie, wm_desktop_cookie;
 
+	xcb_get_property_cookie_t wm_icon_cookie;
+
     geomc = xcb_get_geometry(conn, d);
 
     /* Check if the window is mapped (it could be not mapped when intializing and
@@ -162,6 +164,7 @@ void manage_window(xcb_window_t window, xcb_get_window
     motif_wm_hints_cookie = GET_PROPERTY(A__MOTIF_WM_HINTS, 5 * sizeof(uint64_t));
     wm_user_time_cookie = GET_PROPERTY(A__NET_WM_USER_TIME, UINT32_MAX);
     wm_desktop_cookie = GET_PROPERTY(A__NET_WM_DESKTOP, UINT32_MAX);
+    wm_icon_cookie = GET_PROPERTY(A__NET_WM_ICON, UINT32_MAX);
 
     DLOG("Managing window 0x%08x\n", window);
 
@@ -177,6 +180,7 @@ void manage_window(xcb_window_t window, xcb_get_window
     window_update_class(cwindow, xcb_get_property_reply(conn, class_cookie, NULL), true);
     window_update_name_legacy(cwindow, xcb_get_property_reply(conn, title_cookie, NULL), true);
     window_update_name(cwindow, xcb_get_property_reply(conn, utf8_title_cookie, NULL), true);
+    window_update_icon(cwindow, xcb_get_property_reply(conn, wm_icon_cookie, NULL));
     window_update_leader(cwindow, xcb_get_property_reply(conn, leader_cookie, NULL));
     window_update_transient_for(cwindow, xcb_get_property_reply(conn, transient_cookie, NULL));
     window_update_strut_partial(cwindow, xcb_get_property_reply(conn, strut_cookie, NULL));
