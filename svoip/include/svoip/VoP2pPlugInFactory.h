#ifndef SVOIP_VOP2PPLUGINFACTORY_H
#define SVOIP_VOP2PPLUGINFACTORY_H

#include "VoP2pPlugIn.h"
#include <boost/noncopyable.hpp>
#include <memory>

namespace svoip
{

class Recorder;
class Player;


/**
 * @struct VoP2pPlugInFactory
 * VoP2pPlugIn Factory
 */
struct SVOIP_API VoP2pPlugInFactory : public boost::noncopyable
{

    static VoP2pPlugInPtr create(
        std::auto_ptr<Recorder> recorder, std::auto_ptr<Player> player);

};

} // namespace svoip

#endif // SVOIP_VOP2PPLUGINFACTORY_H
