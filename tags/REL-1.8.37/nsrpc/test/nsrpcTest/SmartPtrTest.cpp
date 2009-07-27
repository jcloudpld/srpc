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
class SmartPtrTest  : public testing::Test
{
private:
    virtual void SetUp() {
        widget_ = new Widget;
    }

    virtual void TearDown() {
        // delete widget_;
    }

protected:
    WidgetPtr widget_;
};


TEST_F(SmartPtrTest, testAddRef)
{
    EXPECT_EQ(1, widget_->getReferenceCount());

    WidgetPtr dummy = widget_;
    EXPECT_EQ(2, widget_->getReferenceCount());

    WidgetPtr dummy2 = dummy;
    EXPECT_EQ(3, widget_->getReferenceCount());
}


TEST_F(SmartPtrTest, testRemoveRef)
{
    {
        WidgetPtr dummy = widget_;
    }
    EXPECT_EQ(1, widget_->getReferenceCount());

    {
        WidgetPtr dummy1 = widget_;
        WidgetPtr dummy2 = dummy1;
    }
    EXPECT_EQ(1, widget_->getReferenceCount());
}


TEST_F(SmartPtrTest, testRelease)
{
    {
        WidgetPtr dummy(new Widget);
        WidgetPtr dummy1 = dummy;
        WidgetPtr dummy2 = dummy1;
    }
    EXPECT_TRUE(Widget::dtorCalled_) << "released";
}
