#include "FROSDeleteObjectServer.h"
#include "Engine/World.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSDeleteObjectServer::FromJson(TSharedPtr<FJsonObject> FJsonObject) const
{
	return TSharedPtr<FROSBridgeSrv::SrvRequest>();
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSDeleteObjectServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> InRequest)
{
	TSharedPtr<std_srvs::Trigger::Request> Request = StaticCastSharedPtr<std_srvs::Trigger::Request>(InRequest);

	UE_LOG(LogTemp, Log, TEXT("Response: Deleting Stuff"));
	// SemLog Delete

	bool bSuccess = true;
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
	  bSuccess = DeleteSemLogObjects();
	}, TStatId(), nullptr, ENamedThreads::GameThread);


	//wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
	return MakeShareable<std_srvs::Trigger::SrvResponse>(new std_srvs::Trigger::Response(bSuccess, ""));
}

bool FROSDeleteObjectServer::DeleteSemLogObjects()
{
	TMap<AActor*, FJsonSerializableKeyValueMap> Actors;
	Actors = FTags::GetActorsToKeyValuePairs(WorkingWorld, "BeliefStateInfo");

	TArray<AActor*> KeyList;
	TArray<AActor*> ActorsToDelete;
	Actors.GetKeys(KeyList);

	TArray<FJsonSerializableKeyValueMap> ValueArray;
	Actors.GenerateValueArray(ValueArray);


	for (int i = 0; i < Actors.Num(); i++)
	{
		if (ValueArray[i].Contains(KeyToDelete))
		{
			ActorsToDelete.Add(KeyList[i]);
		}
	}

	for (AActor* aActor : ActorsToDelete)
	{
		aActor->Destroy();
	}

	return ActorsToDelete.Num() == 0;
}
