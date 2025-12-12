//
// Created by dingjing on 2025/12/12.
//

#include "wayland.h"

#include <scenefx/types/wlr_scene.h>

#include <wlr/backend.h>
#include <wlr/backend/drm.h>
#include <wlr/types/wlr_drm.h>
#include <wlr/backend/multi.h>
#include <wlr/types/wlr_scene.h>
#include <wayland-server-core.h>
#include <wlr/backend/wayland.h>
#include <wlr/backend/headless.h>
#include <wlr/render/allocator.h>
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_keyboard.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_viewporter.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/interfaces/wlr_pointer.h>
#include <wlr/types/wlr_drm_lease_v1.h>
#include <scenefx/types/fx/blur_data.h>
#include <wlr/types/wlr_text_input_v3.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_screencopy_v1.h>
#include <wlr/types/wlr_subcompositor.h>
#include <wlr/types/wlr_xdg_output_v1.h>
#include <wlr/interfaces/wlr_keyboard.h>
#include <wlr/types/wlr_xdg_foreign_v1.h>
#include <wlr/types/wlr_xdg_foreign_v2.h>
#include <wlr/types/wlr_idle_notify_v1.h>
#include <wlr/types/wlr_layer_shell_v1.h>
#include <wlr/types/wlr_keyboard_group.h>
#include <wlr/types/wlr_linux_dmabuf_v1.h>
#include <wlr/types/wlr_data_control_v1.h>
#include <wlr/types/wlr_idle_inhibit_v1.h>
#include <wlr/types/wlr_session_lock_v1.h>
#include <wlr/types/wlr_cursor_shape_v1.h>
#include <wlr/types/wlr_xcursor_manager.h>
#include <wlr/types/wlr_input_method_v2.h>
#include <wlr/types/wlr_export_dmabuf_v1.h>
#include <wlr/types/wlr_gamma_control_v1.h>
#include <wlr/types/wlr_server_decoration.h>
#include <wlr/types/wlr_alpha_modifier_v1.h>
#include <wlr/types/wlr_presentation_time.h>
#include <wlr/types/wlr_xdg_activation_v1.h>
#include <wlr/types/wlr_xdg_decoration_v1.h>
#include <wlr/types/wlr_tearing_control_v1.h>
#include <wlr/types/wlr_virtual_pointer_v1.h>
#include <wlr/types/wlr_virtual_keyboard_v1.h>
#include <wlr/types/wlr_fractional_scale_v1.h>
#include <wlr/types/wlr_ext_data_control_v1.h>
#include <wlr/types/wlr_relative_pointer_v1.h>
#include <wlr/types/wlr_pointer_gestures_v1.h>
#include <wlr/types/wlr_output_management_v1.h>
#include <wlr/types/wlr_linux_drm_syncobj_v1.h>
#include <wlr/types/wlr_primary_selection_v1.h>
#include <wlr/types/wlr_single_pixel_buffer_v1.h>
#include <wlr/types/wlr_pointer_constraints_v1.h>
#include <scenefx/render/fx_renderer/fx_renderer.h>
#include <wlr/types/wlr_ext_image_copy_capture_v1.h>
#include <wlr/types/wlr_output_power_management_v1.h>
#include <wlr/types/wlr_ext_image_capture_source_v1.h>
#include <wlr/types/wlr_keyboard_shortcuts_inhibit_v1.h>
#include <wlr/types/wlr_foreign_toplevel_management_v1.h>

#include "types.h"
#include "common/log.h"
#include "common/utils.h"
#include "cursor-shape-v1-protocol.h"
#include "ext-workspace-v1-protocol.h"
#include "ext-protocol/wlr_ext_workspace_v1.h"


static void _handle_gpu_reset                   (struct wl_listener* listener, void* data);
static void _handle_print_status                (struct wl_listener *listener, void *data);
static void _handle_urgent                      (struct wl_listener *listener, void *data);
static void _handle_power_mgr_set_mode          (struct wl_listener *listener, void *data);
static void _handle_tearing_new_object          (struct wl_listener *listener, void *data);
static void _handle_update_mons                 (struct wl_listener *listener, void *data);
static void _handle_create_monitor              (struct wl_listener *listener, void *data);
static void _handle_create_idle_inhibitor       (struct wl_listener *listener, void *data);
static void _handle_create_layer_surface        (struct wl_listener *listener, void *data);
static void _handle_create_notify               (struct wl_listener *listener, void *data);
static void _handle_create_popup                (struct wl_listener *listener, void *data);
static void _handle_lock_session                (struct wl_listener *listener, void *data);
static void _handle_create_decoration           (struct wl_listener *listener, void *data);
static void _handle_create_pointer_constraint   (struct wl_listener *listener, void *data);
static void _handle_axis_notify                 (struct wl_listener *listener, void *data);
static void _handle_button_press                (struct wl_listener *listener, void *data);
static void _handle_cursor_frame                (struct wl_listener *listener, void *data);
static void _handle_motion_relative             (struct wl_listener *listener, void *data);
static void _handle_motion_absolute             (struct wl_listener *listener, void *data);
static void _handle_set_cursor_shape            (struct wl_listener *listener, void *data);
static void _handle_input_device                (struct wl_listener *listener, void *data);
static void _handle_virtual_keyboard            (struct wl_listener *listener, void *data);
static void _handle_virtual_pointer             (struct wl_listener *listener, void *data);
static void _handle_request_drm_lease           (struct wl_listener *listener, void *data);
static void _handle_start_drag                  (struct wl_listener *listener, void *data);
static void _handle_request_start_drag          (struct wl_listener *listener, void *data);
static void _handle_set_psel                    (struct wl_listener *listener, void *data);
static void _handle_set_sel                     (struct wl_listener *listener, void *data);
static void _handle_set_cursor                  (struct wl_listener *listener, void *data);
static void _handle_output_mgr_apply            (struct wl_listener *listener, void *data);
static void _handle_output_mgr_test             (struct wl_listener *listener, void *data);
static void _handle_keyboard_shortcuts_inhibit_new_inhibitor (struct wl_listener *listener, void *data);

