// Tcohneyn All Rights Reserved

#include "Characters/WarriorHeroCharacter.h"
#include "WarriorDebugHelper.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/Input/WarriorEnhancedInputComponent.h"
// 构造函数：AWarriorHeroCharacter的构造函数
AWarriorHeroCharacter::AWarriorHeroCharacter()
{
    // 初始化胶囊碰撞组件的大小，半径为42.f，高度为96.f
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

    // 设置不使用控制器的俯仰旋转
    bUseControllerRotationPitch = false;
    // 设置不使用控制器的偏航旋转
    bUseControllerRotationYaw = false;
    // 设置不使用控制器的滚转旋转
    bUseControllerRotationRoll = false;

    // 创建默认的弹簧臂组件，并命名为"CameraBoom"
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    // 将弹簧臂组件附加到根组件
    CameraBoom->SetupAttachment(GetRootComponent());
    // 设置弹簧臂的目标长度为200.f
    CameraBoom->TargetArmLength = 200.f;
    // 设置弹簧臂的socket偏移量为(0.f, 55.f, 65.f)
    CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
    // 设置弹簧臂使用角色控制旋转
    CameraBoom->bUsePawnControlRotation = true;

    // 创建默认的相机组件，并命名为"FollowCamera"
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    // 将相机组件附加到弹簧臂组件的Socket
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    // 设置相机组件不使用角色控制旋转
    FollowCamera->bUsePawnControlRotation = false;

    // 获取角色移动组件，并设置其旋转方向与移动方向一致
    GetCharacterMovement()->bOrientRotationToMovement = true;
    // 设置角色移动组件的旋转速度，偏航速度为500.f
    GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
    // 设置角色移动组件的最大行走速度为400.f
    GetCharacterMovement()->MaxWalkSpeed = 400.f;
    // 设置角色移动组件的行走制动减速度为2000.f
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

// AWarriorHeroCharacter类的PossessedBy函数，用于在角色被控制器拥有时执行一些初始化操作
void AWarriorHeroCharacter::PossessedBy(AController* NewController)
{
    // 调用基类的PossessedBy函数，确保基类的初始化逻辑也被执行
    Super::PossessedBy(NewController);
    if (!CharacterStartUpData.IsNull())
    {
        if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
        {
            LoadedData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponent);
        }
    }

}

// AWarriorHeroCharacter类的BeginPlay函数，用于在游戏开始时执行一些初始化操作
void AWarriorHeroCharacter::BeginPlay()
{
    // 调用基类的BeginPlay函数，确保基类的初始化逻辑也被执行
    Super::BeginPlay();
}

// AWarriorHeroCharacter类的SetupPlayerInputComponent函数，用于设置玩家的输入组件
void AWarriorHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // 检查InputConfigDataAsset是否已分配
    checkf(InputConfigDataAsset, TEXT("Forget to assign a valid data asset as input config"));
    
    // 获取本地玩家
    ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
    
    // 获取增强输入本地玩家子系统
    auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
    
    // 检查子系统是否有效
    check(Subsystem);
    
    // 如果子系统无效，直接返回
    if (!Subsystem) return;
    
    // 添加默认的映射上下文到子系统
    Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
    
    // 将PlayerInputComponent转换为UWarriorEnhancedInputComponent类型
    auto WarriorInputComponent = CastChecked<UWarriorEnhancedInputComponent>(PlayerInputComponent);
    
    // 绑定移动输入动作
    WarriorInputComponent->BindNativeInputAction(
        InputConfigDataAsset, WarriorGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &AWarriorHeroCharacter::Input_Move);
    
    // 绑定视角旋转输入动作
    WarriorInputComponent->BindNativeInputAction(
        InputConfigDataAsset, WarriorGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &AWarriorHeroCharacter::Input_Look);
}

// AWarriorHeroCharacter类的Input_Move函数，用于处理移动输入
void AWarriorHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
    // 获取输入的二维向量
    const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
    
    if (!Controller) return;
    // 获取控制器的旋转
    const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
    
    // 如果Y轴输入不为0，则向前移动
    if (MovementVector.Y != 0.f)
    {
        const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
        AddMovementInput(ForwardDirection, MovementVector.Y);
    }

    // 如果X轴输入不为0，则向右移动
    if (MovementVector.X != 0.f)
    {
        const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

// AWarriorHeroCharacter类的Input_Look函数，用于处理视角旋转输入
void AWarriorHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
    // 获取输入的二维向量
    const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
    
    // 如果X轴输入不为0，则旋转视角的Yaw
    if (LookAxisVector.X != 0.f)
    {
        AddControllerYawInput(LookAxisVector.X);
    }
    
    // 如果Y轴输入不为0，则旋转视角的Pitch
    if (LookAxisVector.Y != 0.f)
    {
        AddControllerPitchInput(LookAxisVector.Y);
    }
}