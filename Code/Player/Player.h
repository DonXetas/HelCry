#pragma once

// Needed to register the Actor
#include "GamePlugin.h"

// Needed to use the features of the SimpleActor (Init and PostInit)
#include "ISimpleActor.h"

// Needed to set the GameOject in the Init function
#include <IGameObject.h>

// Needed to set the "Camera" in the UpdateView function
#include <IViewSystem.h>

// Define our class "Player", with this class we will control the player
// Our custom Player has to inherit from the Interface "ISimpleActor" to give us some of the predefined functions we need
// Our custom Player has to inherit from the Interface "IGameObjectView" to set the camera to the Player
// Our custom Player has to inherit from the Interface "IActionListener" to react to the input of the Player
class CPlayer : public CGameObjectExtensionHelper<CPlayer, ISimpleActor>, public IGameObjectView, public IActionListener
{
public:
	// Constructor
	CPlayer();

	// Deconstructor
	~CPlayer();

#pragma region
	
	// Bind to the network etc.
	virtual bool Init(IGameObject* _pGameObject) override;
	// Physicalize, activate actions etc.
	virtual void PostInit(IGameObject* _pGameObject) override;

#pragma endregion ISimpleActor

#pragma region

	// Update the "Camera" 
	virtual void UpdateView(SViewParams& _params) override;

	virtual void PostUpdateView(SViewParams& _params) override {};

#pragma endregion IGameObjectView

	// IActionListener
	// Do something on input
	virtual void OnAction(const ActionId& _action, int _activationMode, float _value) override;
};