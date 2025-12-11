//
// Created by dingjing on 2025/12/10.
//

#include "config.h"

#include <linux/limits.h>

#include "../common/log.h"
#include "common/utils.h"


typedef int (*FuncType)(const Arg *);


static void reset_option(void);
static int parse_button(const char *str);
static int parse_direction(const char *str);
static int parse_fold_state(const char *str);
static uint32_t parse_mod(const char *mod_str);
static int parse_mouse_action(const char *str);
static long int parse_color(const char *hexStr);
static uint32_t parse_num_type(const char *str);
static int parse_circle_direction(const char *str);
static void parse_line(Config* config, const char* lineBuffer);
static KeySymCode parse_key(const char *keyStr, bool isBindSym);
static void parse_option(Config* config, char* key, char* value);
static void convert_hex_to_rgba(float *color, unsigned long int hex);
static bool starts_with_ignore_case(const char *str, const char *prefix);
static int parse_double_array(const char *input, double *output, int max_count);
static int find_keycodes_for_keysym(struct xkb_keymap *keymap, xkb_keysym_t sym, MultiKeyCode *multiKc);
static FuncType parse_func_name(char* funcName, Arg* arg, char* argValue, char* argValue2, char* argValue3, char* argValue4, char* argValue5);


void config_reload(void)
{
    config_parse();
    reset_option();
}

void config_override(void)
{
    // 动画启用
    animations = CLAMP_INT(gConfig.animations, 0, 1);
    layer_animations = CLAMP_INT(gConfig.layerAnimations, 0, 1);

    // 标签动画方向
    tag_animation_direction = CLAMP_INT(gConfig.tagAnimationDirection, 0, 1);

    // 动画淡入淡出设置
    animation_fade_in = CLAMP_INT(gConfig.animationFadeIn, 0, 1);
    animation_fade_out = CLAMP_INT(gConfig.animationFadeOut, 0, 1);
    zoom_initial_ratio = CLAMP_FLOAT(gConfig.zoomInitialRatio, 0.1f, 1.0f);
    zoom_end_ratio = CLAMP_FLOAT(gConfig.zoomEndRatio, 0.1f, 1.0f);
    fadein_begin_opacity = CLAMP_FLOAT(gConfig.fadeinBeginOpacity, 0.0f, 1.0f);
    fadeout_begin_opacity = CLAMP_FLOAT(gConfig.fadeoutBeginOpacity, 0.0f, 1.0f);

    // 打开关闭动画类型
    animation_type_open = gConfig.animationTypeOpen;
    animation_type_close = gConfig.animationTypeClose;

    // layer打开关闭动画类型
    layer_animation_type_open = gConfig.layerAnimationTypeOpen;
    layer_animation_type_close = gConfig.layerAnimationTypeClose;

    // 动画时间限制在合理范围(1-50000ms)
    animation_duration_move     = CLAMP_INT(gConfig.animationDurationMove, 1, 50000);
    animation_duration_open     = CLAMP_INT(gConfig.animationDurationOpen, 1, 50000);
    animation_duration_tag      = CLAMP_INT(gConfig.animationDurationTag, 1, 50000);
    animation_duration_close    = CLAMP_INT(gConfig.animationDurationClose, 1, 50000);
    animation_duration_focus    = CLAMP_INT(gConfig.animationDurationFocus, 1, 50000);

    // 滚动布局设置
    scroller_default_proportion         = CLAMP_FLOAT(gConfig.scrollerDefaultProportion, 0.1f, 1.0f);
    scroller_default_proportion_single  = CLAMP_FLOAT(gConfig.scrollerDefaultProportionSingle, 0.1f, 1.0f);
    scroller_ignore_proportion_single   = CLAMP_INT(gConfig.scrollerIgnoreProportionSingle, 0, 1);
    scroller_focus_center               = CLAMP_INT(gConfig.scrollerFocusCenter, 0, 1);
    scroller_prefer_center              = CLAMP_INT(gConfig.scrollerPreferCenter, 0, 1);
    edge_scroller_pointer_focus         = CLAMP_INT(gConfig.edgeScrollerPointerFocus, 0, 1);
    scroller_structs                    = CLAMP_INT(gConfig.scrollerStructs, 0, 1000);

    // 主从布局设置
    default_mfact               = CLAMP_FLOAT(gConfig.defaultMFact, 0.1f, 0.9f);
    default_nmaster             = CLAMP_INT(gConfig.defaultNMaster, 1, 1000);
    center_master_overspread    = CLAMP_INT(gConfig.centerMasterOverspread, 0, 1);
    center_when_single_stack    = CLAMP_INT(gConfig.centerWhenSingleStack, 0, 1);
    new_is_master               = CLAMP_INT(gConfig.newIsMaster, 0, 1);

    // 概述模式设置
    hotarea_size                = CLAMP_INT(gConfig.hotAreaSize, 1, 1000);
    enable_hotarea              = CLAMP_INT(gConfig.enableHotArea, 0, 1);
    ov_tab_mode                 = CLAMP_INT(gConfig.ovTabMode, 0, 1);
    overviewgappi               = CLAMP_INT(gConfig.overViewGapPi, 0, 1000);
    overviewgappo               = CLAMP_INT(gConfig.overViewGapPo, 0, 1000);

    // 杂项设置
    xwayland_persistence        = CLAMP_INT(gConfig.xWaylandPersistence, 0, 1);
    syncobj_enable              = CLAMP_INT(gConfig.syncObjEnable, 0, 1);
    adaptive_sync               = CLAMP_INT(gConfig.adaptiveSync, 0, 1);
    allow_tearing               = CLAMP_INT(gConfig.allowTearing, 0, 2);
    allow_shortcuts_inhibit     = CLAMP_INT(gConfig.allowShortcutsInhibit, 0, 1);
    allow_lock_transparent      = CLAMP_INT(gConfig.allowLockTransparent, 0, 1);
    axis_bind_apply_timeout     = CLAMP_INT(gConfig.axisBindApplyTimeout, 0, 1000);
    focus_on_activate           = CLAMP_INT(gConfig.focusOnActivate, 0, 1);
    inhibit_regardless_of_visibility    = CLAMP_INT(gConfig.inhibitRegardlessOfVisibility, 0, 1);
    sloppyfocus                         = CLAMP_INT(gConfig.sloppyFocus, 0, 1);
    warpcursor                          = CLAMP_INT(gConfig.warpCursor, 0, 1);
    focus_cross_monitor                 = CLAMP_INT(gConfig.focusCrossMonitor, 0, 1);
    exchange_cross_monitor              = CLAMP_INT(gConfig.exchangeCrossMonitor, 0, 1);
    scratchpad_cross_monitor            = CLAMP_INT(gConfig.scratchpadCrossMonitor, 0, 1);
    focus_cross_tag                     = CLAMP_INT(gConfig.focusCrossTag, 0, 1);
    view_current_to_back                = CLAMP_INT(gConfig.viewCurrentToBack, 0, 1);
    enable_floating_snap                = CLAMP_INT(gConfig.enableFloatingSnap, 0, 1);
    snap_distance                       = CLAMP_INT(gConfig.snapDistance, 0, 99999);
    cursor_size                         = CLAMP_INT(gConfig.cursorSize, 4, 512);
    no_border_when_single               = CLAMP_INT(gConfig.noBorderWhenSingle, 0, 1);
    no_radius_when_single               = CLAMP_INT(gConfig.noRadiusWhenSingle, 0, 1);
    cursor_hide_timeout                 = CLAMP_INT(gConfig.cursorHideTimeout, 0, 36000);
    drag_tile_to_tile                   = CLAMP_INT(gConfig.dragTileToTile, 0, 1);
    single_scratchpad                   = CLAMP_INT(gConfig.singleScratchpad, 0, 1);

    // 键盘设置
    repeat_rate     = CLAMP_INT(gConfig.repeatRate, 1, 1000);
    repeat_delay    = CLAMP_INT(gConfig.repeatDelay, 1, 20000);
    numlockon       = CLAMP_INT(gConfig.numLockOn, 0, 1);

    // 触控板设置
    disable_trackpad    = CLAMP_INT(gConfig.disableTrackpad, 0, 1);
    tap_to_click        = CLAMP_INT(gConfig.tapToClick, 0, 1);
    tap_and_drag        = CLAMP_INT(gConfig.tapAndDrag, 0, 1);
    drag_lock           = CLAMP_INT(gConfig.dragLock, 0, 1);
    trackpad_natural_scrolling  = CLAMP_INT(gConfig.trackpadNaturalScrolling, 0, 1);
    disable_while_typing        = CLAMP_INT(gConfig.disableWhileTyping, 0, 1);
    left_handed                 = CLAMP_INT(gConfig.leftHanded, 0, 1);
    middle_button_emulation     = CLAMP_INT(gConfig.middleButtonEmulation, 0, 1);
    swipe_min_threshold         = CLAMP_INT(gConfig.swipeMinThreshold, 1, 1000);

    // 鼠标设置
    mouse_natural_scrolling     = CLAMP_INT(gConfig.mouseNaturalScrolling, 0, 1);
    accel_profile               = CLAMP_INT(gConfig.accelProfile, 0, 2);
    accel_speed                 = CLAMP_FLOAT(gConfig.accelSpeed, -1.0f, 1.0f);
    scroll_method       = CLAMP_INT(gConfig.scrollMethod, 0, 4);
    scroll_button       = CLAMP_INT(gConfig.scrollButton, 272, 276);
    click_method        = CLAMP_INT(gConfig.clickMethod, 0, 2);
    send_events_mode    = CLAMP_INT(gConfig.sendEventsMode, 0, 2);
    button_map          = CLAMP_INT(gConfig.buttonMap, 0, 1);

    // 外观设置
    gappih = CLAMP_INT(gConfig.gapPiH, 0, 1000);
    gappiv = CLAMP_INT(gConfig.gapPiV, 0, 1000);
    gappoh = CLAMP_INT(gConfig.gapPoH, 0, 1000);
    gappov = CLAMP_INT(gConfig.gapPoV, 0, 1000);
    scratchpad_width_ratio  = CLAMP_FLOAT(gConfig.scratchpadWidthRatio, 0.1f, 1.0f);
    scratchpad_height_ratio = CLAMP_FLOAT(gConfig.scratchpadHeightRatio, 0.1f, 1.0f);
    borderpx    = CLAMP_INT(gConfig.borderPx, 0, 200);
    smartgaps   = CLAMP_INT(gConfig.smartGaps, 0, 1);

    blur            = CLAMP_INT(gConfig.blur, 0, 1);
    blur_layer      = CLAMP_INT(gConfig.blurLayer, 0, 1);
    blur_optimized  = CLAMP_INT(gConfig.blurOptimized, 0, 1);
    border_radius   = CLAMP_INT(gConfig.borderRadius, 0, 100);
    blur_params.num_passes  = CLAMP_INT(gConfig.blurParams.num_passes, 0, 10);
    blur_params.radius      = CLAMP_INT(gConfig.blurParams.radius, 0, 100);
    blur_params.noise       = CLAMP_FLOAT(gConfig.blurParams.noise, 0, 1);
    blur_params.brightness  = CLAMP_FLOAT(gConfig.blurParams.brightness, 0, 1);
    blur_params.contrast    = CLAMP_FLOAT(gConfig.blurParams.contrast, 0, 1);
    blur_params.saturation  = CLAMP_FLOAT(gConfig.blurParams.saturation, 0, 1);
    shadows = CLAMP_INT(gConfig.shadows, 0, 1);
    shadow_only_floating = CLAMP_INT(gConfig.shadowOnlyFloating, 0, 1);
    layer_shadows       = CLAMP_INT(gConfig.layerShadows, 0, 1);
    shadows_size        = CLAMP_INT(gConfig.shadowsSize, 0, 100);
    shadows_blur        = CLAMP_INT(gConfig.shadowsBlur, 0, 100);
    shadows_position_x  = CLAMP_INT(gConfig.shadowsPositionX, -1000, 1000);
    shadows_position_y  = CLAMP_INT(gConfig.shadowsPositionY, -1000, 1000);
    focused_opacity     = CLAMP_FLOAT(gConfig.focusedOpacity, 0.0f, 1.0f);
    unfocused_opacity   = CLAMP_FLOAT(gConfig.unfocusedOpacity, 0.0f, 1.0f);
    memcpy(shadowscolor, gConfig.shadowsColor, sizeof(shadowsColor));

    // 复制颜色数组
    memcpy(rootcolor, gConfig.rootColor, sizeof(rootcolor));
    memcpy(bordercolor, gConfig.borderColor, sizeof(bordercolor));
    memcpy(focuscolor, gConfig.focusColor, sizeof(focuscolor));
    memcpy(maximizescreencolor, gConfig.maximizeScreenColor, sizeof(maximizescreencolor));
    memcpy(urgentcolor, gConfig.urgentColor, sizeof(urgentcolor));
    memcpy(scratchpadcolor, gConfig.scratchpadColor, sizeof(scratchpadcolor));
    memcpy(globalcolor, gConfig.globalColor, sizeof(globalcolor));
    memcpy(overlaycolor, gConfig.overlayColor, sizeof(overlaycolor));

    // 复制动画曲线
    memcpy(animation_curve_move, gConfig.animationCurveMove, sizeof(animation_curve_move));
    memcpy(animation_curve_open, gConfig.animationCurveOpen, sizeof(animation_curve_open));
    memcpy(animation_curve_tag, gConfig.animationCurveTag, sizeof(animation_curve_tag));
    memcpy(animation_curve_close, gConfig.animationCurveClose, sizeof(animation_curve_close));
    memcpy(animation_curve_focus, gConfig.animationCurveFocus, sizeof(animation_curve_focus));
}


