#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.generated.h"

USTRUCT(BlueprintType)
struct FItemSpawnRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass; // <액터>의 하위 클래스를 전부 포괄할 수있는 템플릿, 항상 메모리에 로드된 상태에서 접근
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;
};
