﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "VirtualFileSystem.h"
#include "RedemptionSaveGame.h"
#include "DirectoryNode.h"
#include "PathUtils.h"
#include "FileNode.h"

UDiskNode* UVirtualFileSystem::MapToNode(FString InPath)
{
	TArray<FString> parts;
	UPathUtils::GetPathParts(InPath, parts);

	UDiskNode* node = this->RootDiskNode;

	for(FString part : parts)
	{
		UDiskNode* nextNode = node->GetChild(part);
		node = nextNode;
		if (!node)
			break;
	}

	return node;
}

bool UVirtualFileSystem::ResolveDirectory(FString InPath, UDirectoryNode*& OutNode)
{
	bool result = true;
	TArray<FString> parts;
	UDirectoryNode* node = Cast<UDirectoryNode>(this->RootDiskNode);

	check(node);
	
	UPathUtils::GetPathParts(InPath, parts);

	for(FString part : parts)
	{
		UDirectoryNode* child = Cast<UDirectoryNode>(node->GetChild(part));
		if (child)
		{
			node = child;
		}
		else
		{
			result = false;
			break;
		}
	}

	OutNode = node;
	return result;
}

bool UVirtualFileSystem::ResolveFile(FString InPath, UFileNode*& OutNode)
{
	bool result = true;
	TArray<FString> parts;
	UDirectoryNode* node = Cast<UDirectoryNode>(this->RootDiskNode);

	check(node);
	
	UPathUtils::GetPathParts(InPath, parts);

	for(int i = 0; i < parts.Num() - 1; i++)
	{
		FString part = parts[i];
		UDirectoryNode* child = Cast<UDirectoryNode>(node->GetChild(part));
		if (child)
		{
			node = child;
		}
		else
		{
			result = false;
			break;
		}
	}

	if (node)
	{
		FString filename = parts[parts.Num() - 1];
		UFileNode* fnode = Cast<UFileNode>(node->GetChild(filename));
		if (fnode)
		{
			OutNode = fnode;
		}
		else
		{
			result = false;
		}
	}
	return result;

}

void UVirtualFileSystem::MountRootNode(URedemptionSaveGame* InSaveGame, int InMountId)
{
	check(!this->SaveGame);
	check(!this->RootDiskNode);

	this->SaveGame = InSaveGame;

	UDirectoryNode* Directory = NewObject<UDirectoryNode>();

	int DirectoryIndex = this->SaveGame->DiskRoots[InMountId];

	Directory->LinkToDirectory(nullptr, this->SaveGame, DirectoryIndex);
	this->RootDiskNode = Directory;
}

bool UVirtualFileSystem::MakeDirectory(FString InPath)
{
	bool result = true;
	TArray<FString> parts;
	UDirectoryNode* node = Cast<UDirectoryNode>(this->RootDiskNode);

	UPathUtils::GetPathParts(InPath, parts);

	for(FString part : parts)
	{
		UDiskNode* child = node->GetChild(part);

		if (child)
		{
			UDirectoryNode* dirnode = Cast<UDirectoryNode>(child);
			if (dirnode)
			{
				node = dirnode;
			}
			else
			{
				result = false;
				break;
			}
		}
		else
		{
			UDirectoryNode* newdir = node->MakeChildDirectory(part);
			node = newdir;
		}
	}

	return result;
}

bool UVirtualFileSystem::GetChildDirectoryPaths(FString InPath, TArray<FString>& OutChildren)
{
	UDirectoryNode* node = nullptr;
	if (this->ResolveDirectory(InPath, node))
	{
		for(UDirectoryNode* child : node->GetChildDirectories())
		{
			OutChildren.Add(UPathUtils::GetAbsolutePath(InPath, child->GetDiskNodeName()));
		}

		OutChildren.Sort();
		
		return true;
	}
	else
	{
		return false;
	}
}

bool UVirtualFileSystem::GetChildFilePaths(FString InPath, TArray<FString>& OutChildren)
{
	UDirectoryNode* node = nullptr;
	if (this->ResolveDirectory(InPath, node))
	{
		for(UFileNode* child : node->GetChildFiles())
		{
			OutChildren.Add(UPathUtils::GetAbsolutePath(InPath, child->GetDiskNodeName()));
		}

		OutChildren.Sort();
		
		return true;
	}
	else
	{
		return false;
	}

}

bool UVirtualFileSystem::DirectoryExists(FString InPath)
{
	UDiskNode* node = this->MapToNode(InPath);
	UDirectoryNode* dirNode = Cast<UDirectoryNode>(node);

	return dirNode != nullptr;
}

bool UVirtualFileSystem::FileExists(FString InPath)
{
	UFileNode* node;
	if (this->ResolveFile(InPath, node))
	{
		return true;
	}
	else
	{
		return false;
	}
}

FFileData& UVirtualFileSystem::GetFileData(FString InPath)
{
	UDiskNode* node = this->MapToNode(InPath);
	UFileNode* fnode = Cast<UFileNode>(node);
	if (fnode)
	{
		return fnode->GetFileData();
	}
	else
	{
		return this->Invalid;
	}
}
