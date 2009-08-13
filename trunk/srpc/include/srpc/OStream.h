#ifndef SRPC_OSTREAM_H
#define SRPC_OSTREAM_H

#include "Types.h"
#include "StringTypes.h"
#include "utility/Bits.h"
#include <boost/utility.hpp>
#include <string>
#include <cassert>

namespace srpc {

class OStream;

/** @addtogroup serialization
* @{
*/

namespace
{

/// �⺻ ���������� �ƴ� ���� ������ RpcType���� ����
template <typename T, bool isFundamental>
struct StreamWriterImpl;


template <typename T>
struct StreamWriterImpl<T, true>
{
    static void write(OStream& ostream, T value) {
        ostream.write(value);
    }
};


template <typename T>
struct StreamWriterImpl<T, false>
{
    static void write(OStream& ostream, const T& value) {
        const_cast<boost::remove_const<T>::type&>(value).
            serialize(ostream);
    }
};

} // namespace

/**
 * @class OStream
 *
 * Output stream.
 * - CAUTION: ������ Ÿ���� �������� ��Ʈ���� ����Ѵ�
 */
class OStream : public boost::noncopyable
{
public:
    virtual ~OStream() {}

    /// TODO: valueŸ�Կ� ���۷����� ������� ���� ��� RpcType�� ������ �Ѿ����
    ///       ������ �ִ�. �� ������ �ذ��ϱ� ���� ���۷����� ���� ������,
    ///       �׷����� Primitive-type�� ���۷����� �Ѿ���� �ٸ� ������ ����.
    ///       ����ȭ ��� ����?
    template <typename T>
    OStream& operator<<(const T& value) {
        typedef boost::remove_reference<T>::type RawType;

        StreamWriterImpl<T, boost::is_fundamental<RawType>::value>::
            write(*this, value);
        return *this;
    }

    /// same as boost.serialization
    template <typename T>
    OStream& operator&(const T& value) {
        return operator<<(value);
    }

    /// for enum data type
    void write(int value, int bitCount = Bits<int>::size) {
        write(static_cast<Int32>(value), bitCount);
    }

    /// 64��Ʈ ��ȣ ���� ������ ��Ʈ�����ۿ� ����Ѵ�
    virtual void write(UInt64 value, int bitCount = Bits<UInt64>::size) = 0;

    /// 32��Ʈ ��ȣ ���� ������ ��Ʈ�����ۿ� ����Ѵ�
    virtual void write(UInt32 value, int bitCount = Bits<UInt32>::size) = 0;

    /// 16��Ʈ ��ȣ ���� ������ ��Ʈ�����ۿ� ����Ѵ�
    virtual void write(UInt16 value, int bitCount = Bits<UInt16>::size) = 0;

    /// 8��Ʈ ��ȣ ���� ������ ��Ʈ�����ۿ� ����Ѵ�
    virtual void write(UInt8 value, int bitCount = Bits<UInt8>::size) = 0;

    /// �ο︰ ���� ��Ʈ�����ۿ� ����Ѵ�
    virtual void write(bool value, int bitCount = Bits<bool>::size) = 0;

    /**
     * 64��Ʈ ��ȣ �ִ� ���� Ÿ���� ��Ʈ�����ۿ� ����Ѵ�
     * @param bitCount ����� ��Ʈ ��. 2��Ʈ �̻��̾�� �Ѵ�
     */
    virtual void write(Int64 value, int bitCount = Bits<Int64>::size) = 0;

    /**
    * 32��Ʈ ��ȣ �ִ� ���� Ÿ���� ��Ʈ�����ۿ� ����Ѵ�
    * @param bitCount ����� ��Ʈ ��. 2��Ʈ �̻��̾�� �Ѵ�
    */
    virtual void write(Int32 value, int bitCount = Bits<Int32>::size) = 0;

    /**
     * 16��Ʈ ��ȣ �ִ� ���� Ÿ���� ��Ʈ�����ۿ� ����Ѵ�
     * @param bitCount ����� ��Ʈ ��. 2��Ʈ �̻��̾�� �Ѵ�
     */
    virtual void write(Int16 value, int bitCount = Bits<Int16>::size) = 0;

    /**
     * 8��Ʈ ��ȣ �ִ� ���� Ÿ���� ��Ʈ�����ۿ� ����Ѵ�
     * @param bitCount ����� ��Ʈ ��. 2��Ʈ �̻��̾�� �Ѵ�
     */
    virtual void write(Int8 value, int bitCount = Bits<Int8>::size) = 0;

    /**
     * 32��Ʈ ��ȣ �ִ� �ε� �Ҽ��� Ÿ���� ��Ʈ�����ۿ� ����Ѵ�
     * @param bitCount ����� ��Ʈ ��. 2��Ʈ �̻��̾�� �Ѵ�
     */
    virtual void write(Float32 value, int bitCount = Bits<Int32>::size) = 0;

    /// �ִ� maxLength ������ ���ڿ��� ��Ʈ�����ۿ� ����Ѵ�
    virtual void write(const String& value, size_t maxLength,
        int sizeBitCount) = 0;

    /// �ִ� maxLength ������ ���ڿ��� ��Ʈ�����ۿ� ����Ѵ�
    virtual void write(const WString& value, size_t maxLength,
        int sizeBitCount) = 0;

    /**
     * raw buffer�� ��Ʈ�����ۿ� ����Ѵ�
     * @param buffer raw buffer
     * @param length ������ ����Ʈ��(USHRT_MAX ���� �۰ų� ���ƾ� �Ѵ�)
     */
    virtual void write(const void* buffer, UInt16 length) = 0;

    /// �е��� �߰��Ͽ� ����Ʈ ������ �����Ѵ�
    virtual void align() = 0;

    /**
     * ��Ʈ ��Ʈ���� �ʱ�ȭ�Ѵ�
     * @param resetBuffer ���۸� �ʱ�ȭ�� ���ΰ�?
     */
    virtual void reset(bool resetBuffer) = 0;

    /// ����� �������� ũ��(��Ʈ ����)�� ��´�
    virtual int getTotalBitCount() const = 0;

    /// ����� �������� ũ��(����Ʈ ����)�� ��´�
    virtual int getTotalSize() const = 0;
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_OSTREAM_H
