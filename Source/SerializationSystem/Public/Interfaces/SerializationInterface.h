// Copyright shenkns Serialization System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "UObject/Interface.h"
#include "SerializationInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USerializationInterface : public UInterface
{
	GENERATED_BODY()
};

class SERIALIZATIONSYSTEM_API ISerializationInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Serialization")
	bool PreConvertToSaveData();

	UFUNCTION(BlueprintNativeEvent, Category = "Serialization")
	bool PostConvertFromSaveData();
};
