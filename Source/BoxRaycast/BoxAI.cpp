#include "BoxAI.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"

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
	BoxName = *GetFName().ToString();
	RayEnd = 300;
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
				SetStartAndEnd(0, 0, 50, RayEnd);

				if(BoxMove.Y < NewPoint.Y + Dist) {
					BoxMove.Y += Speed * DeltaTime;
					ActorX = GetActorLocation().X;
					FaceInvisbleActor(ActorX, NewPoint.Y + Dist);
				}
				else ResetVariables();
			}
			else {
				SetStartAndEnd(0, 0, -50, -RayEnd);

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
				SetStartAndEnd(50, RayEnd, 0, 0);

				if(BoxMove.X < NewPoint.X + Dist) {
					BoxMove.X += Speed * DeltaTime;
					ActorY = GetActorLocation().Y;
					FaceInvisbleActor(NewPoint.X + Dist, ActorY);
				}
				else ResetVariables();
			}
			else {
				SetStartAndEnd(-50, -RayEnd, 0, 0);

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
				SetStartAndEnd(50, RayEnd, YRand == 1 ? 50 : -50, YRand == 1 ? 200 : -RayEnd);

				if(BoxMove.X < NewPoint.X + Dist) {
					BoxMove.X += Speed * DeltaTime;
					BoxMove.Y = BoxMove.Y + (Speed * YRand) * DeltaTime;
					FaceInvisbleActor(NewPoint.X + Dist, YRand == 1 ? NewPoint.Y + Dist : NewPoint.Y - Dist);
				}
				else ResetVariables();
			}
			else {
				SetStartAndEnd(-50, -RayEnd, YRand == 1 ? 50 : -50, YRand == 1 ? RayEnd : -RayEnd);

				if(BoxMove.X > NewPoint.X - Dist) {
					BoxMove.X -= Speed * DeltaTime;
					BoxMove.Y = BoxMove.Y + (Speed * YRand) * DeltaTime;
					FaceInvisbleActor(NewPoint.X - Dist, YRand == 1 ? NewPoint.Y + Dist : NewPoint.Y - Dist);
				}
				else ResetVariables();
			}
		}
	}
	
	DetectHit(XStart, XEnd, YStart, YEnd);
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

void ABoxAI::SetStartAndEnd(int _StartX, int _EndX, int _StartY, int _EndY) {
	XStart = _StartX;
	XEnd = _EndX;
	YStart = _StartY;
	YEnd = _EndY;
}

void ABoxAI::DetectHit(int StartX, int EndX, int StartY, int EndY) {
	if(GetWorld()) {
		Start = GetActorLocation() + FVector(StartX, StartY, 0);
		End = Start + FVector(EndX, EndY, 0);
		ActorHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());
		
		if(ActorHit && Hit.GetActor() && (BoxName != Hit.GetActor()->GetFName().ToString()) && !Hit.GetActor()->GetFName().ToString().Contains("Floor")) {
			Destroy();
			Hit.GetActor()->Destroy();
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Explosion, GetActorLocation());
		}
	}
}