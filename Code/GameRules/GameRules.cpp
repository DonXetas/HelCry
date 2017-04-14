// We need StdAfx for CryEngine internal functions, which will be called/used
// Is needed for compiling trough CMake
#include "StdAfx.h"

// Include the Header file, which defines which functions and variables are used in the class
#include "GameRules/GameRules.h"

// We have to register our class so it will be recognized as our gamerules
// We are inherit from IEntityRegistrator, which gives us the functions to register the class
class CGameRulesRegistrator : public IEntityRegistrator
{
	virtual void Register() override
	{
		// Register our class "CGameRules" with the name "GameRules"
		CGamePlugin::RegisterEntityWithDefaultComponent<CGameRules>("GameRules");

		// Get the content of ConsoleVariable "sv_gamerulesdefault"
		ICVar* pDefaultGameRulesCVar = gEnv->pConsole->GetCVar("sv_gamerulesdefault");

		// To do this, we need the <IGameRulesSystem.h>
		// Get the current GameRulesSystem, so we can manipulate it
		auto* pGameRulesSystem = gEnv->pGameFramework->GetIGameRulesSystem();

		//register our class as the new Gamerules to the content of the ConsoleVariable
		pGameRulesSystem->RegisterGameRules(pDefaultGameRulesCVar->GetString(), "GameRules", false);
		pGameRulesSystem->AddGameRulesAlias(pDefaultGameRulesCVar->GetString(), pDefaultGameRulesCVar->GetString());
	}

	// Can be empty, but it has to be implemented because we are forced to implement it, from the parent
	// virtual void Unregister() = 0;
	// "= 0" forces us to implement this function, even if we do not need it
	virtual void Unregister() override
	{

	}
};

// Variable of the registrator, which handles the registration internally
CGameRulesRegistrator g_gameRulesRegistrator;

// If the game closes / the GameRules object gets destroyed, set the current GameRules to nothing, so we can change the rules without a problem
// i.e. from Singleplayer to Multiplayer
CGameRules::~CGameRules()
{
	gEnv->pGameFramework->GetIGameRulesSystem()->SetCurrentGameRules(nullptr);
}

// Initiate the GameRules
bool CGameRules::Init(IGameObject* _pGameObject)
{
	// In the Initfunction, always Set the gameobject
	SetGameObject(_pGameObject);

	// Set the current GameRules to this object, so if this object are the GameRules for Singleplayer, then SingleplayerRules will be set
	gEnv->pGameFramework->GetIGameRulesSystem()->SetCurrentGameRules(this);

	// In CryEngine everything goes over network, even if it is a singleplayermode, still bind this object to the network, so it will be used
	return _pGameObject->BindToNetwork();
}

// If Player is connected to the game
bool CGameRules::OnClientConnect(int _channelId, bool _isReset)
{
	// SpawnPlayer 

	return true;
}

// If Player is ready for the game
bool CGameRules::OnClientEnteredGame(int _channelId, bool _isReset)
{
	return true;
}

void CGameRules::OnClientDisconnect(int _channelId, EDisconnectionCause _cause, const char* _desc, bool _keepClient)
{
	// DespawnPlayer
}