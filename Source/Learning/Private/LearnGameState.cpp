#include "LearnGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "LearnPlayerController.h"
#include "LearnGameInstance.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "CoinItem.h"



ALearnGameState::ALearnGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void ALearnGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ALearnGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 ALearnGameState::GetScore() const
{
	return Score;
}
void  ALearnGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		ULearnGameInstance* LearnGameInstance = Cast<ULearnGameInstance>(GameInstance);
		if (LearnGameInstance)
		{
			LearnGameInstance->AddToScore(Amount);
		}
	}
}

void ALearnGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ALearnPlayerController* LearnPlayerController = Cast<ALearnPlayerController>(PlayerController))
		{
			LearnPlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		ULearnGameInstance* LearnGameInstance = Cast<ULearnGameInstance>(GameInstance);
		if (LearnGameInstance)
		{
			CurrentLevelIndex = LearnGameInstance->CurrentLevelIndex;
		}
	}

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ALearnGameState::OnLevelTimerUp,
		LevelDuration,
		false
	);
}

void ALearnGameState::OnLevelTimerUp()
{
	EndLevel();
}

void ALearnGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	UE_LOG(LogTemp, Warning, TEXT("Coin Collected %d / %d"), 
		CollectedCoinCount,
		SpawnedCoinCount);

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}

void ALearnGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		ULearnGameInstance* LearnGameInstance = Cast<ULearnGameInstance>(GameInstance);
		if (LearnGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			LearnGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
		if (CurrentLevelIndex >= MaxLevels)
		{
			OnGameOver();
			return;
		}

		if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
		{
			UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
		}
		else
		{
			OnGameOver();
		}
	}
}

void ALearnGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ALearnPlayerController* LearnPlayerController = Cast<ALearnPlayerController>(PlayerController))
		{
			LearnPlayerController->SetPause(true);
			LearnPlayerController->ShowMainMenu(true);
		}
	}
}

void ALearnGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ALearnPlayerController* LearnPlayerController = Cast<ALearnPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = LearnPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time : %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						ULearnGameInstance* LearnGameInstance = Cast<ULearnGameInstance>(GameInstance);
						if (LearnGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score %d"), LearnGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level : %d"), CurrentLevelIndex + 1)));
				}
			}
		}
	}
}

