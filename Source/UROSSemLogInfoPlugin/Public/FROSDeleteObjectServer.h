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

	FROSDeleteObjectServer(const FString& InName, TMap<AActor*, FJsonSerializableKeyValueMap> ActorRef) : FROSBridgeSrvServer(InName, TEXT("std_srvs/Trigger")) { this->ActorRef = ActorRef; };

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> FJsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> InRequest) override;

private:
	void DeleteSemLogObjects();
};