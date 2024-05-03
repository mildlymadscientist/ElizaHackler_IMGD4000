#pragma once

#include "CoreMinimal.h"
#include "FlockingManager.generated.h"

UCLASS()
class FLOCKING_API UFlockingManager : public UObject
{

public:
	GENERATED_BODY()

	void Init(UWorld* world, UStaticMeshComponent* mesh);
	void Flock();

private:
	UWorld* World;
	bool initialized;
	TArray<class AAgent*> Agents;

	FVector rule1(AAgent* agentOrig);
	FVector rule2(AAgent* agentOrig);
	FVector rule3(AAgent* agentOrig);
	FVector tend_to_place(AAgent* agentOrig);
};