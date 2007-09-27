#include "stdafx.h"
#include <srpc/detail/ForwardingFunctors.h>
#include "StreamTexture.h"
#include <srpc/RpcTypes.h>

using namespace srpc;

/**
* @class ForwardingFunctorTest 
*
* marshaling functor test
*/
class ForwardingFunctorTest : public BitStreamTexture
{
    CPPUNIT_TEST_SUITE(ForwardingFunctorTest );
    CPPUNIT_TEST(testP0);
    CPPUNIT_TEST(testP1);
    CPPUNIT_TEST(testP2);
    CPPUNIT_TEST(testP3);
    CPPUNIT_TEST(testP4);
    CPPUNIT_TEST(testP5);
    CPPUNIT_TEST(testP6);
    CPPUNIT_TEST(testP7);
    CPPUNIT_TEST(testComplex);
    CPPUNIT_TEST_SUITE_END();
private:
    void testP0();
    void testP1();
    void testP2();
    void testP3();
    void testP4();
    void testP5();
    void testP6();
    void testP7();
    void testComplex();
};

CPPUNIT_TEST_SUITE_REGISTRATION(ForwardingFunctorTest);

void ForwardingFunctorTest::testP0()
{
    ForwardingFunctorT<SRPC_TYPELIST_0()> functor;
    functor(*ostream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        0, ostream_->getTotalSize());
}


void ForwardingFunctorTest::testP1()
{
    ForwardingFunctorT<SRPC_TYPELIST_1(RInt32)> functor(1234);
    functor(*ostream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("ostream size",
        4, ostream_->getTotalSize());

    Int32 value;
    istream_->read(value);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
        1234, value);
}


void ForwardingFunctorTest::testP2()
{
    ForwardingFunctorT<SRPC_TYPELIST_2(RInt32, RInt32)> functor(1234, 4321);
    functor(*ostream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("ostream size",
        4 + 4, ostream_->getTotalSize());

    Int32 value1;
    istream_->read(value1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("value1",
        1234, value1);

    Int32 value2;
    istream_->read(value2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("value2",
        4321, value2);
}


void ForwardingFunctorTest::testP3()
{
    ForwardingFunctorT<SRPC_TYPELIST_3(RInt32, RInt32, RInt32)> functor(0, 1, 2);
    functor(*ostream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("ostream size",
        4 * 3, ostream_->getTotalSize());

    for (int i = 0; i < 3; ++i) {
        Int32 value;
        istream_->read(value);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
            i, value);
    }
}


void ForwardingFunctorTest::testP4()
{
    ForwardingFunctorT<SRPC_TYPELIST_4(RInt32, RInt32, RInt32, RInt32)>
        functor(0, 1, 2, 3);
    functor(*ostream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("ostream size",
        4 * 4, ostream_->getTotalSize());

    for (int i = 0; i < 4; ++i) {
        Int32 value;
        istream_->read(value);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
            i, value);
    }
}


void ForwardingFunctorTest::testP5()
{
    ForwardingFunctorT<SRPC_TYPELIST_5(RInt32, RInt32, RInt32, RInt32, RInt32)>
        functor(0, 1, 2, 3, 4);
    functor(*ostream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("ostream size",
        4 * 5, ostream_->getTotalSize());

    for (int i = 0; i < 5; ++i) {
        Int32 value;
        istream_->read(value);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
            i, value);
    }
}


void ForwardingFunctorTest::testP6()
{
    ForwardingFunctorT<SRPC_TYPELIST_6(RInt32, RInt32, RInt32, RInt32, RInt32,
        RInt32)> functor(0, 1, 2, 3, 4, 5);
    functor(*ostream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("ostream size",
        4 * 6, ostream_->getTotalSize());

    for (int i = 0; i < 6; ++i) {
        Int32 value;
        istream_->read(value);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
            i, value);
    }
}


void ForwardingFunctorTest::testP7()
{
    ForwardingFunctorT<SRPC_TYPELIST_7(RInt32, RInt32, RInt32, RInt32, RInt32,
        RInt32, RInt32)> functor(0, 1, 2, 3, 4, 5, 6);
    functor(*ostream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("ostream size",
        4 * 7, ostream_->getTotalSize());

    for (int i = 0; i < 7; ++i) {
        Int32 value;
        istream_->read(value);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
            i, value);
    }
}


void ForwardingFunctorTest::testComplex()
{
    typedef RpcUIntType<UInt8, 4> RUInt4;
    std::string s("0123456789");
    ForwardingFunctorT<SRPC_TYPELIST_4(RUInt4, RInt8, RInt16, RString)>
        functor(0xFF, -1, -1, s);
    functor(*ostream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("ostream size",
        1 + 1 + 2 + (2 + 10), ostream_->getTotalSize());

    RUInt4 value1;
    value1.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("value1",
        0x0F, static_cast<int>(value1));

    RInt8 value2;
    value2.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("value2",
        -1, static_cast<int>(value2));

    Int16 value3;
    istream_->read(value3);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("value3",
        -1, static_cast<int>(value3));

    std::string value4;
    istream_->read(value4, USHRT_MAX, Bits<UInt16>::size);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("value4",
        s, value4);
}
