#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HelperFunctions.generated.h"

UCLASS()
class QUESTSYSTEM_API UHelperFunctions : public UObject
{
	GENERATED_BODY()

public:
	static const AActor* LineTrace(const UObject* WorldContextObject, const FVector& StartTrace, const FVector& EndTrace, const AActor* IgnoreActor = nullptr, const FCollisionResponseParams& ResponseParam = FCollisionResponseParams::DefaultResponseParam);
	static const AActor* SphereTrace(const UObject* WorldContextObject, const FVector& StartTrace, const FVector& EndTrace, float Radius, const AActor* IgnoreActor = nullptr, const FCollisionResponseParams& ResponseParam = FCollisionResponseParams::DefaultResponseParam);

	UFUNCTION(BlueprintCallable, Category = "Helper Functions")
	static const APlayerCameraManager* GetCameraManager(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Helper Functions")
	static const APlayerController* GetLocalPlayerController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Helper Functions")
	static FTransform CalculateChildRelativeTransform(const FTransform& ParentTransform, const FTransform& ChildTransform);

	UFUNCTION(BlueprintPure, Category = "Helper Functions")
	static FTransform CalculateChildWorldTransform(const FTransform& ParentTransform, const FTransform& RelativeTransform);

	UFUNCTION(BlueprintPure, Category = "Helper Functions")
	static FVector CalculateCircularPathOffset(const FRotator& Rotation, double Radius, const FRotator& AxisRotation);

	UFUNCTION(BlueprintPure, Category = "Helper Functions")
	static double AngleDegBetweenVectors(const FVector& Vector1, const FVector& Vector2);
};
