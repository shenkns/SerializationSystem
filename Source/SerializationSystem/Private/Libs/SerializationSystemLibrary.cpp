// Copyright shenkns Serialization System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Libs/SerializationSystemLibrary.h"

SERIALIZATIONSYSTEM_API DEFINE_LOG_CATEGORY(LogSerializationSystem)

bool USerializationSystemLibrary::ConvertObjectToSaveData(UObject* Object, FObjectSaveData& SaveData)
{
	return ConvertObjectToSaveData<UObject>(Object, SaveData);
}

bool USerializationSystemLibrary::ConvertObjectsToSaveData(TArray<UObject*> Objects, TArray<FObjectSaveData>& SaveData)
{
	return ConvertObjectsToSaveData<UObject>(Objects, SaveData);
}

UObject* USerializationSystemLibrary::ConvertSaveDataToObject(const FObjectSaveData& SaveData, UObject* ObjectOuter)
{
	return ConvertSaveDataToObject<UObject>(SaveData, ObjectOuter);
}

TArray<UObject*> USerializationSystemLibrary::ConvertSaveDataToObjects(const TArray<FObjectSaveData>& SaveData, UObject* ObjectsOuter)
{
	return ConvertSaveDataToObjects<UObject>(SaveData, ObjectsOuter);
}