void config_parse_file(Config* config, const char* filePath)
{
    FILE* file = NULL;
    bool pathOK = false;
    char lineBuffer[4096] = {0};
    char fullPath[PATH_MAX] = {0};

    if (!config || !filePath) {
        return;
    }

    if (filePath[0] == '/') {
        pathOK = true;
        snprintf(fullPath, PATH_MAX - 1, "%s", filePath);
    }
    else if (filePath[0] == '~' && filePath[1] == '/') {
    }
    else {
        // current dir
    }

    if (!pathOK) {
        DWARN("Parse configure file path error!");
        return;
    }

    file = fopen(fullPath, "r");
    if (file == NULL) {
        DWARN("Configure file open error: %s", strerror(errno));
        return;
    }

    while (fgets(lineBuffer, sizeof(lineBuffer) - 1, file) != NULL) {
        if (lineBuffer[0] == '#' || lineBuffer[0] == '\n' || lineBuffer[0] == '\0') {
            continue;
        }
        parse_line(config, lineBuffer);
        memset(lineBuffer, 0, sizeof(lineBuffer));
    }

    fclose(file);
}

static void parse_line(Config* config, const char* lineBuffer)
{
    // 4096 - 1024 = 3072
    char key[1024] = {0};
    char value[2048] = {0};

    if (sscanf(lineBuffer, "%1024[^=]=%2048[^\n]", key, value) != 2) {
        DDEBUG("Invalid line format: %s", lineBuffer);
        return;
    }

    utils_trim_whitespace(key);
    utils_trim_whitespace(value);

    parse_option(config, key, value);
}

