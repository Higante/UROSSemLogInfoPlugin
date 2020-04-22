// Fill out your copyright notice in the Description page of Project Settings.

#include "AActorSemLogPublisher.h"
#include "Engine/World.h"
#include "UTags/Public/Tags.h"
#include "Map.h"

// Sets default values
AActorSemLogPublisher::AActorSemLogPublisher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorSemLogPublisher::BeginPlay()
{
	Super::BeginPlay();
	
	// Checking for nullptr
	check(GetGameInstance());

	UROSBridgeGameInstance* ActiveGameInstance = Cast<UROSBridgeGameInstance>(GetGameInstance());
	check(ActiveGameInstance);

	check(ActiveGameInstance->ROSHandler.IsValid());

	// PoseStamped Publisher
	Publisher = MakeShareable<FROSBridgePublisher>(new FROSBridgePublisher(PoseStampedTopic, TEXT("geometry_msgs/PoseStamped")));
	ActiveGameInstance->ROSHandler->AddPublisher(Publisher);
	TSharedPtr<FROSDeleteObjectServer> ServiceServer = MakeShareable(new FROSDeleteObjectServer(TEXT("/delete_trigger"), GetSemLogObjects()));
	ActiveGameInstance->ROSHandler->AddServiceServer(ServiceServer);
	
	ActiveGameInstance->ROSHandler->Process();
}

// Called every frame
void AActorSemLogPublisher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TSharedPtr<std_msgs::String> StringMsgPtr(new std_msgs::String(TEXT("TEST")));
	TSharedPtr<geometry_msgs::PoseStamped> PoseMsgPtr(new geometry_msgs::PoseStamped(std_msgs::Header(12,FROSTime::Now(),TEXT("Test")), geometry_msgs::Pose(geometry_msgs::Point(1,1,1), geometry_msgs::Quaternion())));

	// Current ROS Game instance.
	UROSBridgeGameInstance* ActiveGameInstance = Cast<UROSBridgeGameInstance>(GetGameInstance());
	check(ActiveGameInstance);

	TimeCounterRosCall += DeltaTime;
	if (TimeCounterRosCall >= 1.00)
	{
		TimeCounterRosCall -= 1.00;
		PublishAllObjectsWithTag(ActiveGameInstance, PoseStampedTopic, "SemLog");
	}
	ActiveGameInstance->ROSHandler->Process();
}

TMap<AActor*, FJsonSerializableKeyValueMap> AActorSemLogPublisher::GetSemLogObjects()
{
	TMap<AActor*, FJsonSerializableKeyValueMap> Actors;

	Actors = FTags::GetActorsToKeyValuePairs(this->GetWorld(), "SemLog");

	return Actors;
}

void AActorSemLogPublisher::PublishAllObjectsWithTag(UROSBridgeGameInstance* Instance, FString Topic, FString Tag)
{
	// Set up a Handler Variable and an Actor Map.
	TSharedPtr<FROSBridgeHandler> Handler = Instance->ROSHandler;
	TMap<AActor*, FJsonSerializableKeyValueMap> ActorMap = FTags::GetActorsToKeyValuePairs(this->GetWorld(), Tag);
	TArray<AActor*> KeyList;
	TArray<FJsonSerializableKeyValueMap> ValueArray;
	ActorMap.GetKeys(KeyList);
	ActorMap.GenerateValueArray(ValueArray);

	for (int i = 0; i < ActorMap.Num(); i++)
	{
		FString* Value = ValueArray[i].Find("Id");

		TSharedPtr<geometry_msgs::PoseStamped> PoseMsgPtr(new geometry_msgs::PoseStamped(std_msgs::Header(0, FROSTime::Now(), *Value), geometry_msgs::Pose(geometry_msgs::Point(KeyList[i]->GetActorLocation()), geometry_msgs::Quaternion(KeyList[i]->GetActorRotation().Quaternion()))));
		Handler->PublishMsg(Topic, PoseMsgPtr);
	}
}