static int  _handle_hide_cursor                 (void* data);

static int key_repeat                           (void *data);
static KeyboardGroup* create_keyboard_group     (void);

static void hold_end                            (struct wl_listener *listener, void *data);
static void pinch_end                           (struct wl_listener *listener, void *data);
static void swipe_end                           (struct wl_listener *listener, void *data);
static void hold_begin                          (struct wl_listener *listener, void *data);
static void swipe_begin                         (struct wl_listener *listener, void *data);
static void pinch_begin                         (struct wl_listener *listener, void *data);
static void swipe_update                        (struct wl_listener *listener, void *data);
static void pinch_update                        (struct wl_listener *listener, void *data);

static void key_press_mod                       (struct wl_listener *listener, void *data);
static int keybinding                           (uint32_t state, bool locked, uint32_t mods, xkb_keysym_t sym, uint32_t keycode);
static void workspaces_init                     (void);
static int on_gesture                           (struct wlr_pointer_swipe_end_event *event);

static bool                                     gsCursorHidden = false;
static uint32_t                                 gsLockedMods = 0;

static uint32_t                                 gsSwipeFingers = 0;
static double                                   gsSwipeDx = 0;
static double                                   gsSwipeDy = 0;

static struct wl_display*                       gsDisplay = NULL;
static struct wl_event_loop*                    gsEventLoop = NULL;
static struct wlr_session*                      gsSession = NULL;
static struct wlr_backend*                      gsBackend = NULL;
static struct wlr_backend*                      gsHeadlessBackend = NULL;
static struct wlr_scene*                        gsScene = NULL;
static struct wlr_scene_rect*                   gsRootBG = NULL;
static struct wlr_scene_tree*                   gsDragIcon = NULL;
static struct wlr_renderer*                     gsDRW = NULL;
static struct wlr_allocator*                    gsAllocator = NULL;
static struct wlr_compositor*                   gsCompositor = NULL;
static struct wlr_xdg_activation_v1*            gsActivation = NULL;
static struct wlr_output_power_manager_v1*      gsPowerMgr = NULL;
static struct wlr_tearing_control_manager_v1*   gsTearingControl = NULL;
static struct wlr_output_layout*                gsOutputLayout = NULL;
static struct wlr_idle_notifier_v1*             gsIdleNotifier = NULL;
static struct wlr_idle_inhibit_manager_v1*      gsIdleInhibitMgr = NULL;
static struct wlr_layer_shell_v1*               gsLayerShell = NULL;
static struct wlr_xdg_shell*                    gsXdgShell = NULL;
static struct wlr_session_lock_manager_v1*      gsSessionLockMgr = NULL;
static struct wlr_scene_rect*                   gsLockedBG = NULL;
static struct wlr_xdg_decoration_manager_v1*    gsXdgDecorationMgr = NULL;
static struct wlr_pointer_constraints_v1*       gsPointerConstraints = NULL;
static struct wlr_relative_pointer_manager_v1*  gsRelativePointerMgr = NULL;
static struct wlr_cursor*                       gsCursor = NULL;
static struct wlr_xcursor_manager*              gsCursorMgr = NULL;
static struct wlr_cursor_shape_manager_v1*      gsCursorShapeMgr = NULL;
static struct wlr_virtual_keyboard_manager_v1*  gsVirtualKeyboardMgr = NULL;
static struct wlr_virtual_pointer_manager_v1*   gsVirtualPointerMgr = NULL;
static struct wlr_pointer_gestures_v1*          gsPointerGestures = NULL;
static struct wlr_foreign_toplevel_manager_v1*  gsForeignToplevelManager = NULL;

static struct wlr_seat*                         gsSeat = NULL;
static KeyboardGroup*                           gsKbGroup = NULL;
static struct wlr_output_manager_v1*            gsOutputMgr = NULL;
static struct wlr_ext_workspace_manager_v1*     gsExtManager = NULL;
static struct wlr_input_method_manager_v2*      gsInputMethodManager = NULL;
static struct wlr_text_input_manager_v3*        gsTextInputManager = NULL;
static struct wlr_keyboard_shortcuts_inhibit_manager_v1*    gsKeyboardShortcutsInhibit = NULL;
static struct wlr_scene_tree*                   gsLayers[LAYERS_NUM] = {0};
static InputMethodRelay*                        gsInputMethodRelay = NULL;
static struct wlr_drm_lease_v1_manager*         gsDrmLeaseManager = NULL;
static struct wl_event_source*                  gsHideSource = NULL;

