#include "LearnGameInstance.h"

ULearnGameInstance::ULearnGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void ULearnGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}