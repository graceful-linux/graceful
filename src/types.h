//
// Created by dingjing on 2025/12/10.
//

#ifndef graceful_GRACEFUL_TYPES_H
#define graceful_GRACEFUL_TYPES_H
#include <stdint.h>
#include <libinput.h>
#include <wlr/util/box.h>
#include <wlr/backend/wayland.h>
#include <xkbcommon/xkbcommon.h>
#include <wlr/types/wlr_keyboard.h>
#include <linux/input-event-codes.h>
#include <scenefx/types/fx/blur_data.h>
#include <scenefx/types/fx/corner_location.h>

#define GRACEFUL_LOG_LEVEL_DEBUG        0
#define GRACEFUL_LOG_LEVEL_INFO         1
#define GRACEFUL_LOG_LEVEL_WARNING      2
#define GRACEFUL_LOG_LEVEL_ERROR        3
#define GRACEFUL_LOG_LEVEL_DIE          4

#define GRACEFUL_LOG_LEVEL_DEFAULT      GRACEFUL_LOG_LEVEL_WARNING

#define BAKED_POINTS_COUNT              256

#define MAX(A, B)                       ((A) > (B) ? (A) : (B))
#define MIN(A, B)                       ((A) < (B) ? (A) : (B))
#define TAG_MASK                        ((1 << LENGTH(tags)) - 1)
#define LENGTH(_x)                      (sizeof(_x) / sizeof(_x[0]))
#define END(A)                          ((A) + LENGTH(A))
#define GET_ZERO(A)                     ((A) >= 0 ? (A) : 0)
#define CLEAN_MASK(mask)                (mask & ~WLR_MODIFIER_CAPS)
#define VISIBLE_ON(C, M)                ((C) && (M) && (C)->monitor == (M) && ((C)->tags & (M)->tagSet[(M)->selTags]))
#define IS_FULLSCREEN(A)                ((A)->isFullscreen || (A)->isMaximizeScreen || (A)->overviewIsMaximizeScreenBak || (A)->overviewIsFullScreenBak)
#define IS_SCROLL_TILED(A)              (A && !(A)->isFloating && !(A)->isMinimized && !(A)->isKilling && !(A)->isUnglobal)
#define IS_TILED(A)                     (A && !(A)->isFloating && !(A)->isMinimized && !(A)->isKilling && !(A)->isMaximizeScreen && !(A)->isFullScreen && !(A)->isUnglobal)
#define INSIDE_MON(A)                   (A->geom.x >= A->mon->m.x && A->geom.y >= A->mon->m.y && A->geom.x + A->geom.width <= A->mon->m.x + A->mon->m.width && A->geom.y + A->geom.height <= A->mon->m.y + A->mon->m.height)
#define LISTEN(E, L, H)                 wl_signal_add((E), ((L)->notify = (H), (L)))
#define LISTEN_STATIC(E, H) \
    do { \
        struct wl_listener *_l = ecalloc(1, sizeof(*_l)); \
        _l->notify = (H); \
        wl_signal_add((E), _l); \
    } while (0)

#define APPLY_INT_PROP(obj, rule, prop) \
    do { \
        if (rule->prop >= 0) { \
            obj->prop = rule->prop; \
        } \
    } while (0)

#define APPLY_FLOAT_PROP(obj, rule, prop) \
    do { \
        if (rule->prop > 0.0f) { \
            obj->prop = rule->prop; \
        } \
    } while (0)

#define APPLY_STRING_PROP(obj, rule, prop) \
    do { \
        if (rule->prop != NULL) { \
            obj->prop = rule->prop; \
        } \
    } while (0)

#define COLOR0(hex)     (((hex >> 24) & 0xFF) / 255.0f)
#define COLOR1(hex)     (((hex >> 16) & 0xFF) / 255.0f)
#define COLOR2(hex)     (((hex >>  8) & 0xFF) / 255.0f)
#define COLOR3(hex)     (((hex      ) & 0xFF) / 255.0f)

#define COLOR(hex) \
    { \
        COLOR0(hex), \
        COLOR1(hex), \
        COLOR2(hex), \
        COLOR3(hex), \
    }


static const char *gTags[] = {
    "1", "2", "3", "4", "5", "6", "7", "8", "9",
};

typedef struct _Client Client;
typedef struct _Monitor Monitor;

