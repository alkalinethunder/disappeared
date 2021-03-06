﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SAveGame.h"
#include "WeekDay.h"
#include "Wallpaper.h"
#include "FileData.h"
#include "SocialGuiState.h"
#include "DirectoryEntry.h"
#include "ChatLog.h"
#include "NewsFeedEntry.h"
#include "Network.h"
#include "Device.h"
#include "SpecialDeviceAsset.h"
#include "NetRoute.h"
#include "RedemptionSaveGame.generated.h"

class UDesktopEnvironment;
class UNetworkAsset;
/**
 * Stores all of the player's progress and important game state.
 */
UCLASS(BlueprintType)
class REDEMPTION_API URedemptionSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UDesktopEnvironment* PlayerDesktop;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FNetRoute> Routes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FDevice> Devices;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FNetwork> Networks;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int PlayernetworkId;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float CurrentDaySeconds;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EWeekDay CurrentDayOfWeek;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FNewsFeedEntry> NewsFeedEntries;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FFileData> Files;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FChatLog> PlayerChatLogs;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FString> CompletedChats;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FString> PlayerContacts;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<int, int> DiskRoots;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FDirectoryEntry> Directories;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int Experience;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int Level;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int UpgradePoints;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FName> UnlockedUpgrades;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UWallpaper* DesktopWallpaper;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int GuiOpenConsoles;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ESocialGuiState LastSocialState;

public:
	UFUNCTION()
	FString GetAvailableLocalIP(int NetworkId);
	
	UFUNCTION()
	int MapDevice(int InDeviceId);

	UFUNCTION()
	int MapNetwork(int InNetworkId);
	
	UFUNCTION()
	int GetNetworkIndex(int NetworkId);

	UFUNCTION()
    int GetPeerCount(int ISP);
	
	UFUNCTION()
	int GetConnectionCount(int ISP);
	
	UFUNCTION()
	bool NetworkHasISP(int NetworkId);
	
	UFUNCTION()
	int FindDevice(USpecialDeviceAsset* InSpecialDevice);
	
	UFUNCTION()
	int FindNetwork(UNetworkAsset* InNetworkAsset);
	
	UFUNCTION()
	bool DeviceTypeExistsInNetwork(const FNetwork& InNetwork, EDeviceType InDeviceType);

	UFUNCTION()
	int GetNextDeviceId();

	UFUNCTION()
	int GetNextNetworkId();
	
	UFUNCTION()
	bool FindFileIndex(int InFileId, int& OutIndex);
	
	UFUNCTION()
	bool FindDirectoryIndex(int DirectoryId, int& OutDirectoryIndex);

	UFUNCTION()
	int GetAvailableFileId();
	
	UFUNCTION()
	int GetAvailableDirectoryId();

	UFUNCTION()
	void PostChatLogs(class UConversationAppWidget* InWidget, class UChatContact* InContact);
};
