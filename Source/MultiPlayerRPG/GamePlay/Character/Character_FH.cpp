#include "Character_FH.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MultiPlayerRPG/GamePlay/PlayerController/PlayerController_FH.h"
#include "MultiPlayerRPG/GlobalVariable/TickTime.h"
#include "Net/UnrealNetwork.h"

ACharacter_FH::ACharacter_FH()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(30.f, 88.f);
	
	Fh_PlayerSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Fh_PlayerSpringArmComponent->SetupAttachment(RootComponent);
	Fh_PlayerSpringArmComponent->bUsePawnControlRotation = true;
	Fh_PlayerSpringArmComponent->SetRelativeLocation(FVector(0.f, 0.f, 75.f));
	Fh_PlayerSpringArmComponent->TargetArmLength = 400.f;
	Fh_PlayerSpringArmComponent->bEnableCameraLag = true;
	Fh_PlayerSpringArmComponent->bEnableCameraRotationLag = true;
	Fh_PlayerSpringArmComponent->CameraLagSpeed = 5.f;
	Fh_PlayerSpringArmComponent->CameraRotationLagSpeed = 10.f;
	Fh_PlayerSpringArmComponent->CameraLagMaxDistance = 100.f;
	Fh_PlayerSpringArmComponent->SocketOffset = FVector(0.f, 80.f, 0.f);

	Fh_PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Fh_PlayerCameraComponent->SetupAttachment(Fh_PlayerSpringArmComponent, USpringArmComponent::SocketName);
	Fh_PlayerCameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->MaxAcceleration = 980.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	bReplicates = true;
	NetUpdateFrequency = TICKNUM_60;
	MinNetUpdateFrequency = TICKNUM_60;
}

void ACharacter_FH::BeginPlay()
{
	Super::BeginPlay();

	Fh_OwnerController = Cast<APlayerController_FH>(Controller);
	if (Fh_OwnerController)
	{
		// Init Character Info Data
		FH_ServerInitCharacterInfoData();
		
		// Bind Input Delegate
		Fh_OwnerController->FHDG_MoveForward.BindUObject(this, &ThisClass::FH_OnMoveForward);
		Fh_OwnerController->FHDG_MoveRight.BindUObject(this, &ThisClass::FH_OnMoveRight);
		Fh_OwnerController->FHDG_PitchCamera.BindUObject(this, &ThisClass::FH_OnPitchCamera);
		Fh_OwnerController->FHDG_YawCamera.BindUObject(this, &ThisClass::FH_OnYawCamera);
		Fh_OwnerController->FHDG_Jump.BindUObject(this, &ThisClass::FH_OnJump);
		Fh_OwnerController->FHDG_StopJumping.BindUObject(this, &ThisClass::FH_OnStopJumping);
	}
}

void ACharacter_FH::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Fh_UserInfoData);
	DOREPLIFETIME(ThisClass, Fh_PlayerInfoData);
	DOREPLIFETIME(ThisClass, Fh_WeaponInfoFirstSlotData);
	DOREPLIFETIME(ThisClass, Fh_WeaponInfoSecondSlotData);
}

void ACharacter_FH::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

/* My Code */
/*
 * Controller Delegate Input Bind
 */
void ACharacter_FH::FH_OnMove(const EAxis::Type Axis, const float Value)
{
	Fh_OwnerController = Fh_OwnerController == nullptr ? Cast<APlayerController_FH>(Controller) : Fh_OwnerController;
	if (Fh_OwnerController && Value != 0.f)
	{
		const FRotator Rotation = Fh_OwnerController->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(Axis);
		AddMovementInput(Direction, Value);
	}
}

void ACharacter_FH::FH_OnMoveForward(const float Value)
{
	FH_OnMove(EAxis::X, Value);
}

void ACharacter_FH::FH_OnMoveRight(const float Value)
{
	FH_OnMove(EAxis::Y, Value);
}

void ACharacter_FH::FH_OnPitchCamera(const float Value)
{
	AddControllerPitchInput(Value);
}

void ACharacter_FH::FH_OnYawCamera(const float Value)
{
	AddControllerYawInput(Value);
}

void ACharacter_FH::FH_OnJump()
{
	Jump();
}

void ACharacter_FH::FH_OnStopJumping()
{
	StopJumping();
}

/*
 * Init UseInfo, PlayerInfo, WeaponInfo From Controller
 */
void ACharacter_FH::FH_ServerInitCharacterInfoData_Implementation()
{
	Fh_OwnerController = Fh_OwnerController == nullptr ? Cast<APlayerController_FH>(Controller) : Fh_OwnerController;
	if (Fh_OwnerController)
	{
		Fh_UserInfoData = Fh_OwnerController->Fh_UserInfoData;
		Fh_PlayerInfoData = Fh_OwnerController->Fh_PlayerInfoData;
		Fh_WeaponInfoFirstSlotData = Fh_OwnerController->Fh_WeaponInfoFirstSlotData;
		Fh_WeaponInfoSecondSlotData = Fh_OwnerController->Fh_WeaponInfoSecondSlotData;
	}
}