static void parse_option(Config* config, char* key, char* value)
{
    if (strcmp(key, "keyMode") == 0) {
        snprintf(config->keyMode, sizeof(config->keyMode), "%.27s", value);
    }
    else if (strcmp(key, "animations") == 0) {
        config->animations = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "layerAnimations") == 0) {
        config->layerAnimations = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "animationTypeOpen") == 0) {
        snprintf(config->animationTypeOpen, sizeof(config->animationTypeOpen), "%.9s", value);
    }
    else if (strcmp(key, "animationTypeClose") == 0) {
        snprintf(config->animationTypeClose, sizeof(config->animationTypeClose), "%.9s", value);
    }
    else if (strcmp(key, "layerAnimationTypeOpen") == 0) {
        snprintf(config->layerAnimationTypeOpen, sizeof(config->layerAnimationTypeOpen), "%.9s", value);
    }
    else if (strcmp(key, "layerAnimationTypeClose") == 0) {
        snprintf(config->layerAnimationTypeClose, sizeof(config->layerAnimationTypeClose), "%.9s", value);
    }
    else if (strcmp(key, "animationFadeIn") == 0) {
        config->animationFadeIn = (int) strtol(value, NULL, 10);
    }
    else if (strcmp(key, "animationFadeOut") == 0) {
        config->animationFadeOut = (int) strtol(value, NULL, 10);
    }
    else if (strcmp(key, "tagAnimationDirection") == 0) {
        config->tagAnimationDirection = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "zoomInitialRatio") == 0) {
        config->zoomInitialRatio = (float) strtod (value, NULL);
    }
    else if (strcmp(key, "zoomEndRatio") == 0) {
        config->zoomEndRatio = (float) strtod (value, NULL);
    }
    else if (strcmp(key, "fadeInBeginOpacity") == 0) {
        config->fadeInBeginOpacity = (float) strtod (value, NULL);
    }
    else if (strcmp(key, "fadeoutBeginOpacity") == 0) {
        config->fadeOutBeginOpacity = (float) strtod (value, NULL);
    }
    else if (strcmp(key, "animationDurationMove") == 0) {
        config->animationDurationMove = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "animationDurationOpen") == 0) {
        config->animationDurationOpen = strtol (value, NULL, 10);
    }
    else if (strcmp(key, "animationDurationTag") == 0) {
        config->animationDurationTag = strtol (value, NULL, 10);
    }
    else if (strcmp(key, "animationDurationClose") == 0) {
        config->animationDurationClose = strtol(value, NULL, 10);
    }
    else if (strcmp(key, "animationDurationFocus") == 0) {
        config->animationDurationFocus = strtol(value, NULL, 10);
    }
    else if (strcmp(key, "animationCurveMove") == 0) {
        int num = parse_double_array(value, config->animationCurveMove, 4);
        if (num != 4) {
            DWARN("Error: Failed to parse animationCurveMove: %s", value);
        }
    }
    else if (strcmp(key, "animationCurveOpen") == 0) {
        int num = parse_double_array(value, config->animationCurveOpen, 4);
        if (num != 4) {
            DWARN("Error: Failed to parse animationCurveOpen: %s", value);
        }
    }
    else if (strcmp(key, "animationCurveTag") == 0) {
        int num = parse_double_array(value, config->animationCurveTag, 4);
        if (num != 4) {
            DWARN("Error: Failed to parse animationCurveTag: %s", value);
        }
    }
    else if (strcmp(key, "animationCurveClose") == 0) {
        int num = parse_double_array(value, config->animationCurveClose, 4);
        if (num != 4) {
            DWARN("Error: Failed to parse animation_curve_close: %s", value);
        }
    }
    else if (strcmp(key, "animationCurveFocus") == 0) {
        int num = parse_double_array(value, config->animationCurveFocus, 4);
        if (num != 4) {
            DWARN("Error: Failed to parse animation_curve_focus: %s", value);
        }
    }
    else if (strcmp(key, "scrollerStructs") == 0) {
        config->scrollerStructs = (int) strtol(value, NULL, 10);
    }
    else if (strcmp(key, "scrollerDefaultProportion") == 0) {
        config->scrollerDefaultProportion = strtof (value, NULL);
    }
    else if (strcmp(key, "scrollerDefaultProportionSingle") == 0) {
        config->scrollerDefaultProportionSingle = strtof(value, NULL);
    }
    else if (strcmp(key, "scrollerIgnoreProportionSingle") == 0) {
        config->scrollerIgnoreProportionSingle = (int) strtol(value, NULL, 10);
    }
    else if (strcmp(key, "scrollerFocusCenter") == 0) {
        config->scrollerFocusCenter = (int) strtol(value, NULL, 10);
    }
    else if (strcmp(key, "scrollerPreferCenter") == 0) {
        config->scrollerPreferCenter = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "edgeScrollerPointerFocus") == 0) {
        config->edgeScrollerPointerFocus = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "focusCrossMonitor") == 0) {
        config->focusCrossMonitor = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "exchangeCrossMonitor") == 0) {
        config->exchangeCrossMonitor = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "scratchpadCrossMonitor") == 0) {
        config->scratchpadCrossMonitor = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "focusCrossTag") == 0) {
        config->focusCrossTag = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "viewCurrentToBack") == 0) {
        config->viewCurrentToBack = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "blur") == 0) {
        config->blur = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "blurLayer") == 0) {
        config->blurLayer = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "blurOptimized") == 0) {
        config->blurOptimized = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "borderRadius") == 0) {
        config->borderRadius = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "blurParamsNumPasses") == 0) {
        config->blurParams.num_passes = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "blurParamsRadius") == 0) {
        config->blurParams.radius = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "blurParamsNoise") == 0) {
        config->blurParams.noise = strtof (value, NULL);
    }
    else if (strcmp(key, "blurParamsBrightness") == 0) {
        config->blurParams.brightness = strtof(value, NULL);
    }
    else if (strcmp(key, "blurParamsContrast") == 0) {
        config->blurParams.contrast = strtof(value, NULL);
    }
    else if (strcmp(key, "blurParamsSaturation") == 0) {
        config->blurParams.saturation = strtof(value, NULL);
    }
    else if (strcmp(key, "shadows") == 0) {
        config->shadows = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "shadowOnlyFloating") == 0) {
        config->shadowOnlyFloating = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "layerShadows") == 0) {
        config->layerShadows = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "shadowsSize") == 0) {
        config->shadowsSize = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "shadowsBlur") == 0) {
        config->shadowsBlur = strtof(value, NULL);
    }
    else if (strcmp(key, "shadowsPositionX") == 0) {
        config->shadowsPositionX = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "shadowsPositionY") == 0) {
        config->shadowsPositionY = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "singleScratchpad") == 0) {
        config->singleScratchpad = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "xWaylandPersistence") == 0) {
        config->xWaylandPersistence = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "syncObjEnable") == 0) {
        config->syncObjEnable = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "adaptiveSync") == 0) {
        config->adaptiveSync = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "allowTearing") == 0) {
        config->allowTearing = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "allowShortcutsInhibit") == 0) {
        config->allowShortcutsInhibit = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "allowLockTransparent") == 0) {
        config->allowLockTransparent = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "noBorderWhenSingle") == 0) {
        config->noBorderWhenSingle = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "noRadiusWhenSingle") == 0) {
        config->noRadiusWhenSingle = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "snapDistance") == 0) {
        config->snapDistance = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "enableFloatingSnap") == 0) {
        config->enableFloatingSnap = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "dragTileToTile") == 0) {
        config->dragTileToTile = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "swipeMinThreshold") == 0) {
        config->swipeMinThreshold = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "focusedOpacity") == 0) {
        config->focusedOpacity = strtof(value, NULL);
    }
    else if (strcmp(key, "unfocusedOpacity") == 0) {
        config->unfocusedOpacity = strtof(value, NULL);
    }
    else if (strcmp(key, "xkbRulesRules") == 0) {
        strncpy(xkbRulesRules, value, sizeof(xkbRulesRules) - 1);
        xkbRulesRules[sizeof(xkbRulesRules) - 1] = '\0';
    } else if (strcmp(key, "xkb_rules_model") == 0) {
        strncpy(xkb_rules_model, value, sizeof(xkb_rules_model) - 1);
        xkb_rules_model[sizeof(xkb_rules_model) - 1] =
            '\0'; // 确保字符串以 null 结尾
    } else if (strcmp(key, "xkb_rules_layout") == 0) {
        strncpy(xkb_rules_layout, value, sizeof(xkb_rules_layout) - 1);
        xkb_rules_layout[sizeof(xkb_rules_layout) - 1] =
            '\0'; // 确保字符串以 null 结尾
    } else if (strcmp(key, "xkb_rules_variant") == 0) {
        strncpy(xkb_rules_variant, value, sizeof(xkb_rules_variant) - 1);
        xkb_rules_variant[sizeof(xkb_rules_variant) - 1] =
            '\0'; // 确保字符串以 null 结尾
    } else if (strcmp(key, "xkb_rules_options") == 0) {
        strncpy(xkb_rules_options, value, sizeof(xkb_rules_options) - 1);
        xkb_rules_options[sizeof(xkb_rules_options) - 1] =
            '\0'; // 确保字符串以 null 结尾
    } else if (strcmp(key, "scroller_proportion_preset") == 0) {
        // 1. 统计 value 中有多少个逗号，确定需要解析的浮点数个数
        int count = 0; // 初始化为 0
        for (const char *p = value; *p; p++) {
            if (*p == ',')
                count++;
        }
        int floatCount = count + 1; // 浮点数的数量是逗号数量加 1

        // 2. 动态分配内存，存储浮点数
        config->scrollerProportionPreset = (float *)malloc(floatCount * sizeof(float));
        if (!config->scrollerProportionPreset) {
            DWARN("Error: Memory allocation failed");
            return;
        }

        // 3. 解析 value 中的浮点数
        char *valueCopy = strdup(value); // 复制 value，因为 strtok 会修改原字符串
        char *token = strtok(valueCopy, ",");
        int i = 0;
        float valueSet;
        while (token != NULL && i < floatCount) {
            if (sscanf(token, "%f", &valueSet) != 1) {
                DWARN("Error: Invalid float value in scroller_proportion_preset: %s", token);
                free(valueCopy);
                free(config->scrollerProportionPreset);
                config->scrollerProportionPreset = NULL;
                return;
            }
            // Clamp the value between 0.0 and 1.0 (or your desired range)
            config->scrollerProportionPreset[i] = CLAMP_FLOAT(value_set, 0.1f, 1.0f);
            token = strtok(NULL, ",");
            i++;
        }

        // 4. 检查解析的浮点数数量是否匹配
        if (i != floatCount) {
            DWARN("Error: Invalid scroller_proportion_preset format: %s", value);
            free(valueCopy);
            free(config->scrollerProportionPreset);  // 释放已分配的内存
            config->scrollerProportionPreset = NULL; // 防止野指针
            config->scrollerProportionPresetCount = 0;
            return;
        }
        config->scrollerProportionPresetCount = floatCount;
        free(valueCopy);
    }
    else if (strcmp(key, "circleLayout") == 0) {
        // 1. 统计 value 中有多少个逗号，确定需要解析的字符串个数
        int count = 0; // 初始化为 0
        for (const char *p = value; *p; p++) {
            if (*p == ',') {
                count++;
            }
        }
        int stringCount = count + 1; // 字符串的数量是逗号数量加 1

        // 2. 动态分配内存，存储字符串指针
        config->circleLayout = (char **)malloc(stringCount * sizeof(char *));
        memset(config->circleLayout, 0, stringCount * sizeof(char *));
        if (!config->circleLayout) {
            DWARN("Error: Memory allocation failed");
            return;
        }

        // 3. 解析 value 中的字符串
        char *valueCopy = strdup(value); // 复制 value，因为 strtok 会修改原字符串
        char *token = strtok(valueCopy, ",");
        int i = 0;
        char *cleanedToken = NULL;
        while (token != NULL && i < stringCount) {
            cleanedToken = utils_sanitize_string(token);
            config->circleLayout[i] = strdup(cleanedToken);
            if (!config->circleLayout[i]) {
                DWARN("Error: Memory allocation failed for string: %s", token);
                for (int j = 0; j < i; j++) {
                    free(config->circleLayout[j]);
                }
                free(config->circleLayout);
                free(valueCopy);
                config->circleLayout = NULL;
                config->circleLayoutCount = 0;
                return;
            }
            token = strtok(NULL, ",");
            i++;
        }

        // 4. 检查解析的字符串数量是否匹配
        if (i != stringCount) {
            DWARN("Error: Invalid circle_layout format: %s", value);
            for (int j = 0; j < i; j++) {
                free(config->circleLayout[j]);
            }
            free(config->circleLayout);
            free(valueCopy);
            config->circleLayout = NULL; // 防止野指针
            config->circleLayoutCount = 0;
            return;
        }
        config->circleLayoutCount = stringCount;

        free(valueCopy);
    }
    else if (strcmp(key, "newIsMaster") == 0) {
        config->newIsMaster = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "defaultMFact") == 0) {
        config->defaultMFact = strtof(value, NULL);
    }
    else if (strcmp(key, "defaultNMaster") == 0) {
        config->defaultNMaster = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "centerMasterOverspread") == 0) {
        config->centerMasterOverspread = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "centerWhenSingleStack") == 0) {
        config->centerWhenSingleStack = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "hotAreaSize") == 0) {
        config->hotAreaSize = (int) strtol(value, NULL, 10);
    }
    else if (strcmp(key, "enableHotArea") == 0) {
        config->enableHotArea = strtol (value, NULL, 10);
    }
    else if (strcmp(key, "ovTabMode") == 0) {
        config->ovTabMode = strtol (value, NULL, 10);
    }
    else if (strcmp(key, "overViewGapPi") == 0) {
        config->overViewGapPi = (int) strtol(value, NULL, 10);
    }
    else if (strcmp(key, "overViewGapPo") == 0) {
        config->overViewGapPo = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "cursorHideTimeout") == 0) {
        config->cursorHideTimeout = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "axisBindApplyTimeout") == 0) {
        config->axisBindApplyTimeout = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "focusOnActivate") == 0) {
        config->focusOnActivate = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "numLockOn") == 0) {
        config->numLockOn = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "inhibitRegardlessOfVisibility") == 0) {
        config->inhibitRegardlessOfVisibility = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "sloppyFocus") == 0) {
        config->sloppyFocus = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "warpCursor") == 0) {
        config->warpCursor = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "smartGaps") == 0) {
        config->smartGaps = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "repeatRate") == 0) {
        config->repeatRate = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "repeatDelay") == 0) {
        config->repeatDelay = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "disableTrackpad") == 0) {
        config->disableTrackpad = (int) strtol(value, NULL, 10);
    }
    else if (strcmp(key, "tapToClick") == 0) {
        config->tapToClick = (int) strtol(value, NULL, 10);
    }
    else if (strcmp(key, "tapAndDrag") == 0) {
        config->tapAndDrag = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "dragLock") == 0) {
        config->dragLock = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "mouseNaturalScrolling") == 0) {
        config->mouseNaturalScrolling = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "trackpadNaturalScrolling") == 0) {
        config->trackpadNaturalScrolling = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "cursorSize") == 0) {
        config->cursorSize = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "cursorTheme") == 0) {
        config->cursorTheme = strdup(value);
    }
    else if (strcmp(key, "disableWhileTyping") == 0) {
        config->disableWhileTyping = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "leftHanded") == 0) {
        config->leftHanded = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "middleButtonEmulation") == 0) {
        config->middleButtonEmulation = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "accelProfile") == 0) {
        config->accelProfile = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "accelSpeed") == 0) {
        config->accelSpeed = strtof (value, NULL);
    }
    else if (strcmp(key, "scrollMethod") == 0) {
        config->scrollMethod = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "scrollButton") == 0) {
        config->scrollButton = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "clickMethod") == 0) {
        config->clickMethod = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "sendEventsMode") == 0) {
        config->sendEventsMode = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "buttonMap") == 0) {
        config->buttonMap = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "gapPiH") == 0) {
        config->gapPiH = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "gapPiV") == 0) {
        config->gapPiV = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "gapPoH") == 0) {
        config->gapPoH = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "gapPoV") == 0) {
        config->gapPoV = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "scratchpadWidthRatio") == 0) {
        config->scratchpadWidthRatio = strtof (value, NULL);
    }
    else if (strcmp(key, "scratchpadHeightRatio") == 0) {
        config->scratchpadHeightRatio = strtof (value, NULL);
    }
    else if (strcmp(key, "borderPx") == 0) {
        config->borderPx = (int) strtol (value, NULL, 10);
    }
    else if (strcmp(key, "rootColor") == 0) {
        long int color = parse_color(value);
        if (color == -1) {
            DWARN("Error: Invalid rootColor format: %s", value);
        }
        else {
            convert_hex_to_rgba(config->rootColor, color);
        }

    }
    else if (strcmp(key, "shadowsColor") == 0) {
        long int color = parse_color(value);
        if (color == -1) {
            DWARN("Error: Invalid shadowsColor format: %s", value);
        }
        else {
            convert_hex_to_rgba(config->shadowsColor, color);
        }
    }
    else if (strcmp(key, "borderColor") == 0) {
        long int color = parse_color(value);
        if (color == -1) {
            DWARN("Error: Invalid bordercolor format: %s", value);
        }
        else {
            convert_hex_to_rgba(config->borderColor, color);
        }
    }
    else if (strcmp(key, "focusColor") == 0) {
        long int color = parse_color(value);
        if (color == -1) {
            DWARN("Error: Invalid focuscolor format: %s", value);
        }
        else {
            convert_hex_to_rgba(config->focusColor, color);
        }
    }
    else if (strcmp(key, "maximizeScreenColor") == 0) {
        long int color = parse_color(value);
        if (color == -1) {
            fprintf(stderr, "Error: Invalid maximizeScreenColor format: %s", value);
        }
        else {
            convert_hex_to_rgba(config->maximizeScreenColor, color);
        }
    }
    else if (strcmp(key, "urgentColor") == 0) {
        long int color = parse_color(value);
        if (color == -1) {
            fprintf(stderr, "Error: Invalid urgentColor format: %s", value);
        }
        else {
            convert_hex_to_rgba(config->urgentColor, color);
        }
    }
    else if (strcmp(key, "scratchpadColor") == 0) {
        long int color = parse_color(value);
        if (color == -1) {
            DWARN("Error: Invalid scratchpadColor format: %s", value);
        }
        else {
            convert_hex_to_rgba(config->scratchpadColor, color);
        }
    }
    else if (strcmp(key, "globalColor") == 0) {
        long int color = parse_color(value);
        if (color == -1) {
            fprintf(stderr, "Error: Invalid globalColor format: %s", value);
        }
        else {
            convert_hex_to_rgba(config->globalColor, color);
        }
    }
    else if (strcmp(key, "overlayColor") == 0) {
        long int color = parse_color(value);
        if (color == -1) {
            fprintf(stderr, "Error: Invalid overlayColor format: %s\n", value);
        }
        else {
            convert_hex_to_rgba(config->overlayColor, color);
        }
    }
    else if (strcmp(key, "tagRule") == 0) {
        config->tagRules = realloc(config->tagRules, (config->tagRulesCount + 1) * sizeof(ConfigTagRule));
        if (!config->tagRules) {
            fprintf(stderr, "Error: Failed to allocate memory for tag rules\n");
            return;
        }

        ConfigTagRule *rule = &config->tagRules[config->tagRulesCount];
        memset(rule, 0, sizeof(ConfigTagRule));

        rule->id = 0;
        rule->layoutName = NULL;
        rule->monitorName = NULL;

        char *token = strtok(value, ",");
        while (token != NULL) {
            char *colon = strchr(token, ':');
            if (colon != NULL) {
                *colon = '\0';
                char *keyT = token;
                char *val = colon + 1;

                utils_trim_whitespace(keyT);
                utils_trim_whitespace(val);

                if (strcmp(keyT, "id") == 0) {
                    rule->id = CLAMP_INT(strtol(val, NULL, 10), 0, LENGTH(tags));
                }
                else if (strcmp(keyT, "layoutName") == 0) {
                    rule->layoutName = strdup(val);
                }
                else if (strcmp(keyT, "monitorName") == 0) {
                    rule->monitorName = strdup(val);
                }
                else if (strcmp(keyT, "noRenderBorder") == 0) {
                    rule->noRenderBorder = CLAMP_INT(strtol(val, NULL, 10), 0, 1);
                }
                else if (strcmp(keyT, "noHide") == 0) {
                    rule->noHide = CLAMP_INT(strtol(val, NULL, 10), 0, 1);
                }
            }
            token = strtok(NULL, ",");
        }

        config->tagRulesCount++;
    }
    else if (strcmp(key, "layerRule") == 0) {
        ConfigLayerRule* tmp = config->layerRules;
        config->layerRules = (ConfigLayerRule*) realloc (tmp, (config->layerRulesCount + 1) * sizeof(ConfigLayerRule));
        if (!config->layerRules) {
            free(tmp);
            DWARN("Error: Failed to allocate memory for layer rules");
            return;
        }
        ConfigLayerRule *rule = &config->layerRules[config->layerRulesCount];
        memset(rule, 0, sizeof(ConfigLayerRule));
        rule->layerName = NULL;
        rule->animationTypeOpen = NULL;
        rule->animationTypeClose = NULL;
        rule->noBlur = 0;
        rule->noAnim = 0;
        rule->noShadow = 0;

        char *token = strtok(value, ",");
        while (token != NULL) {
            char *colon = strchr(token, ':');
            if (colon != NULL) {
                *colon = '\0';
                char *key1 = token;
                char *val = colon + 1;

                utils_trim_whitespace(key1);
                utils_trim_whitespace(val);

                if (strcmp(key1, "layerName") == 0) {
                    rule->layerName = strdup(val);
                } else if (strcmp(key1, "animationTypeOpen") == 0) {
                    rule->animationTypeOpen = strdup(val);
                } else if (strcmp(key1, "animationTypeClose") == 0) {
                    rule->animationTypeClose = strdup(val);
                } else if (strcmp(key1, "noBlur") == 0) {
                    rule->noBlur = CLAMP_INT(strtol(val, NULL, 10), 0, 1);
                } else if (strcmp(key1, "noAnim") == 0) {
                    rule->noAnim = CLAMP_INT(strtol(val, NULL, 10), 0, 1);
                } else if (strcmp(key1, "noShadow") == 0) {
                    rule->noShadow = CLAMP_INT(strtol(val, NULL, 10), 0, 1);
                }
            }
            token = strtok(NULL, ",");
        }
        if (rule->layerName == NULL) {
            rule->layerName = strdup("default");
        }
        config->layerRulesCount++;
    }
    else if (strcmp(key, "windowRule") == 0) {
        ConfigWinRule* tmp = config->windowRules;
        config->windowRules = realloc(tmp, (config->windowRulesCount + 1) * sizeof(ConfigWinRule));
        if (!config->windowRules) {
            free(tmp);
            DWARN("Error: Failed to allocate memory for window rules");
            return;
        }
        ConfigWinRule *rule = &config->windowRules[config->windowRulesCount];
        memset(rule, 0, sizeof(ConfigWinRule));
        rule->isFloating = -1;
        rule->isFullScreen = -1;
        rule->isNoBorder = -1;
        rule->isNoShadow = -1;
        rule->isNoAnimation = -1;
        rule->isOpenSilent = -1;
        rule->isTagsSilent = -1;
        rule->isNamedScratchpad = -1;
        rule->isUnglobal = -1;
        rule->isglobal = -1;
        rule->isOverlay = -1;
        rule->allowShortcutsInhibit = -1;
        rule->ignoreMaximize = -1;
        rule->ignoreMinimize = -1;
        rule->isNoSizeHint = -1;
        rule->isTerm = -1;
        rule->allowCSD = -1;
        rule->forceMaximize = -1;
        rule->forceTearing = -1;
        rule->noSwallow = -1;
        rule->noBlur = -1;
        rule->noFocus = -1;
        rule->noFadein = -1;
        rule->noFadeout = -1;
        rule->noForceCenter = -1;

        // string rule value, relay to a client property
        rule->animationTypeOpen = NULL;
        rule->animationTypeClose = NULL;

        // float rule value, relay to a client property
        rule->focusedOpacity = 0;
        rule->unfocusedOpacity = 0;
        rule->scrollerProportionSingle = 0.0f;
        rule->scrollerProportion = 0;

        // special rule value,not directly set to client property
        rule->tags = 0;
        rule->offsetX = 0;
        rule->offsetY = 0;
        rule->width = 0;
        rule->height = 0;
        rule->monitor = NULL;
        rule->id = NULL;
        rule->title = NULL;

        rule->globalKeyBinding = (KeyBinding){0};

        char *token = strtok(value, ",");
        while (token != NULL) {
            char *colon = strchr(token, ':');
            if (colon != NULL) {
                *colon = '\0';
                char *key = token;
                char *val = colon + 1;

                utils_trim_whitespace(key);
                utils_trim_whitespace(val);

                if (strcmp(key, "isFloating") == 0) {
                    rule->isFloating = atoi(val);
                } else if (strcmp(key, "title") == 0) {
                    rule->title = strdup(val);
                } else if (strcmp(key, "appid") == 0) {
                    rule->id = strdup(val);
                } else if (strcmp(key, "animationTypeOpen") == 0) {
                    rule->animationTypeOpen = strdup(val);
                } else if (strcmp(key, "animationTypeClose") == 0) {
                    rule->animationTypeClose = strdup(val);
                } else if (strcmp(key, "tags") == 0) {
                    rule->tags = 1 << (strtol(val, NULL, 10) - 1);
                } else if (strcmp(key, "monitor") == 0) {
                    rule->monitor = strdup(val);
                } else if (strcmp(key, "offsetX") == 0) {
                    rule->offsetX = (int) strtol(val, NULL, 10);
                } else if (strcmp(key, "offsetY") == 0) {
                    rule->offsetY = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "noFocus") == 0) {
                    rule->noFocus = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "noFadein") == 0) {
                    rule->noFadein = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "noFadeout") == 0) {
                    rule->noFadeout = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "noForceCenter") == 0) {
                    rule->noForceCenter = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "width") == 0) {
                    rule->width = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "height") == 0) {
                    rule->height = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "isNoBorder") == 0) {
                    rule->isNoBorder = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "isNoShadow") == 0) {
                    rule->isNoShadow = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "isNoAnimation") == 0) {
                    rule->isNoAnimation = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "isOpenSilent") == 0) {
                    rule->isOpenSilent = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "isTagSilent") == 0) {
                    rule->isTagSilent = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "isNamedScratchpad") == 0) {
                    rule->isNamedScratchpad = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "isUnglobal") == 0) {
                    rule->isUnglobal = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "isGlobal") == 0) {
                    rule->isGlobal = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "scrollerProportionSingle") == 0) {
                    rule->scrollerProportionSingle = strtof (val, NULL);
                } else if (strcmp(key, "unfocusedOpacity") == 0) {
                    rule->unfocusedOpacity = strtof(val, NULL);
                } else if (strcmp(key, "focusedOpacity") == 0) {
                    rule->focusedOpacity = strtof (val, NULL);
                } else if (strcmp(key, "isOverlay") == 0) {
                    rule->isOverlay = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "allowShortcutsInhibit") == 0) {
                    rule->allowShortcutsInhibit = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "ignoreMaximize") == 0) {
                    rule->ignoreMaximize = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "ignoreMinimize") == 0) {
                    rule->ignoreMinimize = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "isNoSizeHint") == 0) {
                    rule->isNoSizeHint = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "isTerm") == 0) {
                    rule->isTerm = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "allowCSD") == 0) {
                    rule->allowCSD = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "forceMaximize") == 0) {
                    rule->forceMaximize = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "forceTearing") == 0) {
                    rule->forceTearing = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "noSwallow") == 0) {
                    rule->noSwallow = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "noBlur") == 0) {
                    rule->noBlur = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "scrollerProportion") == 0) {
                    rule->scrollerProportion = strtof (val, NULL);
                } else if (strcmp(key, "isFullScreen") == 0) {
                    rule->isFullScreen = (int) strtol (val, NULL, 10);
                } else if (strcmp(key, "globalKeyBinding") == 0) {
                    char modStr[256], keysymStr[256];
                    sscanf(val, "%255[^-]-%255[a-zA-Z]", modStr, keysymStr);
                    utils_trim_whitespace(modStr);
                    utils_trim_whitespace(keysymStr);
                    rule->globalKeyBinding.mod = parse_mod(modStr);
                    rule->globalKeyBinding.keySymCode = parse_key(keysymStr, false);
                }
            }
            token = strtok(NULL, ",");
        }
        config->windowRulesCount++;
    }
    else if (strcmp(key, "monitorRule") == 0) {
        ConfigMonitorRule* tmp = config->monitorRules;
        config->monitorRules = realloc(tmp, (config->monitorRulesCount + 1) * sizeof(ConfigMonitorRule));
        if (!config->monitorRules) {
            free(tmp);
            DWARN("Error: Failed to allocate memory for monitor rules");
            return;
        }
        ConfigMonitorRule *rule = &config->monitorRules[config->monitorRulesCount];
        memset(rule, 0, sizeof(ConfigMonitorRule));
        char rawName[256], rawLayout[256];
        char rawMFact[256], rawNMaster[256], rawRr[256];
        char rawScale[256], rawX[256], rawY[256], rawWidth[256], rawHeight[256], rawRefresh[256];

        int parsed = sscanf(value,
                   "%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255["
                   "^,],%255[^,],%255[^,],%255[^,],%255[^,],%255s",
                   rawName, rawMFact, rawNMaster, rawLayout, rawRr,
                   rawScale, rawX, rawY, rawWidth, rawHeight, rawRefresh);

        if (parsed == 11) {
            // 修剪每个字段的空格
            utils_trim_whitespace(rawName);
            utils_trim_whitespace(rawMFact);
            utils_trim_whitespace(rawNMaster);
            utils_trim_whitespace(rawLayout);
            utils_trim_whitespace(rawRr);
            utils_trim_whitespace(rawScale);
            utils_trim_whitespace(rawX);
            utils_trim_whitespace(rawY);
            utils_trim_whitespace(rawWidth);
            utils_trim_whitespace(rawHeight);
            utils_trim_whitespace(rawRefresh);

            // 转换修剪后的字符串为特定类型
            rule->name = strdup(rawName);
            rule->layout = strdup(rawLayout);
            rule->mFact = strtof(rawMFact, NULL);
            rule->nMaster = (int) strtol (rawNMaster, NULL, 10);
            rule->rr = (int) strtol (rawRr, NULL, 10);
            rule->scale = strtof(rawScale, NULL);
            rule->x = (int) strtol(rawX, NULL, 10);
            rule->y = (int) strtol(rawY, NULL, 10);
            rule->width = (int) strtol (rawWidth, NULL, 10);
            rule->height = (int) strtol (rawHeight, NULL, 10);
            rule->refresh = strtof (rawRefresh, NULL);
            if (!rule->name || !rule->layout) {
                if (rule->name) {
                    free((void *)rule->name);
                }
                if (rule->layout) {
                    free((void *)rule->layout);
                }
                DWARN("Error: Failed to allocate memory for monitor rule");
                return;
            }
            config->monitorRulesCount++;
        }
        else {
            DWARN("Error: Invalid monitorRule format: %s", value);
        }
    }
    else if (strncmp(key, "env", 3) == 0) {
        char env_type[256], env_value[256];
        if (sscanf(value, "%255[^,],%255[^\n]", env_type, env_value) < 2) {
            DWARN("Error: Invalid bind format: %s", value);
            return;
        }
        utils_trim_whitespace(env_type);
        utils_trim_whitespace(env_value);
        ConfigEnv *env = calloc(1, sizeof(ConfigEnv));
        env->type = strdup(env_type);
        env->value = strdup(env_value);
        ConfigEnv** tmp = config->env;
        config->env = realloc(tmp, (config->envCount + 1) * sizeof(ConfigEnv));
        if (!config->env) {
            free(env->type);
            free(env->value);
            free(env);
            free(tmp);
            DWARN("Error: Failed to allocate memory for env");
            return;
        }
        config->env[config->envCount] = env;
        config->envCount++;
    }
    else if (strncmp(key, "exec", 9) == 0) {
        char **newExec = realloc(config->exec, (config->execCount + 1) * sizeof(char *));
        if (!newExec) {
            DWARN("Error: Failed to allocate memory for exec");
            return;
        }
        config->exec = newExec;
        config->exec[config->execCount] = strdup(value);
        if (!config->exec[config->execCount]) {
            DWARN("Error: Failed to duplicate exec string");
            return;
        }
        config->execCount++;
    }
    else if (strncmp(key, "exec-once", 9) == 0) {
        char **newExecOnce = realloc(config->execOnce, (config->execOnceCount + 1) * sizeof(char *));
        if (!newExecOnce) {
            DWARN("Error: Failed to allocate memory for exec_once");
            return;
        }
        config->execOnce = newExecOnce;

        config->execOnce[config->execOnceCount] = strdup(value);
        if (!config->execOnce[config->execOnceCount]) {
            DWARN("Error: Failed to duplicate exec_once string");
            return;
        }
        config->execOnceCount++;
    }
    else if (utils_regex_match("^bind[s|l|r]*$", key)) {
        KeyBinding* tmp = config->keyBindings;
        config->keyBindings = realloc(tmp, (config->keyBindingsCount + 1) * sizeof(KeyBinding));
        if (!config->keyBindings) {
            free(tmp);
            DWARN("Error: Failed to allocate memory for key bindings");
            return;
        }
        KeyBinding *binding = &config->keyBindings[config->keyBindingsCount];
        memset(binding, 0, sizeof(KeyBinding));
        char modStr[256], keysymStr[256], funcName[256],
            argValue[256] = "0\0", argValue2[256] = "0\0",
            argValue3[256] = "0\0", argValue4[256] = "0\0",
            argValue5[256] = "0\0";
        if (sscanf(value,
                   "%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255["
                   "^,],%255[^\n]",
                   modStr, keysymStr, funcName, argValue, argValue2,
                   argValue3, argValue4, argValue5) < 3) {
            DWARN("Error: Invalid bind format: %s", value);
            return;
        }
        utils_trim_whitespace(modStr);
        utils_trim_whitespace(keysymStr);
        utils_trim_whitespace(funcName);
        utils_trim_whitespace(argValue);
        utils_trim_whitespace(argValue2);
        utils_trim_whitespace(argValue3);
        utils_trim_whitespace(argValue4);
        utils_trim_whitespace(argValue5);
        strcpy(binding->mode, config->keyMode);
        if (strcmp(binding->mode, "common") == 0) {
            binding->isCommonMode = true;
            binding->isDefaultMode = false;
        } else if (strcmp(binding->mode, "default") == 0) {
            binding->isDefaultMode = true;
            binding->isCommonMode = false;
        } else {
            binding->isDefaultMode = false;
            binding->isCommonMode = false;
        }

        parse_bind_flags(key, binding);
        binding->keySymCode = parse_key(keysymStr, binding->keySymCode.type == KEY_TYPE_SYM);
        binding->mod = parse_mod(modStr);
        binding->arg.v = NULL;
        binding->arg.v2 = NULL;
        binding->arg.v3 = NULL;
        binding->func = parse_func_name(funcName, &binding->arg, argValue, argValue2, argValue3, argValue4, argValue5);
        if (!binding->func) {
            if (binding->arg.v) {
                free(binding->arg.v);
                binding->arg.v = NULL;
            }
            if (binding->arg.v2) {
                free(binding->arg.v2);
                binding->arg.v2 = NULL;
            }
            if (binding->arg.v3) {
                free(binding->arg.v3);
                binding->arg.v3 = NULL;
            }
            DWARN("Error: Unknown function in bind: %s", funcName);
        }
        else {
            config->keyBindingsCount++;
        }
    }
    else if (strncmp(key, "mouseBind", 9) == 0) {
        MouseBinding* tmp = config->mouseBindings;
        config->mouseBindings = realloc(tmp, (config->mouseBindingsCount + 1) * sizeof(MouseBinding));
        if (!config->mouseBindings) {
            free(tmp);
            DWARN("Error: Failed to allocate memory for mouse bindings");
            return;
        }
        MouseBinding *binding = &config->mouseBindings[config->mouseBindingsCount];
        memset(binding, 0, sizeof(MouseBinding));
        char modStr[256], buttonStr[256], funcName[256],
            argValue[256] = "0\0", argValue2[256] = "0\0",
            argValue3[256] = "0\0", argValue4[256] = "0\0",
            argValue5[256] = "0\0";
        if (sscanf(value,
                   "%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255["
                   "^,],%255[^\n]",
                   modStr, buttonStr, funcName, argValue, argValue2,
                   argValue3, argValue4, argValue5) < 3) {
            DWARN("Error: Invalid mouseBind format: %s", value);
            return;
        }
        utils_trim_whitespace(modStr);
        utils_trim_whitespace(buttonStr);
        utils_trim_whitespace(funcName);
        utils_trim_whitespace(argValue);
        utils_trim_whitespace(argValue2);
        utils_trim_whitespace(argValue3);
        utils_trim_whitespace(argValue4);
        utils_trim_whitespace(argValue5);

        binding->mod = parse_mod(modStr);
        binding->button = parse_button(buttonStr);
        binding->arg.v = NULL;
        binding->arg.v2 = NULL;
        binding->arg.v3 = NULL;
        binding->func = parse_func_name(funcName, &binding->arg, argValue, argValue2, argValue3, argValue4, argValue5);
        if (!binding->func) {
            if (binding->arg.v) {
                free(binding->arg.v);
                binding->arg.v = NULL;
            }
            if (binding->arg.v2) {
                free(binding->arg.v2);
                binding->arg.v2 = NULL;
            }
            if (binding->arg.v3) {
                free(binding->arg.v3);
                binding->arg.v3 = NULL;
            }
            DWARN("Error: Unknown function in mouseBind: %s", funcName);
        } else {
            config->mouseBindingsCount++;
        }
    } else if (strncmp(key, "axisBind", 8) == 0) {
        AxisBinding* tmp = config->axisBindings;
        config->axisBindings = realloc(tmp, (config->axisBindingsCount + 1) * sizeof(AxisBinding));
        if (!config->axisBindings) {
            free(tmp);
            DWARN("Error: Failed to allocate memory for axis bindings");
            return;
        }
        AxisBinding *binding = &config->axisBindings[config->axisBindingsCount];
        memset(binding, 0, sizeof(AxisBinding));
        char modStr[256], dirStr[256], funcName[256],
            argValue[256] = "0\0", argValue2[256] = "0\0",
            argValue3[256] = "0\0", argValue4[256] = "0\0",
            argValue5[256] = "0\0";
        if (sscanf(value,
                   "%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255["
                   "^,],%255[^\n]",
                   modStr, dirStr, funcName, argValue, argValue2,
                   argValue3, argValue4, argValue5) < 3) {
            fprintf(stderr, "Error: Invalid axisBind format: %s\n", value);
            return;
        }
        utils_trim_whitespace(modStr);
        utils_trim_whitespace(dirStr);
        utils_trim_whitespace(funcName);
        utils_trim_whitespace(argValue);
        utils_trim_whitespace(argValue2);
        utils_trim_whitespace(argValue3);
        utils_trim_whitespace(argValue4);
        utils_trim_whitespace(argValue5);
        binding->mod = parse_mod(modStr);
        binding->dir = parse_direction(dirStr);
        binding->arg.v = NULL;
        binding->arg.v2 = NULL;
        binding->arg.v3 = NULL;
        binding->func = parse_func_name(funcName, &binding->arg, argValue, argValue2, argValue3, argValue4, argValue5);
        if (!binding->func) {
            if (binding->arg.v) {
                free(binding->arg.v);
                binding->arg.v = NULL;
            }
            if (binding->arg.v2) {
                free(binding->arg.v2);
                binding->arg.v2 = NULL;
            }
            if (binding->arg.v3) {
                free(binding->arg.v3);
                binding->arg.v3 = NULL;
            }
            DWARN("Error: Unknown function in axisBind: %s", funcName);
        } else {
            config->axisBindingsCount++;
        }
    } else if (strncmp(key, "switchBind", 10) == 0) {
        SwitchBinding* tmp = config->switchBindings;
        config->switchBindings = realloc(tmp, (config->switchBindingsCount + 1) * sizeof(SwitchBinding));
        if (!config->switchBindings) {
            free(tmp);
            DWARN("Error: Failed to allocate memory for switch bindings");
            return;
        }
        SwitchBinding *binding = &config->switchBindings[config->switchBindingsCount];
        memset(binding, 0, sizeof(SwitchBinding));
        char foldStr[256], funcName[256],
            argValue[256] = "0\0", argValue2[256] = "0\0",
            argValue3[256] = "0\0", argValue4[256] = "0\0",
            argValue5[256] = "0\0";
        if (sscanf(value,
                   "%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255["
                   "^\n]",
                   foldStr, funcName, argValue, argValue2, argValue3,
                   argValue4, argValue5) < 3) {
            DWARN("Error: Invalid switchBind format: %s", value);
            return;
        }
        utils_trim_whitespace(foldStr);
        utils_trim_whitespace(funcName);
        utils_trim_whitespace(argValue);
        utils_trim_whitespace(argValue2);
        utils_trim_whitespace(argValue3);
        utils_trim_whitespace(argValue4);
        utils_trim_whitespace(argValue5);
        binding->fold = parse_fold_state(foldStr);
        binding->func = parse_func_name(funcName, &binding->arg, argValue, argValue2, argValue3, argValue4, argValue5);
        if (!binding->func) {
            if (binding->arg.v) {
                free(binding->arg.v);
                binding->arg.v = NULL;
            }
            if (binding->arg.v2) {
                free(binding->arg.v2);
                binding->arg.v2 = NULL;
            }
            if (binding->arg.v3) {
                free(binding->arg.v3);
                binding->arg.v3 = NULL;
            }
            DWARN("Error: Unknown function in switchBind: %s", funcName);
        } else {
            config->switchBindingsCount++;
        }
    }
    else if (strncmp(key, "gestureBind", 11) == 0) {
        GestureBinding* tmp = config->gestureBindings;
        config->gestureBindings = realloc(tmp, (config->gestureBindingsCount + 1) * sizeof(GestureBinding));
        if (!config->gestureBindings) {
            free(tmp);
            DWARN("Error: Failed to allocate memory for axis gestureBind");
            return;
        }
        GestureBinding *binding = &config->gestureBindings[config->gestureBindingsCount];
        memset(binding, 0, sizeof(GestureBinding));
        char modStr[256], motionStr[256], fingersCountStr[256],
            funcName[256], argValue[256] = "0\0", argValue2[256] = "0\0",
                            argValue3[256] = "0\0", argValue4[256] = "0\0",
                            argValue5[256] = "0\0";
        if (sscanf(value,
                   "%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255["
                   "^,],%255[^,],%255[^\n]",
                   modStr, motionStr, fingersCountStr, funcName, argValue,
                   argValue2, argValue3, argValue4, argValue5) < 4) {
            DWARN("Error: Invalid gestureBind format: %s", value);
            return;
        }
        utils_trim_whitespace(modStr);
        utils_trim_whitespace(motionStr);
        utils_trim_whitespace(fingersCountStr);
        utils_trim_whitespace(funcName);
        utils_trim_whitespace(argValue);
        utils_trim_whitespace(argValue2);
        utils_trim_whitespace(argValue3);
        utils_trim_whitespace(argValue4);
        utils_trim_whitespace(argValue5);

        binding->mod = parse_mod(modStr);
        binding->motion = parse_direction(motionStr);
        binding->fingersCount = (int) strtol (fingersCountStr, NULL, 10);
        binding->arg.v = NULL;
        binding->arg.v2 = NULL;
        binding->arg.v3 = NULL;
        binding->func = parse_func_name(funcName, &binding->arg, argValue, argValue2, argValue3, argValue4, argValue5);
        if (!binding->func) {
            if (binding->arg.v) {
                free(binding->arg.v);
                binding->arg.v = NULL;
            }
            if (binding->arg.v2) {
                free(binding->arg.v2);
                binding->arg.v2 = NULL;
            }
            if (binding->arg.v3) {
                free(binding->arg.v3);
                binding->arg.v3 = NULL;
            }
            DWARN("Error: Unknown function in axisBind: %s", funcName);
        }
        else {
            config->gestureBindingsCount++;
        }
    }
    else if (strncmp(key, "source", 6) == 0) {
        config_parse_file(config, value);
    }
    else {
        DWARN("Error: Unknown key: %s", key);
    }
}

