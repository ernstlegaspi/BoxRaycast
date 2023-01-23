#include "BoxAI.h"
#include "Kismet/KismetMathLibrary.h"

ABoxAI::ABoxAI() {
	PrimaryActorTick.bCanEverTick = true;
}

void ABoxAI::BeginPlay() {
	Super::BeginPlay();

	BoxMove = GetActorLocation();
	NewPoint = BoxMove;
	Dist = FMath::Floor(FMath::RandRange(200.f, 600.f));;
	Speed = FMath::Floor(FMath::RandRange(250.f, 500.f));
	XDir = FMath::Floor(FMath::RandRange(0.f, 2.f));
	YDir = FMath::Floor(FMath::RandRange(0.f, 2.f));
	IsBothAxis = FMath::Floor(FMath::RandRange(0.f, 3.f)); // 2 = both axis, 1 = x axis, 0 = y axis
	YRand = YDir == 1.f ? 1.f : -1.f;
	RestTime = -1.f;
	AInvisibleActor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorTransform());
}

void ABoxAI::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if(RestTime >= 0.f && RestTime <= 1.5f) {
		RestTime -= DeltaTime * 1;
		
		if(CanReassign) {
			XDir = FMath::Floor(FMath::RandRange(0.f, 2.f));
			YDir = FMath::Floor(FMath::RandRange(0.f, 2.f));
			IsBothAxis = FMath::Floor(FMath::RandRange(0.f, 3.f));
			YRand = YDir == 1.f ? 1.f : -1.f;
			NewPoint = BoxMove;
			CanReassign = false;
		}
	}
	else {
		if(IsBothAxis == 0.f) {
			if(YDir == 1.f) {
				if(BoxMove.Y < NewPoint.Y + Dist) {
					BoxMove.Y += Speed * DeltaTime;
					ActorX = GetActorLocation().X;
					FaceInvisbleActor(ActorX, NewPoint.Y + Dist);
				}
				else ResetVariables();
			}
			else {
				if(BoxMove.Y > NewPoint.Y - Dist) {
					BoxMove.Y -= Speed * DeltaTime;
					ActorX = GetActorLocation().X;
					FaceInvisbleActor(ActorX, NewPoint.Y - Dist);
				}
				else ResetVariables();
			}
		}
		else if(IsBothAxis == 1.f) {
			if(XDir == 1.f) {
				if(BoxMove.X < NewPoint.X + Dist) {
					BoxMove.X += Speed * DeltaTime;
					ActorY = GetActorLocation().Y;
					FaceInvisbleActor(NewPoint.X + Dist, ActorY);
				}
				else ResetVariables();
			}
			else {
				if(BoxMove.X > NewPoint.X - Dist) {
					BoxMove.X -= Speed * DeltaTime;
					ActorY = GetActorLocation().Y;
					FaceInvisbleActor(NewPoint.X - Dist, ActorY);
				}
				else ResetVariables();
			}
		}
		else {
			if(XDir == 1.f) {
				if(BoxMove.X < NewPoint.X + Dist) {
					BoxMove.X += Speed * DeltaTime;
					BoxMove.Y = BoxMove.Y + (Speed * YRand) * DeltaTime;
					FaceInvisbleActor(NewPoint.X + Dist, YRand == 1 ? NewPoint.Y + Dist : NewPoint.Y - Dist);
				}
				else ResetVariables();
			}
			else {
				if(BoxMove.X > NewPoint.X - Dist) {
					BoxMove.X -= Speed * DeltaTime;
					BoxMove.Y = BoxMove.Y + (Speed * YRand) * DeltaTime;
					FaceInvisbleActor(NewPoint.X - Dist, YRand == 1 ? NewPoint.Y + Dist : NewPoint.Y - Dist);
				}
				else ResetVariables();
			}
		}
	}
}

void ABoxAI::FaceInvisbleActor(int x, int y) {
	AInvisibleActor->SetActorLocation(FVector(x, y, 0));
	InvisibleActorRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AInvisibleActor->GetActorLocation());
	SetActorRotation(FRotator(0, InvisibleActorRotation.Yaw, 0));
	SetActorLocation(BoxMove);
}

void ABoxAI::ResetVariables() {
	RestTime = .5f;
	CanReassign = true;
}