enum
{
    NUM_TYPE_MINUS,
    NUM_TYPE_PLUS,
    NUM_TYPE_DEFAULT,
};

enum
{
    KEY_TYPE_CODE,
    KEY_TYPE_SYM,
};

enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UNDIR
}; /* smart move win */

enum
{
    NONE,
    OPEN,
    MOVE,
    CLOSE,
    TAG,
    FOCUS
};

enum
{
    UNFOLD,
    FOLD,
    INVALIDFOLD
};

enum
{
    PREV,
    NEXT
};

enum
{
    STATE_UNSPECIFIED = 0,
    STATE_ENABLED,
    STATE_DISABLED
};

typedef enum
{
    VERTICAL = 1,
    HORIZONTAL,
} Direction;

typedef enum
{
    SWIPE_UP,
    SWIPE_DOWN,
    SWIPE_LEFT,
    SWIPE_RIGHT,
} SwipeDirection;

typedef enum
{
    CURSOR_NORMAL,
    CURSOR_PRESSED,
    CURSOR_MOVE,
    CURSOR_RESIZE,
} Cursor;

typedef enum
{
    CLIENT_TYPE_XDG_SHELL,
    CLIENT_TYPE_LAYER_SHELL,
    CLIENT_TYPE_X11,
} ClientType;

typedef enum
{
    MOUSE_AXIS_UP,
    MOUSE_AXIS_DOWN,
    MOUSE_AXIS_LEFT,
    MOUSE_AXIS_RIGHT,
} MouseAxis;

typedef enum
{
    LAYERS_BG,
    LAYERS_BLUR,
    LAYERS_BOTTOM,
    LAYERS_TILE,
    LAYERS_TOP,
    LAYERS_FADE_OUT,
    LAYERS_OVERLAY,
    LAYERS_IM_POPUP,
    LAYERS_BLOCK,
    LAYERS_NUM,
} Layers;

typedef enum
{
    TEARING_DISABLED = 0,
    TEARING_ENABLED,
    TEARING_FULLSCREEN_ONLY,
} TearingMode;

typedef enum
{
    SHORTCUTS_INHIBIT_DISABLE,
    SHORTCUTS_INHIBIT_ENABLE,
} SeatConfigShortcutsInhibit;

typedef enum
{
    PRINT_ACTIVE        = 1 << 0,
    PRINT_TAG           = 1 << 1,
    PRINT_LAYOUT        = 1 << 2,
    PRINT_TITLE         = 1 << 3,
    PRINT_APPID         = 1 << 4,
    PRINT_LAYOUT_SYMBOL = 1 << 5,
    PRINT_FULLSCREEN    = 1 << 6,
    PRINT_FLOATING      = 1 << 7,
    PRINT_X             = 1 << 8,
    PRINT_Y             = 1 << 9,
    PRINT_WIDTH         = 1 << 10,
    PRINT_HEIGHT        = 1 << 11,
    PRINT_LAST_LAYER    = 1 << 12,
    PRINT_KB_LAYOUT     = 1 << 13,
    PRINT_KEY_MODE      = 1 << 14,
    PRINT_SCALE_FACTOR  = 1 << 15,
    PRINT_FRAME         = 1 << 16,
    PRINT_ALL           = (1 << 17) - 1
} PrintEventType;

typedef struct
{
    double          x, y;
} DVec2;

typedef struct
{
    int             x, y;
    int             width, height;
} IVec2;

typedef struct
{
    int                         i;
    int                         i2;
    float                       f;
    float                       f2;
    char*                       v;
    char*                       v2;
    char*                       v3;
    uint32_t                    ui;
    uint32_t                    ui2;
} Arg;

typedef struct
{
    uint32_t        mod;
    uint32_t        button;
    int (*func) (const Arg*);
    const Arg arg;
} Button;

typedef struct
{
    char            mode[28];
    bool            isDefault;
} KeyMode;

typedef struct
{
    uint32_t        mod;
    uint32_t        dir;
    int (*func) (const Arg*);
    const Arg       arg;
} Axis;

typedef struct
{
    uint32_t        id;
    const char*     symbol;
    void (*arrange) (Monitor *);
    const char*     name;
} WMLayout;

typedef struct
{
    uint32_t                    curTag, prevTag;                // current and previous tag
    int                         nMasters[LENGTH(gTags) + 1];    // number of windows in master area
    float                       mFacts[LENGTH(gTags) + 1];      // mfacts per tag
    bool                        noHide[LENGTH(gTags) + 1];      // no_hide per tag
    const WMLayout*             ltIdxS[LENGTH(gTags) + 1];      // matrix of tags and layouts indexes
} PerTag;

