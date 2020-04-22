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
	//Time Counter
	float TimeCounterRosCall = 0.00;
};
