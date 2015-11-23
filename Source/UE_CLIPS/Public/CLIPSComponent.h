#pragma once

#include "Components/SceneComponent.h"
#include "CLIPSWorker.h"
#include "CLIPSComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_CLIPS_API UCLIPSComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UCLIPSComponent();

	virtual void BeginPlay() override;
	
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void BeginDestroy();

	void InitCLIPS();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CLIPS")
		bool loadFromFile = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CLIPS")
		FText buildCode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CLIPS")
		FText fileLocation;
	UFUNCTION(BlueprintCallable, Category = "CLIPS")
		void CLIPSBuild(FString fs);
	UFUNCTION(BlueprintCallable, Category = "CLIPS")
		void CLIPSLoad(FString fs);
	UFUNCTION(BlueprintCallable, Category = "CLIPS")
		FString CLIPSOutput();
	UFUNCTION(BlueprintCallable, Category = "CLIPS")
		void CLIPSClear();
private:
		FCLIPSWorker* CLIPSWorker;
};
