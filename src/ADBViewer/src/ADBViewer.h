
#pragma once

#if !defined(__APPALLDEFINE_H)
#define __APPALLDEFINE_H 1

#include "loader.h"

#define __W_default 640
#define __H_default 400
#define __MENU_H_dragable 330

#define ID_CMD_POP_MENU0 20180
#define ID_CMD_POP_MENU1 20181
#define ID_CMD_POP_MENU2 20182
#define ID_CMD_POP_MENU3 20183
#define ID_CMD_POP_MENU4 20184
#define ID_CMD_POP_MENU5 20185

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

#  include "App/AppConfig.h"
#  include "App/plugin/AppPluginManager.h"
#  include "App/gui/gui.h"
#  include "App/AppVideo.h"
#  include "App/AppMenuBar.h"
#  include "App/AppMenuPopUp.h"
#  include "App/App.h"

#endif
