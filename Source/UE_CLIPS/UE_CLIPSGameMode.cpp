#include "UE_CLIPS.h"
#include "UE_CLIPSGameMode.h"
#include "UE_CLIPSBall.h"

AUE_CLIPSGameMode::AUE_CLIPSGameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = AUE_CLIPSBall::StaticClass();
}
