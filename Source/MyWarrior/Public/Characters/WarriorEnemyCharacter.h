// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/WarriorBaseCharacter.h"
#include "Interfaces/Enemy_Death_Interface.h"
#include "Components/TimelineComponent.h"
#include "Engine/StreamableManager.h"
#include "WarriorEnemyCharacter.generated.h"

class UEnemyCombatComponent;
class UTimelineComponent;
class UNiagaraSystem;
class UEnemyUIComponent;
class UWidgetComponent;
class UBoxComponent;

UCLASS()
class MYWARRIOR_API AWarriorEnemyCharacter : public AWarriorBaseCharacter, public IEnemy_Death_Interface
{
    GENERATED_BODY()

public:
    AWarriorEnemyCharacter();

    virtual UEnemyUIComponent* GetEnemyUIComponent() const override;

    //~ Begin IPawnCombatComponent Interface
    virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
    //~ End IPawnCombatComponent Interface
    //~ Begin IEnemy_Death Interface
    virtual void OnEnemyDeath(TSoftObjectPtr<UNiagaraSystem> Dissolve_Niagara_System) override;
    //~ End IEnemy_Death Interface
    //~ Begin IPawnUIInterface Interface
    virtual UPawnUIComponent* GetPawnUIComponent() const override;
    //~ End  IPawnUIInterface Interface
protected:
    //~ Begin APawn Interface
    virtual void PossessedBy(AController* NewController) override;
    //~ End APawn Interface

    virtual void BeginPlay() override;

#if WITH_EDITOR
    //~ Begin UObject Interface.
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);
    //~ End UObject Interface
#endif
    UPROPERTY(EditDefaultsOnly, Category = "Dissolve")
    FName ParameterName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    TObjectPtr<UEnemyCombatComponent> EnemyCombatComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    UBoxComponent* LeftHandCollisionBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    FName LeftHandCollisionBoxAttachBoneName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    UBoxComponent* RightHandCollisionBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    FName RightHandCollisionBoxAttachBoneName;

    UPROPERTY(VisibleAnywhere, Category = "UI")
    TObjectPtr<UEnemyUIComponent> EnemyUIComponent;

    UPROPERTY(VisibleAnywhere, Category = "UI")
    TObjectPtr<UWidgetComponent> EnemyHealthWidgetComponent;

    UPROPERTY(EditDefaultsOnly, Category = "SpawnStone")
    FGameplayTag SpawnStoneTag;

    UFUNCTION()
    virtual void OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    FStreamableManager StreamableManager;

#pragma region DissolveCurve

    // 委托声明
    FOnTimelineFloat OnDissolveTimelineUpdate;
    UPROPERTY()
    FOnTimelineFloat OnReverseTimelineUpdate;
    FOnTimelineEvent OnDissolveTimelineFinished;
    //FStreamableDelegate Delegate;

    UPROPERTY(EditDefaultsOnly, Category = "Dissolve")
    float DissolveTime = 2.0f;  // 溶解动画的持续时间
    UPROPERTY(EditDefaultsOnly, Category = "Dissolve")
    UCurveFloat* DissolveCurve;  // 控制溶解程度的曲线（0~1）
    UPROPERTY(EditDefaultsOnly, Category = "Dissolve")
    FName DissolveParameterName; 
    UPROPERTY(EditDefaultsOnly, Category = "Dissolve")
    FString ParameterNameString;

    UPROPERTY()
    UTimelineComponent* DissolveTimeline;

    // 绑定函数
    UFUNCTION()
    void OnDissolveUpdate(float Value);
    //void OnReverseUpdate(float Value);
    UFUNCTION()
    void OnDissolveFinished();

    UFUNCTION(BlueprintCallable)
    void BindReverseTimelineUpdate(UTimelineComponent* Timelines);

    void SetDissolveTimeline();
    void LoadNiagaraAsync(TSoftObjectPtr<UNiagaraSystem> Dissolve_Niagara_System);
    void OnNiagaraLoaded();
    UNiagaraSystem* DissolveSystem;
#pragma endregion

private:
    void InitEnemyStartUpData();
    FTimerHandle StoneSpawnTimerHandle;
    FTimerDelegate StoneSpawnDelegate;
    void StoneDestroy();

    UPROPERTY(EditDefaultsOnly,Category="Delay")
    float StoneSpawnDelayTime = 0.5f;
public:
    FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
    FORCEINLINE UBoxComponent* GetLeftHandCollisionBox() const { return LeftHandCollisionBox; }
    FORCEINLINE UBoxComponent* GetRightHandCollisionBox() const { return RightHandCollisionBox; }
};
