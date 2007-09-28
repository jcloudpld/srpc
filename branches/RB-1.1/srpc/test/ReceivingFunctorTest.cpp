#include "stdafx.h"
#include "StreamTexture.h"
#include <srpc/RpcTypes.h>
#include <srpc/detail/ReceivingFunctors.h>

using namespace srpc;

/**
* @class ReceivingFunctorTest 
*
* marshaling functor test
*/
class ReceivingFunctorTest : public BitStreamTexture
{
    CPPUNIT_TEST_SUITE(ReceivingFunctorTest );
    CPPUNIT_TEST(testP0);
    CPPUNIT_TEST(testP1);
    CPPUNIT_TEST(testP2);
    CPPUNIT_TEST(testP3);
    CPPUNIT_TEST(testP4);
    CPPUNIT_TEST(testP5);
    CPPUNIT_TEST(testP6);
    CPPUNIT_TEST(testP7);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
private:
    void testP0();
    void testP1();
    void testP2();
    void testP3();
    void testP4();
    void testP5();
    void testP6();
    void testP7();
private:
    void handler0(const void* /*rpcHint*/) {
        callee_ = "handler0";
    }
    void handler1(const RInt32& p1, const void* /*rpcHint*/) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
            0, p1.get());
        callee_ = "handler1";
    }
    void handler2(const RInt32& p1, const RInt32& p2,
        const void* /*rpcHint*/) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 0, p1.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 1, p2.get());
        callee_ = "handler2";
    }
    void handler3(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const void* /*rpcHint*/) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 0, p1.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 1, p2.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p3", 2, p3.get());
        callee_ = "handler3";
    }
    void handler4(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const RInt32& p4, const void* /*rpcHint*/) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 0, p1.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 1, p2.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p3", 2, p3.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p4", 3, p4.get());
        callee_ = "handler4";
    }
    void handler5(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const RInt32& p4, const RInt32& p5, const void* /*rpcHint*/) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 0, p1.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 1, p2.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p3", 2, p3.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p4", 3, p4.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p5", 4, p5.get());
        callee_ = "handler5";
    }
    void handler6(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const RInt32& p4, const RInt32& p5, const RInt32& p6,
        const void* /*rpcHint*/) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 0, p1.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 1, p2.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p3", 2, p3.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p4", 3, p4.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p5", 4, p5.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p6", 5, p6.get());
        callee_ = "handler6";
    }
    void handler7(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const RInt32& p4, const RInt32& p5, const RInt32& p6,
        const RInt32& p7, const void* /*rpcHint*/) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 0, p1.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 1, p2.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p3", 2, p3.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p4", 3, p4.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p5", 4, p5.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p6", 5, p6.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p7", 6, p7.get());
        callee_ = "handler7";
    }
    void handler8(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const RInt32& p4, const RInt32& p5, const RInt32& p6,
        const RInt32& p7, const RInt32& p8, const void* /*rpcHint*/) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 0, p1.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 1, p2.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p3", 2, p3.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p4", 3, p4.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p5", 4, p5.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p6", 5, p6.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p7", 6, p7.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p8", 7, p8.get());
        callee_ = "handler8";
    }
    void handler9(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const RInt32& p4, const RInt32& p5, const RInt32& p6,
        const RInt32& p7, const RInt32& p8, const RInt32& p9,
        const void* /*rpcHint*/) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 0, p1.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 1, p2.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p3", 2, p3.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p4", 3, p4.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p5", 4, p5.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p6", 5, p6.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p7", 6, p7.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p8", 7, p8.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p9", 8, p9.get());
        callee_ = "handler9";
    }
    void handler10(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const RInt32& p4, const RInt32& p5, const RInt32& p6,
        const RInt32& p7, const RInt32& p8, const RInt32& p9,
        const RInt32& p10, const void* /*rpcHint*/) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 0, p1.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 1, p2.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p3", 2, p3.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p4", 3, p4.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p5", 4, p5.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p6", 5, p6.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p7", 6, p7.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p8", 7, p8.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p9", 8, p9.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("p10", 9, p10.get());
        callee_ = "handler10";
    }