static struct wlr_box                           gsSgeom;

static struct wl_list                           gsMonitors;
static struct wl_list                           gsClients;          // tiling order
static struct wl_list                           gsFstack;           // focus order
static struct wl_list                           gsFadeoutClients;
static struct wl_list                           gsFadeoutLayers;
static struct wl_list                           gsInputDevices;
static struct wl_list                           gsKeyboardShortcutInhibitors;

static struct wl_list                           gsWorkspaces;

static struct wl_signal                         gsPrintStatus;

// listener
static struct wl_listener                   gsGPUReset              = {.notify = _handle_gpu_reset};
static struct wl_listener                   gsPrintStatusListener   = {.notify = _handle_print_status};
static struct wl_listener                   gsRequestActivate       = {.notify = _handle_urgent};
static struct wl_listener                   gsOutputPowerMgrSetMode = {.notify = _handle_power_mgr_set_mode};
static struct wl_listener                   gsTearingNewObject      = {.notify = _handle_tearing_new_object };
static struct wl_listener                   gsLayoutChange          = {.notify = _handle_update_mons};
static struct wl_listener                   gsNewOutput             = {.notify = _handle_create_monitor};
static struct wl_listener                   gsNewIdleInhibitor      = {.notify = _handle_create_idle_inhibitor};
static struct wl_listener                   gsNewLayerSurface       = {.notify = _handle_create_layer_surface};
static struct wl_listener                   gsNewXdgToplevel        = {.notify = _handle_create_notify};
static struct wl_listener                   gsNewXdgPopup           = {.notify = _handle_create_popup};
static struct wl_listener                   gsNewSessionLock        = {.notify = _handle_lock_session};
static struct wl_listener                   gsNewXdgDecoration      = {.notify = _handle_create_decoration};
static struct wl_listener                   gsNewPointerConstraint  = {.notify = _handle_create_pointer_constraint};
static struct wl_listener                   gsCursorAxis            = {.notify = _handle_axis_notify};
static struct wl_listener                   gsCursorButton          = {.notify = _handle_button_press};
static struct wl_listener                   gsCursorFrame           = {.notify = _handle_cursor_frame};
static struct wl_listener                   gsCursorMotion          = {.notify = _handle_motion_relative};
static struct wl_listener                   gsCursorMotionAbsolute  = {.notify = _handle_motion_absolute};
static struct wl_listener                   gsRequestSetCursorShape = {.notify = _handle_set_cursor_shape};
static struct wl_listener                   gsNewInputDevice        = {.notify = _handle_input_device};
static struct wl_listener                   gsNewVirtualKeyboard    = {.notify = _handle_virtual_keyboard};
static struct wl_listener                   gsNewVirtualPointer     = {.notify = _handle_virtual_pointer};
static struct wl_listener                   gsDrmLeaseRequest       = {.notify = _handle_request_drm_lease};
static struct wl_listener                   gsStartDrag             = {.notify = _handle_start_drag};
static struct wl_listener                   gsRequestStartDrag      = {.notify = _handle_request_start_drag};
static struct wl_listener                   gsRequestSetPSel        = {.notify = _handle_set_psel};
static struct wl_listener                   gsRequestSetSel         = {.notify = _handle_set_sel};
static struct wl_listener                   gsRequestCursor         = {.notify = _handle_set_cursor};
static struct wl_listener                   gsOutputMgrApply        = {.notify = _handle_output_mgr_apply};
static struct wl_listener                   gsOutputMgrTest         = {.notify = _handle_output_mgr_test};
static struct wl_listener                   gsKeyboardShortcutsInhibitNewInhibitor = {.notify = _handle_keyboard_shortcuts_inhibit_new_inhibitor};

void wayland_set_log_level(enum wlr_log_importance level)
{
    wlr_log_init(level, NULL);
}

void wayland_quit(void)
{
    C_RETURN_IF_FAIL(gsDisplay);

    wl_display_terminate(gsDisplay);
}

