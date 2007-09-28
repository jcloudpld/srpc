#ifndef NSRPC_SESSIONFACTORY_H
#define NSRPC_SESSIONFACTORY_H

#include <nsrpc/config/Proactor.h>
#include <boost/noncopyable.hpp>

class NSRPC_Proactor;

namespace nsrpc
{

class Session;
class SessionDestroyer;

/**
 * @class SessionFactory
 *
 * Session Factory
 */
class SessionFactory : public boost::noncopyable
{
public:
    /**
     * @param proactor Proactor 인스턴스.
     * @param useBitPacking 비트 패킹 사용?
     */
    SessionFactory(NSRPC_Proactor* proactor = 0, bool useBitPacking = true) :
        proactor_(proactor),
        useBitPacking_(useBitPacking) {}
    virtual ~SessionFactory() {}

    /// 새로운 세션을 생성한다.
    virtual Session* create(SessionDestroyer& sessionDestroyer) const = 0;
protected:
    NSRPC_Proactor* getProactor() const {
        return proactor_;
    }

    bool useBitPacking() const {
        return useBitPacking_;
    }
private:
    NSRPC_Proactor* proactor_;
    bool useBitPacking_;
};

} // namespace nsrpc

#endif // NSRPC_SESSIONFACTORY_H