static int parse_double_array(const char *input, double *output, int max_count)
{
    char *dup = strdup(input);
    char *token = NULL;
    int count = 0;

    memset(output, 0, max_count * sizeof(double));

    token = strtok(dup, ",");
    while (token != NULL && count < max_count) {
        utils_trim_whitespace(token);
        char *endPtr = NULL;
        double val = strtod(token, &endPtr);
        if (endPtr == token || *endPtr != '\0') {
            DWARN("Error: Invalid number in array: %s", token);
            free(dup);
            return -1;
        }
        output[count] = val;
        count++;
        token = strtok(NULL, ",");
    }
    free(dup);
    return count;
}

static int parse_circle_direction(const char *str)
{
    char lowerStr[10];
    int i = 0;
    while (str[i] && i < 9) {
        lowerStr[i] = (char) tolower(str[i]);
        i++;
    }
    lowerStr[i] = '\0';

    if (strcmp(lowerStr, "next") == 0) {
        return NEXT;
    }
    else {
        return PREV;
    }
}

static int parse_fold_state(const char *str)
{
    // 将输入字符串转换为小写
    char lowerStr[10];
    int i = 0;
    while (str[i] && i < 9) {
        lowerStr[i] = (char) tolower(str[i]);
        i++;
    }
    lowerStr[i] = '\0';

    // 根据转换后的小写字符串返回对应的枚举值
    if (strcmp(lowerStr, "fold") == 0) {
        return FOLD;
    } else if (strcmp(lowerStr, "unfold") == 0) {
        return UNFOLD;
    } else {
        return INVALIDFOLD;
    }
}

