// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraTransition.generated.h"

UCLASS()
class AUTOCAMERA_API ACameraTransition : public AActor {

	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACameraTransition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	AActor* CameraOne;

	UPROPERTY(EditAnywhere)
	AActor* CameraTwo;

	float TimeToNextCameraChange;

};
