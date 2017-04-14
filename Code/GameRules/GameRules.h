#pragma once
// Header files surrounded with "" are custom files, we can modify or are our files
// Header files surrounded with <> are internal files, we can't modify them nor are they our files

// We need this file, to register our classes inside the project, so they are getting compiled and we can use them inside the project
#include "GamePlugin.h"

// We need this file, for internal functions of the registrationsystem
#include <IGameRulesSystem.h>

// Define our class "GameRules", with this class we will control all rules in the game
// Our custom GameRules have to inherit from the Interface "IGameRules" to give us some of the predefined functions we need
class CGameRules : public CGameObjectExtensionHelper<CGameRules, IGameRules>
{
public: 
	virtual ~CGameRules();

	//IGameObjectExtension
#pragma region
	virtual void PostInit(IGameObject* _pGameObject) override {};
	virtual void InitClient(int _channelId) override {};
	virtual void PostInitClient(int _channelId) override {};
	virtual bool ReloadExtension(IGameObject* _pGameObject, const SEntitySpawnParams& _params) override { return false; };
	virtual void PostReloadExtension(IGameObject* _pGameObject, const SEntitySpawnParams& _params) override {};
	virtual void FullSerialize(TSerialize _ser) override {};
	virtual bool NetSerialize(TSerialize _ser, EEntityAspects _aspect, uint8 _profile, int _pflags) override { return true; };
	virtual NetworkAspectType GetNetSerializeAspects() { return eEA_All; }
	virtual void PostSerialize() override {};
	virtual void                 SerializeSpawnInfo(TSerialize _ser) override {};
	virtual ISerializableInfoPtr GetSpawnInfo() override { return nullptr; };
	virtual void Update(SEntityUpdateContext& _ctx, int _updateSlot) override {};
	virtual void HandleEvent(const SGameObjectEvent& _event) override {};
	virtual void GameSerialize(TSerialize _ser) { FullSerialize(_ser); };
	virtual void SetChannelId(uint16 _id) override {};
	virtual void SetAuthority(bool _auth) override {};
	virtual const void* GetRMIBase() const override { return 0; };
	virtual void PostUpdate(float _frameTime) override {};
	virtual void PostRemoteSpawn() override {};
#pragma endregion Functions we do not use, but we had to implement these because they are pure virtual given by the inherited interface

	// Functions we use from the inherited interface
	virtual bool Init(IGameObject* _pGameObject) override;
	//~IGameObjectExtension

	//IGameRules
#pragma region
	virtual bool ShouldKeepClient(int _channelId, EDisconnectionCause _cause, const char* _desc) const override { return true; }
	virtual void PrecacheLevel() override {}
	virtual void PrecacheLevelResource(const char* _resourceName, EGameResourceType _resourceType) override {}
	virtual XmlNodeRef FindPrecachedXmlFile(const char* _sFilename) override { return 0; }
	virtual void OnConnect(struct INetChannel* _pNetChannel) override {}
	virtual void OnDisconnect(EDisconnectionCause _cause, const char* _desc) override {}
	virtual void OnEntitySpawn(IEntity* _pEntity) override {}
	virtual void OnEntityRemoved(IEntity* _pEntity) override {}
	virtual void OnEntityReused(IEntity* _pEntity, SEntitySpawnParams& _params, EntityId _prevId) override {}
	virtual void SendTextMessage(ETextMessageType _type, const char* _msg, uint32 _to = eRMI_ToAllClients, int _channelId = -1,
		const char* _p0 = 0, const char* _p1 = 0, const char* _p2 = 0, const char* _p3 = 0) override {}
	virtual void SendChatMessage(EChatMessageType type, EntityId sourceId, EntityId targetId, const char* msg) override {}
	virtual void ClientHit(const HitInfo& _hitInfo) override {}
	virtual void ServerHit(const HitInfo& _hitInfo) override {}
	virtual int GetHitTypeId(const uint32 _crc) const override { return 0; }
	virtual int GetHitTypeId(const char* _type) const override { return 0; }
	virtual const char* GetHitType(int _id) const override { return ""; }
	virtual void OnVehicleDestroyed(EntityId _id) override {}
	virtual void OnVehicleSubmerged(EntityId _id, float _ratio) override {}
	virtual bool CanEnterVehicle(EntityId _playerId) override { return true; }
	virtual void CreateEntityRespawnData(EntityId _entityId) override {}
	virtual bool HasEntityRespawnData(EntityId _entityId) const override { return true; }
	virtual void ScheduleEntityRespawn(EntityId _entityId, bool _unique, float _timer) override {}
	virtual void AbortEntityRespawn(EntityId _entityId, bool _destroyData) override {}
	virtual void ScheduleEntityRemoval(EntityId _entityId, float _timer, bool _visibility) override {}
	virtual void AbortEntityRemoval(EntityId _entityId) override {}
	virtual void AddHitListener(IHitListener* _pHitListener) override {}
	virtual void RemoveHitListener(IHitListener* _pHitListener) override {}
	virtual bool OnCollision(const SGameCollision& _event) override { return true; }
	virtual void OnCollision_NotifyAI(const EventPhys* _pEvent) override {}
	virtual void ShowStatus() override {}
	virtual bool IsTimeLimited() const override { return true; }
	virtual float GetRemainingGameTime() const override { return 0.0f; }
	virtual void SetRemainingGameTime(float _seconds) override {}
	virtual void ClearAllMigratingPlayers(void) override {}
	virtual EntityId SetChannelForMigratingPlayer(const char* _name, uint16 _channelID) override { return INVALID_ENTITYID; }
	virtual void StoreMigratingPlayer(IActor* _pActor) override {}
	virtual const char* GetTeamName(int _teamId) const override { return ""; }
#pragma endregion Functions we do not use, but we had to implement these because they are pure virtual given by the inherited interface

#pragma region
	//if Player is connected to the game
	virtual bool OnClientConnect(int _channelId, bool _isReset) override;
	//If Player is ready for the game
	virtual bool OnClientEnteredGame(int _channelId, bool _isReset) override;
	//If Player is disconnected from the game
	virtual void OnClientDisconnect(int _channelId, EDisconnectionCause _cause, const char* _desc, bool _keepClient) override;
#pragma endregion Functions we use from the inherited interface
	//~IGameRules
};