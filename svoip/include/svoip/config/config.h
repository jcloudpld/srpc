#ifndef SVOIP_CONFIG_H
#define SVOIP_CONFIG_H

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

typedef srpc::UInt16 Sample;

} // namespace svoip

#endif // SVOIP_CONFIG_H
