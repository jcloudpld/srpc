#ifndef SRPC_STREAMBUFFER_H
#define SRPC_STREAMBUFFER_H

#include "Types.h"

namespace srpc {

/** @addtogroup serialization
* ��Ʈ�� IO
* @{
*/

/**
 * @class StreamBuffer
 *
 * std::queue�� ���� ����� �����ϴ�
 * - ������ �߻��� ��� StreamingException�� ������(throw) �Ѵ�
 */
class StreamBuffer
{
public:
    typedef UInt8 Item;
public:
    virtual ~StreamBuffer() {}

    /**
     * �������� ���� ���� �߰��Ѵ�
     * @warning ��å�� ���� �޸� ó���� ������ ���־�� �Ѵ�
     * @exception StreamingException ������ ���
     */
    virtual void push(Item item) = 0;

    /**
     * ���� �տ��� ������ �ϳ��� �����Ѵ�
     * @pre ! empty()
     */
    virtual void pop() = 0;

    /**
     * ���۷� ���� ����Ʈ ��Ʈ���� �����Ѵ�
     * @param buffer ���� ����
     * @param bufferSize buffer ����Ʈ ��
     * @exception StreamingException ������ ���
     */
    virtual void copyFrom(const Item* buffer, size_t bufferSize) = 0;

    /**
    * ���۷� ����Ʈ ��Ʈ���� �����Ѵ�
    * @param buffer ��� ����
    * @param bufferSize buffer ����Ʈ ��
    * @pre size() >= bufferSize
    * @exception StreamingException ������ ���
    */
    virtual void copyTo(Item* buffer, size_t bufferSize) = 0;

    /// �׸��� �ʱ�ȭ�Ѵ�
    virtual void reset() = 0;

    /**
     * ���� ���� ���� �����͸� �����Ѵ�
     * @pre ! empty()
     */
    virtual Item front() const = 0;

    /**
     * ���� ���� ���� �������� ���۷����� �����Ѵ�
     * @pre ! empty()
     */
    virtual Item& front() = 0;

    /**
     * ���� ���� ���� �����͸� �����Ѵ�
     * @pre ! empty()
     */
    virtual Item back() const = 0;

    /**
     * ���� ���� ���� �������� ���۷����� �����Ѵ�
     * @pre ! empty()
     */
    virtual Item& back() = 0;

    /// ������ ũ�⸦ ��´�
    virtual size_t size() const = 0;

    /// ���۰� ��� �ִ°�?
    virtual bool empty() const = 0;

    /// ���ۿ� ���� �� ���� ũ��(capacity - size())
    virtual size_t space() const = 0;
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_STREAMBUFFER_H
