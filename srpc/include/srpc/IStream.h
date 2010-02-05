#ifndef SRPC_ISTREAM_H
#define SRPC_ISTREAM_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "Types.h"
#include "StringTypes.h"
#include "utility/Bits.h"
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4819)
#endif
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_fundamental.hpp>
#include <boost/utility.hpp>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <string>
#include <cassert>

namespace srpc {

class IStream;

/** @addtogroup serialization
* @{
*/

namespace
{

/// �⺻ ���������� �ƴ� ���� ������ RpcType���� ����
template <typename T, bool isFundamental, bool isEnum>
struct StreamReaderImpl;

// fundamental type
template <typename T>
struct StreamReaderImpl<T, true, false>
{
    static void read(IStream& istream, T& value) {
        istream.read(value);
    }
};

// enum type
template <typename T>
struct StreamReaderImpl<T, false, true>
{
    static void read(IStream& istream, T& value) {
        Int32 intValue;
        istream.read(intValue);
        value = static_cast<T>(intValue);
    }
};


template <typename T>
struct StreamReaderImpl<T, false, false>
{
    static void read(IStream& istream, T& value) {
        value.serialize(istream);
    }
};

} // namespace

/**
 * @class IStream
 *
 * Input stream.
 * - CAUTION: ������ Ÿ���� �������� ��Ʈ������ �о�´�
 */
class IStream : public boost::noncopyable
{
public:
    virtual ~IStream() {}

    template <typename T>
    IStream& operator>>(T& value) {
        StreamReaderImpl<T, boost::is_fundamental<T>::value,
            boost::is_enum<T>::value>::read(*this, value);
        return *this;
    }

    /// same as boost.serialization
    template <typename T>
    IStream& operator&(T& value) {
        return operator>>(value);
    }

    /// ��Ʈ�����۷� ���� 64��Ʈ ��ȣ ���� ���� Ÿ���� �о� �´�
    virtual void read(UInt64& value, int bitCount = Bits<UInt64>::size) = 0;

    /// ��Ʈ�����۷� ���� 32��Ʈ ��ȣ ���� ���� Ÿ���� �о� �´�
    virtual void read(UInt32& value, int bitCount = Bits<UInt32>::size) = 0;

    /// ��Ʈ�����۷� ���� 16��Ʈ ��ȣ ���� ���� Ÿ���� �о� �´�
    virtual void read(UInt16& value, int bitCount = Bits<UInt16>::size) = 0;

    /// ��Ʈ�����۷� ���� 8��Ʈ ��ȣ ���� ���� Ÿ���� �о� �´�
    virtual void read(UInt8& value, int bitCount = Bits<UInt8>::size) = 0;

    /// ��Ʈ�����۷� ���� 64��Ʈ ��ȣ �ִ� ���� Ÿ���� �о� �´�
    virtual void read(Int64& value, int bitCount = Bits<Int64>::size) = 0;

    /// ��Ʈ�����۷� ���� 32��Ʈ ��ȣ �ִ� ���� Ÿ���� �о� �´�
    virtual void read(Int32& value, int bitCount = Bits<Int32>::size) = 0;

    /// ��Ʈ�����۷� ���� 16��Ʈ ��ȣ �ִ� ���� Ÿ���� �о� �´�
    virtual void read(Int16& value, int bitCount = Bits<Int16>::size) = 0;

    /// ��Ʈ�����۷� ���� 8��Ʈ ��ȣ �ִ� ���� Ÿ���� �о� �´�
    virtual void read(Int8& value, int bitCount = Bits<Int8>::size) = 0;

    /// ��Ʈ�����۷� ���� �ο︰���� �о� �´�
    virtual void read(bool& value, int bitCount = Bits<bool>::size) = 0;

    /// ��Ʈ�����۷� ���� 32��Ʈ �ε� �Ҽ��� Ÿ���� �о� �´�
    virtual void read(Float32& value, int bitCount = Bits<Int32>::size) = 0;

    /// �ִ� maxLength ������ ���ڿ��� �о�´�
    virtual void read(String& value, size_t maxLength,
        int sizeBitCount) = 0;

    /// �ִ� maxLength ������ ���ڿ��� �о�´�
    virtual void read(WString& value, size_t maxLength,
        int sizeBitCount) = 0;

    /**
     * ��Ʈ�����۷� ���� raw buffer�� �о� �´�
     * @param buffer �Է� ����. �ݵ� length �̻��� �޸𸮸� Ȯ���ؾ� �Ѵ�.
     * @param length �о�� ����Ʈ ��
     */
    virtual void read(void* buffer, UInt16 length) = 0;

    /// ��Ʈ���� ����Ʈ ������ �����Ѵ�(���ۿ� �����ִ� ��Ʈ�� ������)
    virtual void align() = 0;

    /**
     * ��Ʈ���� �ʱ�ȭ�Ѵ�.
     * @param resetBuffer ���۸� �ʱ�ȭ�� ���ΰ�?
     */
    virtual void reset(bool resetBuffer) = 0;

    /// ���ۿ� ���� �ִ� �������� �� ũ��(��Ʈ ����)�� ��´�
    virtual int getTotalBitCount() const = 0;

    /// ���ۿ� ���� �ִ� �������� �� ũ��(����Ʈ ����)�� ��´�
    virtual int getTotalSize() const = 0;

    template <typename T>
    void do_read(T& value, int sizeBitCount, const boost::false_type&) {
        read(value, sizeBitCount);
    }

    template <typename T>
    void do_read(T& value, int sizeBitCount, const boost::true_type&) {
        Int32 temp;
        read(temp, sizeBitCount);
        value = static_cast<T>(temp);
    }
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_ISTREAM_H
