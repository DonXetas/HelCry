// We need StdAfx for CryEngine internal functions, which will be called/used
// Is needed for compiling trough CMake
#include "StdAfx.h"

#include "Player.h"

// We have to register our class so it will be recognized as our Player
// We are inherit from IEntityRegistrator, which gives us the functions to register the class
class CPlayerRegistrator : public IEntityRegistrator
{
	virtual void Register() override
	{
		// When we want to spawn this Player, we have to pass the same string to the createfunction = "Player"
		CGamePlugin::RegisterEntityWithDefaultComponent<CPlayer>("Player");
	}

	virtual void Unregister() override
	{

	}
};

CPlayerRegistrator g_playerRegistrator;

CPlayer::CPlayer()
{
	
}

CPlayer::~CPlayer()
{
	// Remove this Actor from the system so we do not get any errors
	gEnv->pGameFramework->GetIActorSystem()->RemoveActor(GetEntityId());
}

// Bind to the network etc.
bool CPlayer::Init(IGameObject* _pGameObject)
{
	// In the Initfunction, always Set the gameobject
	SetGameObject(_pGameObject);

	// In CryEngine everything goes over network, even if it is a singleplayermode, still bind this object to the network, so it will be used
	return _pGameObject->BindToNetwork();
}

// Physicalize, activate actions etc.
void CPlayer::PostInit(IGameObject* _pGameObject)
{
	// Add this Actor to the system, so we can use/spawn this actor
	gEnv->pGameFramework->GetIActorSystem()->AddActor(GetEntityId(), this);
}