bool wayland_display_init(void)
{
    int i = 0, drmFd = 0;

    C_RETURN_VAL_IF_FAIL(!gsDisplay, true);

    gsDisplay = wl_display_create();
    if (!gsDisplay) {
        DDIE("Display create error!");
        return false;
    }

    gsEventLoop = wl_display_get_event_loop(gsDisplay);
    if (!gsEventLoop) {
        DDIE("Event loop create error!");
        return false;
    }

    gsBackend = wlr_backend_autocreate(gsEventLoop, &gsSession);
    if (!gsBackend) {
        DDIE("Backend create error!");
        return false;
    }

    gsHeadlessBackend = wlr_headless_backend_create(gsEventLoop);
    if (!gsHeadlessBackend) {
        DWARN("Failed to create secondary headless backend");
        return false;
    }
    else {
        wlr_multi_backend_add(gsBackend, gsHeadlessBackend);
    }

    gsScene = wlr_scene_create();
    if (!gsScene) {
        DWARN("Failed to create scene!");
        return false;
    }

    gsRootBG = wlr_scene_rect_create(&gsScene->tree, 0, 0, gGlobalConfig.rootColor);
    for (i = 0; i < LAYERS_NUM; ++i) {
        gsLayers[i] = wlr_scene_tree_create(&gsScene->tree);
    }
    gsDragIcon = wlr_scene_tree_create(&gsScene->tree);
    wlr_scene_node_place_below(&gsDragIcon->node, &gsLayers[LAYERS_BLOCK]->node);

    gsDRW = fx_renderer_create(gsBackend);
    if (!gsDRW) {
        DDIE("Couldn't create renderer.");
        return false;
    }
    wl_signal_add(&gsDRW->events.lost, &gsGPUReset);

    if (!wlr_renderer_init_wl_shm (gsDRW, gsDisplay)) {
        DWARN("Error create shm!");
        return false;
    }

    if (wlr_renderer_get_texture_formats(gsDRW, WLR_BUFFER_CAP_DMABUF)) {
        wlr_drm_create(gsDisplay, gsDRW);
        wlr_scene_set_linux_dmabuf_v1(gsScene, wlr_linux_dmabuf_v1_create_with_renderer(gsDisplay, 5, gsDRW));
    }

    if (gGlobalConfig.syncObjEnable && (drmFd = wlr_renderer_get_drm_fd(gsDRW)) >= 0
        && gsDRW->features.timeline && gsBackend->features.timeline)
    {
        wlr_linux_drm_syncobj_manager_v1_create (gsDisplay, 1, drmFd);
    }

    if (!(gsAllocator = wlr_allocator_autocreate(gsBackend, gsDRW))) {
        DDIE("Couldn't create allocator!");
    }

    gsCompositor = wlr_compositor_create (gsDisplay, 6, gsDRW);
    wlr_export_dmabuf_manager_v1_create(gsDisplay);
    wlr_screencopy_manager_v1_create(gsDisplay);
    wlr_ext_image_copy_capture_manager_v1_create(gsDisplay, 1);
    wlr_ext_output_image_capture_source_manager_v1_create(gsDisplay, 1);
    wlr_data_control_manager_v1_create(gsDisplay);
    wlr_data_device_manager_create(gsDisplay);
    wlr_primary_selection_v1_device_manager_create(gsDisplay);
    wlr_viewporter_create(gsDisplay);
    wlr_single_pixel_buffer_manager_v1_create(gsDisplay);
    wlr_fractional_scale_manager_v1_create(gsDisplay, 1);
    wlr_presentation_create(gsDisplay, gsBackend, 2);
    wlr_subcompositor_create(gsDisplay);
    wlr_alpha_modifier_v1_create(gsDisplay);
    wlr_ext_data_control_manager_v1_create(gsDisplay, 1);

    wl_signal_init (&gsPrintStatus);
    wl_signal_add(&gsPrintStatus, &gsPrintStatusListener);

    /* Initializes the interface used to implement urgency hints */
    gsActivation = wlr_xdg_activation_v1_create(gsDisplay);
    wl_signal_add(&gsActivation->events.request_activate, &gsRequestActivate);

    wlr_scene_set_gamma_control_manager_v1(gsScene, wlr_gamma_control_manager_v1_create(gsDisplay));

    gsPowerMgr = wlr_output_power_manager_v1_create(gsDisplay);
    wl_signal_add(&gsPowerMgr->events.set_mode, &gsOutputPowerMgrSetMode);

    gsTearingControl = wlr_tearing_control_manager_v1_create(gsDisplay, 1);
    wl_signal_add(&gsTearingControl->events.new_object, &gsTearingNewObject);

    gsOutputLayout = wlr_output_layout_create(gsDisplay);
    wl_signal_add(&gsOutputLayout->events.change, &gsLayoutChange);
    wlr_xdg_output_manager_v1_create(gsDisplay, gsOutputLayout);

    wl_list_init(&gsMonitors);
    wl_signal_add(&gsBackend->events.new_output, &gsNewOutput);

    wl_list_init(&gsClients);
    wl_list_init(&gsFstack);
    wl_list_init(&gsFadeoutClients);
    wl_list_init(&gsFadeoutLayers);

    gsIdleNotifier = wlr_idle_notifier_v1_create(gsDisplay);

    gsIdleInhibitMgr = wlr_idle_inhibit_v1_create(gsDisplay);
    wl_signal_add(&gsIdleInhibitMgr->events.new_inhibitor, &gsNewIdleInhibitor);

    gsLayerShell = wlr_layer_shell_v1_create(gsDisplay, 4);
    wl_signal_add(&gsLayerShell->events.new_surface, &gsNewLayerSurface);

    gsXdgShell = wlr_xdg_shell_create(gsDisplay, 6);
    wl_signal_add(&gsXdgShell->events.new_toplevel, &gsNewXdgToplevel);
    wl_signal_add(&gsXdgShell->events.new_popup, &gsNewXdgPopup);

    gsSessionLockMgr = wlr_session_lock_manager_v1_create(gsDisplay);
    wl_signal_add(&gsSessionLockMgr->events.new_lock, &gsNewSessionLock);

    gsLockedBG = wlr_scene_rect_create(gsLayers[LAYERS_BLOCK], gsSgeom.width, gsSgeom.height, (float[4]){0.1, 0.1, 0.1, 1.0});
    wlr_scene_node_set_enabled(&gsLockedBG->node, false);

    wlr_server_decoration_manager_set_default_mode(wlr_server_decoration_manager_create(gsDisplay), WLR_SERVER_DECORATION_MANAGER_MODE_SERVER);
    gsXdgDecorationMgr = wlr_xdg_decoration_manager_v1_create(gsDisplay);
    wl_signal_add(&gsXdgDecorationMgr->events.new_toplevel_decoration, &gsNewXdgDecoration);

    gsPointerConstraints = wlr_pointer_constraints_v1_create(gsDisplay);
    wl_signal_add(&gsPointerConstraints->events.new_constraint, &gsNewPointerConstraint);

    gsRelativePointerMgr = wlr_relative_pointer_manager_v1_create(gsDisplay);

    gsCursor = wlr_cursor_create();
    wlr_cursor_attach_output_layout(gsCursor, gsOutputLayout);

    gsCursorMgr = wlr_xcursor_manager_create(gGlobalConfig.cursorTheme, gGlobalConfig.cursorSize);

    wl_signal_add(&gsCursor->events.axis,               &gsCursorAxis);
    wl_signal_add(&gsCursor->events.frame,              &gsCursorFrame);
    wl_signal_add(&gsCursor->events.motion,             &gsCursorMotion);
    wl_signal_add(&gsCursor->events.button,             &gsCursorButton);
    wl_signal_add(&gsCursor->events.motion_absolute,    &gsCursorMotionAbsolute);

    gsCursorShapeMgr = wlr_cursor_shape_manager_v1_create(gsDisplay, 1);
    wl_signal_add(&gsCursorShapeMgr->events.request_set_shape, &gsRequestSetCursorShape);
    gsHideSource = wl_event_loop_add_timer(wl_display_get_event_loop(gsDisplay), _handle_hide_cursor, gsCursor);

    wl_list_init(&gsInputDevices);
    wl_list_init(&gsKeyboardShortcutInhibitors);
    wl_signal_add(&gsBackend->events.new_input, &gsNewInputDevice);
    gsVirtualKeyboardMgr = wlr_virtual_keyboard_manager_v1_create(gsDisplay);
    wl_signal_add(&gsVirtualKeyboardMgr->events.new_virtual_keyboard, &gsNewVirtualKeyboard);
    gsVirtualPointerMgr = wlr_virtual_pointer_manager_v1_create(gsDisplay);
    wl_signal_add(&gsVirtualPointerMgr->events.new_virtual_pointer, &gsNewVirtualPointer);

    gsPointerGestures = wlr_pointer_gestures_v1_create(gsDisplay);
    LISTEN_STATIC(&gsCursor->events.swipe_begin,    swipe_begin);
    LISTEN_STATIC(&gsCursor->events.swipe_update,   swipe_update);
    LISTEN_STATIC(&gsCursor->events.swipe_end,      swipe_end);
    LISTEN_STATIC(&gsCursor->events.pinch_begin,    pinch_begin);
    LISTEN_STATIC(&gsCursor->events.pinch_update,   pinch_update);
    LISTEN_STATIC(&gsCursor->events.pinch_end,      pinch_end);
    LISTEN_STATIC(&gsCursor->events.hold_begin,     hold_begin);
    LISTEN_STATIC(&gsCursor->events.hold_end,       hold_end);

    gsSeat = wlr_seat_create(gsDisplay, "seat0");
    wl_signal_add(&gsSeat->events.start_drag,                       &gsStartDrag);
    wl_signal_add(&gsSeat->events.request_set_cursor,               &gsRequestCursor);
    wl_signal_add(&gsSeat->events.request_set_selection,            &gsRequestSetSel);
    wl_signal_add(&gsSeat->events.request_set_primary_selection,    &gsRequestSetPSel);
    wl_signal_add(&gsSeat->events.request_start_drag,               &gsRequestStartDrag);

    gsKbGroup = create_keyboard_group();
    wl_list_init(&gsKbGroup->destroy.link);

    gsKeyboardShortcutsInhibit = wlr_keyboard_shortcuts_inhibit_v1_create(gsDisplay);
    wl_signal_add(&gsKeyboardShortcutsInhibit->events.new_inhibitor, &gsKeyboardShortcutsInhibitNewInhibitor);

    gsOutputMgr = wlr_output_manager_v1_create(gsDisplay);
    wl_signal_add(&gsOutputMgr->events.apply,   &gsOutputMgrApply);
    wl_signal_add(&gsOutputMgr->events.test,    &gsOutputMgrTest);

    // blur
    wlr_scene_set_blur_data(gsScene,
        gGlobalConfig.blurParams.num_passes,
        gGlobalConfig.blurParams.radius,
        gGlobalConfig.blurParams.noise,
        gGlobalConfig.blurParams.brightness,
        gGlobalConfig.blurParams.contrast,
        gGlobalConfig.blurParams.saturation);

    /* create text_input-, and input_method-protocol relevant globals */
    gsInputMethodManager = wlr_input_method_manager_v2_create(gsDisplay);
    gsTextInputManager = wlr_text_input_manager_v3_create(gsDisplay);

    // todo://
    // gsInputMethodRelay = dwl_im_relay_create();

    gsDrmLeaseManager = wlr_drm_lease_v1_manager_create(gsDisplay, gsBackend);
    if (gsDrmLeaseManager) {
        wl_signal_add(&gsDrmLeaseManager->events.request, &gsDrmLeaseRequest);
    }
    else {
        DDEBUG("Failed to create wlr_drm_lease_device_v1.");
        DINFO("VR will not be available.");
    }

    // todo://
    // wl_global_create(gsDisplay, &zdwl_ipc_manager_v2_interface, 2, NULL, dwl_ipc_manager_bind);

    // 创建顶层管理句柄
    gsForeignToplevelManager = wlr_foreign_toplevel_manager_v1_create(gsDisplay);
    struct wlr_xdg_foreign_registry *foreignRegistry = wlr_xdg_foreign_registry_create(gsDisplay);
    wlr_xdg_foreign_v1_create(gsDisplay, foreignRegistry);
    wlr_xdg_foreign_v2_create(gsDisplay, foreignRegistry);

    // ext-workspace协议
    workspaces_init();

    return true;
}

