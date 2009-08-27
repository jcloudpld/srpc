#include "stdafx.h"
#include "svoip/VoP2pPlugInFactory.h"
#include "svoip/Recorder.h"
#include "svoip/Player.h"
#include "VoP2pPlugInImpl.h"

namespace svoip
{

VoP2pPlugInPtr VoP2pPlugInFactory::create(
    std::auto_ptr<Recorder> recorder, std::auto_ptr<Player> player)
{
    return VoP2pPlugInPtr(new VoP2pPlugInImpl(recorder, player));
}

} // namespace svoip
