#include "stdafx.h"

#ifdef USE_CPPUNIT_PLUGIN_TESTER

#include <cppunit/plugin/TestPlugIn.h>

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4100)
#endif

// Implements all the plug-in stuffs, WinMain...
CPPUNIT_PLUGIN_IMPLEMENT();

#ifdef _MSC_VER
#  pragma warning (pop)
#endif

#else // USE_CPPUNIT_PLUGIN_TESTER

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable : 4100 4511 4512)
#endif
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <ace/OS_main.h>

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
    bool useXml = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-xml") {
            useXml = true;
        }
    }

    // Create the event manager and test controller
    CppUnit::TestResult controller;

    // Add a listener that colllects test result
    CppUnit::TestResultCollector result;
    controller.addListener(&result);

    // Add a listener that print dots as test run.
    boost::scoped_ptr<CppUnit::TestListener> progress;
    if (! useXml) {
        progress.reset(new CppUnit::TextTestProgressListener);
        controller.addListener(progress.get());
    }

    // Add the top suite to the test runner
    CppUnit::TestRunner runner;
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
    runner.run(controller);

    // Print test in a compiler compatible format.
    boost::scoped_ptr<CppUnit::Outputter> outputter;
    if (useXml) {
        outputter.reset(new CppUnit::XmlOutputter(&result, std::cout));
    }
    else {
        outputter.reset(new CppUnit::TextOutputter(&result, std::cout));
    }
    outputter->write();

    return result.wasSuccessful() ? 0 : 1;
}

#endif // USE_CPPUNIT_PLUGIN_TESTER