// todo://
static void _handle_gpu_reset (struct wl_listener* listener, void* data)
{

}

static void _handle_print_status(struct wl_listener *listener, void *data)
{}

static void _handle_urgent (struct wl_listener *listener, void *data)
{

}

static void _handle_power_mgr_set_mode (struct wl_listener *listener, void *data)
{

}

static void _handle_tearing_new_object (struct wl_listener *listener, void *data)
{

}

static void _handle_update_mons (struct wl_listener *listener, void *data)
{}

static void _handle_create_monitor (struct wl_listener *listener, void *data)
{}

static void _handle_create_idle_inhibitor (struct wl_listener *listener, void *data)
{

}

static void _handle_create_layer_surface (struct wl_listener *listener, void *data)
{}

static void _handle_create_notify (struct wl_listener *listener, void *data)
{}

static void _handle_create_popup (struct wl_listener *listener, void *data)
{}

static void _handle_lock_session (struct wl_listener *listener, void *data)
{}

static void _handle_create_decoration (struct wl_listener *listener, void *data)
{}

static void _handle_create_pointer_constraint (struct wl_listener *listener, void *data)
{}

static void _handle_axis_notify (struct wl_listener *listener, void *data)
{}

static void _handle_button_press (struct wl_listener *listener, void *data)
{}

