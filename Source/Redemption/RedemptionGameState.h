﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UpgradeAsset.h"
#include "ConversationManager.h"
#include "Conversation.h"
#include "ChatContact.h"
#include "DeviceTypeRules.h"
#include "RedemptionGameModeBase.h"
#include "GrapevinePost.h"
#include "NetworkManager.h"
#include "NetworkTypeRules.h"
#include "HackableAsset.h"
#include "Payload.h"
#include "LatentGameplayTask.h"
#include "RedemptionGameState.generated.h"

class UHackTrace;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FContactAddedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDoNotDisturbChanged, bool, InIsDoNotDisturb);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewsFeedUpdatedEvent, FNewsFeedEntry, InNewEntry);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHackTraceBegun, UHackTrace*, InHackTrace);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHackTraceEnded, UHackTrace*, InHackTrace);

UCLASS(BlueprintType)
class REDEMPTION_API ARedemptionGameState : public AGameStateBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<ULatentGameplayTask*> LatentTasks;
	
	UPROPERTY()
	TArray<UHackTrace*> Traces;
	
	UPROPERTY()
	TArray<UPayload*> Payloads;
	
	UPROPERTY()
	UDeviceTypeRules* DefaultDeviceTypeRules = nullptr;

	UPROPERTY()
	TArray<UDeviceTypeRules*> DeviceRules;
	
	UPROPERTY()
	UNetworkTypeRules* DefaultNetworkRules = nullptr;
	
	UPROPERTY()
	TArray<UNetworkTypeRules*> NetworkTypeRules;
	
	UPROPERTY()
	TArray<UHackableAsset*> Hackables;
	
	UPROPERTY()
	UNetworkManager* NetworkManager;
	
	UPROPERTY()
	FRandomStream Random;
	
	UPROPERTY()
	TArray<UPerson*> People;
	
	UPROPERTY()
	float SecondsPerDay = 1440.f;

	UPROPERTY()
	float WorldClock = 0.f;

	UPROPERTY()
	int CurrentDay = 0;
	
	UPROPERTY()
	int Hour;

	UPROPERTY()
	int Minute;
	
	UPROPERTY()
	TArray<UGrapevinePost*> GrapevinePosts;
	
	UPROPERTY()
	ARedemptionGameModeBase* MyGameMode;
	
	UPROPERTY()
	bool bDoNotDisturb = false;
	
	UPROPERTY()
	TArray<UChatContact*> Contacts;

	UPROPERTY()
	TArray<UConversation*> Conversations;
	
	UPROPERTY()
	AConversationManager* ConversationManager;
	
	UPROPERTY()
	TArray<UUpgradeAsset*> Upgrades;

	UPROPERTY()
	URedemptionGameInstance* MyGameInstance;

public:
	UPROPERTY(BlueprintAssignable)
	FHackTraceBegun HackTraceBegun;

	UPROPERTY(BlueprintAssignable)
	FHackTraceEnded HackTraceEnded;
	
	UPROPERTY(BlueprintAssignable)
	FDoNotDisturbChanged DoNotDisturbChanged;
	
	UPROPERTY(BlueprintAssignable)
	FContactAddedEvent OnContactAdded;

	UPROPERTY(BlueprintAssignable)
	FOnNewsFeedUpdatedEvent NewsFeedUpdated;

public:
	// Sets default values for this actor's properties
	ARedemptionGameState();

private:
	UFUNCTION()
	UDeviceTypeRules* GetDeviceRules(EDeviceType DeviceType);
	
	UFUNCTION()
	UNetworkTypeRules* GetNetworkRules(ENetworkType NetworkType);
	
	UFUNCTION()
	void MakeRoutes();
	
	UFUNCTION()
	void MakeISPs();
	
	UFUNCTION()
	void CreateMissingRouters();
	
	UFUNCTION()
	void AssignLocalIPs();
	
	UFUNCTION()
	FString GeneratePublicIP();
	
	UFUNCTION()
	void AssignIPAddresses();
	
	UFUNCTION()
	void PostGrapevineAds();
	
	UFUNCTION()
    void UpdateFriendlyTime();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	ULatentGameplayTask* CreateLatentTask(float InTime);
	
	UFUNCTION()
	ARedemptionGameModeBase* GetGameMode();
	
	UFUNCTION()
	TArray<UUpgradeAsset*> GetAllUpgrades();

	UFUNCTION()
	URedemptionGameInstance* GetGameInstance();
	
	UFUNCTION()
	UChatContact* GetContactByName(FString InName);

public:
	UFUNCTION()
	void BeginTrace(UHackSession* InHackSession, UHackableinfo* InTraceSource);
	
	UFUNCTION()
	void GenerateNetworkHackables(int NetworkId);
	
	UFUNCTION(BlueprintPure)
	bool IsDoNotDisturbActive();

	UFUNCTION(BlueprintCallable)
	void ToggleDoNotDisturb();

	UFUNCTION(BlueprintPure)
	UNetworkManager* GetNetworkManager();
	
	UFUNCTION(BlueprintPure)
	AConversationManager* GetConversationManager();

	UFUNCTION(BlueprintCallable)
	void ActivateConversation(UChatContact* InContact, UConversationAppWidget* InWidget);

	UFUNCTION()
	void PostThought(UPerson* InPerson, FString InThoughtText);

	UFUNCTION()
    UPerson* FindPersonById(FString InPersonId);

	UFUNCTION()
    TArray<FNewsFeedEntry> GetNewsFeed();

	UFUNCTION(BlueprintPure)
    bool IsCurrentDay(FDayList InDayList);

	UFUNCTION()
	int GetMinute();

	UFUNCTION()
	int GetHour();
	
public:
	UFUNCTION(Exec)
	void GenerateHackables(int DeviceIndex, EDifficulty Difficulty);
	
	UFUNCTION(Exec)
	void ListDevices();
	
	UFUNCTION(Exec)
	void ListNets();
	
	UFUNCTION(Exec)
	void AddContact(FString InContactName);
	
	UFUNCTION(Exec)
	void LoadedContacts();

	UFUNCTION(Exec)
	void TraceNetRoute(int Start, int Destination);
};
