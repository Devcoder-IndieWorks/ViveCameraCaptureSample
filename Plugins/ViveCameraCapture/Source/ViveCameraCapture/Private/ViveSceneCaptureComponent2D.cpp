#include "ViveSceneCaptureComponent2D.h"
#include "ViveCameraCapture.h"
#include <Camera/CameraComponent.h>
#include <Misc/App.h>

struct FVivePostMoveSettings
{
    FVivePostMoveSettings()
    {
        Pivot = FVector2D( 0.5f, 0.5f );
        Translation = FVector2D( 0.0f, 0.0f );
        RotationAngle = 0.0f;
        Scale = 1.0f;
    }

    FMatrix GetProjectionMatrix( float InHorizontalFOVAngle, float InAspectRatio ) const
    {
        float halfFOV = 0.5f * FMath::DegreesToRadians( InHorizontalFOVAngle );
        FMatrix originalProjectionMatrix = FReversedZPerspectiveMatrix(
            halfFOV, 
            halfFOV, 
            1.0f, 
            InAspectRatio, 
            GNearClippingPlane, 
            GNearClippingPlane );

        FVector2D normalizedViewRect = FMath::Tan( halfFOV ) * FVector2D( 1.0f, 1.0f / InAspectRatio );
        FVector2D normalizedPostMoveTranslation = 2.0f * normalizedViewRect * Translation;
        FVector2D normalizedPivot = normalizedViewRect * (Pivot - 0.5f) * FVector2D( 2.0f, 2.0f );

        FMatrix scaleMatrix( 
            FPlane( Scale, 0.0f, 0.0f, 0.0f ), 
            FPlane( 0.0f, Scale, 0.0f, 0.0f ),
            FPlane( 0.0f, 0.0f, 1.0f, 0.0f ), 
            FPlane( 0.0f, 0.0f, 0.0f, 1.0f ) );

        FMatrix preRotationMatrix(
            FPlane( 1.0f, 0.0f, 0.0f, 0.0f ),
            FPlane( 0.0f, 1.0f, 0.0f, 0.0f ), 
            FPlane(-normalizedPivot.X, -normalizedPivot.Y, 1.0f, 0.0f ),
            FPlane( 0.0f, 0.0f, 0.0f, 1.0f ) );

        float rotation = FMath::DegreesToRadians( RotationAngle );
        FMatrix rotationMatrix(
            FPlane(FMath::Cos(rotation), FMath::Sin(rotation), 0.0f, 0.0f),
            FPlane(-FMath::Sin(rotation), FMath::Cos(rotation), 0.0f, 0.0f),
            FPlane( 0.0f, 0.0f, 1.0f, 0.0f ),
            FPlane( 0.0f, 0.0f, 0.0f, 1.0f ) );

        FMatrix postRotationMatrix(
            FPlane( 1.0f, 0.0f, 0.0f, 0.0f ),
            FPlane( 0.0f, 1.0f, 0.0f, 0.0f ),
            FPlane(normalizedPivot.X, normalizedPivot.Y, 1.0f, 0.0f),
            FPlane( 0.0f, 0.0f, 0.0f, 1.0f ) );

        FMatrix translationMatrix(
            FPlane( 1.0f, 0.0f, 0.0f, 0.0f ),
            FPlane( 0.0f, 1.0f, 0.0f, 0.0f ),
            FPlane(normalizedPostMoveTranslation.X, normalizedPostMoveTranslation.Y, 1.0f, 0.0f),
            FPlane( 0.0f, 0.0f, 0.0f, 1.0f ) );

        return preRotationMatrix * scaleMatrix * rotationMatrix * postRotationMatrix * translationMatrix * originalProjectionMatrix;
    }

    //-------------------------------------------------------------------------

    FVector2D Pivot;
    FVector2D Translation;
    float RotationAngle;
    float Scale;
};

//-----------------------------------------------------------------------------

UViveSceneCaptureComponent2D::UViveSceneCaptureComponent2D( const FObjectInitializer& ObjectInitializer )
    : Super( ObjectInitializer )
{
    // README: 최종적으로 캡쳐할 이미지는 톤매핑이 적용된 최종 렌더링 이미지이므로 CaptureSource를 다음 처럼 설정 한다.
    CaptureSource = ESceneCaptureSource::SCS_FinalToneCurveHDR;
}

