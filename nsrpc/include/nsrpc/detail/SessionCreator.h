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
 * ������ �����Ѵ�.
 */
class SessionCreator
{
public:
    virtual ~SessionCreator() {}

    /// Session ������ �����Ѵ�.
    virtual Session* acquire() = 0;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSIONCREATOR_H
