#include "ThreadingBase.h"
#include <string.h>
#include "Queue.h"
#include "Map.h"
#include "clipscpp.h"				   

class FCLIPSWorker : public FRunnable
{
	static FCLIPSWorker* Runnable;

	FRunnableThread* Thread;
	
	FThreadSafeCounter StopTaskCounter;

public:

	DECLARE_EVENT_TwoParams(FCLIPSWorker, FEvalEndEvent, int, FString)
	FEvalEndEvent& OnEvalEnd(){ return EvalEndEvent; }

	bool IsFinished() const
	{
		return isStopped;
	}

	FCLIPSWorker();
	virtual ~FCLIPSWorker();

	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

	void EnsureCompletion();

	void CLIPSBuild(FString fs);
	void CLIPSLoad(FString fs);
	void CLIPSRun(int inInt);
	void CLIPSReset();
	std::string GetOutput();
	int32 CLIPSEval(FString fs);
	void ClearOutput();

	static FCLIPSWorker* ClipsInit();
	static FCLIPSWorker* GetClipsWorker();
	static void Shutdown();
	static bool IsThreadFinished();


private:
	void HandleEval();
	static void EmptyQueue();
	UPROPERTY()
	static CLIPS::CLIPSCPPEnv* theEnv;

	UPROPERTY()
	static CLIPS::CLIPSCPPRouter* theRouter;

	UPROPERTY()
	bool isStopped = false;

	UPROPERTY()
	static FCriticalSection CriticalSection;
	UPROPERTY()
	static FCriticalSection InitCriticalSection;
	UPROPERTY()
	static FCriticalSection ShutdownCriticalSection;
	UPROPERTY()
	static TMap<int, FString> EvalMap;
	UPROPERTY()
	static TQueue<int, EQueueMode::Mpsc> EvalQueue;
	UPROPERTY()
	FEvalEndEvent EvalEndEvent;
};