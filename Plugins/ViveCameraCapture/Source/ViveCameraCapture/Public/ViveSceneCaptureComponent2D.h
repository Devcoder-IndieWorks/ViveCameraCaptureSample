// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include <CoreMinimal.h>
#include <Components/SceneCaptureComponent2D.h>
#include "ViveSceneCaptureComponent2D.generated.h"

UCLASS( hidecategories=(Projection/*, PostProcessVolume*/), ClassGroup=(ViveCameraSystem) )
class VIVECAMERACAPTURE_API UViveSceneCaptureComponent2D : public USceneCaptureComponent2D
{
    GENERATED_UCLASS_BODY()
public:
    void BeginPlay() override;

public:
    void SetOwnerSceneCapture( class AViveCameraCapture* InActor );

private:
    void UpdateSceneCaptureContents( FSceneInterface* InScene ) override;

private:
    void SetupTextureTarget();
    void RefreshTargetedActorList();
    void UpdateCaptureCameraSettings();
    void CopyCameraSettingsToSceneCapture();

private:
    TWeakObjectPtr<class AViveCameraCapture> OwnerSceneCapture;
};
