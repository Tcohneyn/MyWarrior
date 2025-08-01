// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "HeroGameplayAbility_TargetLock.generated.h"

class UWarriorWidgetBase;
class UInputMappingContext;
UCLASS()
class MYWARRIOR_API UHeroGameplayAbility_TargetLock : public UWarriorHeroGameplayAbility
{
    GENERATED_BODY()
protected:
    //~ Begin UGameplayAbility Interface
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    //~ End UGameplayAbility Interface

    UFUNCTION(BlueprintCallable)
    void OnTargetLockTick(float DeltaTime);

    UFUNCTION(BlueprintCallable)
    void SwitchTarget(const FGameplayTag& InSwitchDirectionTag);

private:
    void TryLockOnTarget();
    void GetAvailableActorsToLock();
    AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors);

    void GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight);
    void DrawTargetLockWidget();
    void SetTargetLockWidgetPosition();

    void InitTargetLockMovement();

    void InitTargetLockMappingContext();

    void CancelTargetLockAbility();

    void Cleanup();

    void ResetTargetLockMovement();

    void ResetTargetLockMappingContext();

    UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
    float BoxTraceDistance = 5000.f;

    UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
    FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

    UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
    TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;

    UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
    bool bShowPersistentDebugShape = false;

    UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
    TSubclassOf<UWarriorWidgetBase> TargetLockWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
    float TargetLockRotationInterpSpeed = 5.f;

    UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
    float TargetLockMaxWalkSpeed = 150.f;

    UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
    UInputMappingContext* TargetLockMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
    float TargetLockCameraOffsetDistance = 20.f;

    UPROPERTY()
    TArray<AActor*> AvailableActorsToLock;

    UPROPERTY()
    AActor* CurrentLockedActor;

    UPROPERTY()
    UWarriorWidgetBase* DrawnTargetLockWidget;

    UPROPERTY()
    FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;

    UPROPERTY()
    float CachedDefaultMaxWalkSpeed = 0.f;
};
