// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraTransition.h"
#include "AutoCamera.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

// Sets default values
ACameraTransition::ACameraTransition()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraTransition::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraTransition::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	const float TimeBetweenCameraChanges = 2.0f;
	const float SmoothBlendTime = 1.0f;

	TimeToNextCameraChange -= DeltaTime;

	if (TimeToNextCameraChange <= 0.0f) {
		TimeToNextCameraChange += TimeBetweenCameraChanges;

		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);

		if (OurPlayerController) {

			if ((OurPlayerController->GetViewTarget() != CameraOne) && (CameraOne != nullptr)) {

				OurPlayerController->SetViewTarget(CameraOne);

			} else if ((OurPlayerController->GetViewTarget() != CameraTwo) && (CameraTwo != nullptr)) {
				
				//OurPlayerController->SetViewTarget(CameraTwo);
				OurPlayerController->SetViewTargetWithBlend(CameraTwo, SmoothBlendTime);

			}
		}
	}





}

