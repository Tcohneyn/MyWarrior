// Tcohneyn All Rights Reserved

#include "Items/Weapons/WarriorWeaponBase.h"
#include "Components/BoxComponent.h"
#include "WarriorDebugHelper.h"
AWarriorWeaponBase::AWarriorWeaponBase()
{

    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    SetRootComponent(WeaponMesh);

    WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBoxCollisionBox"));
    WeaponCollisionBox->SetupAttachment(GetRootComponent());
    WeaponCollisionBox->SetBoxExtent(FVector(20.f));
    WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnCollisonBoxBeginOverlap);
    WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this,&ThisClass::OnCollisonBoxEndOverlap);
}

void AWarriorWeaponBase::OnCollisonBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APawn* WeaponOwningPawn = GetInstigator<APawn>();
    checkf(WeaponOwningPawn, TEXT("Forget to assign an instiagtor as the owning pawn for the weapon %s"),*GetName());
    if (APawn* HitPawn = Cast<APawn>(OtherActor))
    {
        if (WeaponOwningPawn != HitPawn)
        {
            //Debug::Print(GetName() + TEXT("begin overlap with")+HitPawn->GetName(),FColor::Green);
            OnWeaponHitTarget.ExecuteIfBound(OtherActor);
            //TODO:Implement hit chek for enemy characters
        }
    }
}

void AWarriorWeaponBase::OnCollisonBoxEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    APawn* WeaponOwningPawn = GetInstigator<APawn>();
    checkf(WeaponOwningPawn, TEXT("Forget to assign an instiagtor as the owning pawn for the weapon %s"), *GetName());
    if (APawn* HitPawn = Cast<APawn>(OtherActor))
    {
        if (WeaponOwningPawn != HitPawn)
        {
            //Debug::Print(GetName() + TEXT("end overlap with")+HitPawn->GetName(),FColor::Red);
            OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
            // TODO:Implement hit chek for enemy characters
        }
    }
}




