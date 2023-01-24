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
	FHitResult Hit;
	FRotator InvisibleActorRotation;
	FString BoxName;
	FVector BoxMove, NewPoint, Start, End, Forward, Right;
	bool CanReassign, ActorHit;
	float Speed, RestTime, XDir, YDir, YRand, IsBothAxis, ActorX, ActorY, XStart, XEnd, YStart, YEnd;
	short int Dist;

	void ResetVariables();
	void FaceInvisbleActor(int x, int y);
	void DetectHit(int StartX, int EndX, int StartY, int EndY);
	void SetStartAndEnd(int _StartX, int _EndX, int _StartY, int _EndY);

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};