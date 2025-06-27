// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enemy_Death_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemy_Death_Interface : public UInterface
{
	GENERATED_BODY()
};

class UNiagaraSystem;

class MYWARRIOR_API IEnemy_Death_Interface
{
	GENERATED_BODY()

public:
    virtual void OnEnemyDeath(TSoftObjectPtr<UNiagaraSystem> Dissolve_Niagara_System) = 0;
};
