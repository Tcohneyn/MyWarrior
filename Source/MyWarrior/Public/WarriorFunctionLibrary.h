// Tcohneyn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "WarriorGameplayTags.h"
#include "WarriorFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct FHitReactResult
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly)
    float Angle;
    UPROPERTY(BlueprintReadOnly)
    FGameplayTag Tag;
};

class UWarriorAbilitySystemComponent;
class UPawnCombatComponent;
struct FScalableFloat;


UCLASS()
class MYWARRIOR_API UWarriorFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    static UWarriorAbilitySystemComponent* NativeGetWarriorASCFromActor(AActor* InActor);

    UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
    static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

    UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
    static void RemoveGameplayTagFromActor(AActor* InActor, FGameplayTag TagToRemove);

    UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
    static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

    UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary",
        meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
    static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType);

    static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

    UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary",
        meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValidType"))
    static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType);

    UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
    static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

    UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
    static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel = 1.f);

    UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
    static FHitReactResult ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim);
};