void UViveSceneCaptureComponent2D::BeginPlay()
{
    Super::BeginPlay();

    SetupTextureTarget();
}

void UViveSceneCaptureComponent2D::SetupTextureTarget()
{
    if ( ensure( TextureTarget != nullptr ) && 
         ensure( OwnerSceneCapture.IsValid() ) ) {
        auto renderFormat = OwnerSceneCapture->GetRenderFormat();
        auto renderResolution = OwnerSceneCapture->GetRenderResolution();

        TextureTarget->RenderTargetFormat = renderFormat;
        TextureTarget->InitAutoFormat( renderResolution.X, renderResolution.Y );
        TextureTarget->UpdateResourceImmediate( true );
    }
}

void UViveSceneCaptureComponent2D::UpdateSceneCaptureContents( FSceneInterface* InScene )
{
    RefreshTargetedActorList();
    UpdateCaptureCameraSettings();

    Super::UpdateSceneCaptureContents( InScene );
}

void UViveSceneCaptureComponent2D::RefreshTargetedActorList()
{
    if ( ensure( OwnerSceneCapture.IsValid() ) ) {
        ClearShowOnlyComponents();
        ClearHiddenComponents();

        PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_LegacySceneCapture;

        TArray<AActor*> actorList;
        bool isEmpty = OwnerSceneCapture->ConstructTargetedActorsList( actorList );
        if ( !isEmpty ) {
            auto selectionType = OwnerSceneCapture->GetSelectionType();

            PrimitiveRenderMode = selectionType == EViveActorSelection::Include ? ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList : 
                                  selectionType == EViveActorSelection::Exclude ? ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives : 
                                  ESceneCapturePrimitiveRenderMode::PRM_LegacySceneCapture;

            auto setComponent = selectionType == EViveActorSelection::Include ? TFunction<void(AActor*)>([this]( AActor* a ){
                                    ShowOnlyActorComponents( a );
                                }) : 
                                selectionType == EViveActorSelection::Exclude ? TFunction<void(AActor*)>([this]( AActor* a ){
                                    HideActorComponents( a );
                                }) : 
                                TFunction<void(AActor*)>([]( AActor* a ){});

            for ( auto actor : actorList )
                setComponent( actor );
        }
    }
}

void UViveSceneCaptureComponent2D::UpdateCaptureCameraSettings()
{
    if ( ensure( OwnerSceneCapture.IsValid() ) ) {
        if ( OwnerSceneCapture->GetCameraComponent() != nullptr ) {
            ProfilingEventName = TEXT( "ViveSceneCaptureComponent2D" );
		    
            CopyCameraSettingsToSceneCapture();
		    
            bUseCustomProjectionMatrix = true;
            auto cameraComp = OwnerSceneCapture->GetCameraComponent();
            FVivePostMoveSettings postMoveSettings;
            float aspectRation = (float)TextureTarget->SizeX / (float)TextureTarget->SizeY;
            CustomProjectionMatrix = postMoveSettings.GetProjectionMatrix( cameraComp->FieldOfView, aspectRation );
        }
    }
}

void UViveSceneCaptureComponent2D::CopyCameraSettingsToSceneCapture()
{
    auto cameraComp = OwnerSceneCapture->GetCameraComponent();
    SetWorldLocationAndRotation( cameraComp->GetComponentLocation(), cameraComp->GetComponentRotation() );
    FOVAngle = cameraComp->FieldOfView;

    FMinimalViewInfo cameraViewInfo;
    cameraComp->GetCameraView( FApp::GetDeltaTime(), cameraViewInfo );

    const auto& srcPPSettings = cameraViewInfo.PostProcessSettings;
    auto& destPPSettings = PostProcessSettings;

    destPPSettings = srcPPSettings;
}

void UViveSceneCaptureComponent2D::SetOwnerSceneCapture( AViveCameraCapture* InActor )
{
    OwnerSceneCapture = InActor;
}
