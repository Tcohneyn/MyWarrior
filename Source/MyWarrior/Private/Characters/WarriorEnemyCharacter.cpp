// Tcohneyn All Rights Reserved

#include "Characters/WarriorEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "NiagaraComponent.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "Engine/StreamableManager.h"
#include "NiagaraFunctionLibrary.h"
//#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "WarriorDebugHelper.h"
AWarriorEnemyCharacter::AWarriorEnemyCharacter()
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bUseControllerDesiredRotation = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
    GetCharacterMovement()->MaxWalkSpeed = 300.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

    EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("CombatComponent"));
}

UPawnCombatComponent* AWarriorEnemyCharacter::GetPawnCombatComponent() const
{
    return EnemyCombatComponent;
}

void AWarriorEnemyCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    InitEnemyStartUpData();
}

void AWarriorEnemyCharacter::InitEnemyStartUpData()
{
    if (CharacterStartUpData.IsNull())
    {
        return;
    }
    UAssetManager::GetStreamableManager().RequestAsyncLoad(
        CharacterStartUpData.ToSoftObjectPath(), FStreamableDelegate::CreateLambda(
                                                     [this]()
                                                     {
                                                         if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
                                                         {
                                                             LoadedData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponent);
                                                             // Debug::Print(TEXT("Enemy StartUpData Loaded"), FColor::Green);
                                                         }
                                                     }));
}

void AWarriorEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    DissolveTimeline = NewObject<UTimelineComponent>(this, TEXT("DissolveTimeline"));
    DissolveTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
    this->BlueprintCreatedComponents.Add(DissolveTimeline);  // 避免GC回收
    DissolveTimeline->RegisterComponent();
}

void AWarriorEnemyCharacter::OnEnemyDeath(TSoftObjectPtr<UNiagaraSystem> Dissolve_Niagara_System)
{
    auto Task1 = [this]
    {
        GetMesh()->bPauseAnims = true;
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    };
    auto Task2 = [this, Dissolve_Niagara_System]
    {
        LoadNiagaraAsync(Dissolve_Niagara_System);
        check(DissolveTimeline);
        DissolveTimeline->SetPlayRate(1.0f / DissolveTime);
        SetDissolveTimeline();
    };
    Task1();
    Task2();
}

void AWarriorEnemyCharacter::SetDissolveTimeline()
{
    DissolveTimeline->PlayFromStart();
    // 绑定委托
    OnDissolveTimelineUpdate.BindDynamic(this, &ThisClass::OnDissolveUpdate);
    OnDissolveTimelineFinished.BindDynamic(this, &ThisClass::OnDissolveFinished);
    // 关联曲线与委托
    if (DissolveCurve)
    {
        DissolveTimeline->AddInterpFloat(DissolveCurve, OnDissolveTimelineUpdate);
    }
    DissolveTimeline->SetTimelineFinishedFunc(OnDissolveTimelineFinished);
    DissolveTimeline->SetLooping(false);  // 不循环
}

void AWarriorEnemyCharacter::OnDissolveUpdate(float Value)
{
    // float time = DissolveTimeline->GetPlaybackPosition();
    GetMesh()->SetScalarParameterValueOnMaterials(ParameterName, Value);
    AWarriorWeaponBase* EnemyWeapon = EnemyCombatComponent->GetCharacterCurrentEquippedWeapon();
    if (EnemyWeapon)
    {
        EnemyWeapon->WeaponMesh->SetScalarParameterValueOnMaterials(ParameterName, Value);
    }
}

void AWarriorEnemyCharacter::OnDissolveFinished()
{
    Destroy();
    AWarriorWeaponBase* EnemyWeapon = EnemyCombatComponent->GetCharacterCurrentEquippedWeapon();
    if (EnemyWeapon)
    {
        EnemyWeapon->Destroy();
    }
}
void AWarriorEnemyCharacter::LoadNiagaraAsync(TSoftObjectPtr<UNiagaraSystem> Dissolve_Niagara_System)
{
    DissolveSystem = Dissolve_Niagara_System.Get();
    FSoftObjectPath NiagaraPath = Dissolve_Niagara_System.ToSoftObjectPath();
    FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnNiagaraLoaded);
    StreamableManager.RequestAsyncLoad(NiagaraPath, Delegate);
}

void AWarriorEnemyCharacter::OnNiagaraLoaded()
{
    if (DissolveSystem)
    {
        // 1. 生成Niagara粒子系统
        UNiagaraComponent* DissolveNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(DissolveSystem, GetMesh(), FName("Root"),
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::KeepRelativeOffset, true);
        // 2. 创建动态材质实例（MID）并应用到网格
        UMaterialInterface* BaseMaterial = GetMesh()->GetMaterial(0);
        UMaterialInstanceDynamic* DynamicMaterial = 
            GetMesh()->CreateDynamicMaterialInstance(0, BaseMaterial);
        // 3. 从MID获取颜色参数值
        FLinearColor DissolveColor = DynamicMaterial->K2_GetVectorParameterValue(DissolveParameterName);
        // 假设DissolveParameterName是材质中的向量参数（如"DissolveColor"）
        if (DissolveNiagaraComponent)
        {
            // 4. 设置Niagara变量（确保ParameterNameString与Niagara中的变量名匹配）
            DissolveNiagaraComponent->SetNiagaraVariableLinearColor(ParameterNameString, DissolveColor);
        }
        //else
        //{
        //    UE_LOG(LogTemp, Error, TEXT("Failed to get parameter %s from material"), *DissolveParameterName.ToString());
        //}
        /*FLinearColor NiagaraColor = UKismetMaterialLibrary::GetVectorParameterValue(GetWorld(), Collection, DissolveParameterName);*/
    }
}

