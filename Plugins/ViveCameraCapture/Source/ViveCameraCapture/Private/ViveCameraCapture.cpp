#include "ViveCameraCapture.h"
#include "ViveSceneCaptureComponent2D.h"
#include <Camera/CameraComponent.h>
#include <Camera/CameraActor.h>
#include <Engine/Engine.h>
#include <EngineUtils.h>

AViveCameraCapture::AViveCameraCapture( const FObjectInitializer& ObjectInitializer )
    : Super( ObjectInitializer )
{
    RootComponent = CreateDefaultSubobject<USceneComponent>( TEXT( "Root" ) );

    auto component = CreateDefaultSubobject<UViveSceneCaptureComponent2D>( TEXT( "ViveSceneCaptureComponent" ) );
    component->SetOwnerSceneCapture( this );
    SceneCaptureComponent = component;
    SceneCaptureComponent->SetupAttachment( RootComponent );

    RenderResolution = FIntPoint( 1920, 1080 );
    RenderFormat = ETextureRenderTargetFormat::RTF_RGBA16f;
}

bool AViveCameraCapture::ConstructTargetedActorsList( TArray<AActor*>& OutActors )
{
    bool hasValidEntries = false;

    for ( const auto& actorLayer : CaptureActors ) {
        if ( !actorLayer.Name.IsEqual( TEXT( "None" ) ) ) {
            hasValidEntries = true;
            auto actors = GetActorsFromActorLayer( actorLayer );
            if ( actors.Num() > 0 )
                OutActors.Append( actors );
        }
    }

    hasValidEntries &= CaptureActors.IsValidIndex( 0 );
    return !hasValidEntries;
}

TArray<AActor*> AViveCameraCapture::GetActorsFromActorLayer( const FActorLayer& InActorLayer )
{
    if ( ensure( GetWorld() != nullptr ) ) {
        TArray<AActor*> actors;
        for ( const TWeakObjectPtr<AActor> weakActor : FActorRange( GetWorld() ) ) {
            auto actor = weakActor.Get();
            if ( actor != nullptr && actor->Layers.Contains( InActorLayer.Name ) )
                actors.Add( actor );
        }

        return actors;
    }

    return TArray<AActor*>();
}

FIntPoint AViveCameraCapture::GetRenderResolution() const
{
    return RenderResolution;
}

ETextureRenderTargetFormat AViveCameraCapture::GetRenderFormat() const
{
    return RenderFormat.GetValue();
}

UCameraComponent* AViveCameraCapture::GetCameraComponent() const
{
    return CameraActor != nullptr ? CameraActor->GetCameraComponent() : nullptr;
}

EViveActorSelection AViveCameraCapture::GetSelectionType() const
{
    return SelectionType;
}
