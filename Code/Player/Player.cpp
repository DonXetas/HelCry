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
	// Detach the "Camera" from the Player/GameObject
	GetGameObject()->ReleaseView(this);
	// Detach the listener for actions from the Player/GameObject 
	GetGameObject()->ReleaseActions(this);

	// Remove this Actor from the system so we do not get any errors
	gEnv->pGameFramework->GetIActorSystem()->RemoveActor(GetEntityId());
}

// Bind to the network etc.
bool CPlayer::Init(IGameObject* _pGameObject)
{
	// In the Initfunction, always Set the gameobject
	SetGameObject(_pGameObject);

	// Set this Vector 3D variables to Zero (0, 0, 0)
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

	// We want the player to have physics, therefore add the physicalizeparams with PE_LIVING which equals a Rigidbody, so we can collide and move the player
	// The type "LIVING" gives the player specific attributes like "pe_player_dimensions" or "pe_player_dynamics" and he is beeing moved by "pe_action_move"
	SEntityPhysicalizeParams entityPhysicalizeParams;
	entityPhysicalizeParams.type = PE_LIVING;
	entityPhysicalizeParams.mass = 90.0f;

	// PlayerDimensions
	pe_player_dimensions playerDimensions;

	// Prefer usage of a cylinder instead of capsule
	playerDimensions.bUseCapsule = 0;

	// Specify the size of our cylinder
	playerDimensions.sizeCollider = Vec3(0.45f, 0.45f, 0.45f);

	// Keep pivot at the player's feet (defined in player geometry) 
	playerDimensions.heightPivot = 0.f;
	// Offset collider upwards
	playerDimensions.heightCollider = 1.f;

	// If the player is 0.004 meter above the ground, it will not be grounded anymore (needed for flying/jumping actions)
	playerDimensions.groundContactEps = 0.004f;

	// Get the reference to the settings we just set, so they can be applied later in the physics
	entityPhysicalizeParams.pPlayerDimensions = &playerDimensions;

	// PlayerDynamics
	pe_player_dynamics dynamics;

	// We do not want to allow aircontrol so set it to 0, is going from 0 to 1
	dynamics.kAirControl = 0.0f;

	// Specify the mass of the PlayerDynamics
	dynamics.mass = entityPhysicalizeParams.mass;

	// Get the reference to the settings we just set, so they can be applied later in the physics
	entityPhysicalizeParams.pPlayerDynamics = &dynamics;

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
	// Get the current transformation in the world
	Matrix34 playerTransform = GetEntity()->GetWorldTM();

	// from the current playerposition create the angles for the rotation
	Ang3 yawPitchRoll = CCamera::CreateAnglesYPR(Matrix33(playerTransform));
	
	// Define the rotation on the X Axis with the value we got from the action multiplied by the time passed by and the rotatespeed for smooth rotation
	yawPitchRoll.x += m_mouseDeltaRotation.x * _ctx.fFrameTime * 0.05f;

	// Define the rotation on the Y Axis with the value we got from the action multiplied by the time passed by and the rotatespeed for smooth rotation
	yawPitchRoll.y += m_mouseDeltaRotation.y * _ctx.fFrameTime * 0.05f;
	yawPitchRoll.y = clamp_tpl(yawPitchRoll.y, -(float)g_PI * 0.5f, (float)g_PI * 0.5f);

	// We do NOT want to rotate around the Z axis
	yawPitchRoll.z = 0;
	
	// Set the rotation of the transform by the Orientation of the angles we calculated above
	playerTransform.SetRotation33(CCamera::CreateOrientationYPR(yawPitchRoll));

	// Set this rotation, so the player actually rotates
	GetEntity()->SetRotation(Quat(yawPitchRoll));

	// Set the rotation to (0, 0, 0) so we do not rotate the camera, even if we stopped the input
	m_mouseDeltaRotation = ZERO;

	// Get the PhysicalEntity of the player
	IPhysicalEntity *pPhysicalEntity = GetEntity()->GetPhysicalEntity();

	// If the PhysicalEntity is available get the status of the retrieved PhysicalEntity into the variable "livingStatus"
	if (pPhysicalEntity != nullptr)
	{
		pe_status_living livingStatus;

		if (pPhysicalEntity->GetStatus(&livingStatus) != 0)
		{
			// If we are not above the 0,004 meter we set in the initialization perform movement
			if (!livingStatus.bFlying)
			{
				// Set the move properties, we need to move the Player with
				pe_action_move moveAction;

				// Possible 1 where the speed applies instantly and 2 where the speed adds to the current velocity
				moveAction.iJump = 2;
				// Get the movedirection based on the current rotation multiplied by the movedirection (x or y Axis), the speed and the time, so we have a smooth movement
				moveAction.dir = GetEntity()->GetWorldRotation() * m_moveDirection * 20.0f * _ctx.fFrameTime;

				// Perform the action and let the Player move
				pPhysicalEntity->Action(&moveAction);
			}
		}
	}
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
		CryLog("m_moveDirection.x changed to the value: %f", m_moveDirection.x);
		return;
	}
	// If the string is "moveright", set the movedirection on the X Axis plus the value
	else if (!strcmp(_action.c_str(), "moveright"))
	{
		m_moveDirection.x = _value;
		CryLog("m_moveDirection.x changed to the value: %f", m_moveDirection.x);
		return;
	}
	// If the string is "moveforward", set the movedirection on the Y Axis plus the value
	else if (!strcmp(_action.c_str(), "moveforward"))
	{
		m_moveDirection.y = _value;
		CryLog("m_moveDirection.y changed to the value: %f", m_moveDirection.y);
		return;
	}
	// If the string is "moveback", set the movedirection on the Y Axis minus the value
	else if (!strcmp(_action.c_str(), "moveback"))
	{
		m_moveDirection.y = -_value;
		CryLog("m_moveDirection.y changed to the value: %f", m_moveDirection.y);
		return;
	}
	// If the string is "mouse_rotateyaw", set the m_mouseDeltaRotation on the X Axis minus the value
	else if (!strcmp(_action.c_str(), "mouse_rotateyaw"))
	{
		m_mouseDeltaRotation.x -= _value;
		return;
	}
	// If the string is "mouse_rotatepitch", set the m_mouseDeltaRotation on the Y Axis minus the value
	else if (!strcmp(_action.c_str(), "mouse_rotatepitch"))
	{
		m_mouseDeltaRotation.y -= _value;
		return;
	}
}