#include "FROSDeleteObjectServer.h"
#include "Engine/World.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSDeleteObjectServer::FromJson(TSharedPtr<FJsonObject> FJsonObject) const
{
	/*
	TSharedPtr<std_srvs::Trigger::Request> Request = MakeShareable(std_srvs::Trigger::Request());
	
	Request->FromJson(FJsonObject);
	*/

	return TSharedPtr<FROSBridgeSrv::SrvRequest>();
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSDeleteObjectServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> InRequest)
{
	TSharedPtr<std_srvs::Trigger::Request> Request = StaticCastSharedPtr<std_srvs::Trigger::Request>(InRequest);

	UE_LOG(LogTemp, Log, TEXT("Response: Deleting Stuff"));

	// SemLog Delete
	DeleteSemLogObjects();

	return MakeShareable<std_srvs::Trigger::SrvResponse>(new std_srvs::Trigger::Response(true, ""));
}

void FROSDeleteObjectServer::DeleteSemLogObjects()
{
	TMap<AActor*, FJsonSerializableKeyValueMap> ActorMap = ActorRef;

	TArray<AActor*> KeyList;
	ActorMap.GetKeys(KeyList);

	for (AActor* aActor : KeyList)
	{
		aActor->Destroy();
	}
}