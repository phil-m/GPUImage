#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreMedia/CoreMedia.h>
#import "GPUImageOpenGLESContext.h"
#import "GPUImageOutput.h"

//Delegate Protocal for Face Detection.
@protocol GPUImageVideoCameraDelegate <NSObject>

@optional
- (void)willOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer;
@end


/**
 A GPUImageOutput that provides frames from either camera
*/
@interface GPUImageVideoCamera : GPUImageOutput <AVCaptureVideoDataOutputSampleBufferDelegate, AVCaptureAudioDataOutputSampleBufferDelegate>
{
    CVOpenGLESTextureCacheRef coreVideoTextureCache;    

    NSUInteger numberOfFramesCaptured;
    CGFloat totalFrameTimeDuringCapture;
    CGFloat _currentFrameTimeDuringCapture;
    
    AVCaptureSession *_captureSession;
    AVCaptureDevice *_inputCamera;
    AVCaptureDevice *_microphone;
    AVCaptureDeviceInput *videoInput;

    BOOL capturePaused;
    GPUImageRotationMode outputRotation;
    dispatch_semaphore_t frameRenderingSemaphore;

    id<GPUImageVideoCameraDelegate> _delegate;
}

/// The AVCaptureSession used to capture from the camera
@property(readonly, retain, nonatomic) AVCaptureSession *captureSession;

/// This enables the capture session preset to be changed on the fly
@property (readwrite, nonatomic, copy) NSString *captureSessionPreset;

/// This sets the frame rate of the camera (iOS 5 and above only)
/**
 Setting this to 0 or below will set the frame rate back to the default setting for a particular preset.
 */
@property (readwrite) NSInteger frameRate;

/// Easy way to tell if front-facing camera is present on device
@property (readonly, getter = isFrontFacingCameraPresent) BOOL frontFacingCameraPresent;

/// This enables the benchmarking mode, which logs out instantaneous and average frame times to the console
@property(readwrite, nonatomic) BOOL runBenchmark;

/// When benchmarking is enabled, this logs out instantaneous and average frame times to the console
@property(readwrite, nonatomic) BOOL logBenchmark;

/// When benchmarking is enabled, this can be used to access the current frame time for use with fps calculations
@property(readonly, nonatomic) CGFloat currentFrameTimeDuringCapture;

/// Use this property to manage camera settings. Focus point, exposure point, etc.
@property(readonly) AVCaptureDevice *inputCamera;

/// This determines the rotation applied to the output image, based on the source material
@property(readwrite, nonatomic) UIInterfaceOrientation outputImageOrientation;

/// These properties determine whether or not the two camera orientations should be mirrored. By default, both are NO.
@property(readwrite, nonatomic) BOOL horizontallyMirrorFrontFacingCamera, horizontallyMirrorRearFacingCamera;

@property(nonatomic, retain) id<GPUImageVideoCameraDelegate> delegate;

/// @name Initialization and teardown

/** Begin a capture session
 
 See AVCaptureSession for acceptable values
 
 @param sessionPreset Session preset to use
 @param cameraPosition Camera to capture from
 */
- (id)initWithSessionPreset:(NSString *)sessionPreset cameraPosition:(AVCaptureDevicePosition)cameraPosition;

/** Tear down the capture session
 */
- (void)removeInputsAndOutputs;

/// @name Manage the camera video stream

/** Start camera capturing
 */
- (void)startCameraCapture;

/** Stop camera capturing
 */
- (void)stopCameraCapture;

/** Pause camera capturing
 */
- (void)pauseCameraCapture;

/** Resume camera capturing
 */
- (void)resumeCameraCapture;

/** Process a video sample
 @param sampleBuffer Buffer to process
 */
- (void)processVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer;

/** Process an audio sample
 @param sampleBuffer Buffer to process
 */
- (void)processAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer;

/** Get the position (front, rear) of the source camera
 */
- (AVCaptureDevicePosition)cameraPosition;

/** Get the AVCaptureConnection of the source camera
 */
- (AVCaptureConnection *)videoCaptureConnection;

/** This flips between the front and rear cameras
 */
- (void)rotateCamera;

/// @name Benchmarking

/** When benchmarking is enabled, this will keep a running average of the time from uploading, processing, and final recording or display
 */
- (CGFloat)averageFrameDurationDuringCapture;

@end
