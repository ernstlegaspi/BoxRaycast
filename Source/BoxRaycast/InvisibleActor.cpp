#include "InvisibleActor.h"

AInvisibleActor::AInvisibleActor() {
	PrimaryActorTick.bCanEverTick = true;
}

void AInvisibleActor::BeginPlay() {
	Super::BeginPlay();
}

void AInvisibleActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}