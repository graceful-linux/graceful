//
// Created by dingjing on 2025/12/10.
//

#ifndef graceful_GRACEFUL_TYPES_H
#define graceful_GRACEFUL_TYPES_H
#include <stdint.h>
#include <xkbcommon/xkbcommon.h>

#define GRACEFUL_LOG_LEVEL_DEBUG        0
#define GRACEFUL_LOG_LEVEL_INFO         1
#define GRACEFUL_LOG_LEVEL_WARNING      2
#define GRACEFUL_LOG_LEVEL_ERROR        3
#define GRACEFUL_LOG_LEVEL_DIE          4

#define GRACEFUL_LOG_LEVEL_DEFAULT      GRACEFUL_LOG_LEVEL_WARNING

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
    uint32_t                    keyCode1;
    uint32_t                    keyCode2;
    uint32_t                    keyCode3;
} MultiKeyCode;

typedef struct
{
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
    int                         isTagsSilent;
    int                         isNamedScratchpad;
    int                         isSunGlobal;
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

typedef int (*gFuncType) (const Arg*);

#endif // graceful_GRACEFUL_TYPES_H
