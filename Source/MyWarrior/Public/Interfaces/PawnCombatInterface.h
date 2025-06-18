// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnCombatInterface.generated.h"

class UPawnCombatComponent;

UINTERFACE(MinimalAPI)
class UPawnCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYWARRIOR_API IPawnCombatInterface
{
	GENERATED_BODY()


public:
    virtual UPawnCombatComponent* GetPawnCombatComponent() const = 0;
};