typedef struct
{
    struct wl_list              link;
    struct wlr_input_device*    wlrDevice;
    struct libinput_device*     inputDevice;
    struct wl_listener          destroyListener;    // 监听设备销毁事件
    void*                       deviceData;
} InputDevice;

typedef struct
{
    struct wl_list              link;
    struct wlr_switch*          wlrSwitch;
    struct wl_listener          toggle;
    InputDevice*                inputDev;
} Switch;

typedef struct
{
    bool                        shouldAnimate;
    bool                        running;
    bool                        tagInIng;
    bool                        tagOuted;
    bool                        tagOutIng;
    bool                        beginFadeIn;
    bool                        tagFromRule;
    uint32_t                    timeStarted;
    uint32_t                    duration;
    struct wlr_box              initial;
    struct wlr_box              current;
    int                         action;
} WMAnimation;

typedef struct
{
    bool                        running;
    float                       currentOpacity;
    float                       targetOpacity;
    float                       initialOpacity;
    uint32_t                    timeStarted;
    uint32_t                    duration;
    float                       targetBorderColor[4];
    float                       currentBorderColor[4];
    float                       initialBorderColor[4];
} WMOpacityAnimation;

struct _Monitor
{
    struct wl_list                              link;
    struct wlr_output*                          wlrOutput;
    struct wlr_scene_output*                    sceneOutput;
    struct wlr_output_state                     pending;
    struct wl_listener                          frame;
    struct wl_listener                          destroy;
    struct wl_listener                          requestState;
    struct wl_listener                          destroyLockSurface;
    struct wlr_session_lock_surface_v1*         lockSurface;
    struct wlr_box                              m;  // monitor area, layout-relative
    struct wlr_box                              w;  // window area, layout-relative
    struct wl_list                              layers[4]; // LayerSurface::link
    const WMLayout*                             lt;
    uint32_t                                    selTags;
    uint32_t                                    tagset[2];
    double                                      mFact;
    int                                         nMaster;
    struct wl_list                              ipcOutputs;
    int                                         gapPiH; /* horizontal gap between windows */
    int                                         gapPiV; /* vertical gap between windows */
    int                                         gapPoH; /* horizontal outer gaps */
    int                                         gapPoV; /* vertical outer gaps */
    PerTag*                                     perTag;
    Client*                                     sel, *prevSel;
    int                                         isOverview;
    int                                         isInHotArea;
    int                                         asleep;
    uint32_t                                    visibleClients;
    uint32_t                                    visibleTilingClients;
    uint32_t                                    visibleScrollTilingClients;
    bool                                        hasVisibleFullscreenClient;
    struct wlr_scene_optimized_blur*            blur;
    char                                        lastSurfaceWsName[256];
    struct wlr_ext_workspace_group_handle_v1*   extGroup;
};

