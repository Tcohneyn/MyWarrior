// Tcohneyn All Rights Reserved


#include "Items/WarriorProjectileBase.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "WarriorDebugHelper.h"

AWarriorProjectileBase::AWarriorProjectileBase()
{
    PrimaryActorTick.bCanEverTick = false;

    ProjectileCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
    SetRootComponent(ProjectileCollisionBox);
    ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
    ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    ProjectileCollisionBox->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHit);
    ProjectileCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);

    ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponent"));
    ProjectileNiagaraComponent->SetupAttachment(GetRootComponent());

    ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
    ProjectileMovementComp->InitialSpeed = 700.f;
    ProjectileMovementComp->MaxSpeed = 900.f;
    ProjectileMovementComp->Velocity = FVector(1.f, 0.f, 0.f);
    ProjectileMovementComp->ProjectileGravityScale = 0.f;

    InitialLifeSpan = 4.f;
}

void AWarriorProjectileBase::BeginPlay()
{
    Super::BeginPlay();

    if (ProjectileDamagePolicy == EProjectileDamagePolicy::OnBeginOverlap)
    {
        ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    }
}

void AWarriorProjectileBase::OnProjectileHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    BP_OnSpawnProjectileHitFX(Hit.ImpactPoint);

    APawn* HitPawn = Cast<APawn>(OtherActor);

    if (!HitPawn || !UWarriorFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn))
    {
        Destroy();
        return;
    }

    bool bIsValidBlock = false;

    const bool bIsPlayerBlocking = UWarriorFunctionLibrary::NativeDoesActorHaveTag(HitPawn, WarriorGameplayTags::Player_Status_Blocking);

    if (bIsPlayerBlocking)
    {
        bIsValidBlock = UWarriorFunctionLibrary::IsValidBlock(this, HitPawn);
    }

    FGameplayEventData Data;
    Data.Instigator = this;
    Data.Target = HitPawn;

    if (bIsValidBlock)
    {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitPawn, WarriorGameplayTags::Player_Event_SuccessfulBlock, Data);
    }
    else
    {
        HandleApplyProjectileDamage(HitPawn, Data);
    }

    Destroy();
}
//当投射物的碰撞体和其它物体发生重叠时，碰撞回调
void AWarriorProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //防止重复命中
    if (OverlappedActors.Contains(OtherActor))
    {
        return;
    }

    OverlappedActors.AddUnique(OtherActor);
    //判断是否是角色
    if (APawn* HitPawn = Cast<APawn>(OtherActor))
    {
        FGameplayEventData Data;
        Data.Instigator = GetInstigator();
        Data.Target = HitPawn;
    //判断敌我关系并应用伤害
        if (UWarriorFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn) && ProjectileDamageEffectSpecHandle.IsValid())
        {
            HandleApplyProjectileDamage(HitPawn, Data);
        }
    }
}


void AWarriorProjectileBase::HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload)
{
    checkf(ProjectileDamageEffectSpecHandle.IsValid(), TEXT("Forgot to assign a valid spec handle to the projectile: %s"),
        *GetActorNameOrLabel());

    const bool bWasApplied =
        UWarriorFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), InHitPawn, ProjectileDamageEffectSpecHandle);

    if (bWasApplied)
    {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(InHitPawn, WarriorGameplayTags::Shared_Event_HitReact, InPayload);
    }
}




void AWarriorProjectileBase::InitDamageSpec(const FGameplayEffectSpecHandle& SpecHandle) 
{
    ProjectileDamageEffectSpecHandle = SpecHandle;
}