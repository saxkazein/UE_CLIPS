#pragma once

#include "Components/ActorComponent.h"
#include "Array.h"
#include "IDelegateInstance.h"
#include "CLIPSClient.generated.h"

class FCLIPSWorker;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCLIPSClientEventResult, FString, Result);
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE_CLIPS_API UCLIPSClient : public UActorComponent
{
	GENERATED_BODY()

public:
	UCLIPSClient();

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "CLIPS")
		void EvalQuery(const FString fs);
	UFUNCTION(BlueprintCallable, Category = "CLIPS")
		void CheckFact(const FString tipo, const FString id);
	UFUNCTION(BlueprintCallable, Category = "CLIPS")
		void AssertFact(const FString tipo, const FString id);
	UFUNCTION(BlueprintCallable, Category = "CLIPS")
		void RetractFact(const FString tipo, const FString id);
	UPROPERTY(BlueprintAssignable, Category = "CLIPS")
		FCLIPSClientEventResult OnQueryResult;

private:
	UFUNCTION()
		void EvalQueryCallback(int32 queryID, FString result);
	void LinkToWorker();
	FDelegateHandle evalQueryHandler;
	TArray<int32> querysInQueue;
	FCLIPSWorker* CLIPSWorker;
	bool bQueryResult = false;
	FString sQueryResult = "";

};
