#include "UE_CLIPS.h"
#include "CLIPSComponent.h"
#include <iostream>
#include <sstream>
#include <string>
UCLIPSComponent::UCLIPSComponent()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


void UCLIPSComponent::BeginPlay()
{
	Super::BeginPlay();

	this->InitCLIPS();
}

void UCLIPSComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

void UCLIPSComponent::BeginDestroy()
{
	CLIPSWorker = NULL;
	FCLIPSWorker::Shutdown();
	Super::BeginDestroy();
}

void UCLIPSComponent::InitCLIPS()
{
	if (CLIPSWorker == nullptr)
		CLIPSWorker = FCLIPSWorker::ClipsInit();
	if (loadFromFile)
	{
		CLIPSLoad(fileLocation.ToString());
	}
	else
	{
		CLIPSBuild(buildCode.ToString());
	}
}

void UCLIPSComponent::CLIPSBuild(FString fs)
{
	CLIPSWorker->CLIPSBuild(fs);
}

void UCLIPSComponent::CLIPSLoad(FString fs)
{
	CLIPSWorker->CLIPSLoad(fs);
}

FString UCLIPSComponent::CLIPSOutput()
{
	std::string outString = CLIPSWorker->GetOutput();
	FString retString = ANSI_TO_TCHAR(outString.c_str());
	return retString;
}

void UCLIPSComponent::CLIPSClear()
{
	CLIPSWorker->ClearOutput();
}