struct _Client
{
    uint32_t                    type; // XDGShell or X11
    struct wlr_box              geom, pending, floatGeom, animaInitGeom, overviewBackupGeom, current, dragBeginGeom; // layout-relative, includes border
    Monitor*                    monitor;
    struct wlr_scene_tree*      scene;
    struct wlr_scene_rect*      border;     // top, bottom, left, right
    struct wlr_scene_shadow*    shadow;
    struct wlr_scene_tree*      sceneSurface;
    struct wl_list              link;
    struct wl_list              flink;
    struct wl_list              fadeoutLink;
    union {
        struct wlr_xdg_surface*         xdg;
        struct wlr_xwayland_surface*    xWayland;
    } surface;
    struct wl_listener          commit;
    struct wl_listener          map;
    struct wl_listener          maximize;
    struct wl_listener          minimize;
    struct wl_listener          unmap;
    struct wl_listener          destroy;
    struct wl_listener          setTitle;
    struct wl_listener          fullscreen;
    uint32_t                    bw;
    uint32_t                    tags, oldTags, miniRestoreTag;
    bool                        dirty;
    uint32_t                    configureSerial;
    struct wlr_foreign_toplevel_handle_v1*  foreignToplevel;
    bool                        isFloating, isUrgent, isFullscreen, isFakeFullscreen,
                                needFloatSizeReduce, isMinimized, isOverlay, isNoSizeHint,
                                ignoreMaximize, ignoreMinimize;
    int                         isMaximizeScreen;
    int                         overviewBackupBw;
    int                         fullscreenBackupX, fullscreenBackupY, fullscreenBackupW, fullscreenBackupH;
    int                         overviewIsFullscreenBak, overviewIsMaximizeScreenBak,
                                overviewIsFloatingBak;
    struct wlr_xdg_toplevel_decoration_v1 *decoration;
    struct wl_listener          foreignActivateRequest;
    struct wl_listener          foreignFullscreenRequest;
    struct wl_listener          foreignCloseRequest;
    struct wl_listener          foreignDestroy;
    struct wl_listener          setDecorationMode;
    struct wl_listener          destroy_decoration;
    const char*                 animationTypeOpen;
    const char*                 animationTypeClose;
    int                         isInScratchpad;
    int                         isCustomSize;
    int                         isScratchpadShow;
    int                         isGlobal;
    bool                        isNoBorder;
    bool                        isNoShadow;
    bool                        isNoAnimation;
    bool                        isOpenSilent;
    bool                        isTagSilent;
    bool                        isKilling;
    bool                        isTagSwitching;
    bool                        isNamedScratchpad;
    bool                        isPendingOpenAnimation;
    bool                        isRestoringFromOv;
    float                       scrollerProportion;
    bool                        needOutputFlush;
    WMAnimation                 animation;
    WMOpacityAnimation          opacityAnimation;
    int                         isTerm, noSwallow;
    int                         allowCsd;
    int                         forceMaximize;
    pid_t                       pid;
    Client*                     swallowing, *swallowedBy;
    bool                        isClipToHide;
    bool                        dragToTile;
    bool                        scratchpadSwitchingMon;
    bool                        fakeNoBorder;
    int                         noFocus;
    int                         noFadein;
    int                         noFadeout;
    int                         noForceCenter;
    int                         isUnglobal;
    float                       focusedOpacity;
    float                       unfocusedOpacity;
    char                        oldMonName[128];
    int                         noBlur;
    double                      masterMFactPer, masterInnerPer, stackInnderPer;
    double                      oldMasterMFactPer, oldMasterInnerPer, oldStackInnderPer;
    double                      oldScrollerPProportion;
    bool                        isMaster;
    bool                        cursorInUpperHalf, cursorInLeftHalf;
    bool                        isLeftStack;
    int                         tearingHint;
    int                         forceTearing;
    int                         allowShortcutsInhibit;
    float                       scrollerProportionSingle;
    bool                        isFocusing;
};

typedef struct
{
    struct wl_list              link;
    struct wl_resource*         resource;
    Monitor*                    monitor;
} IpcOutput;

typedef struct
{
    uint32_t                    mod;
    xkb_keysym_t                keySym;
    int (*func) (const Arg*);
    const Arg                   args;
} Key;

typedef struct
{
    struct wlr_keyboard_group*  wlrGroup;
    int                         nSyms;
    const xkb_keysym_t*         keySyms;
    uint32_t                    mods;
    uint32_t                    keyCode;
    struct wl_event_source*     keyRepeatSource;
    struct wl_listener          modifiers;
    struct wl_listener          key;
    struct wl_listener          destroy;
} KeyboardGroup;

typedef struct
{
    struct wlr_keyboard_shortcuts_inhibitor_v1 *inhibitor;
    struct wl_listener          destroy;
    struct wl_list              link;
} KeyboardShortcutsInhibitor;

typedef struct
{
    uint32_t                            type; /* LayerShell */
    struct wlr_box                      geom, current, pending, animaInitGeom;
    Monitor*                            monitor;
    struct wlr_scene_tree*              scene;
    struct wlr_scene_tree*              popups;
    struct wlr_scene_shadow*            shadow;
    struct wlr_scene_layer_surface_v1*  sceneLayer;
    struct wl_list                      link;
    struct wl_list                      fadeoutLink;
    int                                 mapped;
    struct wlr_layer_surface_v1*        layerSurface;
    struct wl_listener                  destroy;
    struct wl_listener                  map;
    struct wl_listener                  unmap;
    struct wl_listener                  surfaceCommit;
    WMAnimation                         animation;
    bool                                dirty;
    int                                 noBlur;
    int                                 noAnim;
    int                                 noShadow;
    char*                               animationTypeOpen;
    char*                               animationTypeClose;
    bool                                needOutputFlush;
} LayerSurface;

