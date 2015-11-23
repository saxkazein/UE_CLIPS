
#include "UE_CLIPS.h"
#include "CLIPSWorker.h"
#include "CLIPSClient.h"
UCLIPSClient::UCLIPSClient()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

}


void UCLIPSClient::BeginPlay()
{
	Super::BeginPlay();
	LinkToWorker();
}

void UCLIPSClient::LinkToWorker()
{
	CLIPSWorker = FCLIPSWorker::GetClipsWorker();
	if (CLIPSWorker == nullptr)
		return;
	evalQueryHandler = CLIPSWorker->OnEvalEnd().AddUObject(this, &UCLIPSClient::EvalQueryCallback);
}


void UCLIPSClient::BeginDestroy()
{
	Super::BeginDestroy();
	if (CLIPSWorker != nullptr)
	{
		CLIPSWorker->OnEvalEnd().Remove(evalQueryHandler);
		CLIPSWorker = nullptr;
	}
}

void UCLIPSClient::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bQueryResult)
	{
		bQueryResult = false;
		if (!sQueryResult.IsEmpty())
			OnQueryResult.Broadcast(sQueryResult);
		sQueryResult = "";
	}
}

void UCLIPSClient::EvalQuery(const FString fs)
{
	if (CLIPSWorker == nullptr)
		LinkToWorker();
	if (CLIPSWorker == nullptr)
		return;
	if (querysInQueue.Num() > 10)
		return;
	int32 id = CLIPSWorker->CLIPSEval(fs);
	if (id > 0)
		querysInQueue.Add(id);
}

void UCLIPSClient::CheckFact(const FString tipo, const FString id)
{
	EvalQuery("(find-fact ((?f "+ tipo +")) (eq ?f:id "+ id +"))");
}

void UCLIPSClient::AssertFact(const FString tipo, const FString id)
{
	EvalQuery("(assert ("+ tipo +" (id "+ id +")))");
}


void UCLIPSClient::RetractFact(const FString tipo, const FString id)
{
	EvalQuery("(do-for-all-facts ((?f "+tipo+")) (eq ?f:id "+id+") (retract ?f))");
}

void UCLIPSClient::EvalQueryCallback(int32 queryID, FString result)
{
	if (querysInQueue.Find(queryID) != INDEX_NONE)
	{
		querysInQueue.Remove(queryID);
		bQueryResult = true;
		sQueryResult = result;
	}
}
