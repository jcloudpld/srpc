#ifndef NSRPC_SESSIONFACTORY_H
#define NSRPC_SESSIONFACTORY_H

#ifdef _MSC_VER
#  pragma once
#endif

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
    virtual ~SessionFactory() {}

    virtual void setSessionDestroyer(SessionDestroyer& sessionDestroyer) = 0;

    /// 货肺款 技记阑 积己茄促.
    virtual Session* create() const = 0;
};

} // namespace nsrpc

#endif // NSRPC_SESSIONFACTORY_H
