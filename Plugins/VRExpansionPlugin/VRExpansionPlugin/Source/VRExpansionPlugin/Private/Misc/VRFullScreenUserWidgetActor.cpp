// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/VRFullScreenUserWidgetActor.h"
#include "Misc/VRFullScreenUserWidget.h"


#define LOCTEXT_NAMESPACE "VRFullScreenUserWidgetActor"

/////////////////////////////////////////////////////
// AVRFullScreenUserWidgetActor

AVRFullScreenUserWidgetActor::AVRFullScreenUserWidgetActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
#if WITH_EDITOR
	, bEditorDisplayRequested(false)
#endif //WITH_EDITOR
{
	ScreenUserWidget = CreateDefaultSubobject<UVRFullScreenUserWidget>(TEXT("ScreenUserWidget"));

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	bAllowTickBeforeBeginPlay = true;
	SetActorTickEnabled(true);
	SetHidden(false);
}

void AVRFullScreenUserWidgetActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

#if WITH_EDITOR
	bEditorDisplayRequested = true;
#endif //WITH_EDITOR
}

void AVRFullScreenUserWidgetActor::PostLoad()
{
	Super::PostLoad();

#if WITH_EDITOR
	bEditorDisplayRequested = true;
#endif //WITH_EDITOR
}

void AVRFullScreenUserWidgetActor::PostActorCreated()
{
	Super::PostActorCreated();

#if WITH_EDITOR
	bEditorDisplayRequested = true;
#endif //WITH_EDITOR
}

void AVRFullScreenUserWidgetActor::Destroyed()
{
	if (ScreenUserWidget)
	{
		ScreenUserWidget->Hide();
	}
	Super::Destroyed();
}

void AVRFullScreenUserWidgetActor::BeginPlay()
{
	RequestGameDisplay();

	Super::BeginPlay();
}

void AVRFullScreenUserWidgetActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (ScreenUserWidget)
	{
		UWorld* ActorWorld = GetWorld();
		if (ActorWorld && (ActorWorld->WorldType == EWorldType::Game || ActorWorld->WorldType == EWorldType::PIE))
		{
			ScreenUserWidget->Hide();
		}
	}
}

void AVRFullScreenUserWidgetActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if WITH_EDITOR
	if (bEditorDisplayRequested)
	{
		bEditorDisplayRequested = false;
		RequestEditorDisplay();
	}
#endif //WITH_EDITOR

	if (ScreenUserWidget)
	{
		ScreenUserWidget->Tick(DeltaSeconds);
	}
}

void AVRFullScreenUserWidgetActor::RequestEditorDisplay()
{
#if WITH_EDITOR
	UWorld* ActorWorld = GetWorld();
	if (ScreenUserWidget && ActorWorld && ActorWorld->WorldType == EWorldType::Editor)
	{
		ScreenUserWidget->Display(ActorWorld);
	}
#endif //WITH_EDITOR
}

void AVRFullScreenUserWidgetActor::RequestGameDisplay()
{
	UWorld* ActorWorld = GetWorld();
	if (ScreenUserWidget && ActorWorld && (ActorWorld->WorldType == EWorldType::Game || ActorWorld->WorldType == EWorldType::PIE))
	{
		ScreenUserWidget->Display(ActorWorld);
	}
}

#undef LOCTEXT_NAMESPACE
