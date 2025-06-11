// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/WarriorBaseCharacter.h"
#include "WarriorEnemyCharacter.generated.h"

class UEnemyCombatComponent;

UCLASS()
class MYWARRIOR_API AWarriorEnemyCharacter : public AWarriorBaseCharacter
{
    GENERATED_BODY()

public:
    AWarriorEnemyCharacter();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Combat")
    TObjectPtr<UEnemyCombatComponent> EnemyCombatComponent;

public:
    FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
};