static int parse_direction(const char *str)
{
    char lowerStr[10];
    int i = 0;
    while (str[i] && i < 9) {
        lowerStr[i] = tolower(str[i]);
        i++;
    }
    lowerStr[i] = '\0';

    // 根据转换后的小写字符串返回对应的枚举值
    if (strcmp(lowerStr, "up") == 0) {
        return UP;
    }
    else if (strcmp(lowerStr, "down") == 0) {
        return DOWN;
    }
    else if (strcmp(lowerStr, "left") == 0) {
        return LEFT;
    }
    else if (strcmp(lowerStr, "right") == 0) {
        return RIGHT;
    }
    else {
        return UNDIR;
    }
}

static long int parse_color(const char *hexStr)
{
    char *endPtr = NULL;
    long int hexNum = strtol(hexStr, &endPtr, 16);
    if (*endPtr != '\0') {
        return -1;
    }

    return hexNum;
}

static bool starts_with_ignore_case(const char *str, const char *prefix)
{
    while (*prefix) {
        if (tolower(*str) != tolower(*prefix)) {
            return false;
        }
        str++;
        prefix++;
    }
    return true;
}

static uint32_t parse_mod(const char *mod_str)
{
    if (!mod_str || !*mod_str) {
        return 0;
    }

    uint32_t mod = 0;
    char input_copy[256];
    char *token = NULL;
    char *saveptr = NULL;

    // 复制并转换为小写
    strncpy(input_copy, mod_str, sizeof(input_copy) - 1);
    input_copy[sizeof(input_copy) - 1] = '\0';
    for (char *p = input_copy; *p; p++) {
        *p = tolower(*p);
    }

    // 分割处理每个部分
    token = strtok_r(input_copy, "+", &saveptr);
    while (token != NULL) {
        // 去除空白
        while (*token == ' ' || *token == '\t')
            token++;

        if (strncmp(token, "code:", 5) == 0) {
            // 处理 code: 形式
            char *endptr;
            long keycode = strtol(token + 5, &endptr, 10);
            if (endptr != token + 5 && (*endptr == '\0' || *endptr == ' ')) {
                switch (keycode) {
                case 133:
                case 134:
                    mod |= WLR_MODIFIER_LOGO;
                    break;
                case 37:
                case 105:
                    mod |= WLR_MODIFIER_CTRL;
                    break;
                case 50:
                case 62:
                    mod |= WLR_MODIFIER_SHIFT;
                    break;
                case 64:
                case 108:
                    mod |= WLR_MODIFIER_ALT;
                    break;
                }
            }
        } else {
            // 完整的 modifier 检查（保留原始所有检查项）
            if (strstr(token, "super") || strstr(token, "super_l") ||
                strstr(token, "super_r")) {
                mod |= WLR_MODIFIER_LOGO;
                }
            if (strstr(token, "ctrl") || strstr(token, "ctrl_l") ||
                strstr(token, "ctrl_r")) {
                mod |= WLR_MODIFIER_CTRL;
                }
            if (strstr(token, "shift") || strstr(token, "shift_l") ||
                strstr(token, "shift_r")) {
                mod |= WLR_MODIFIER_SHIFT;
                }
            if (strstr(token, "alt") || strstr(token, "alt_l") ||
                strstr(token, "alt_r")) {
                mod |= WLR_MODIFIER_ALT;
                }
            if (strstr(token, "hyper") || strstr(token, "hyper_l") ||
                strstr(token, "hyper_r")) {
                mod |= WLR_MODIFIER_MOD3;
                }
        }
        token = strtok_r(NULL, "+", &saveptr);
    }

    return mod;
}