static void _handle_cursor_frame (struct wl_listener *listener, void *data)
{}

static void _handle_motion_relative (struct wl_listener *listener, void *data)
{}

static void _handle_motion_absolute (struct wl_listener *listener, void *data)
{}

static void _handle_set_cursor_shape (struct wl_listener *listener, void *data)
{}

static void _handle_input_device (struct wl_listener *listener, void *data)
{}

static void _handle_virtual_keyboard (struct wl_listener *listener, void *data)
{}

static void _handle_virtual_pointer (struct wl_listener *listener, void *data)
{}

static void _handle_request_drm_lease (struct wl_listener *listener, void *data)
{}

static void _handle_start_drag (struct wl_listener *listener, void *data)
{}

static void _handle_request_start_drag (struct wl_listener *listener, void *data)
{}

static void _handle_set_psel (struct wl_listener *listener, void *data)
{}

static void _handle_set_sel (struct wl_listener *listener, void *data)
{}

static void _handle_set_cursor (struct wl_listener *listener, void *data)
{}

static void _handle_output_mgr_apply (struct wl_listener *listener, void *data)
{}

static void _handle_output_mgr_test (struct wl_listener *listener, void *data)
{}

static void _handle_keyboard_shortcuts_inhibit_new_inhibitor (struct wl_listener *listener, void *data)
{
}

static int _handle_hide_cursor (void* data)
{
    wlr_cursor_unset_image(gsCursor);
    gsCursorHidden = true;

    return 1;
}

void swipe_begin(struct wl_listener *listener, void *data)
{
    struct wlr_pointer_swipe_begin_event* event = data;

    wlr_pointer_gestures_v1_send_swipe_begin(gsPointerGestures, gsSeat, event->time_msec, event->fingers);
}

void swipe_update(struct wl_listener *listener, void *data)
{
    struct wlr_pointer_swipe_update_event *event = data;

    gsSwipeFingers = event->fingers;
    gsSwipeDx += event->dx;
    gsSwipeDy += event->dy;

    wlr_pointer_gestures_v1_send_swipe_update(gsPointerGestures, gsSeat, event->time_msec, event->dx, event->dy);
}