private:
    String callee_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ReceivingFunctorTest );

void ReceivingFunctorTest::setUp()
{
    BitStreamTexture::setUp();

    for (int i = 0; i < 10; ++i) {
		RInt32 value(i);
		value.write(*ostream_);
    }
}


void ReceivingFunctorTest::testP0()
{
    ReceivingFunctorT<ReceivingFunctorTest, SRPC_TYPELIST_0()>
        functor(&ReceivingFunctorTest::handler0);
    functor(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        4 * 10, istream_->getTotalSize());

    functor.call(this, 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("callee",
        String("handler0"), callee_);
}


void ReceivingFunctorTest::testP1()
{
    ReceivingFunctorT<ReceivingFunctorTest, SRPC_TYPELIST_1(RInt32)>
        functor(&ReceivingFunctorTest::handler1);
    functor(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        (4 * 10) - 4, istream_->getTotalSize());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        0, functor.p1_.get());

    functor.call(this, 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("callee",
        String("handler1"), callee_);
}


void ReceivingFunctorTest::testP2()
{
    ReceivingFunctorT<ReceivingFunctorTest, SRPC_TYPELIST_2(RInt32, RInt32)>
        functor(&ReceivingFunctorTest::handler2);
    functor(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        (4 * 10) - (4 * 2), istream_->getTotalSize());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2",
        1, functor.p2_.get());

    functor.call(this, 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("callee",
        String("handler2"), callee_);
}


void ReceivingFunctorTest::testP3()
{
    ReceivingFunctorT<ReceivingFunctorTest,
        SRPC_TYPELIST_3(RInt32, RInt32, RInt32)>
        functor(&ReceivingFunctorTest::handler3);
    functor(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        (4 * 10) - (4 * 3), istream_->getTotalSize());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p3",
        2, functor.p3_.get());

    functor.call(this, 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("callee",
        String("handler3"), callee_);
}


void ReceivingFunctorTest::testP4()
{
    ReceivingFunctorT<ReceivingFunctorTest,
        SRPC_TYPELIST_4(RInt32, RInt32, RInt32, RInt32)>
        functor(&ReceivingFunctorTest::handler4);
    functor(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        (4 * 10) - (4 * 4), istream_->getTotalSize());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p4",
        3, functor.p4_.get());

    functor.call(this, 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("callee",
        String("handler4"), callee_);
}


void ReceivingFunctorTest::testP5()
{
    ReceivingFunctorT<ReceivingFunctorTest,
        SRPC_TYPELIST_5(RInt32, RInt32, RInt32, RInt32, RInt32)>
        functor(&ReceivingFunctorTest::handler5);
    functor(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        (4 * 10) - (4 * 5), istream_->getTotalSize());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p5",
        4, functor.p5_.get());

    functor.call(this, 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("callee",
        String("handler5"), callee_);
}


void ReceivingFunctorTest::testP6()
{
    ReceivingFunctorT<ReceivingFunctorTest,
        SRPC_TYPELIST_6(RInt32, RInt32, RInt32, RInt32, RInt32, RInt32)>
        functor(&ReceivingFunctorTest::handler6);
    functor(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        (4 * 10) - (4 * 6), istream_->getTotalSize());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p6",
        5, functor.p6_.get());

    functor.call(this, 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("callee",
        String("handler6"), callee_);
}


void ReceivingFunctorTest::testP7()
{
    ReceivingFunctorT<ReceivingFunctorTest,
        SRPC_TYPELIST_7(RInt32, RInt32, RInt32, RInt32, RInt32, RInt32,
        RInt32)> functor(&ReceivingFunctorTest::handler7);
    functor(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        (4 * 10) - (4 * 7), istream_->getTotalSize());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p7",
        6, functor.p7_.get());

    functor.call(this, 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("callee",
        String("handler7"), callee_);
}
