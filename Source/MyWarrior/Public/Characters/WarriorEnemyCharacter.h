// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
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

    UPROPERTY(EditDefaultsOnly, Category = "Dissolve")
    FName ParameterName;

    UPROPERTY(VisibleAnywhere, Category = "Combat")
    TObjectPtr<UEnemyCombatComponent> EnemyCombatComponent;

    UPROPERTY(VisibleAnywhere, Category = "UI")
    TObjectPtr<UEnemyUIComponent> EnemyUIComponent;

    UPROPERTY(VisibleAnywhere, Category = "UI")
    TObjectPtr<UWidgetComponent> EnemyHealthWidgetComponent;

    FStreamableManager StreamableManager;

#pragma region DissolveCurve

    // 委托声明
    FOnTimelineFloat OnDissolveTimelineUpdate;
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
    UFUNCTION()
    void OnDissolveFinished();

    void SetDissolveTimeline();
    void LoadNiagaraAsync(TSoftObjectPtr<UNiagaraSystem> Dissolve_Niagara_System);
    void OnNiagaraLoaded();
    UNiagaraSystem* DissolveSystem;
#pragma endregion

private:
    void InitEnemyStartUpData();

public:
    FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
};