typedef struct
{
    uint32_t                            id;
    const char*                         symbol;
    void (*arrange)(Monitor*);
    const char*                         name;
} Layout;

typedef struct
{
    struct wlr_pointer_constraint_v1*   constraint;
    struct wl_listener                  destroy;
} PointerConstraint;

typedef struct
{
    struct wlr_scene_tree*              scene;
    struct wlr_session_lock_v1*         lock;
    struct wl_listener                  newSurface;
    struct wl_listener                  unlock;
    struct wl_listener                  destroy;
} SessionLock;

typedef struct
{
    int                         width;
    int                         height;
    float                       widthScale;
    float                       heightScale;
    enum corner_location        cornerLocation;
} WMBufferData;

typedef struct
{
    uint32_t                    keyCode1;
    uint32_t                    keyCode2;
    uint32_t                    keyCode3;
} MultiKeyCode;

typedef struct
{
    uint8_t                     type;
    xkb_keysym_t                keySym;
    MultiKeyCode                keyCode;
} KeySymCode;

typedef struct
{
    uint32_t                    mod;
    KeySymCode                  keySymCode;
    int (*func) (const Arg*);
    Arg                         arg;
    char                        mode[28];
    bool                        isCommonMode;
    bool                        isDefaultMode;
    bool                        isLockApply;
    bool                        isReleaseApply;
} KeyBinding;

typedef struct
{
    char*                       type;
    char*                       value;
} ConfigEnv;

typedef struct
{
    const char*                 id;
    const char*                 title;
    uint32_t                    tags;
    int                         isFloating;
    int                         isFullScreen;
    float                       scrollerProportion;
    const char*                 animationTypeOpen;
    const char*                 animationTypeClose;
    const char*                 layerAnimationTypeOpen;
    const char*                 layerAnimationTypeClose;
    int                         isNoBorder;
    int                         isNoShadow;
    int                         isNoAnimation;
    int                         isOpenSilent;
    int                         isTagSilent;
    int                         isNamedScratchpad;
    int                         isUnglobal;
    int                         isOverlay;
    int                         allowShortcutsInhibit;
    int                         ignoreMaximize;
    int                         ignoreMinimize;
    int                         isNoSizeHint;
    const char*                 monitor;
    int                         offsetX;
    int                         offsetY;
    int                         width;
    int                         height;
    int                         noFocus;
    int                         noFadein;
    int                         noFadeout;
    int                         noForceCenter;
    int                         isTerm;
    int                         allowCSD;
    int                         forceMaximize;
    int                         forceTearing;
    int                         noSwallow;
    int                         noBlur;
    float                       focusedOpacity;
    float                       unfocusedOpacity;
    float                       scrollerProportionSingle;
    uint32_t                    passMod;
    xkb_keysym_t                keySym;
    KeyBinding                  globalKeyBinding;
} ConfigWinRule;

typedef struct
{
    const char*                 name;                   // 显示器名称
    float                       mFact;                  // 主区域比例
    int                         nMaster;                // 主区域窗口数量
    const char*                 layout;                 // 布局名称
    int                         rr;                     // 旋转和翻转
    float                       scale;                  // 显示器缩放比例
    int                         x, y;                   // 显示器位置
    int                         width, height;          // 显示器分辨率
    float                       refresh;                // 刷新率
} ConfigMonitorRule;

typedef struct
{
    uint32_t                    mod;
    uint32_t                    button;
    int (*func) (const Arg*);
    Arg                         arg;
} MouseBinding;

typedef struct
{
    uint32_t                    mod;
    uint32_t                    dir;
    int (*func) (const Arg*);
    Arg                         arg;
} AxisBinding;

typedef struct
{
    uint32_t                    fold;
    int (*func) (const Arg*);
    Arg                         arg;
} SwitchBinding;

typedef struct
{
    uint32_t                    mod;
    uint32_t                    motion;
    uint32_t                    fingersCount;
    int (*func) (const Arg*);
    Arg                         arg;
} GestureBinding;

typedef struct
{
    int                         id;
    char*                       layoutName;
    char*                       monitorName;
    int                         noRenderBorder;
    int                         noHide;
} ConfigTagRule;

