// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Painting.h"
#include "Player/JamPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
APainting::APainting()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PaintingMesh"));
	SetRootComponent(StaticMesh);
}

// Called when the game starts or when spawned
void APainting::BeginPlay()
{
	Super::BeginPlay();

	// Get/set a reference to the player character when the game starts
	PlayerCharacter = Cast<AJamPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

// Called every frame
void APainting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APainting::UnHighlightMesh()
{
	// If the actor the player is looking at ISN'T this actor:
	if (PlayerCharacter->LookAtActor != this)
	{
		// Disable highlight and clear the highlight timer
		StaticMesh->SetRenderCustomDepth(false);
		GetWorldTimerManager().ClearTimer(UnHighlightTimerHandle);
	}
}

void APainting::LookAt_Implementation()
{
	// if the player character reference isn't valid, return
	if (!PlayerCharacter) return;

	// bool to make sure we don't repeatedly set the custom depth stencil value to 250. this only needs to run once so we'll set a bool to ensure that happens
	bool hasSetStencilValue = false;

	// Allow the item to be highlighted, then set a timer that runs on tick to check if the player stopped looking at this actor.
	StaticMesh->SetRenderCustomDepth(true);
	GetWorldTimerManager().SetTimer(UnHighlightTimerHandle, this, &APainting::UnHighlightMesh, GetWorld()->GetDeltaSeconds(), true);

	// Finally, if we haven't already set the stencil value, set it.
	if (!hasSetStencilValue)
	{
		StaticMesh->SetCustomDepthStencilValue(250);
	}
}

void APainting::Interact_Implementation()
{
}
