#pragma once

#include "ISimpleActor.h"

#include "GamePlugin.h"

#include <IGameObject.h>

// Define our class "Player", with this class we will control the player
// Our custom Player has to inherit from the Interface "ISimpleActor" to give us some of the predefined functions we need
class CPlayer : public CGameObjectExtensionHelper<CPlayer, ISimpleActor>
{
public:
	// Constructor
	CPlayer();

	// Deconstructor
	~CPlayer();

	// Bind to the network etc.
	virtual bool Init(IGameObject* _pGameObject) override;

	// Physicalize, activate actions etc.
	virtual void PostInit(IGameObject* _pGameObject) override;
};