#ifndef VOP2PTEST_OPENALVOP2PPLUGIN_H
#define VOP2PTEST_OPENALVOP2PPLUGIN_H

#include "OpenAlRecorder.h"
#include "OpenAlPlayer.h"
#include <svoip/VoP2pPlugIn.h>

/**
* @class OpenAlVoP2pPlugIn
* Voice Over P2P Plug-in using OpenAL
*/
class OpenAlVoP2pPlugIn : public svoip::VoP2pPlugIn
{
private:
    virtual std::auto_ptr<svoip::Recorder> createRecorder() {
        return std::auto_ptr<svoip::Recorder>(new OpenAlRecorder(this));
    }
    virtual std::auto_ptr<svoip::Player> createPlayer() {
        return std::auto_ptr<svoip::Player>(new OpenAlPlayer);
    }
};

#endif // VOP2PTEST_OPENALVOP2PPLUGIN_H