static KeySymCode parse_key(const char *keyStr, bool isBindSym)
{
    KeySymCode kc = {0};

    if (gConfig.keyMap == NULL || gConfig.ctx == NULL) {
        kc.type = KEY_TYPE_SYM;
        kc.keySym = XKB_KEY_NoSymbol;
        return kc;
    }

    // 处理 code: 前缀的情况
    if (strncmp(keyStr, "code:", 5) == 0) {
        char *endPtr = NULL;
        xkb_keycode_t keycode = (xkb_keycode_t)strtol(keyStr + 5, &endPtr, 10);
        kc.type = KEY_TYPE_CODE;
        kc.keyCode.keyCode1 = keycode; // 只设置第一个
        kc.keyCode.keyCode2 = 0;
        kc.keyCode.keyCode3 = 0;
        return kc;
    }

    // 普通键名直接转换
    xkb_keysym_t sym = xkb_keysym_from_name(keyStr, XKB_KEYSYM_NO_FLAGS);

    if (isBindSym) {
        kc.type = KEY_TYPE_SYM;
        kc.keySym = sym;
        return kc;
    }

    if (sym != XKB_KEY_NoSymbol) {
        int foundCount = find_keycodes_for_keysym(gConfig.keyMap, sym, &kc.keyCode);
        if (foundCount > 0) {
            kc.type = KEY_TYPE_CODE;
            kc.keySym = sym;
        }
        else {
            kc.type = KEY_TYPE_SYM;
            kc.keySym = sym;
        }
    }
    else {
        kc.type = KEY_TYPE_SYM;
        kc.keySym = XKB_KEY_NoSymbol;
    }

    return kc;
}