typedef struct
{
    char*                       layerName;
    char*                       animationTypeOpen;
    char*                       animationTypeClose;
    int                         noBlur;
    int                         noAnim;
    int                         noShadow;
} ConfigLayerRule;

typedef struct
{
    int                         animations;
    int                         layerAnimations;
    char                        animationTypeOpen[10];
    char                        animationTypeClose[10];
    char                        layerAnimationTypeOpen[10];
    char                        layerAnimationTypeClose[10];
    int                         animationFadeIn;
    int                         animationFadeOut;
    int                         tagAnimationDirection;
    float                       zoomInitialRatio;
    float                       zoomEndRatio;
    float                       fadeInBeginOpacity;
    float                       fadeOutBeginOpacity;
    uint32_t                    animationDurationMove;
    uint32_t                    animationDurationOpen;
    uint32_t                    animationDurationTag;
    uint32_t                    animationDurationClose;
    uint32_t                    animationDurationFocus;
    double                      animationCurveMove[4];
    double                      animationCurveOpen[4];
    double                      animationCurveTag[4];
    double                      animationCurveClose[4];
    double                      animationCurveFocus[4];

    int                         scrollerStructs;
    float                       scrollerDefaultProportion;
    float                       scrollerDefaultProportionSingle;
    int                         scrollerIgnoreProportionSingle;
    int                         scrollerFocusCenter;
    int                         scrollerPreferCenter;
    int                         edgeScrollerPointerFocus;
    int                         focusCrossMonitor;
    int                         exchangeCrossMonitor;
    int                         scratchpadCrossMonitor;
    int                         focusCrossTag;
    int                         viewCurrentToBack;
    int                         noBorderWhenSingle;
    int                         noRadiusWhenSingle;
    int                         snapDistance;
    int                         enableFloatingSnap;
    int                         dragTileToTile;
    uint32_t                    swipeMinThreshold;
    float                       focusedOpacity;
    float                       unfocusedOpacity;
    float*                      scrollerProportionPreset;
    int                         scrollerProportionPresetCount;
    char**                      circleLayout;
    int                         circleLayoutCount;
    uint32_t                    newIsMaster;
    float                       defaultMFact;
    uint32_t                    defaultNMaster;
    int                         centerMasterOverspread;
    int                         centerWhenSingleStack;
    uint32_t                    hotAreaSize;
    uint32_t                    enableHotArea;
    uint32_t                    ovTabMode;
    int                         overViewGapPi;
    int                         overViewGapPo;
    uint32_t                    cursorHideTimeout;
    uint32_t                    axisBindApplyTimeout;
    uint32_t                    focusOnActivate;
    int                         inhibitRegardlessOfVisibility;
    int                         sloppyFocus;
    int                         warpCursor;

    // keyboard
    int                         repeatRate;
    int                         repeatDelay;
    uint32_t                    numLockOn;

    // Trackpad
    int                         disableTrackpad;
    int                         tapToClick;
    int                         tapAndDrag;
    int                         dragLock;
    int                         mouseNaturalScrolling;
    int                         trackpadNaturalScrolling;
    int                         disableWhileTyping;
    int                         leftHanded;
    int                         middleButtonEmulation;
    uint32_t                    accelProfile;
    double                      accelSpeed;
    uint32_t                    scrollMethod;
    uint32_t                    scrollButton;
    uint32_t                    clickMethod;
    uint32_t                    sendEventsMode;
    uint32_t                    buttonMap;

    int                         blur;
    int                         blurLayer;
    int                         blurOptimized;
    int                         borderRadius;
    struct blur_data            blurParams;
    int                         shadows;
    int                         shadowOnlyFloating;
    int                         layerShadows;
    uint32_t                    shadowsSize;
    float                       shadowsBlur;
    int                         shadowsPositionX;
    int                         shadowsPositionY;
    float                       shadowsColor[4];

    int                         smartGaps;
    uint32_t                    gapPiH;
    uint32_t                    gapPiV;
    uint32_t                    gapPoH;
    uint32_t                    gapPoV;
    uint32_t                    borderPx;

    float                       scratchpadWidthRatio;
    float                       scratchpadHeightRatio;
    float                       rootColor[4];
    float                       borderColor[4];
    float                       focusColor[4];
    float                       maximizeScreenColor[4];
    float                       urgentColor[4];
    float                       scratchpadColor[4];
    float                       globalColor[4];
    float                       overlayColor[4];
    char                        autostart[3][256];

    int                         tagRulesCount;
    ConfigTagRule*              tagRules;

    int                         layerRulesCount;
    ConfigLayerRule*            layerRules;

    int                         windowRulesCount;
    ConfigWinRule*              windowRules;

    int                         monitorRulesCount;
    ConfigMonitorRule*          monitorRules;

    int                         keyBindingsCount;
    KeyBinding*                 keyBindings;

    int                         mouseBindingsCount;
    MouseBinding*               mouseBindings;

    int                         axisBindingsCount;
    AxisBinding*                axisBindings;

    int                         switchBindingsCount;
    SwitchBinding*              switchBindings;

    int                         gestureBindingsCount;
    GestureBinding*             gestureBindings;

    int                         envCount;
    ConfigEnv**                 env;

    int                         execCount;
    char**                      exec;

    int                         execOnceCount;
    char**                      execOnce;

    uint32_t                    cursorSize;
    char*                       cursorTheme;

    int                         singleScratchpad;
    int                         xWaylandPersistence;
    int                         syncObjEnable;
    int                         adaptiveSync;
    int                         allowTearing;
    int                         allowShortcutsInhibit;
    int                         allowLockTransparent;

    struct xkb_rule_names       xkbRules;
    char                        keyMode[28];
    struct xkb_context*         ctx;
    struct xkb_keymap*          keyMap;
} Config;

