#ifndef SVOIP_CONFIG_H
#define SVOIP_CONFIG_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/utility/SmartPtr.h>
#include <srpc/Types.h>

#if defined (_MSC_VER)
#   include "config_msvc.h"
#elif defined(__GNUC__)
#   include "config_gcc.h"
#else
#   error 현재는 MSVC와 GCC만 지원합니다
#endif

namespace svoip
{

/// 8000Hz is what Speex narrow-band mode needs
const int frequency = 8000;

const int bufferSize = 4096;

/// audio capture is always MONO16 (and that's what speex wants!).
///  2048 will cover 12 uncompressed frames in narrow-band mode.
const int sampleBufferSize = 2048;

/// 12 == 240ms of audio.
const int maximumFrames = 12;


/// Multiply recorded audio by this value after denoising.
/// This is to make you more audible if denoising eats away too much data.
/// Set this to 1.0 to get no change, less to be quieter. 
const float audioMult = 2.0f;


typedef srpc::Int16 Sample;

typedef srpc::Int8 EncodedSample;


typedef srpc::UInt8 Speech;
typedef srpc::UInt32 Sequence;


/// @internal
const size_t sampleSizeLen = sizeof(EncodedSample);

} // namespace svoip

#endif // SVOIP_CONFIG_H
