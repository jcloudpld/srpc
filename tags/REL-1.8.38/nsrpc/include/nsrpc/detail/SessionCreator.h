#ifndef NSRPC_SESSIONCREATOR_H
#define NSRPC_SESSIONCREATOR_H

namespace nsrpc
{

class Session;

/** @addtogroup session
* @{
*/

/**
 * @class SessionCreator
 *
 * 技记阑 积己茄促.
 */
class SessionCreator
{
public:
    virtual ~SessionCreator() {}

    /// Session 技记阑 积己茄促.
    virtual Session* acquire() = 0;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSIONCREATOR_H