void swipe_end(struct wl_listener *listener, void *data)
{
    struct wlr_pointer_swipe_end_event *event = data;
    on_gesture(event);
    gsSwipeDx = 0;
    gsSwipeDy = 0;
    wlr_pointer_gestures_v1_send_swipe_end(gsPointerGestures, gsSeat, event->time_msec, event->cancelled);
}

void pinch_begin(struct wl_listener *listener, void *data)
{
    struct wlr_pointer_pinch_begin_event *event = data;

    wlr_pointer_gestures_v1_send_pinch_begin(gsPointerGestures, gsSeat, event->time_msec, event->fingers);
}

void pinch_update(struct wl_listener *listener, void *data)
{
    struct wlr_pointer_pinch_update_event *event = data;

    wlr_pointer_gestures_v1_send_pinch_update(gsPointerGestures, gsSeat, event->time_msec, event->dx, event->dy, event->scale, event->rotation);
}

void pinch_end(struct wl_listener *listener, void *data)
{
    struct wlr_pointer_pinch_end_event *event = data;

    wlr_pointer_gestures_v1_send_pinch_end(gsPointerGestures, gsSeat, event->time_msec, event->cancelled);
}

void hold_begin(struct wl_listener *listener, void *data)
{
    struct wlr_pointer_hold_begin_event *event = data;
    wlr_pointer_gestures_v1_send_hold_begin(gsPointerGestures, gsSeat, event->time_msec, event->fingers);
}

void hold_end(struct wl_listener *listener, void *data)
{
    struct wlr_pointer_hold_end_event *event = data;

    wlr_pointer_gestures_v1_send_hold_end(gsPointerGestures, gsSeat, event->time_msec, event->cancelled);
}

static int key_repeat(void *data)
{
    int i = 0;
    KeyboardGroup *group = data;
    if (!group->nSyms || group->wlrGroup->keyboard.repeat_info.rate <= 0) {
        return 0;
    }

    wl_event_source_timer_update(group->keyRepeatSource, 1000 / group->wlrGroup->keyboard.repeat_info.rate);
    for (i = 0; i < group->nSyms; i++) {
        keybinding(WL_KEYBOARD_KEY_STATE_PRESSED, false, group->mods, group->keySyms[i], group->keyCode);
    }

    return 0;
}

static int keybinding(uint32_t state, bool locked, uint32_t mods, xkb_keysym_t sym, uint32_t keycode)
{
    int handled = 0;
#if 0
    const KeyBinding *k;
    int ji;
    int isbreak = 0;

    if (keycode == 50 || keycode == 37 || keycode == 133 || keycode == 64 ||
        keycode == 62 || keycode == 108 || keycode == 105 || keycode == 134)
        return false;

    if (is_keyboard_shortcut_inhibitor(seat->keyboard_state.focused_surface)) {
        return false;
    }

    for (ji = 0; ji < config.key_bindings_count; ji++) {
        if (config.key_bindings_count < 1)
            break;

        if (locked && config.key_bindings[ji].islockapply == false)
            continue;

        if (state == WL_KEYBOARD_KEY_STATE_RELEASED &&
            config.key_bindings[ji].isreleaseapply == false)
            continue;

        if (state == WL_KEYBOARD_KEY_STATE_PRESSED &&
            config.key_bindings[ji].isreleaseapply == true)
            continue;

        if (state != WL_KEYBOARD_KEY_STATE_PRESSED &&
            state != WL_KEYBOARD_KEY_STATE_RELEASED)
            continue;

        k = &config.key_bindings[ji];
        if ((k->iscommonmode || (k->isdefaultmode && keymode.isdefault) ||
             (strcmp(keymode.mode, k->mode) == 0)) &&
            CLEANMASK(mods) == CLEANMASK(k->mod) &&
            ((k->keysymcode.type == KEY_TYPE_SYM &&
              xkb_keysym_to_lower(sym) ==
                  xkb_keysym_to_lower(k->keysymcode.keysym)) ||
             (k->keysymcode.type == KEY_TYPE_CODE &&
              (keycode == k->keysymcode.keycode.keycode1 ||
               keycode == k->keysymcode.keycode.keycode2 ||
               keycode == k->keysymcode.keycode.keycode3))) &&
            k->func) {

            isbreak = k->func(&k->arg);
            handled = 1;

            if (isbreak)
                break;
            }
    }
#endif

    return handled;
}


