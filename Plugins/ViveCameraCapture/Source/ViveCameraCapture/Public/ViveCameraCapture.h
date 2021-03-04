// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include <GameFramework/Actor.h>
#include <Engine/TextureRenderTarget2D.h>
#include <ActorLayerUtilities.h>
#include "ViveCameraCapture.generated.h"

UENUM()
enum class EViveActorSelection : uint8
{
    Include UMETA(DisplayName="Include Capture"),
    Exclude UMETA(DisplayName="Exclude Capture")
};

//-----------------------------------------------------------------------------

UCLASS( ClassGroup=(ViveCameraSystem), Blueprintable )
class VIVECAMERACAPTURE_API AViveCameraCapture : public AActor
{
    GENERATED_UCLASS_BODY()
public:
    class UCameraComponent* GetCameraComponent() const;

public:
    bool ConstructTargetedActorsList( TArray<AActor*>& OutActors );
    FIntPoint GetRenderResolution() const;
    ETextureRenderTargetFormat GetRenderFormat() const;
    EViveActorSelection GetSelectionType() const;
    TArray<AActor*> GetActorsFromActorLayer( const FActorLayer& InActorLayer );

private:
    UPROPERTY( EditAnywhere, BlueprintReadOnly, Category="ViveSceneCapture|Properties", meta=(AllowPrivateAccess=true) )
    EViveActorSelection SelectionType;
    UPROPERTY( EditAnywhere, BlueprintReadOnly, Category="ViveSceneCapture|Properties", meta=(AllowPrivateAccess=true) )
    TArray<FActorLayer> CaptureActors;
    UPROPERTY( EditAnywhere, BlueprintReadOnly, Category="ViveSceneCapture|Properties", meta=(AllowPrivateAccess=true) )
    FIntPoint RenderResolution;
    UPROPERTY( EditAnywhere, BlueprintReadOnly, Category="ViveSceneCapture|Properties", meta=(AllowPrivateAccess=true) )
    TEnumAsByte<ETextureRenderTargetFormat> RenderFormat;
    UPROPERTY( EditAnywhere, BlueprintReadOnly, Category="ViveSceneCapture|Properties", meta=(AllowPrivateAccess=true) )
    class ACameraActor* CameraActor;

    UPROPERTY( VisibleAnywhere, Category="ViveSceneCapture|Properties", meta=(AllowPrivateAccess=true) )
    class USceneCaptureComponent2D* SceneCaptureComponent;
};
