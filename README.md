# Real Time Camera Capture 시스템

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

UE4의 Camera Actor 및 Cine Camera Actor에서 본 Scene 화면을 캡쳐하여 Texture로 저장하고 저장된 Texture을 실시간으로 이용하기 위한 시스템을 개발.

![](https://github.com/Devcoder-IndieWorks/ViveCameraCaptureSample/blob/master/Images/Result_CameraCapture.png)

이 시스템에서 제공하는 기능은 다음과 같다.

* Camera Actor, Cine Camera Actor에서 본 Scene 화면을 Capture
* Camera Actor, Cine Camera Actor에서 설정한 PostProcessSettings 속성값이 Capture에서도 적용
* Capture할 Actor를 필터링하는 기능(Include, Exclude)



## 사용법

* Texture Render Target 2D UAsset를 하나 생성하여 만듦
* ViveCameraCapture 컨텐츠에서 **BP_ViveCameraCapture** Actor를 레벨에 배치 함
* 레벨에 Camera Actor 혹은 Cine Camera Actor를 배치 함
* BP_ViveCameraCapture의 **ViveSceneCapture**에서 **Render Resolution, Render Format** 항목을 설정하고 **Camera Actor**에 레벨에 배치된 Camera Actor 혹은 Cine Camera Actor를 설정 함.
* 만들어 두었던 Texture Render Target 2D UAsset을 BP_ViveCameraCapture의 **SceneCapture**에서 **Texture Target**에 설정 함.
* 옵션 항목으로 Capture 할 Actor들을 선택적으로 설정 할 수 있음. **Catpure Actors**에 Actor Layer를 추가하고, **Selection Type**으로 선택된 Actor들만 Capture 할지, 선택된 Actor들을 전체 Scene에서 Capture할 목록에서 배제 할지 설정 함. **Selection Type은 Include Capture와 Exclude Capture** 2종류가 있음.

![](https://github.com/Devcoder-IndieWorks/ViveCameraCaptureSample/blob/master/Images/CameraCapture_Detail.png)

