#include "stdafx.h"
#include "OpenAlRecorder.h"
#include "openal_framework/Framework.h"

OpenAlRecorder::OpenAlRecorder(svoip::RecorderCallback* callback) :
    Recorder(callback),
    captureDevice_(0),
    isRecording_(false)
{
    resetTarget();
}


OpenAlRecorder::~OpenAlRecorder()
{
    if (captureDevice_) {
        alcCaptureCloseDevice(captureDevice_);
        captureDevice_ = 0;
    }
}


bool OpenAlRecorder::open()
{
    if (! Recorder::open()) {
        return false;
    }

    // Check for Capture Extension support
    ALCcontext* pContext = alcGetCurrentContext();
    ALCdevice* pDevice = alcGetContextsDevice(pContext);
    if (AL_FALSE == alcIsExtensionPresent(pDevice, "ALC_EXT_CAPTURE")) {
        ALFWprintf("Failed to detect Capture Extension\n");
        ALFWShutdownOpenAL();
        ALFWShutdown();
        return 0;
    }

    // Get list of available Capture Devices
    const ALchar* pDeviceList =
        alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
    if (pDeviceList) {
        ALFWprintf("\nAvailable Capture Devices are:-\n");

        while (*pDeviceList) {
            ALFWprintf("%s\n", pDeviceList);
            pDeviceList += strlen(pDeviceList) + 1;
        }
    }

    // Get the name of the 'default' capture device
    const ALCchar* szDefaultCaptureDevice =
        alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
    ALFWprintf("\nDefault Capture Device is '%s'\n\n", szDefaultCaptureDevice);

    // Open the default Capture device
    captureDevice_ = alcCaptureOpenDevice(szDefaultCaptureDevice,
        svoip::frequency, AL_FORMAT_MONO16, svoip::bufferSize);
    return captureDevice_ != 0;
}


void OpenAlRecorder::start()
{
    resetTarget();
    record();
}


void OpenAlRecorder::start(nsrpc::PeerId to)
{
    resetTarget();
    targetPeerId_ = to;
    record();
}


void OpenAlRecorder::start(nsrpc::GroupId to)
{
    resetTarget();
    targetGroupId_ = to;
    record();
}


void OpenAlRecorder::stop()
{
    assert(captureDevice_ != 0);

    alcCaptureStop(captureDevice_);

    isRecording_ = false;
}


bool OpenAlRecorder::run()
{
    const size_t samples = getAvailableSamples();
    //ALFWprintf("Samples available : %d\r", samples);

    if (samples <= 0) {
        return true;
    }

    const ALCsizei frameSize = static_cast<ALCsizei>(getFrameSize());
    const int mult = isRecording_ ? 12 : 1; // 12 == 240ms of audio.

    // enough data buffered in audio hardware to process yet?
    if (samples >= static_cast<size_t>(frameSize * mult)) {
        // audio capture is always MONO16 (and that's what speex wants!).
        //  2048 will cover 12 uncompressed frames in narrow-band mode.
        static svoip::Sample sampleBuffer[svoip::sampleBufferSize];

        ALCsizei adjustedSamples = static_cast<ALCsizei>(samples);
        if (adjustedSamples > (frameSize * 12)) {
            adjustedSamples = (frameSize * 12);
        }
        adjustedSamples -= (adjustedSamples % frameSize);

        alcCaptureSamples(captureDevice_, sampleBuffer, adjustedSamples);
        if (alGetError() != AL_NO_ERROR) {
            return false;
        }

        encode(targetPeerId_, targetGroupId_, sampleBuffer, adjustedSamples);
    }

    return true;
}


void OpenAlRecorder::record()
{
    assert(captureDevice_ != 0);

    alcCaptureStart(captureDevice_);

    isRecording_ = true;

    setNewSpeech();
}


size_t OpenAlRecorder::getAvailableSamples() const
{
    if (! captureDevice_) {
        return 0;
    }

    ALint samples = 0;
    alcGetIntegerv(captureDevice_, ALC_CAPTURE_SAMPLES, sizeof(samples),
        &samples);
    return samples;
}
