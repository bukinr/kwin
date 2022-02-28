#define KWIN_PLUGIN_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"

#cmakedefine01 KWIN_BUILD_DECORATIONS
#cmakedefine01 KWIN_BUILD_SCREENLOCKER
#cmakedefine01 KWIN_BUILD_TABBOX
#cmakedefine01 KWIN_BUILD_ACTIVITIES
#cmakedefine01 KWIN_BUILD_CMS
#define KWIN_NAME "${KWIN_NAME}"
#define KWIN_INTERNAL_NAME_X11 "${KWIN_INTERNAL_NAME_X11}"
#define KWIN_CONFIG "${KWIN_NAME}rc"
#define KWIN_VERSION_STRING "${PROJECT_VERSION}"
#define XCB_VERSION_STRING "${XCB_VERSION}"
#define KWIN_KILLER_BIN "${CMAKE_INSTALL_FULL_LIBEXECDIR}/kwin_killer_helper"
#define KWIN_RULES_DIALOG_BIN "${CMAKE_INSTALL_FULL_LIBEXECDIR}/kwin_rules_dialog"
#define KWIN_XCLIPBOARD_SYNC_BIN "${CMAKE_INSTALL_FULL_LIBEXECDIR}/org_kde_kwin_xclipboard_syncer"
#cmakedefine01 HAVE_X11_XCB
#cmakedefine01 HAVE_X11_XINPUT
#cmakedefine01 HAVE_GBM_BO_GET_FD_FOR_PLANE
#cmakedefine01 HAVE_WAYLAND_EGL
#cmakedefine01 HAVE_PR_SET_DUMPABLE
#cmakedefine01 HAVE_PR_SET_PDEATHSIG
#cmakedefine01 HAVE_PROC_TRACE_CTL
#cmakedefine01 HAVE_BREEZE_DECO
#cmakedefine01 HAVE_LIBCAP
#cmakedefine01 HAVE_SCHED_RESET_ON_FORK
#cmakedefine01 HAVE_ACCESSIBILITY
#if HAVE_BREEZE_DECO
#define BREEZE_KDECORATION_PLUGIN_ID "${BREEZE_KDECORATION_PLUGIN_ID}"
#endif

#cmakedefine01 XCB_ICCCM_FOUND
#if !XCB_ICCCM_FOUND
#define XCB_ICCCM_WM_STATE_WITHDRAWN 0
#define XCB_ICCCM_WM_STATE_NORMAL 1
#define XCB_ICCCM_WM_STATE_ICONIC 3
#endif

#cmakedefine01 PipeWire_FOUND

#cmakedefine01 HAVE_XWAYLAND_LISTENFD