static void convert_hex_to_rgba(float *color, unsigned long int hex)
{
    color[0] = (float) ((hex >> 24) & 0xFF) / 255.0f;
    color[1] = (float) ((hex >> 16) & 0xFF) / 255.0f;
    color[2] = (float) ((hex >> 8) & 0xFF) / 255.0f;
    color[3] = (float) (hex & 0xFF) / 255.0f;
}

static uint32_t parse_num_type(const char *str)
{
    switch (str[0]) {
    case '-':
        return NUM_TYPE_MINUS;
    case '+':
        return NUM_TYPE_PLUS;
    default:
        return NUM_TYPE_DEFAULT;
    }
}

static int find_keycodes_for_keysym(struct xkb_keymap *keymap, xkb_keysym_t sym, MultiKeyCode *multiKc)
{
    int foundCount = 0;

    xkb_keycode_t minKeycode = xkb_keymap_min_keycode(keymap);
    xkb_keycode_t maxKeycode = xkb_keymap_max_keycode(keymap);

    multiKc->keyCode1 = 0;
    multiKc->keyCode2 = 0;
    multiKc->keyCode3 = 0;


    for (xkb_keycode_t keycode = minKeycode; keycode <= maxKeycode && foundCount < 3; keycode++) {
        const xkb_keysym_t *syms;
        int num_syms = xkb_keymap_key_get_syms_by_level(keymap, keycode, 0, 0, &syms);
        for (int i = 0; i < num_syms; i++) {
            if (syms[i] == sym) {
                switch (foundCount) {
                    case 0: {
                        multiKc->keyCode1 = keycode;
                        break;
                    }
                    case 1: {
                        multiKc->keyCode2 = keycode;
                        break;
                    }
                    case 2: {
                        multiKc->keyCode3 = keycode;
                        break;
                    }
                    default: {
                        break;
                    }
                }
                foundCount++;
                break;
            }
        }
    }

    return foundCount;
}

static int parse_button(const char *str)
{
    int i = 0;
    char lowerStr[20] = {0};
    while (str[i] && i < 19) {
        lowerStr[i] = (char) tolower(str[i]);
        i++;
    }
    lowerStr[i] = '\0'; // 确保字符串正确终止

    // 根据转换后的小写字符串返回对应的按钮编号
    if (strcmp(lowerStr, "btn_left") == 0) {
        return BTN_LEFT;
    }
    else if (strcmp(lowerStr, "btn_right") == 0) {
        return BTN_RIGHT;
    }
    else if (strcmp(lowerStr, "btn_middle") == 0) {
        return BTN_MIDDLE;
    }
    else if (strcmp(lowerStr, "btn_side") == 0) {
        return BTN_SIDE;
    }
    else if (strcmp(lowerStr, "btn_extra") == 0) {
        return BTN_EXTRA;
    }
    else if (strcmp(lowerStr, "btn_forward") == 0) {
        return BTN_FORWARD;
    }
    else if (strcmp(lowerStr, "btn_back") == 0) {
        return BTN_BACK;
    }
    else if (strcmp(lowerStr, "btn_task") == 0) {
        return BTN_TASK;
    }
    else {
        return 0;
    }
}

static int parse_mouse_action(const char *str)
{
    char lowerStr[20];
    int i = 0;
    while (str[i] && i < 19) {
        lowerStr[i] = (char) tolower(str[i]);
        i++;
    }
    lowerStr[i] = '\0';

    if (strcmp(lowerStr, "curmove") == 0) {
        return CurMove;
    }
    else if (strcmp(lowerStr, "curresize") == 0) {
        return CurResize;
    }
    else if (strcmp(lowerStr, "curnormal") == 0) {
        return CurNormal;
    }
    else if (strcmp(lowerStr, "curpressed") == 0) {
        return CurPressed;
    }
    else {
        return 0;
    }
}

