#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxAI.generated.h"

UCLASS()
class BOXRAYCAST_API ABoxAI : public AActor {
	GENERATED_BODY()
	
public:	
	ABoxAI();

	UPROPERTY(EditAnywhere, Category = "ABoxAI")
	TSubclassOf<AActor> ActorToSpawn;

protected:
	AActor* AInvisibleActor;
	FRotator InvisibleActorRotation;
	FVector BoxMove, NewPoint;
	bool CanReassign;
	float Speed, RestTime, XDir, YDir, YRand, IsBothAxis, ActorX, ActorY;
	short int Dist;

	void ResetVariables();
	void FaceInvisbleActor(int x, int y);

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};