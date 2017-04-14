#pragma once
// Header files surrounded with "" are custom files, we can modify or are our files
// Header files surrounded with <> are internal files, we can't modify them nor are they our files

// We need StdAfx for CryEngine internal functions, which will be called/used
#include "StdAfx.h"

// We need this file, to register our classes inside the project, so they are getting compiled and we can use them inside the project
#include "GamePlugin.h"

// We need this file, for internal functions of the registrationsystem
#include <IGameRulesSystem.h>

// Define our class "GameRules", with this class we will control all rules in the game
class CGameRules
{

};