static FuncType parse_func_name(char* funcName, Arg* arg, char* argValue, char* argValue2, char* argValue3, char* argValue4, char* argValue5)
{
    FuncType func = NULL;
    (*arg).v = NULL;
    (*arg).v2 = NULL;
    (*arg).v3 = NULL;

    if (strcmp(funcName, "focusStack") == 0) {
        func = focus_stack;
        (*arg).i = parse_circle_direction(argValue);
    } else if (strcmp(funcName, "focusDir") == 0) {
        func = focus_dir;
        (*arg).i = parse_direction(argValue);
    } else if (strcmp(funcName, "incNMaster") == 0) {
        func = incn_master;
        (*arg).i = (int) strtol(argValue, NULL, 10);
    } else if (strcmp(funcName, "setMFact") == 0) {
        func = set_mfact;
        (*arg).f = strtof(argValue, NULL);
    } else if (strcmp(funcName, "zoom") == 0) {
        func = zoom;
    } else if (strcmp(funcName, "exchangeClient") == 0) {
        func = exchange_client;
        (*arg).i = parse_direction(argValue);
    } else if (strcmp(funcName, "exchangeStackClient") == 0) {
        func = exchange_stack_client;
        (*arg).i = parse_circle_direction(argValue);
    } else if (strcmp(funcName, "toggleGlobal") == 0) {
        func = toggle_global;
    } else if (strcmp(funcName, "toggleOverview") == 0) {
        func = toggle_overview;
        (*arg).i = (int) strtol(argValue, NULL, 10);
    } else if (strcmp(funcName, "setProportion") == 0) {
        func = set_proportion;
        (*arg).f = strtof(argValue, NULL);
    } else if (strcmp(funcName, "switchProportionPreset") == 0) {
        func = switch_proportion_preset;
    } else if (strcmp(funcName, "viewToLeft") == 0) {
        func = view_to_left;
        (*arg).i = (int) strtol (argValue, NULL, 10);
    } else if (strcmp(funcName, "viewToRight") == 0) {
        func = view_to_right;
        (*arg).i = (int) strtol(argValue, NULL, 10);
    } else if (strcmp(funcName, "tagSilent") == 0) {
        func = tag_silent;
        (*arg).ui = 1 << (strtol(argValue, NULL, 10) - 1);
    } else if (strcmp(funcName, "tagToLeft") == 0) {
        func = tag_to_left;
        (*arg).i = (int) strtol (argValue, NULL, 10);
    } else if (strcmp(funcName, "tagToRight") == 0) {
        func = tag_to_right;
        (*arg).i = (int) strtol (argValue, NULL, 10);
    } else if (strcmp(funcName, "killClient") == 0) {
        func = kill_client;
    } else if (strcmp(funcName, "centerWin") == 0) {
        func = center_win;
    } else if (strcmp(funcName, "focusLast") == 0) {
        func = focus_last;
    } else if (strcmp(funcName, "toggleTrackpadEnable") == 0) {
        func = toggle_trackpad_enable;
    } else if (strcmp(funcName, "setOption") == 0) {
        func = set_option;
        (*arg).v = strdup(argValue);
        const char *non_empty_params[4] = {NULL};
        int param_index = 0;
        if (argValue2 && argValue2[0] != '\0') {
            non_empty_params[param_index++] = argValue2;
        }
        if (argValue3 && argValue3[0] != '\0') {
            non_empty_params[param_index++] = argValue3;
        }
        if (argValue4 && argValue4[0] != '\0') {
            non_empty_params[param_index++] = argValue4;
        }
        if (argValue5 && argValue5[0] != '\0') {
            non_empty_params[param_index++] = argValue5;
        }

        if (param_index == 0) {
            (*arg).v2 = strdup("");
        }
        else {
            size_t len = 0;
            for (int i = 0; i < param_index; i++) {
                len += strlen(non_empty_params[i]);
            }
            len += (param_index - 1) + 1;

            char *temp = malloc(len);
            if (temp) {
                char *cursor = temp;
                for (int i = 0; i < param_index; i++) {
                    if (i > 0) {
                        *cursor++ = ',';
                    }
                    size_t param_len = strlen(non_empty_params[i]);
                    memcpy(cursor, non_empty_params[i], param_len);
                    cursor += param_len;
                }
                *cursor = '\0';
                (*arg).v2 = temp;
            }
        }
    } else if (strcmp(funcName, "setKeyMode") == 0) {
        func = set_key_mode;
        (*arg).v = strdup(argValue);
    } else if (strcmp(funcName, "switchKeyboardLayout") == 0) {
        func = switch_keyboard_layout;
    } else if (strcmp(funcName, "setLayout") == 0) {
        func = set_layout;
        (*arg).v = strdup(argValue);
    } else if (strcmp(funcName, "switchLayout") == 0) {
        func = switch_layout;
    } else if (strcmp(funcName, "toggleFloating") == 0) {
        func = toggle_floating;
    } else if (strcmp(funcName, "toggleFullscreen") == 0) {
        func = toggle_fullscreen;
    } else if (strcmp(funcName, "toggleFakeFullscreen") == 0) {
        func = toggle_fake_fullscreen;
    } else if (strcmp(funcName, "toggleOverlay") == 0) {
        func = toggle_overlay;
    } else if (strcmp(funcName, "minimized") == 0) {
        func = minimized;
    } else if (strcmp(funcName, "restoreMinimized") == 0) {
        func = restore_minimized;
    } else if (strcmp(funcName, "toggleScratchpad") == 0) {
        func = toggle_scratchpad;
    } else if (strcmp(funcName, "toggleRenderBorder") == 0) {
        func = toggle_render_border;
    } else if (strcmp(funcName, "focusMon") == 0) {
        func = focus_mon;
        (*arg).i = parse_direction(argValue);
        if ((*arg).i == UNDIR) {
            (*arg).v = strdup(argValue);
        }
    } else if (strcmp(funcName, "tagMon") == 0) {
        func = tag_mon;
        (*arg).i = parse_direction(argValue);
        (*arg).i2 = (int) strtol (argValue2, NULL, 10);
        if ((*arg).i == UNDIR) {
            (*arg).v = strdup(argValue);
        };
    } else if (strcmp(funcName, "incGaps") == 0) {
        func = inc_gaps;
        (*arg).i = (int) strtol(argValue, NULL, 10);
    } else if (strcmp(funcName, "toggleGaps") == 0) {
        func = toggle_gaps;
    } else if (strcmp(funcName, "chvt") == 0) {
        func = chvt;
        (*arg).ui = strtol(argValue, NULL, 10);
    } else if (strcmp(funcName, "spawn") == 0) {
        func = spawn;
        (*arg).v = strdup(argValue);
    } else if (strcmp(funcName, "spawnShell") == 0) {
        func = spawnShell;
        (*arg).v = strdup(argValue);
    } else if (strcmp(funcName, "spawnOnEmpty") == 0) {
        func = spawn_on_empty;
        (*arg).v = strdup(argValue);
        (*arg).ui = 1 << (atoi(argValue2) - 1);
    } else if (strcmp(funcName, "quit") == 0) {
        func = quit;
    } else if (strcmp(funcName, "createVirtualOutput") == 0) {
        func = create_virtual_output;
    } else if (strcmp(funcName, "destroyAllVirtualOutput") == 0) {
        func = destroy_all_virtual_output;
    } else if (strcmp(funcName, "moveResize") == 0) {
        func = move_resize;
        (*arg).ui = parse_mouse_action(argValue);
    } else if (strcmp(funcName, "toggleMaximizeScreen") == 0) {
        func = toggle_maximize_screen;
    } else if (strcmp(funcName, "viewToLeftHaveClient") == 0) {
        func = view_to_left_have_client;
        (*arg).i = (int) strtol (argValue, NULL, 10);
    } else if (strcmp(funcName, "viewToRightHaveClient") == 0) {
        func = view_to_right_have_client;
        (*arg).i = (int) strtol (argValue, NULL, 10);
    } else if (strcmp(funcName, "reloadConfig") == 0) {
        func = reload_config;
    } else if (strcmp(funcName, "tag") == 0) {
        func = tag;
        (*arg).ui = 1 << (atoi(argValue) - 1);
        (*arg).i = atoi(argValue2);
    } else if (strcmp(funcName, "view") == 0) {
        func = bind_to_view;

        uint32_t mask = 0;
        char *token;
        char *arg_copy = strdup(argValue);
        if (arg_copy != NULL) {
            char *savePtr = NULL;
            token = strtok_r(arg_copy, "|", &savePtr);
            while (token != NULL) {
                int num = (int) strtol (token, NULL, 10);
                if (num > 0 && num <= LENGTH(tags)) {
                    mask |= (1 << (num - 1));
                }
                token = strtok_r(NULL, "|", &savePtr);
            }
            free(arg_copy);
        }
        if (mask) {
            (*arg).ui = mask;
        } else {
            (*arg).ui = atoi(argValue);
        }
        (*arg).i = atoi(argValue2);
    } else if (strcmp(funcName, "viewCrossMon") == 0) {
        func = view_cross_mon;
        (*arg).ui = 1 << (atoi(argValue) - 1);
        (*arg).v = strdup(argValue2);
    } else if (strcmp(funcName, "tagCrossMon") == 0) {
        func = tag_cross_mon;
        (*arg).ui = 1 << (atoi(argValue) - 1);
        (*arg).v = strdup(argValue2);
    } else if (strcmp(funcName, "toggleTag") == 0) {
        func = toggle_tag;
        (*arg).ui = 1 << (atoi(argValue) - 1);
    } else if (strcmp(funcName, "toggleView") == 0) {
        func = toggle_view;
        (*arg).ui = 1 << (atoi(argValue) - 1);
    } else if (strcmp(funcName, "comboView") == 0) {
        func = combo_view;
        (*arg).ui = 1 << (atoi(argValue) - 1);
    } else if (strcmp(funcName, "smartMoveWin") == 0) {
        func = smart_move_win;
        (*arg).i = parse_direction(argValue);
    } else if (strcmp(funcName, "smartResizeWin") == 0) {
        func = smart_resize_win;
        (*arg).i = parse_direction(argValue);
    } else if (strcmp(funcName, "resizeWin") == 0) {
        func = resize_win;
        (*arg).ui = parse_num_type(argValue);
        (*arg).ui2 = parse_num_type(argValue2);
        (*arg).i = (*arg).ui == NUM_TYPE_DEFAULT ? atoi(argValue) : atoi(argValue + 1);
        (*arg).i2 = (*arg).ui2 == NUM_TYPE_DEFAULT ? atoi(argValue2) : atoi(argValue2 + 1);
    } else if (strcmp(funcName, "moveWin") == 0) {
        func = move_win;
        (*arg).ui = parse_num_type(argValue);
        (*arg).ui2 = parse_num_type(argValue2);
        (*arg).i = (*arg).ui == NUM_TYPE_DEFAULT ? atoi(argValue) : atoi(argValue + 1);
        (*arg).i2 = (*arg).ui2 == NUM_TYPE_DEFAULT ? atoi(argValue2) : atoi(argValue2 + 1);
    } else if (strcmp(funcName, "toggleNamedScratchpad") == 0) {
        func = toggle_named_scratchpad;
        (*arg).v = strdup(argValue);
        (*arg).v2 = strdup(argValue2);
        (*arg).v3 = strdup(argValue3);
    } else if (strcmp(funcName, "disableMonitor") == 0) {
        func = disable_monitor;
        (*arg).v = strdup(argValue);
    } else if (strcmp(funcName, "enableMonitor") == 0) {
        func = enable_monitor;
        (*arg).v = strdup(argValue);
    } else if (strcmp(funcName, "toggleMonitor") == 0) {
        func = toggle_monitor;
        (*arg).v = strdup(argValue);
    } else {
        return NULL;
    }
    return func;
}

static void reset_option(void)
{
    init_baked_points();
    handlecursoractivity();
    reset_keyboard_layout();
    reset_blur_params();
    set_env();
    run_exec();

    reapply_cursor_style();
    reapply_border();
    reapply_keyboard();
    reapply_pointer();
    reapply_master();

    reapply_tagrule();
    reapply_monitor_rules();

    arrange(selmon, false);
}