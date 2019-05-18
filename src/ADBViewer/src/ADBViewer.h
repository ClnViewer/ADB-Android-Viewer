
#pragma once

#if !defined(__APPALLDEFINE_H)
#define __APPALLDEFINE_H 1

#include "loader.h"

#define __W_default 640
#define __H_default 400
#define __MENU_H_dragable 330

enum MgrType
{
    MGR_MAIN,
    MGR_MENU
};

#  include "../../ADBViewerDLL/src/ResManager.h"
using Resources::ResManager;

#  include "../../ADBDriverDLL/src/ADBDriver.h"
using GameDev::ADBDriver;

#  include "App/AppMenu.h"
#  include "App/App.h"

#endif