static void key_press(struct wl_listener *listener, void *data)
{
#if 0
    int i = 0;
    int pass = 0;
    int nSyms = 0;
    int handled = 0;
    bool hit_global = false;
    const xkb_keysym_t* syms = NULL;
    KeyboardGroup *group = wl_container_of(listener, group, key);
    struct wlr_keyboard_key_event *event = data;
    struct wlr_surface *lastSurface = gsSeat->keyboard_state.focused_surface;
    struct wlr_xdg_surface* xdgSurface = lastSurface ? wlr_xdg_surface_try_from_wlr_surface(lastSurface) : NULL;

    /* Translate libinput keycode -> xkbcommon */
    uint32_t keycode = event->keycode + 8;
    nSyms = xkb_state_key_get_syms(group->wlrGroup->keyboard.xkb_state, keycode, &syms);
    uint32_t mods = wlr_keyboard_get_modifiers(&group->wlrGroup->keyboard);
    wlr_idle_notifier_v1_notify_activity(idle_notifier, gsSeat);

    if (gGlobalConfig.ovTabMode && !locked && group == kb_group &&
		event->state == WL_KEYBOARD_KEY_STATE_RELEASED &&
		(keycode == 133 || keycode == 37 || keycode == 64 || keycode == 50 ||
		 keycode == 134 || keycode == 105 || keycode == 108 || keycode == 62) &&
		selmon && selmon->sel) {
		if (selmon->isoverview && selmon->sel) {
			toggleoverview(&(Arg){.i = 1});
		}
	}

    for (i = 0; i < nSyms; i++) {
        handled = keybinding(event->state, locked, mods, syms[i], keycode) || handled;
    }

    if (event->state == WL_KEYBOARD_KEY_STATE_RELEASED) {
        tag_combo = false;
    }

    if (handled && group->wlrGroup->keyboard.repeat_info.delay > 0) {
        group->mods = mods;
        group->keySyms = syms;
        group->keyCode = keycode;
        group->nSyms = nSyms;
        wl_event_source_timer_update(group->keyRepeatSource, group->wlrGroup->keyboard.repeat_info.delay);
    }
    else {
        group->nSyms = 0;
        wl_event_source_timer_update(group->keyRepeatSource, 0);
    }

    if (handled) {
        return;
    }

    pass = (xdg_surface && xdg_surface->role != WLR_XDG_SURFACE_ROLE_POPUP) || !last_surface;
    if (pass && syms) {
        hit_global = keypressglobal(last_surface, &group->wlrGroup->keyboard, event, mods, syms[0], keycode);
    }
    if (hit_global) {
        return;
    }

    if (!dwl_im_keyboard_grab_forward_key(group, event)) {
        wlr_seat_set_keyboard(seat, &group->wlr_group->keyboard);
        wlr_seat_keyboard_notify_key(seat, event->time_msec, event->keycode, event->state);
    }
#endif
}


static KeyboardGroup* create_keyboard_group(void)
{
    struct xkb_keymap* keymap = NULL;
    struct xkb_context* context = NULL;
    KeyboardGroup *group = utils_calloc(1, sizeof(*group));

    group->wlrGroup = wlr_keyboard_group_create();
    group->wlrGroup->data = group;

    /* Prepare an XKB keymap and assign it to the keyboard group. */
    context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    if (!(keymap = xkb_keymap_new_from_names(context, &gGlobalConfig.xkbRules, XKB_KEYMAP_COMPILE_NO_FLAGS))) {
        DDIE("failed to compile keymap");
    }

    wlr_keyboard_set_keymap(&group->wlrGroup->keyboard, keymap);

    if (gGlobalConfig.numLockOn) {
        xkb_mod_index_t modIndex = xkb_keymap_mod_get_index(keymap, XKB_MOD_NAME_NUM);
        if (modIndex != XKB_MOD_INVALID) {
            gsLockedMods |= (uint32_t)1 << modIndex;
        }
    }

    if (gGlobalConfig.capslock) {
        xkb_mod_index_t modIndex = xkb_keymap_mod_get_index(keymap, XKB_MOD_NAME_CAPS);
        if (modIndex != XKB_MOD_INVALID) {
            gsLockedMods |= (uint32_t)1 << modIndex;
        }
    }

    if (gsLockedMods) {
        wlr_keyboard_notify_modifiers(&group->wlrGroup->keyboard, 0, 0, gsLockedMods, 0);
    }

    xkb_keymap_unref(keymap);
    xkb_context_unref(context);

    wlr_keyboard_set_repeat_info(&group->wlrGroup->keyboard, gGlobalConfig.repeatRate, gGlobalConfig.repeatDelay);

    /* Set up listeners for keyboard events */
    LISTEN(&group->wlrGroup->keyboard.events.key, &group->key, key_press);
    LISTEN(&group->wlrGroup->keyboard.events.modifiers, &group->modifiers, key_press_mod);

    group->keyRepeatSource = wl_event_loop_add_timer(gsEventLoop, key_repeat, group);

    wlr_seat_set_keyboard(gsSeat, &group->wlrGroup->keyboard);

    return group;
}

void key_press_mod(struct wl_listener *listener, void *data)
{
#if 0
    KeyboardGroup *group = wl_container_of(listener, group, modifiers);

    if (!dwl_im_keyboard_grab_forward_modifiers(group)) {

        wlr_seat_set_keyboard(seat, &group->wlr_group->keyboard);
        /* Send modifiers to the client. */
        wlr_seat_keyboard_notify_modifiers(
            seat, &group->wlr_group->keyboard.modifiers);
    }
#endif
}

static void workspaces_init()
{
    // gsExtManager = wlr_ext_workspace_manager_v1_create(gsDisplay, 1);
    wl_list_init(&gsWorkspaces);
}

static int on_gesture(struct wlr_pointer_swipe_end_event *event)
{
    return 0;
}
