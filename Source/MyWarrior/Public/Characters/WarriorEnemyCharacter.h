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

    //~ Begin IPawnCombatComponent Interface
    virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
    //~ End IPawnCombatComponent Interface

protected:
    //~ Begin APawn Interface
    virtual void PossessedBy(AController* NewController) override;
    //~ End APawn Interface

    UPROPERTY(VisibleAnywhere, Category = "Combat")
    TObjectPtr<UEnemyCombatComponent> EnemyCombatComponent;

private:
    void InitEnemyStartUpData();

public:
    FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
};
