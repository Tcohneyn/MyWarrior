// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorWeaponBase.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate,AActor*)

UCLASS()
class MYWARRIOR_API AWarriorWeaponBase : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AWarriorWeaponBase();

    FOnTargetInteractedDelegate OnWeaponHitTarget;
    FOnTargetInteractedDelegate OnWeaponPulledFromTarget;
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<UStaticMeshComponent> WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<UBoxComponent> WeaponCollisionBox;

    UFUNCTION()
    virtual void OnCollisonBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnCollisonBoxEndOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
    FORCEINLINE UBoxComponent* GetWeaponCollisionBox() { return WeaponCollisionBox; }
};
