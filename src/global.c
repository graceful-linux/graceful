//
// Created by dingjing on 2025/12/10.
//

#include "global.h"

#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "keymap.h"


Config                      gConfig;
const char*                 gConfigFilePath = NULL;
int                         gLogLevel = GRACEFUL_LOG_LEVEL_WARNING;
GlobalConfig                gGlobalConfig;


void init_global_config(void)
{
    memset(&gGlobalConfig, 0, sizeof(GlobalConfig));

    gGlobalConfig.animations                        = true;
    gGlobalConfig.layerAnimations                   = true;
    gGlobalConfig.animationFadeIn                   = true;
    gGlobalConfig.animationFadeOut                  = true;
    gGlobalConfig.focusOnActivate                   = true;
    gGlobalConfig.newIsMaster                       = true;
    gGlobalConfig.centerWhenSingleStack             = true;
    gGlobalConfig.enableHotArea                     = true;

    gGlobalConfig.ovTabMode                         = false;
    gGlobalConfig.capslock                          = false;
    gGlobalConfig.numLockOn                         = false;
    gGlobalConfig.centerMasterOverspread            = false;

    gGlobalConfig.zoomInitialRatio                  = 0.30F;
    gGlobalConfig.zoomEndRatio                      = 0.80F;
    gGlobalConfig.fadeinBeginOpacity                = 0.50F;
    gGlobalConfig.fadeoutBeginOpacity               = 0.50F;

    gGlobalConfig.scratchpadWidthRatio              = 0.80F;
    gGlobalConfig.scratchpadHeightRatio             = 0.90F;

    gGlobalConfig.scrollerDefaultProportion         = 0.50F;
    gGlobalConfig.scrollerDefaultProportionSingle   = 1.00F;

    gGlobalConfig.defaultMFact                      = 0.55F;

    gGlobalConfig.smartGaps                         = 0;
    gGlobalConfig.sloppyFocus                       = 1;
    gGlobalConfig.defaultNMaster                    = 1;

    gGlobalConfig.scrollerStructs                   = 20;

    gGlobalConfig.gapPiH                            = 5;
    gGlobalConfig.gapPiV                            = 5;
    gGlobalConfig.gapPoH                            = 10;
    gGlobalConfig.gapPoV                            = 10;

    gGlobalConfig.hotAreaSize                       = 10;

    gGlobalConfig.axisBindApplyTimeout              = 100;

    gGlobalConfig.animationDurationTag              = 300;
    gGlobalConfig.animationDurationMove             = 500;
    gGlobalConfig.animationDurationOpen             = 400;
    gGlobalConfig.animationDurationClose            = 300;
    gGlobalConfig.animationDurationFocus            = 0;

    gGlobalConfig.scrollerIgnoreProportionSingle    = 0;
    gGlobalConfig.scrollerFocusCenter               = 0;
    gGlobalConfig.scrollerPreferCenter              = 0;
    gGlobalConfig.focusCrossMonitor                 = 0;
    gGlobalConfig.focusCrossTag                     = 0;
    gGlobalConfig.exchangeCrossMonitor              = 0;
    gGlobalConfig.scratchpadCrossMonitor            = 0;
    gGlobalConfig.viewCurrentToBack                 = 1;
    gGlobalConfig.noBorderWhenSingle                = 0;
    gGlobalConfig.noRadiusWhenSingle                = 0;
    gGlobalConfig.snapDistance                      = 30;
    gGlobalConfig.enableFloatingSnap                = 0;
    gGlobalConfig.dragTileToTile                    = 0;
    gGlobalConfig.cursorSize                        = 24;
    gGlobalConfig.cursorHideTimeout                 = 0;
    gGlobalConfig.swipeMinThreshold                 = 1;
    gGlobalConfig.inhibitRegardlessOfVisibility     = 0;
    gGlobalConfig.borderPx                          = 4;

    gGlobalConfig.overviewGapPi                     = 5;
    gGlobalConfig.overviewGapPo                     = 50;

    gGlobalConfig.warpCursor                        = 1;
    gGlobalConfig.xWaylandPersistence               = 1;
    gGlobalConfig.syncObjEnable                     = 0;
    gGlobalConfig.adaptiveSync                      = 0;
    gGlobalConfig.allowLockTransParent              = 0;

    gGlobalConfig.repeatRate                        = 25;
    gGlobalConfig.repeatDelay                       = 600;

    gGlobalConfig.disableTrackpad                   = 0;
    gGlobalConfig.tapToClick                        = 1;
    gGlobalConfig.tapAndDrag                        = 1;
    gGlobalConfig.dragLock                          = 1;
    gGlobalConfig.mouseNaturalScrolling             = 0;
    gGlobalConfig.trackpadNaturalScrolling          = 0;
    gGlobalConfig.disableWhileTyping                = 1;
    gGlobalConfig.leftHanded                        = 0;
    gGlobalConfig.middleButtonEmulation             = 0;
    gGlobalConfig.singleScratchpad                  = 1;
    gGlobalConfig.edgeScrollerPointerFocus          = 1;

    gGlobalConfig.scrollButton                      = 274;

    gGlobalConfig.borderRadius                      = 0;

    gGlobalConfig.shadows                           = 0;
    gGlobalConfig.shadowOnlyFloating                = 1;
    gGlobalConfig.layerShadows                      = 10;
    gGlobalConfig.shadowsPositionX                  = 0;
    gGlobalConfig.shadowsPositionY                  = 0;

    gGlobalConfig.blur                              = 0;
    gGlobalConfig.blurLayer                         = 0;
    gGlobalConfig.blurOptimized                     = 1;

    gGlobalConfig.blurParamsNumPasses               = 1;
    gGlobalConfig.blurParamsRadius                  = 5;
    gGlobalConfig.blurParamsNoise                   = 0.02F;
    gGlobalConfig.blurParamsBrightness              = 0.90F;
    gGlobalConfig.blurParamsContrast                = 0.90F;
    gGlobalConfig.blurParamsSaturation              = 1.20F;

    gGlobalConfig.dragRefreshInterval               = 30.0F;

    gGlobalConfig.animationCurveTag[0]              = 1.46F;
    gGlobalConfig.animationCurveTag[1]              = 1.00F;
    gGlobalConfig.animationCurveTag[2]              = 0.29F;
    gGlobalConfig.animationCurveTag[3]              = 0.99F;

    gGlobalConfig.animationCurveMove[0]             = 1.46F;
    gGlobalConfig.animationCurveMove[1]             = 1.00F;
    gGlobalConfig.animationCurveMove[2]             = 0.29F;
    gGlobalConfig.animationCurveMove[3]             = 0.99F;

    gGlobalConfig.animationCurveOpen[0]             = 1.46F;
    gGlobalConfig.animationCurveOpen[1]             = 1.00F;
    gGlobalConfig.animationCurveOpen[2]             = 0.29F;
    gGlobalConfig.animationCurveOpen[3]             = 0.99F;

    gGlobalConfig.animationCurveClose[0]            = 1.46F;
    gGlobalConfig.animationCurveClose[1]            = 1.00F;
    gGlobalConfig.animationCurveClose[2]            = 0.29F;
    gGlobalConfig.animationCurveClose[3]            = 0.99F;

    gGlobalConfig.animationCurveFocus[0]            = 1.46F;
    gGlobalConfig.animationCurveFocus[1]            = 1.00F;
    gGlobalConfig.animationCurveFocus[2]            = 0.29F;
    gGlobalConfig.animationCurveFocus[3]            = 0.99F;

    gGlobalConfig.accelSpeed                        = 0.00F;

    gGlobalConfig.focusedOpacity                    = 1.00F;
    gGlobalConfig.unfocusedOpacity                  = 1.00F;

    gGlobalConfig.tagAnimationDirection             = HORIZONTAL;
    gGlobalConfig.allowTearing                      = TEARING_DISABLED;
    gGlobalConfig.borderRadiusLocationDefault       = CORNER_LOCATION_ALL;
    gGlobalConfig.allowShortcutsInhibit             = SHORTCUTS_INHIBIT_ENABLE;
    gGlobalConfig.scrollMethod                      = LIBINPUT_CONFIG_SCROLL_2FG;
    gGlobalConfig.buttonMap                         = LIBINPUT_CONFIG_TAP_MAP_LRM;
    gGlobalConfig.sendEventsMode                    = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;
    gGlobalConfig.accelProfile                      = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
    gGlobalConfig.clickMethod                       = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

    strncpy(gGlobalConfig.animationTypeOpen,        "slide", sizeof(gGlobalConfig.animationTypeOpen) - 1);
    strncpy(gGlobalConfig.animationTypeClose,       "slide", sizeof(gGlobalConfig.animationTypeClose) - 1);
    strncpy(gGlobalConfig.layerAnimationTypeOpen,   "slide", sizeof(gGlobalConfig.layerAnimationTypeOpen) - 1);
    strncpy(gGlobalConfig.layerAnimationTypeClose,  "slide", sizeof(gGlobalConfig.layerAnimationTypeClose) - 1);

    gGlobalConfig.fullscreenBG[0]           = 0.10F;
    gGlobalConfig.fullscreenBG[1]           = 0.10F;
    gGlobalConfig.fullscreenBG[2]           = 0.10F;
    gGlobalConfig.fullscreenBG[3]           = 1.00F;

    gGlobalConfig.rootColor[0]              = COLOR0(0x323232ff);
    gGlobalConfig.rootColor[1]              = COLOR1(0x323232ff);
    gGlobalConfig.rootColor[2]              = COLOR2(0x323232ff);
    gGlobalConfig.rootColor[3]              = COLOR3(0x323232ff);

    gGlobalConfig.focusColor[0]             = COLOR0(0xc66b25ff);
    gGlobalConfig.focusColor[1]             = COLOR1(0xc66b25ff);
    gGlobalConfig.focusColor[2]             = COLOR2(0xc66b25ff);
    gGlobalConfig.focusColor[3]             = COLOR3(0xc66b25ff);

    gGlobalConfig.urgentColor[0]            = COLOR0(0xad401fff);
    gGlobalConfig.urgentColor[1]            = COLOR1(0xad401fff);
    gGlobalConfig.urgentColor[2]            = COLOR2(0xad401fff);
    gGlobalConfig.urgentColor[3]            = COLOR3(0xad401fff);

    gGlobalConfig.borderColor[0]            = COLOR0(0x444444ff);
    gGlobalConfig.borderColor[1]            = COLOR1(0x444444ff);
    gGlobalConfig.borderColor[2]            = COLOR2(0x444444ff);
    gGlobalConfig.borderColor[3]            = COLOR3(0x444444ff);

    gGlobalConfig.globalColor[0]            = COLOR0(0xb153a7ff);
    gGlobalConfig.globalColor[1]            = COLOR1(0xb153a7ff);
    gGlobalConfig.globalColor[2]            = COLOR2(0xb153a7ff);
    gGlobalConfig.globalColor[3]            = COLOR3(0xb153a7ff);

    gGlobalConfig.overlayColor[0]           = COLOR0(0x14a57cff);
    gGlobalConfig.overlayColor[1]           = COLOR1(0x14a57cff);
    gGlobalConfig.overlayColor[2]           = COLOR2(0x14a57cff);
    gGlobalConfig.overlayColor[3]           = COLOR3(0x14a57cff);

    gGlobalConfig.scratchpadColor[0]        = COLOR0(0x516c93ff);
    gGlobalConfig.scratchpadColor[1]        = COLOR1(0x516c93ff);
    gGlobalConfig.scratchpadColor[2]        = COLOR2(0x516c93ff);
    gGlobalConfig.scratchpadColor[3]        = COLOR3(0x516c93ff);

    gGlobalConfig.maximizeScreenColor[0]    = COLOR0(0x89aa61ff);
    gGlobalConfig.maximizeScreenColor[1]    = COLOR1(0x89aa61ff);
    gGlobalConfig.maximizeScreenColor[2]    = COLOR2(0x89aa61ff);
    gGlobalConfig.maximizeScreenColor[3]    = COLOR3(0x89aa61ff);

    gGlobalConfig.shadowsColor[0]           = COLOR0(0x000000ff);
    gGlobalConfig.shadowsColor[1]           = COLOR1(0x000000ff);
    gGlobalConfig.shadowsColor[2]           = COLOR2(0x000000ff);
    gGlobalConfig.shadowsColor[3]           = COLOR3(0x000000ff);

    gGlobalConfig.xkbFallbackRules.layout   = gLayoutMappings[US].abbr;
}

