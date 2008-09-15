#ifndef SVOIP_CONFIG_H
#define SVOIP_CONFIG_H

#include <srpc/Types.h>

#if defined (_MSC_VER)
#   include "config_msvc.h"
#elif defined(__GNUC__)
#   include "config_gcc.h"
#else
#   error ����� MSVC�� GCC�� �����մϴ�
#endif

namespace svoip
{

/// 8000Hz is what Speex narrow-band mode needs
const int frequency = 8000;

const int bufferSize = 4096;

/// audio capture is always MONO16 (and that's what speex wants!).
///  2048 will cover 12 uncompressed frames in narrow-band mode.
const int sampleBufferSize = 2048;


/// Multiply recorded audio by this value after denoising.
/// This is to make you more audible if denoising eats away too much data.
/// Set this to 1.0 to get no change, less to be quieter. 
const float audioMult = 2.0f;


typedef srpc::Int16 Sample;

typedef srpc::Int8 EncodedSample;

} // namespace svoip

#endif // SVOIP_CONFIG_H