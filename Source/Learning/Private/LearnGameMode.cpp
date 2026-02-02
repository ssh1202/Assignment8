#include "LearnGameMode.h"
#include "LearnCharacter.h"
#include "LearnPlayerController.h"
#include "LearnGameState.h"

ALearnGameMode::ALearnGameMode()
{
	DefaultPawnClass = ALearnCharacter::StaticClass();
	PlayerControllerClass = ALearnPlayerController::StaticClass();
	GameStateClass = ALearnGameState::StaticClass();
}
