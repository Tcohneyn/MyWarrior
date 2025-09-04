#pragma once

UENUM()
enum class EWarriorConfirmType : uint8
{
    Yes,
    No
};

UENUM()
enum class EWarriorValidType : uint8
{
    Valid,
    InValid
};

UENUM(BlueprintType)
enum class EWarriorSuccessType : uint8
{
    Successful,
    Failed
};

UENUM()
enum class EWarriorCountDownActionInput : uint8
{
    Start,
    Cancel
};

UENUM()
enum class EWarriorCountDownActionOutput : uint8
{
    Updated,
    Completed,
    Cancelled
};

UENUM()
enum class EWarriorDecoratorOperaty : uint8
{
    IsLessThan,
    IsLessThanOrEqualTo,
    IsEqualTo,
    IsGreaterThanOrEqualTo,
    IsGreaterThan
};

UENUM(BlueprintType)
enum class EWarriorGameDifficulty : uint8
{
    Easy,
    Normal,
    Hard,
    VeryHard
};