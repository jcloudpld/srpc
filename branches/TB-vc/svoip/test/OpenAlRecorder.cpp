#include "stdafx.h"
#include "OpenAlRecorder.h"
#include "openal_framework/Framework.h"

OpenAlRecorder::OpenAlRecorder() :
    captureDevice_(0)
{
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
    assert(captureDevice_ != 0);

    alcCaptureStart(captureDevice_);
}


void OpenAlRecorder::stop()
{
    assert(captureDevice_ != 0);

    alcCaptureStop(captureDevice_);
}


bool OpenAlRecorder::run()
{
    const size_t samples = getAvailableCaptureSamples();
    ALFWprintf("Samples available : %d\r", samples);

    return true;
}


svoip::Sample* OpenAlRecorder::getSample(size_t& samples)
{
    static svoip::Sample sampleBuffer[svoip::sampleBufferSize];

    samples = getAvailableCaptureSamples();
    if (samples > 0) {
        alcCaptureSamples(captureDevice_, sampleBuffer,
            static_cast<ALCsizei>(samples));
        return sampleBuffer;
    }

    return 0;
}


size_t OpenAlRecorder::getAvailableCaptureSamples() const
{
    if (! captureDevice_) {
        return 0;
    }

    ALint samples = 0;
    alcGetIntegerv(captureDevice_, ALC_CAPTURE_SAMPLES, sizeof(samples),
        &samples);
    return samples;
}
