// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgePublisher.h"
#include "ROSBridgeGameInstance.h"
#include "FROSDeleteObjectServer.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Quaternion.h"
#include "geometry_msgs/Pose.h"
#include "Array.h"
#include "AActorSemLogPublisher.generated.h"

UCLASS()
class UROSSEMLOGINFOPLUGIN_API AActorSemLogPublisher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorSemLogPublisher();
	TSharedPtr<FROSBridgePublisher> Publisher;

	UPROPERTY(EditAnywhere)
	FString PoseStampedTopic = FString("/sem_log_plugin/posestamped_messages");
	UPROPERTY(EditAnywhere)
	FString DeleteServiceName = FString("/sem_log_plugin/delete_all_objects");

	UPROPERTY(EditAnywhere)
		FString TypeToDelete = "Robosherlock";
	UPROPERTY(EditAnywhere)
		FString KeyToDelete = "spawned";

	FString TagToTrack = "Tracked";

private:
	// Time Counter
	float TimeCounterRosCall = 0.00;

	// Variables for Stability Check
	int TrackingSavedStates = 5;
	int StatesSavedRunTime = 0;
	// Memory for Tracked Object.
	TArray<FVector> ActorsSavedStates;
	TArray<AActor*> TrackedActorsInScene; 
	TSharedPtr<FROSDeleteObjectServer> ServiceServer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Method to search all Sem_Log Items. Deprecated - Me Idiota
	TMap<AActor*, FJsonSerializableKeyValueMap> GetSemLogObjects();

	void PublishAllObjectsWithTag(UROSBridgeGameInstance* Instance, FString Topic, FString Tag);

private:
	// Collects all the Actors with the "Tracked" Tag into an array.
	void CollectTrackedActors();
	// Goes through the collected Tracked Actors and saves their positions. 
	void SaveTrackedActorStates();
	// Chesk whether or not the given Tracked Actor is stable or not.
	bool IsStableTrackedActor(AActor* RefActor);
};
