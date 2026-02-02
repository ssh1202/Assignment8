#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LearnGameInstance.generated.h"

UCLASS()
class LEARNING_API ULearnGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	ULearnGameInstance();

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
};