typedef struct
{
    char                            animationTypeOpen[8];           // 是否启用动画: slide,zoom
    char                            animationTypeClose[8];          // 是否启用动画: slide,zoom
    char                            layerAnimationTypeOpen[8];      // 是否启用layer动画: slide,zoom
    char                            layerAnimationTypeClose[8];     // 是否启用layer动画: slide,zoom
    bool                            animations;                     // 是否启用动画
    bool                            layerAnimations;                // 是否启用layer动画
    Direction                       tagAnimationDirection;          // 标签动画方向
    bool                            animationFadeIn;                // Enable animation fade in
    bool                            animationFadeOut;               // Enable animation fade out
    float                           zoomInitialRatio;               // 动画起始窗口比例
    float                           zoomEndRatio;                   // 动画结束窗口比例
    float                           fadeinBeginOpacity;             // Begin opac window ratio for animations
    float                           fadeoutBeginOpacity;            // Begin opac window ratio for animations
    uint32_t                        animationDurationMove;          // Animation move speed
    uint32_t                        animationDurationOpen;          // Animation open speed
    uint32_t                        animationDurationTag;           // Animation tag speed
    uint32_t                        animationDurationClose;         // Animation close speed
    uint32_t                        animationDurationFocus;         // Animation focus opacity speed
    double                          animationCurveMove[4];          // 动画曲线
    double                          animationCurveOpen[4];          // 动画曲线
    double                          animationCurveTag[4];           // 动画曲线
    double                          animationCurveClose[4];         // 动画曲线
    double                          animationCurveFocus[4];         // 动画曲线

    /* appearance */
    uint32_t                        axisBindApplyTimeout;           // 滚轮绑定动作的触发的时间间隔
    bool                            focusOnActivate;                // 收到窗口激活请求是否自动跳转聚焦
    bool                            newIsMaster;                    // 新窗口是否插在头部
    double                          defaultMFact;                   // master 窗口比例
    uint32_t                        defaultNMaster;                 // 默认master数量
    bool                            centerMasterOverspread;         // 中心master时是否铺满
    bool                            centerWhenSingleStack;          // 单个stack时是否居中

    bool                            numLockOn;                      // 是否打开右边小键盘
    bool                            capslock;                       // 是否启用快捷键
    uint32_t                        ovTabMode;                      // alt tab切换模式
    uint32_t                        hotAreaSize;                    // hotArea大小
    bool                            enableHotArea;                  // 是否启用鼠标热区
    int                             smartGaps;                      // 1 means no outer gap when there is only one window
    int                             sloppyFocus;                    // focus follows mouse
    uint32_t                        gapPiH;                         // horiz inner gap between windows
    uint32_t                        gapPiV;                         // vert inner gap between windows
    uint32_t                        gapPoH;                         // horiz outer gap between windows and screen edge
    uint32_t                        gapPoV;                         // vert outer gap between windows and screen edge
    float                           scratchpadWidthRatio;
    float                           scratchpadHeightRatio;

    int                             scrollerStructs;
    float                           scrollerDefaultProportion;
    float                           scrollerDefaultProportionSingle;
    int                             scrollerIgnoreProportionSingle;
    int                             scrollerFocusCenter;
    int                             scrollerPreferCenter;
    int                             focusCrossMonitor;
    int                             focusCrossTag;
    int                             exchangeCrossMonitor;
    int                             scratchpadCrossMonitor;
    int                             viewCurrentToBack;
    int                             noBorderWhenSingle;
    int                             noRadiusWhenSingle;
    int                             snapDistance;
    int                             enableFloatingSnap;
    int                             dragTileToTile;
    uint32_t                        cursorSize;
    uint32_t                        cursorHideTimeout;
    uint32_t                        swipeMinThreshold;
    int                             inhibitRegardlessOfVisibility;

    uint32_t                        borderPx;                       // border pixel of windows
    float                           rootColor[4];
    float                           borderColor[4];
    float                           focusColor[4];
    float                           maximizeScreenColor[4];
    float                           urgentColor[4];
    float                           scratchpadColor[4];
    float                           globalColor[4];
    float                           overlayColor[4];

    int                             overviewGapPi;                  // overview时 窗口与边缘 缝隙大小
    int                             overviewGapPo;                  // overview时 窗口与窗口 缝隙大小

    float                           fullscreenBG[4];

    int                             warpCursor;                     // Warp cursor to focused client
    int                             xWaylandPersistence;            // xWayland persistence
    int                             syncObjEnable;
    int                             adaptiveSync;
    int                             allowLockTransParent;
    double                          dragRefreshInterval;
    int                             allowTearing;
    int                             allowShortcutsInhibit;

    /* keyboard */
    char                            xkbRulesRules[256];
    char                            xkbRulesModel[256];
    char                            xkbRulesLayout[256];
    char                            xkbRulesVariant[256];
    char                            xkbRulesOptions[256];

    struct xkb_rule_names           xkbFallbackRules;
    struct xkb_rule_names           xkbDefaultRules;
    struct xkb_rule_names           xkbRules;
    int                             repeatRate;
    int                             repeatDelay;

    /* Trackpad */
    int                             disableTrackpad;
    int                             tapToClick;
    int                             tapAndDrag;
    int                             dragLock;
    int                             mouseNaturalScrolling;
    int                             trackpadNaturalScrolling;
    int                             disableWhileTyping;
    int                             leftHanded;
    int                             middleButtonEmulation;
    int                             singleScratchpad;
    int                             edgeScrollerPointerFocus;

    /* You can choose between:
     * LIBINPUT_CONFIG_SCROLL_NO_SCROLL
     * LIBINPUT_CONFIG_SCROLL_2FG
     * LIBINPUT_CONFIG_SCROLL_EDGE
     * LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
     */
    enum libinput_config_scroll_method  scrollMethod;

    uint32_t                            scrollButton;

    /* You can choose between:
     * LIBINPUT_CONFIG_CLICK_METHOD_NONE
     * LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
     * LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
     */
    enum libinput_config_click_method   clickMethod;

    /* You can choose between:
     * LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
     * LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
     * LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
     */
    uint32_t                            sendEventsMode;

    /* You can choose between:
     * LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
     * LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
     */
    enum libinput_config_accel_profile  accelProfile;
    double                              accelSpeed;

    /* You can choose between:
     * LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
     * LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
     */
    enum libinput_config_tap_button_map buttonMap;

    /* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
    float                               focusedOpacity;
    float                               unfocusedOpacity;
    int                                 borderRadius;
    int                                 borderRadiusLocationDefault;
    int                                 blur;
    int                                 blurLayer;
    int                                 blurOptimized;
    struct blur_data                    blurParams;
    int                                 blurParamsNumPasses;
    int                                 blurParamsRadius;
    float                               blurParamsNoise;
    float                               blurParamsBrightness;
    float                               blurParamsContrast;
    float                               blurParamsSaturation;

    int                                 shadows;
    int                                 shadowOnlyFloating;
    int                                 layerShadows;
    uint32_t                            shadowsSize;
    double                              shadowsBlur;
    int                                 shadowsPositionX;
    int                                 shadowsPositionY;
    float                               shadowsColor[4];
} GlobalConfig;

typedef int (*gFuncType) (const Arg*);

#endif // graceful_GRACEFUL_TYPES_H
