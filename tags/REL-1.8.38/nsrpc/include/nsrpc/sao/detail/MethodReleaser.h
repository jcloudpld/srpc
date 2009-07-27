#ifndef NSRPC_SAO_METHODRELEASER_H
#define NSRPC_SAO_METHODRELEASER_H

#ifdef _MSC_VER
#  pragma once
#endif

namespace nsrpc
{

namespace sao
{

/**
 * @class MethodReleaser
 *
 * Method �޸𸮸� �����Ѵ�
 * - ObjectPool���� ����ϱ� ����
 */
class MethodReleaser
{
public:
    virtual ~MethodReleaser() {}

    virtual void release(void* method) = 0;
};

} // namespace sao

} // namespace nsrpc

#endif // NSRPC_SAO_METHODRELEASER_H
