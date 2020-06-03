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
	
	ActorsSavedStates = TArray<FVector>();
	ActorsSavedStates.Init(FVector(), TrackingSavedStates * 100);

	CollectTrackedActors();

	// Checking for nullptr
	check(GetGameInstance());

	UROSBridgeGameInstance* ActiveGameInstance = Cast<UROSBridgeGameInstance>(GetGameInstance());
	check(ActiveGameInstance);

	check(ActiveGameInstance->ROSHandler.IsValid());

	// PoseStamped Publisher
	Publisher = MakeShareable<FROSBridgePublisher>(new FROSBridgePublisher(PoseStampedTopic, TEXT("geometry_msgs/PoseStamped")));
	ActiveGameInstance->ROSHandler->AddPublisher(Publisher);
	ServiceServer = MakeShareable(new FROSDeleteObjectServer(DeleteServiceName, this->GetWorld(), GetSemLogObjects(), KeyToDelete));
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
	// 1 Sekunde belassen fürs erst.
	TimeCounterRosCall += DeltaTime;
	if (TimeCounterRosCall >= 1.00)
	{
		TimeCounterRosCall -= 1.00;
		PublishAllObjectsWithTag(ActiveGameInstance, PoseStampedTopic, TypeToDelete);
	}
	ActiveGameInstance->ROSHandler->Process();

	// Debug Stability out side ROS
	SaveTrackedActorStates();

	for (AActor* Actor : TrackedActorsInScene)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is stable: %s"), *Actor->GetName(), IsStableTrackedActor(Actor));
	}
}

TMap<AActor*, FJsonSerializableKeyValueMap> AActorSemLogPublisher::GetSemLogObjects()
{
	TMap<AActor*, FJsonSerializableKeyValueMap> Actors;

	Actors = FTags::GetActorsToKeyValuePairs(this->GetWorld(), TypeToDelete);

	return Actors;
}

void AActorSemLogPublisher::PublishAllObjectsWithTag(UROSBridgeGameInstance* Instance, FString Topic, FString Tag)
{
	// Set up a Handler Variable and an Actor Map.
	TSharedPtr<FROSBridgeHandler> Handler = Instance->ROSHandler;
	TMap<AActor*, FJsonSerializableKeyValueMap> ActorMap = FTags::GetActorsToKeyValuePairs(this->GetWorld(), "SemLog");
	TArray<AActor*> KeyList;
	TArray<FJsonSerializableKeyValueMap> ValueArray;
	ActorMap.GetKeys(KeyList);
	ActorMap.GenerateValueArray(ValueArray);

	for (int i = 0; i < ActorMap.Num(); i++)
	{
		FString* Value = ValueArray[i].Find("Id");
		if (!Value)
		{
			UE_LOG(LogTemp, Error, TEXT("ERROR: ID Tag not found in SemLog"));
			return;
		}
			
		TSharedPtr<geometry_msgs::PoseStamped> PoseMsgPtr(new geometry_msgs::PoseStamped(std_msgs::Header(0, FROSTime::Now(), *Value), geometry_msgs::Pose(geometry_msgs::Point(KeyList[i]->GetActorLocation()), geometry_msgs::Quaternion(KeyList[i]->GetActorRotation().Quaternion()))));
		Handler->PublishMsg(Topic, PoseMsgPtr);
	}
}

void AActorSemLogPublisher::CollectTrackedActors()
{
	TMap<AActor*, FJsonSerializableKeyValueMap> Actors;
	TArray<AActor*> OutActorArray;

	Actors = FTags::GetActorsToKeyValuePairs(this->GetWorld(), TagToTrack);
	Actors.GenerateKeyArray(OutActorArray);
	TrackedActorsInScene = OutActorArray;
}

void AActorSemLogPublisher::SaveTrackedActorStates()
{
	for (int i = 0; i < TrackedActorsInScene.Num(); i++)
	{
		ActorsSavedStates[i + 2] = ActorsSavedStates[i + 1];
		ActorsSavedStates[i + 1] = ActorsSavedStates[i];
		ActorsSavedStates[i] = TrackedActorsInScene[i]->GetActorLocation();
	}

	if (StatesSavedRunTime < 3)
		StatesSavedRunTime++;
}

bool AActorSemLogPublisher::IsStableTrackedActor(AActor * RefActor)
{
	if (!TrackedActorsInScene.Contains(RefActor))
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR: Given Actor is not tracked: %s"), *RefActor->GetName());
		return false;
	} else if (StatesSavedRunTime < 3)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR: Not enough Saved States to determine. Saved States: %i"), StatesSavedRunTime);
	}

	bool bResult = false;
	int PosActor = TrackedActorsInScene.Find(RefActor);

	bResult = ActorsSavedStates[PosActor].Equals(ActorsSavedStates[PosActor + 1]) 
		&& ActorsSavedStates[PosActor].Equals(ActorsSavedStates[PosActor + 2])
		&& ActorsSavedStates[PosActor + 1].Equals(ActorsSavedStates[PosActor + 2]);

	return bResult;
}

