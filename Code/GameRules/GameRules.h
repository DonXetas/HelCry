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
// Our custom GameRules have to inherit from the Interface "IGameRules" to give us some of the predefined functions we need
class CGameRules : public CGameObjectExtensionHelper<CGameRules, IGameRules>
{
public: 
	virtual ~CGameRules();

	//IGameObjectExtension
#pragma region
	virtual void PostInit(IGameObject* pGameObject) override {};
	virtual void InitClient(int channelId) override {};
	virtual void PostInitClient(int channelId) override {};
	virtual bool ReloadExtension(IGameObject* pGameObject, const SEntitySpawnParams& params) override { return false; };
	virtual void PostReloadExtension(IGameObject* pGameObject, const SEntitySpawnParams& params) override {};
	virtual void FullSerialize(TSerialize ser) override {};
	virtual bool NetSerialize(TSerialize ser, EEntityAspects aspect, uint8 profile, int pflags) override { return true; };
	virtual NetworkAspectType GetNetSerializeAspects() { return eEA_All; }
	virtual void PostSerialize() override {};
	virtual void                 SerializeSpawnInfo(TSerialize ser) override {};
	virtual ISerializableInfoPtr GetSpawnInfo() override { return nullptr; };
	virtual void Update(SEntityUpdateContext& ctx, int updateSlot) override {};
	virtual void HandleEvent(const SGameObjectEvent& event) override {};
	virtual void GameSerialize(TSerialize ser) { FullSerialize(ser); };
	virtual void SetChannelId(uint16 id) override {};
	virtual void SetAuthority(bool auth) override {};
	virtual const void* GetRMIBase() const override { return 0; };
	virtual void PostUpdate(float frameTime) override {};
	virtual void PostRemoteSpawn() override {};
#pragma endregion Functions we do not use, but we had to implement these because they are pure virtual given by the inherited interface

	// Functions we use from the inherited interface
	virtual bool Init(IGameObject* pGameObject) override;
	//~IGameObjectExtension

	//IGameRules
#pragma region
	virtual bool ShouldKeepClient(int channelId, EDisconnectionCause cause, const char* desc) const override { return true; }
	virtual void PrecacheLevel() override {}
	virtual void PrecacheLevelResource(const char* resourceName, EGameResourceType resourceType) override {}
	virtual XmlNodeRef FindPrecachedXmlFile(const char* sFilename) override { return 0; }
	virtual void OnConnect(struct INetChannel* pNetChannel) override {}
	virtual void OnDisconnect(EDisconnectionCause cause, const char* desc) override {}
	virtual void OnEntitySpawn(IEntity* pEntity) override {}
	virtual void OnEntityRemoved(IEntity* pEntity) override {}
	virtual void OnEntityReused(IEntity* pEntity, SEntitySpawnParams& params, EntityId prevId) override {}
	virtual void SendTextMessage(ETextMessageType type, const char* msg, uint32 to = eRMI_ToAllClients, int channelId = -1,
		const char* p0 = 0, const char* p1 = 0, const char* p2 = 0, const char* p3 = 0) override {}
	virtual void SendChatMessage(EChatMessageType type, EntityId sourceId, EntityId targetId, const char* msg) override {}
	virtual void ClientHit(const HitInfo& hitInfo) override {}
	virtual void ServerHit(const HitInfo& hitInfo) override {}
	virtual int GetHitTypeId(const uint32 crc) const override { return 0; }
	virtual int GetHitTypeId(const char* type) const override { return 0; }
	virtual const char* GetHitType(int id) const override { return ""; }
	virtual void OnVehicleDestroyed(EntityId id) override {}
	virtual void OnVehicleSubmerged(EntityId id, float ratio) override {}
	virtual bool CanEnterVehicle(EntityId playerId) override { return true; }
	virtual void CreateEntityRespawnData(EntityId entityId) override {}
	virtual bool HasEntityRespawnData(EntityId entityId) const override { return true; }
	virtual void ScheduleEntityRespawn(EntityId entityId, bool unique, float timer) override {}
	virtual void AbortEntityRespawn(EntityId entityId, bool destroyData) override {}
	virtual void ScheduleEntityRemoval(EntityId entityId, float timer, bool visibility) override {}
	virtual void AbortEntityRemoval(EntityId entityId) override {}
	virtual void AddHitListener(IHitListener* pHitListener) override {}
	virtual void RemoveHitListener(IHitListener* pHitListener) override {}
	virtual bool OnCollision(const SGameCollision& event) override { return true; }
	virtual void OnCollision_NotifyAI(const EventPhys* pEvent) override {}
	virtual void ShowStatus() override {}
	virtual bool IsTimeLimited() const override { return true; }
	virtual float GetRemainingGameTime() const override { return 0.0f; }
	virtual void SetRemainingGameTime(float seconds) override {}
	virtual void ClearAllMigratingPlayers(void) override {}
	virtual EntityId SetChannelForMigratingPlayer(const char* name, uint16 channelID) override { return INVALID_ENTITYID; }
	virtual void StoreMigratingPlayer(IActor* pActor) override {}
	virtual const char* GetTeamName(int teamId) const override { return ""; }
#pragma endregion Functions we do not use, but we had to implement these because they are pure virtual given by the inherited interface

#pragma region
	//if Player is connected to the game
	virtual bool OnClientConnect(int channelId, bool isReset) override;
	//If Player is ready for the game
	virtual bool OnClientEnteredGame(int channelId, bool isReset) override;
	//If Player is disconnected from the game
	virtual void OnClientDisconnect(int channelId, EDisconnectionCause cause, const char* desc, bool keepClient) override;
#pragma endregion Functions we use from the inherited interface
	//~IGameRules
};