#include "stdafx.h"
#include <nsrpc/utility/SmartPtr.h>

using namespace nsrpc;

class Widget : public SharedObject
{
public:
    ~Widget() {
        dtorCalled_ = true;
    }

    static bool dtorCalled_;
};

bool Widget::dtorCalled_ = false;

typedef SmartPtr<Widget> WidgetPtr;

/**
* @class SmartPtrTest 
*
* SmartPtr test
*/
class SmartPtrTest  : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SmartPtrTest );
    CPPUNIT_TEST(testAddRef);
    CPPUNIT_TEST(testRemoveRef);
    CPPUNIT_TEST(testRelease);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testAddRef();
    void testRemoveRef();
    void testRelease();
private:
    WidgetPtr widget_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(SmartPtrTest );

void SmartPtrTest::setUp()
{
    widget_ = new Widget;
}


void SmartPtrTest::tearDown()
{
    // delete widget_;
}


void SmartPtrTest::testAddRef()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("ref count",
        1, widget_->getReferenceCount());


    WidgetPtr dummy = widget_;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("ref count",
        2, widget_->getReferenceCount());

    WidgetPtr dummy2 = dummy;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("ref count",
        3, widget_->getReferenceCount());
}


void SmartPtrTest::testRemoveRef()
{
    {
        WidgetPtr dummy = widget_;
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("ref count",
        1, widget_->getReferenceCount());

    {
        WidgetPtr dummy1 = widget_;
        WidgetPtr dummy2 = dummy1;
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("ref count",
        1, widget_->getReferenceCount());
}


void SmartPtrTest::testRelease()
{
    {
        WidgetPtr dummy(new Widget);
        WidgetPtr dummy1 = dummy;
        WidgetPtr dummy2 = dummy1;
    }
    CPPUNIT_ASSERT_MESSAGE("released",
        Widget::dtorCalled_);
}
