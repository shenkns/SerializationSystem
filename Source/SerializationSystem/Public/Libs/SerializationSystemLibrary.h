// Copyright shenkns Serialization System Developed With Unreal Engine. All Rights Reserved 2023.

#pragma once

#include "Log.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Interfaces/SerializationInterface.h"
#include "Log/Details/LocalLogCategory.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

#include "SerializationSystemLibrary.generated.h"

SERIALIZATIONSYSTEM_API DECLARE_LOG_CATEGORY_EXTERN(LogSerializationSystem, Log, All);
DEFINE_LOG_CATEGORY_LOCAL(LogSerializationSystem);

USTRUCT(BlueprintType)
struct FObjectSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Serialization")
	TArray<uint8> BinaryData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Serialization")
	TSubclassOf<UObject> ObjectClass;
};

UCLASS()
class SERIALIZATIONSYSTEM_API USerializationSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// Serialization
	UFUNCTION(BlueprintCallable, Category = "SerializationSystem")
	static bool ConvertObjectToSaveData(UObject* Object, FObjectSaveData& SaveData);

	UFUNCTION(BlueprintCallable, Category = "SerializationSystem")
	static bool ConvertObjectsToSaveData(TArray<UObject*> Objects, TArray<FObjectSaveData>& SaveData);

	UFUNCTION(BlueprintCallable, Category = "SerializationSystem")
	static UObject* ConvertSaveDataToObject(const FObjectSaveData& SaveData, UObject* ObjectOuter = nullptr);

	UFUNCTION(BlueprintCallable, Category = "SerializationSystem")
	static TArray<UObject*> ConvertSaveDataToObjects(const TArray<FObjectSaveData>& SaveData, UObject* ObjectsOuter = nullptr);

	template<typename T>
	static bool ConvertObjectToSaveData(T* Object, FObjectSaveData& SaveData);
	
	template<typename T>
	static bool ConvertObjectsToSaveData(TArray<T*>& Objects, TArray<FObjectSaveData>& SaveData);

	template<typename T>
	static T* ConvertSaveDataToObject(const FObjectSaveData& SaveData, UObject* ObjectOuter = nullptr);

	template<typename T>
	static TArray<T*> ConvertSaveDataToObjects(const TArray<FObjectSaveData>& SaveData, UObject* ObjectsOuter = nullptr);

};

template <typename T>
bool USerializationSystemLibrary::ConvertObjectToSaveData(T* Object, FObjectSaveData& SaveData)
{
	if(!Object) return false;

	UObject* InterfaceObject = static_cast<UObject*>(Object);
	if(!InterfaceObject) return false;
	
	LOG(Display, "{} Serialization Started", InterfaceObject);

	if(InterfaceObject->GetClass()->ImplementsInterface(USerializationInterface::StaticClass()))
	{
		if(!ISerializationInterface::Execute_PreConvertToSaveData(InterfaceObject)) return false;
	}

	FMemoryWriter Writer(SaveData.BinaryData);
	FObjectAndNameAsStringProxyArchive Archive(Writer, true);

	InterfaceObject->Serialize(Archive);

	SaveData.ObjectClass = InterfaceObject->GetClass();

	LOG(Display, "{} Serialized Successful", InterfaceObject);

	return true;
}

template <typename T>
bool USerializationSystemLibrary::ConvertObjectsToSaveData(TArray<T*>& Objects, TArray<FObjectSaveData>& SaveData)
{
	SaveData.Empty();

	// Iterate Objects
	for(int i = 0; i < Objects.Num(); i++)
	{
		FObjectSaveData ObjectSaveData;

		if(USerializationSystemLibrary::ConvertObjectToSaveData<T>(Objects[i], ObjectSaveData))
		{
			SaveData.Add(ObjectSaveData);
			
			LOG(Display, "Serialized {}% Of Objects", FMath::CeilToInt((float)(i + 1) / (float)Objects.Num() * 100.f));
		}
	}

	LOG(Display, "{} Objects Serialized Successful", SaveData.Num());
        
	return SaveData.Num() == Objects.Num();
}

template <typename T>
T* USerializationSystemLibrary::ConvertSaveDataToObject(const FObjectSaveData& SaveData, UObject* ObjectOuter)
{
	if(SaveData.ObjectClass == nullptr || SaveData.BinaryData.Num() == 0) return nullptr;

	if(UObject* Object = NewObject<T>(ObjectOuter, SaveData.ObjectClass))
	{
		LOG(Display, "{} Deserialization Started", Object);
		
		FMemoryReader Reader(SaveData.BinaryData);
		FObjectAndNameAsStringProxyArchive Archive(Reader, true);
	
		Object->Serialize(Archive);

		if(Object->GetClass()->ImplementsInterface(USerializationInterface::StaticClass()))
		{
			if(!ISerializationInterface::Execute_PostConvertFromSaveData(Object)) return nullptr;
		}

		if(Object)
		{
			LOG(Display, "{} Deserialized Successful", Object);
			return static_cast<T*>(Object);
		}

		return nullptr;
	}

	return nullptr;
}

template <typename T>
TArray<T*> USerializationSystemLibrary::ConvertSaveDataToObjects(const TArray<FObjectSaveData>& SaveData, UObject* ObjectsOuter)
{
	TArray<T*> Objects;

	for(int i = 0; i < SaveData.Num(); i++)
	{
		if(T* Object = USerializationSystemLibrary::ConvertSaveDataToObject<T>(SaveData[i], ObjectsOuter))
		{
			Objects.Add(Object);

			LOG(Display, "Deserialized {}% Of Objects", FMath::CeilToInt((float)(i + 1) / (float)SaveData.Num() * 100.f));
		}
	}
	
	LOG(Display, "{} Objects Deserialized Successful", Objects.Num());

	return Objects;
}