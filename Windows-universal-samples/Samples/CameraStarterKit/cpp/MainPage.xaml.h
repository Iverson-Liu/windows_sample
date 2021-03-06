//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include "MainPage.g.h"
#include "CameraRotationHelper.h"

namespace CameraStarterKit
{
    ref class ReencodeState sealed
    {
    internal:
        Windows::Graphics::Imaging::BitmapDecoder^ Decoder;
        Windows::Graphics::Imaging::BitmapEncoder^ Encoder;
        Windows::Storage::StorageFile^ File;
        Windows::Storage::FileProperties::PhotoOrientation Orientation;
    };

    public ref class MainPage sealed
    {
    private:
        // Rotation Helper to simplify handling rotation compensation for the camera streams
        CameraRotationHelper^ _rotationHelper;

        // Prevent the screen from sleeping while the camera is running
        Windows::System::Display::DisplayRequest^ _displayRequest;  

        // For listening to media property changes
        Windows::Media::SystemMediaTransportControls^ _systemMediaControls;

        // MediaCapture and its state variables
        Platform::Agile<Windows::Media::Capture::MediaCapture^> _mediaCapture;
        bool _isInitialized;
        bool _isPreviewing;
        bool _isRecording;

        // UI state
        bool _isSuspending = false;
        bool _isActivePage = false;
        bool _isUIActive = false;
        concurrency::task<void> _setupTask = concurrency::task_from_result();

        // Information about the camera device
        bool _externalCamera;
        bool _mirroringPreview;

        // Rotation metadata to apply to the preview stream and recorded videos (MF_MT_VIDEO_ROTATION)
        // Reference: http://msdn.microsoft.com/en-us/library/windows/apps/xaml/hh868174.aspx
        const GUID RotationKey;

        // Folder in which the captures will be stored (availability check performed in SetupUiAsync)
        Windows::Storage::StorageFolder^ _captureFolder;

        // Event tokens
        Windows::Foundation::EventRegistrationToken _applicationSuspendingEventToken;
        Windows::Foundation::EventRegistrationToken _applicationResumingEventToken;
        Windows::Foundation::EventRegistrationToken _windowVisibilityChangedEventToken;
        Windows::Foundation::EventRegistrationToken _mediaControlPropChangedEventToken;
        Windows::Foundation::EventRegistrationToken _displayInformationEventToken;
        Windows::Foundation::EventRegistrationToken _recordLimitationExceededEventToken;
        Windows::Foundation::EventRegistrationToken _mediaCaptureFailedEventToken;
        Windows::Foundation::EventRegistrationToken _orientationChangedEventToken;
        Windows::Foundation::EventRegistrationToken _hardwareCameraButtonEventToken;

        // MediaCapture methods
        Concurrency::task<void> InitializeCameraAsync();
        Concurrency::task<void> CleanupCameraAsync();
        Concurrency::task<void> StartPreviewAsync();
        Concurrency::task<void> SetPreviewRotationAsync();
        Concurrency::task<void> StopPreviewAsync();
        Concurrency::task<void> TakePhotoAsync();
        Concurrency::task<void> StartRecordingAsync();
        Concurrency::task<void> StopRecordingAsync();

        // Helpers
        Concurrency::task<Windows::Devices::Enumeration::DeviceInformation^> FindCameraDeviceByPanelAsync(Windows::Devices::Enumeration::Panel panel);
        Concurrency::task<void> ReencodeAndSavePhotoAsync(Windows::Storage::Streams::IRandomAccessStream^ stream, Windows::Storage::StorageFile^ file, Windows::Storage::FileProperties::PhotoOrientation photoOrientation);
        void UpdateCaptureControls();
        Concurrency::task<void> SetUpBasedOnStateAsync();
        Concurrency::task<void> SetupUiAsync();
        Concurrency::task<void> CleanupUiAsync();
        void RegisterEventHandlers();
        void UnregisterEventHandlers();
        void WriteLine(Platform::String^ str);
        void WriteException(Platform::Exception^ ex);
        void UpdateButtonOrientation();

        // UI event handlers
        void Application_Suspending(Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
        void Application_Resuming(Object^ sender, Object^ args);
        void Window_VisibilityChanged(Object^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ e);
        void PhotoButton_Click(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void VideoButton_Click(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e); 
        void SystemMediaControls_PropertyChanged(Windows::Media::SystemMediaTransportControls^ sender, Windows::Media::SystemMediaTransportControlsPropertyChangedEventArgs^ args);
        void RotationHelper_OrientationChanged(Object^, bool);
        void HardwareButtons_CameraPressed(Platform::Object^, Windows::Phone::UI::Input::CameraEventArgs^);

        // Camera event handlers
        void HardwareButtons_CameraPress(Platform::Object^, Windows::Phone::UI::Input::CameraEventArgs^);
        void MediaCapture_RecordLimitationExceeded(Windows::Media::Capture::MediaCapture ^currentCaptureObject);
        void MediaCapture_Failed(Windows::Media::Capture::MediaCapture ^currentCaptureObject, Windows::Media::Capture::MediaCaptureFailedEventArgs^ errorEventArgs);

    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
        virtual void OnNavigatingFrom(Windows::UI::Xaml::Navigation::NavigatingCancelEventArgs^ e) override;

    public:
        MainPage();
        virtual ~MainPage();    
    };
}
