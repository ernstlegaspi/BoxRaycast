#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InvisibleActor.generated.h"

UCLASS()
class BOXRAYCAST_API AInvisibleActor : public AActor {
	GENERATED_BODY()
	
public:	
	AInvisibleActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};