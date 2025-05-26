// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorWeaponBase.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class MYWARRIOR_API AWarriorWeaponBase : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AWarriorWeaponBase();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<UStaticMeshComponent> WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<UBoxComponent> WeaponCollisionBox;

public:
    FORCEINLINE UBoxComponent* GetWeaponCollisionBox() { return WeaponCollisionBox; }
};
