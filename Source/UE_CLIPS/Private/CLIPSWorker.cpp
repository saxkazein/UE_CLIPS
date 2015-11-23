#include "UE_CLIPS.h"
#include "CLIPSWorker.h"
#include "CustomFileRouter.h"
#include <sstream>
#include "Queue.h"

FCLIPSWorker* FCLIPSWorker::Runnable = NULL;
CLIPS::CLIPSCPPRouter* FCLIPSWorker::theRouter = NULL;

CLIPS::CLIPSCPPEnv* FCLIPSWorker::theEnv = NULL;
FCriticalSection FCLIPSWorker::CriticalSection;
FCriticalSection FCLIPSWorker::InitCriticalSection;
FCriticalSection FCLIPSWorker::ShutdownCriticalSection;
TMap<int, FString> FCLIPSWorker::EvalMap;
TQueue<int, EQueueMode::Mpsc> FCLIPSWorker::EvalQueue;

FCLIPSWorker::FCLIPSWorker()
{
	Thread = FRunnableThread::Create(this, TEXT("FCLIPSWorker"), 0, TPri_BelowNormal);
}

FCLIPSWorker::~FCLIPSWorker()
{
	delete Thread;
	Thread = NULL;
}

bool FCLIPSWorker::Init()
{
	FScopeLock ScopeLock(&InitCriticalSection);
	if (theEnv == NULL)
	{
		EvalMap.Empty();
		if (!EvalQueue.IsEmpty())
		{
			EmptyQueue();
		}
		theEnv = new CLIPS::CLIPSCPPEnv();
		theRouter = new CustomFileRouter();
		theEnv->AddRouter("unreal", 100, theRouter);
	}
	return true;
}

uint32 FCLIPSWorker::Run()
{
	FPlatformProcess::Sleep(0.03);
	while (StopTaskCounter.GetValue() == 0)
	{
		theEnv->Run(10);
		FScopeLock ScopeLock(&CriticalSection);
		if (!EvalQueue.IsEmpty())
		{
			HandleEval();
		}
	}
	return 0;
}

void FCLIPSWorker::HandleEval()
{
	FScopeLock ScopeLock(&CriticalSection);
	int index = -1;
	EvalQueue.Dequeue(index);
	if (index < 0)
		return;
	char* inText;
	FString* auxstr;
	CLIPS::DataObject theDO = CLIPS::DataObject();
	auxstr = EvalMap.Find(index);
	if (auxstr == nullptr)
		return;
	else
	{
		inText = TCHAR_TO_ANSI(**auxstr);
		EvalMap.Remove(index);
		theDO = theEnv->Eval(inText);

		std::stringstream outstream;
		theDO.getValue()->print(outstream);
		FString retString = FString(ANSI_TO_TCHAR(outstream.str().c_str()));
		EvalEndEvent.Broadcast(index, retString);
	}
}

void FCLIPSWorker::EmptyQueue()
{
	int a;
	while (!EvalQueue.IsEmpty())
	{
		EvalQueue.Dequeue(a);
	}
}

void FCLIPSWorker::CLIPSRun(int inInt)
{
	/* Deprecated */
	//	theEnv->Run(inInt);
}

void FCLIPSWorker::CLIPSReset()
{
	theEnv->Reset();
}

std::string FCLIPSWorker::GetOutput()
{
	if (theRouter != NULL)
	{
		return ((CustomFileRouter *)theRouter)->GetOutput();
	}
	return "NULL ROUTER";
}

int32 FCLIPSWorker::CLIPSEval(FString fs)
{
	int32 gen = -1;
	int evalQueueMaxSize = 1000;
	{
		FScopeLock ScopeLock(&CriticalSection);
		if (EvalMap.Num() >= evalQueueMaxSize)
			return gen;
		for (int i = 0; i < evalQueueMaxSize; i++)
		{
			gen = i + FMath::RandRange(0, evalQueueMaxSize - 1) * 31 * 7;
			if (EvalMap.Find(gen) == nullptr)
				break;
		}
		EvalQueue.Enqueue(gen);
		EvalMap.Add(gen, fs);
	}
	return gen;
}

void FCLIPSWorker::ClearOutput()
{
	if (theRouter != NULL)
	{
		((CustomFileRouter *)theRouter)->ClearOutput();
	}
}

void FCLIPSWorker::Stop()
{
	StopTaskCounter.Increment();
}

void FCLIPSWorker::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void FCLIPSWorker::CLIPSBuild(FString fs)
{
	FScopeLock ScopeLock(&CriticalSection);
	char* inText = TCHAR_TO_ANSI(*fs);
	theEnv->Build(inText);
	CLIPSReset();
}

void FCLIPSWorker::CLIPSLoad(FString fs)
{
	FScopeLock ScopeLock(&CriticalSection);
	FString pathDir = FPaths::GameContentDir() + fs;
	char* inText = (TCHAR_TO_ANSI(*pathDir));
	theEnv->Load(inText);
	CLIPSReset();
}

FCLIPSWorker* FCLIPSWorker::ClipsInit()
{
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		FScopeLock ScopeLock(&CriticalSection);
		if (!Runnable)
			Runnable = new FCLIPSWorker();
	}
	return Runnable;
}

FCLIPSWorker* FCLIPSWorker::GetClipsWorker()
{
	if (Runnable)
		return Runnable;
	return NULL;
}

void FCLIPSWorker::Shutdown()
{
	FScopeLock ScopeLock(&ShutdownCriticalSection);
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		//EvalQueue.~TQueue<int, EQueueMode::Mpsc>();
		EmptyQueue();
		EvalMap.Empty();
		delete theEnv;
		theEnv = NULL;
		delete theRouter;
		theRouter = NULL;
		delete Runnable;
		Runnable = NULL;
	}
}

bool FCLIPSWorker::IsThreadFinished()
{
	if (Runnable) return Runnable->IsFinished();
	return true;
}

