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

	m_moveDirection = ZERO;
	m_mouseDeltaRotation = ZERO;

	// In CryEngine everything goes over network, even if it is a singleplayermode, still bind this object to the network, so it will be used
	return _pGameObject->BindToNetwork();
}

// Physicalize, activate actions etc.
void CPlayer::PostInit(IGameObject* _pGameObject)
{
	// Add this Actor to the system, so we can use/spawn this actor
	gEnv->pGameFramework->GetIActorSystem()->AddActor(GetEntityId(), this);

	// Set the actors position in the world
	// Commented out due to overwriting the spawnposition in the GameRules.cpp
	// GetEntity()->SetWorldTM(Matrix34::Create(Vec3(1, 1, 1), IDENTITY, Vec3(50, 50, 50)));

	// Load the Mesh from sphere into the player
	// Loading a wrong Mesh, will not physicalize the player
	GetEntity()->LoadGeometry(0, "objects/sphere.cgf");

	// We want the player to have physics, therefore add the physicalizeparams with PE_RIGID which equals a Rigidbody, so we can collide and move the player
	SEntityPhysicalizeParams entityPhysicalizeParams;
	entityPhysicalizeParams.type = PE_RIGID;
	entityPhysicalizeParams.mass = 100.0f;

	// Apply the properties from above to the Player
	GetEntity()->Physicalize(entityPhysicalizeParams);

	// Attach the "Camera" to the Player/GameObject
	GetGameObject()->CaptureView(this);

	// Attach the Capturelistener to the Actor, so the Actor reacts on Keyinput
	GetGameObject()->CaptureActions(this);

	// Get the ActionMapManager, so we can initialize the Keyinput and the profile used for the character
	IActionMapManager* pActionMapManager = gEnv->pGameFramework->GetIActionMapManager();

	// Initialize the given xml file
	pActionMapManager->InitActionMaps("libs/config/DefaultProfile.xml");
	// Enable the file so we can activate a profile inside the file
	pActionMapManager->Enable(true);
	// Enable the profile inside the file 
	pActionMapManager->EnableActionMap("simple", true);

	// Get the keybindings inside the profile
	IActionMap* pActionMap = pActionMapManager->GetActionMap("simple");

	// If the profile is available set this actor as the listener, so the actor can react to the input
	if (pActionMap != nullptr)
	{
		pActionMap->SetActionListener(GetEntityId());
	}

	// Enable the update function of this gameObject/Actor, without this, Update will not be called
	_pGameObject->EnableUpdateSlot(this, 0);
}

// Do something every frame
void CPlayer::Update(SEntityUpdateContext& _ctx, int _updateSlot)
{
	// Get the current position in the world
	Matrix34 playerTransform = GetEntity()->GetWorldTM();

	// Get the current movedirection from the input and multiply it by 20, it will be variable in the future
	Vec3 moveDirection = m_moveDirection * 20.0f;

	// Add the distance which will be traveled in the passed time, relative to the rotation of the Actor/Player
	playerTransform.AddTranslation(GetEntity()->GetWorldRotation() * moveDirection * _ctx.fFrameTime);

	//Set this position, so the player actually moves
	GetEntity()->SetWorldTM(playerTransform);
}

//Update the "Camera"
void CPlayer::UpdateView(SViewParams& _params)
{
	// Get the position behind the Player
	_params.position = GetEntity()->GetWorldPos() - GetEntity()->GetWorldRotation().GetColumn1() * 5;

	// Rotate the "Camera" to the Player
	_params.rotation = Quat::CreateRotationVDir((GetEntity()->GetWorldPos() - _params.position).GetNormalized());

	// Set the Field of View to 75 Degrees
	_params.fov = DEG2RAD(75);
}

// Do something on input
void CPlayer::OnAction(const ActionId& _action, int _activationMode, float _value)
{
	// strcmp = string compare
	// compare the string inside of "_action" to "jump"
	// if they are equal it returns "0" so we have to compare the result to 0 and do the following: 
	// (strcmp(_action.c_str(), "jump") == 0)
	// (!strcmp(_action.c_str(), "jump")) works aswell
	if (!strcmp(_action.c_str(), "jump"))
	{
		// Define an impulse, which will be attached to the Player, so he can jump and the player gets forced
		pe_action_impulse physicImpulse;
		physicImpulse.impulse = Vec3(0, 0, 500);

		// Get the PhysicalEntity from the Actor, so we can call the physicsfunction
		IPhysicalEntity* pPhysicalEntity = GetEntity()->GetPhysicalEntity();

		// If the PhysicalEntity is available then use the impulse on the Player and let him jump
		if (pPhysicalEntity != nullptr)
		{
			pPhysicalEntity->Action(&physicImpulse);
		}
	}
	// If the string is "moveleft", set the movedirection on the X Axis minus the value
	else if (!strcmp(_action.c_str(), "moveleft"))
	{
		m_moveDirection.x = -_value;
	}
	// If the string is "moveright", set the movedirection on the X Axis plus the value
	else if (!strcmp(_action.c_str(), "moveright"))
	{
		m_moveDirection.x = _value;
	}
	// If the string is "moveforward", set the movedirection on the Y Axis plus the value
	else if (!strcmp(_action.c_str(), "moveforward"))
	{
		m_moveDirection.y = _value;
	}
	// If the string is "moveback", set the movedirection on the Y Axis minus the value
	else if (!strcmp(_action.c_str(), "moveback"))
	{
		m_moveDirection.y = -_value;
	}
}