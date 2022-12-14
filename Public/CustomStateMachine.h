// Copyright Epic Games, Inc. All Rights Reserved.
// Custom State Machine reference video : https://www.youtube.com/watch?v=EZBnx-ZSoS0&t=622s by Altray Digital, Last day accessed: Dec 14 2022

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FCustomStateMachineModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
