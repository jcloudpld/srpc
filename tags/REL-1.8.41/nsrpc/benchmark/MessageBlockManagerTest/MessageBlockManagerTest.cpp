#include "stdafx.h"
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4702)
#endif
#include <nsrpc/utility/MessageBlockManager.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include "ace/High_Res_Timer.h"

using namespace nsrpc;

const size_t n_chunks = 10;
const size_t chunk_size = 512;

void new_delete_speed_test(ACE_UINT32 loops)
{
#ifndef ACE_LACKS_FLOATING_POINT
    double tt    = 0.0,
        ut    = 0.0,
        utus  = 0.0,
        speed = 0.0;
#endif /* ACE_LACKS_FLOATING_POINT */

    ACE_Time_Value tc;
    ACE_UINT32 i = loops;

    printf("new/delete speed test...\n");

    ACE_High_Res_Timer timer;
    timer.reset();

    timer.start();

    while (i--) {
        ACE_Message_Block* block = new ACE_Message_Block(chunk_size);
        block->release();
    }

    timer.stop();

    timer.elapsed_time(tc);

    printf("Iterations : %d\n", loops);
#ifdef ACE_LACKS_FLOATING_POINT
    printf("Total time : %d s, %d us\n", tc.sec, tc.usec());
#elif !defined ACE_LACKS_FLOATING_POINT
    tt    = tc.sec() + tc.usec()*1.0e-6;
    ut    = tt/loops;
    utus  = ut*1.0e6;
    speed = loops/tt;

    printf("Total time : %.6g [s]\n", tt);
    printf("Unit time  : %.6g [us]\n", utus);
    printf("Speed      : %.6g [1/s]\n", speed);

#endif /* !defined ACE_LACKS_FLOATING_POINT */
}


void NoSynchMessageBlockManager_speed_test(ACE_UINT32 loops)
{
#ifndef ACE_LACKS_FLOATING_POINT
    double tt    = 0.0,
        ut    = 0.0,
        utus  = 0.0,
        speed = 0.0;
#endif /* ACE_LACKS_FLOATING_POINT */

    ACE_Time_Value tc;
    ACE_UINT32 i = loops;

    printf("NoSynchMessageBlockManager speed test...\n");

    NoSynchMessageBlockManager manager(n_chunks, chunk_size);

    ACE_High_Res_Timer timer;
    timer.reset();

    timer.start();

    while (i--) {
        ACE_Message_Block* block = manager.create(chunk_size);
        block->release();
    }

    timer.stop();

    timer.elapsed_time(tc);

    printf("Iterations : %d\n", loops);
#ifdef ACE_LACKS_FLOATING_POINT
    printf("Total time : %d s, %d us\n", tc.sec, tc.usec());
#elif !defined ACE_LACKS_FLOATING_POINT
    tt    = tc.sec() + tc.usec()*1.0e-6;
    ut    = tt/loops;
    utus  = ut*1.0e6;
    speed = loops/tt;

    printf("Total time : %.6g [s]\n", tt);
    printf("Unit time  : %.6g [us]\n", utus);
    printf("Speed      : %.6g [1/s]\n", speed);

#endif /* !defined ACE_LACKS_FLOATING_POINT */
}


void SynchMessageBlockManager_speed_test(ACE_UINT32 loops)
{
#ifndef ACE_LACKS_FLOATING_POINT
    double tt    = 0.0,
        ut    = 0.0,
        utus  = 0.0,
        speed = 0.0;
#endif /* ACE_LACKS_FLOATING_POINT */

    ACE_Time_Value tc;
    ACE_UINT32 i = loops;

    printf("SynchMessageBlockManager speed test...\n");

    SynchMessageBlockManager manager(n_chunks, chunk_size);

    ACE_High_Res_Timer timer;
    timer.reset();

    timer.start();

    while (i--) {
        ACE_Message_Block* block = manager.create(chunk_size);
        block->release();
    }

    timer.stop();

    timer.elapsed_time(tc);

    printf("Iterations : %d\n", loops);
#ifdef ACE_LACKS_FLOATING_POINT
    printf("Total time : %d s, %d us\n", tc.sec, tc.usec());
#elif !defined ACE_LACKS_FLOATING_POINT
    tt    = tc.sec() + tc.usec()*1.0e-6;
    ut    = tt/loops;
    utus  = ut*1.0e6;
    speed = loops/tt;

    printf("Total time : %.6g [s]\n", tt);
    printf("Unit time  : %.6g [us]\n", utus);
    printf("Speed      : %.6g [1/s]\n", speed);

#endif /* !defined ACE_LACKS_FLOATING_POINT */
}


int ACE_TMAIN(int /*argc*/, ACE_TCHAR* /*argv*/[])
{
    ACE_UINT32 loops = 10000000;
    new_delete_speed_test(loops);
    NoSynchMessageBlockManager_speed_test(loops);
    SynchMessageBlockManager_speed_test(loops);

    return 0;
}
