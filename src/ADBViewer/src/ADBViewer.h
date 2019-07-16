
#pragma once

#if !defined(__APPALLDEFINE_H)
#define __APPALLDEFINE_H 1

#include "loader.h"

#define __W_default 640
#define __H_default 400

#define ID_CMD_POP_MENU0 20180
#define ID_CMD_POP_MENU1 20181  // Install APK
#define ID_CMD_POP_MENU2 20182  // Stop ADB connect, send event ID_CMD_POP_MENU98
#define ID_CMD_POP_MENU3 20183
#define ID_CMD_POP_MENU4 20184  // Save Screen to default name
#define ID_CMD_POP_MENU5 20185  // Save Screen to select user dialog name
#define ID_CMD_POP_MENU6 20186  // Editor LUA script - Start/Stop
#define ID_CMD_POP_MENU7 20187  // Editor LUA script - cancel and clear pixels list
#define ID_CMD_POP_MENU8 20188  // Editor LUA script - add endpoint action
#define ID_CMD_POP_MENU9 20189  // Editor LUA script - Type: SCR_CHECK_AND_CLICK
#define ID_CMD_POP_MENU10 20190 // Editor LUA script - Type:SCR_CLICK_ONLY

#define ID_CMD_POP_MENU11 20191
#define ID_CMD_POP_MENU12 20192
#define ID_CMD_POP_MENU13 20193
#define ID_CMD_POP_MENU14 20194

#define ID_CMD_POP_MENU15 20195
#define ID_CMD_POP_MENU16 20196
#define ID_CMD_POP_MENU17 20197
#define ID_CMD_POP_MENU18 20198
#define ID_CMD_POP_MENU19 20199
#define ID_CMD_POP_MENU20 20200
#define ID_CMD_POP_MENU21 20201
#define ID_CMD_POP_MENU22 20202 // Copy Screen to clipboard
#define ID_CMD_POP_MENU23 20203 // Display rotate menu (current status disabled)
#define ID_CMD_POP_MENU24 20204 // Display Terminal open/close (trigger)
#define ID_CMD_POP_MENU25 20205 // Display Terminal close (is resize (internal))

#define ID_CMD_POP_MENU26 20206 // Change cursor (SDL_SYSTEM_CURSOR_ARROW)
#define ID_CMD_POP_MENU27 20207 // Change cursor (SDL_SYSTEM_CURSOR_HAND)

#define ID_CMD_POP_MENU28 20208 // Device info
#define ID_CMD_POP_MENU29 20209 // Version Information

#define ID_CMD_POP_MENU30 20300 // Android ADB command ID_CMD_POP_MENU30 - ID_CMD_POP_MENU40
#define ID_CMD_POP_MENU31 20301
#define ID_CMD_POP_MENU32 20302
#define ID_CMD_POP_MENU33 20303
#define ID_CMD_POP_MENU34 20304
#define ID_CMD_POP_MENU35 20305
#define ID_CMD_POP_MENU36 20306
#define ID_CMD_POP_MENU37 20307
#define ID_CMD_POP_MENU38 20308
#define ID_CMD_POP_MENU39 20309
#define ID_CMD_POP_MENU40 20310

#define ID_CMD_POP_MENU50 20311 // Android ADB command 2 x SDLK_PAGEUP = Notify open/expand
#define ID_CMD_POP_MENU51 20312 // Android ADB command 2 x SDLK_PAGEDOWN = Notify close/collapse

#define ID_CMD_POP_MENU97 20401 // Connect device         - MenuBar event
#define ID_CMD_POP_MENU98 20402 // Disconnect device      - MenuBar event
#define ID_CMD_POP_MENU99 20403 // Restart connect device - MenuBar event

#define ID_CMD_POP_MENU100 20501 // Input text box start
#define ID_CMD_POP_MENU101 20502 // Input text box stop ???

#define ID_CMD_POP_MENU102 20503 // Change menu enable/disable

#define ID_CMD_POP_MENU210 40010  // Terminal - indent from bottom edge (40010 - 40050)
#define ID_CMD_POP_MENU220 40020
#define ID_CMD_POP_MENU230 40030
#define ID_CMD_POP_MENU240 40040
#define ID_CMD_POP_MENU250 40050


#define ID_CMD_MSGBAR     49999

enum MgrType
{
    MGR_MAIN,
    MGR_MENU
};

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#  include "../../ADBViewerDLL/src/ResManager.h"
using Resources::ResManager;

#  include "../../ADBDriverDLL/src/ADBDriver.h"
using GameDev::ADBDriver;

#  include "App/AppTimer.h"
#  include "App/AppConfig.h"
#  include "App/AppMessageQueue.h"
#  include "App/AppSysDialog.h"
#  include "App/plugin/AppPluginManager.h"
#  include "App/gui/gui.h"
#  include "App/AppMessageBar.h"
#  include "App/AppCursor.h"
#  include "App/AppScreen.h"
#  include "App/AppEditor.h"
#  include "App/AppAnimationBender.h"
#  include "App/AppVideo.h"
#  include "App/AppMenuBar.h"
#  include "App/AppMenuPopUp.h"
#  include "App/AppInputText.h"
#  include "App/AppTerminalPage.h"
#  include "App/AppTerminalPageNuber.h"
#  include "App/AppTerminalInput.h"
#  include "App/AppTerminalOutput.h"
#  include "App/AppTerminal.h"
#  include "App/App.h"

#endif
