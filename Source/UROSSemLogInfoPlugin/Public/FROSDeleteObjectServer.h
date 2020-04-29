#pragma once

#include "CoreMinimal.h"
#include "UTags.h"
#include "UTags/Public/Tags.h"
#include "std_srvs/Trigger.h"
#include "ROSBridgeSrvServer.h"

class UROSSEMLOGINFOPLUGIN_API FROSDeleteObjectServer : public FROSBridgeSrvServer
{

public:
	TMap<AActor*, FJsonSerializableKeyValueMap> ActorRef;
	FString TypeToDelete;
	FString KeyToDelete;

	FROSDeleteObjectServer(const FString& InName, TMap<AActor*, FJsonSerializableKeyValueMap> ActorRef, FString KeytoDelete, FString TypeToDelete) : FROSBridgeSrvServer(InName, TEXT("std_srvs/Trigger")) 
	{ 
		this->ActorRef = ActorRef;
		this->TypeToDelete = TypeToDelete;
		this->KeyToDelete = KeytoDelete;
	};

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> FJsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> InRequest) override;

private:
	bool DeleteSemLogObjects();
};