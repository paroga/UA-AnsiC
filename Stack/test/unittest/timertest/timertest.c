/* Copyright (c) 1996-2016, OPC Foundation. All rights reserved.

   The source code in this file is covered under a dual-license scenario:
     - RCL: for OPC Foundation members in good-standing
     - GPL V2: everybody else

   RCL license terms accompanied with this source code. See http://opcfoundation.org/License/RCL/1.00/

   GNU General Public License as published by the Free Software Foundation;
   version 2 of the License are accompanied with this source code. See http://opcfoundation.org/License/GPLv2

   This source code is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "timertest.h"
#include <unittest.h>
/* base */
#include <opcua_proxystub.h>

/* core */
#include <opcua_core.h>
#include <opcua_mutex.h>
#include <opcua_semaphore.h>
#include <opcua_thread.h>
#include <opcua_list.h>

/* self */
#include <opcua_timer.h>
#include <opcua_p_socket.h>
#include <opcua_p_timer.h>
#include <opcua_p_datetime.h>
#include <opcua_p_socket_interface.h>

#ifndef _WIN32
# include <unistd.h>
#endif

#define sleepShutdownDelay 1000
#define IDBASE 16000

void OpcUa_TimerTest_SleepForHalfSecond (void)
{
#if OPCUA_MULTITHREADED
    OpcUa_Thread_Sleep(500);
#else
    OpcUa_TimeVal   Start;
    OpcUa_TimeVal   End;
    OpcUa_Int32     TotalWaitTime   = (OpcUa_Int32)500;
    OpcUa_Int32     WaitTime        = TotalWaitTime;
    OpcUa_Int32     Diff;
    OpcUa_Int32     DiffMS;

    OpcUa_DateTime_GetTimeOfDay( &Start );

    while(WaitTime > 0)
    {
        OpcUa_SocketManager_Loop( OpcUa_Null, WaitTime, OpcUa_True );

        OpcUa_DateTime_GetTimeOfDay( &End );
        Diff = ( End.uintSeconds - Start.uintSeconds ) * 1000;

        if(End.uintMicroSeconds < Start.uintMicroSeconds)
        {
            /* overflow */
            DiffMS = End.uintMicroSeconds + (1000000 - Start.uintMicroSeconds);
            Diff -= 1000;
        }
        else
        {
            DiffMS = End.uintMicroSeconds - Start.uintMicroSeconds;
        }

        DiffMS /= 1000;
        DiffMS += Diff;

        if(DiffMS > TotalWaitTime)
        {
            WaitTime = 0;
        }
        else
        {
            WaitTime = TotalWaitTime - DiffMS;
        }
    }
#endif
}

/* forward declarations of test cases */
OpcUa_StatusCode OpcUa_TimerTest_Create();
OpcUa_StatusCode OpcUa_TimerTest_Delete();
OpcUa_StatusCode OpcUa_TimerTest_CreateDelete();
/* TODO: add test cases here */

/** Test Entry Point */
void OpcUa_TimerTest_Run()
{
    OpcUa_TimerTest_Create();
    OpcUa_TimerTest_Delete();
    OpcUa_TimerTest_CreateDelete();
}

OpcUa_UInt32 a_TimerCallbacks [OPCUA_P_TIMER_NO_OF_TIMERS + 1];
OpcUa_UInt32 a_KillCallbacks [OPCUA_P_TIMER_NO_OF_TIMERS + 1];
OpcUa_Timer a_hTimers [OPCUA_P_TIMER_NO_OF_TIMERS + 1];

void
InitializeTimerCounts (
    void
)
{
    memset (&a_TimerCallbacks [0], 0, sizeof (a_TimerCallbacks));
    memset (&a_KillCallbacks [0], 0, sizeof (a_TimerCallbacks));
}

OpcUa_StatusCode
OPCUA_DLLCALL
TimerCallback (
    OpcUa_Void* pvCallbackData,
    OpcUa_Timer hTimer,
    OpcUa_UInt32 msecElapsed
)
{
    OpcUa_UInt32 uTimerNumber = (OpcUa_UInt32)(size_t)pvCallbackData;

    OpcUa_ReferenceParameter (hTimer);
    OpcUa_ReferenceParameter (msecElapsed);
    /*OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "TimerCallback\n");*/
    ++a_TimerCallbacks [uTimerNumber];
    return OpcUa_Good;
}

OpcUa_StatusCode
OPCUA_DLLCALL
OneShotTimerCallback (
    OpcUa_Void* pvCallbackData,
    OpcUa_Timer hTimer,
    OpcUa_UInt32 msecElapsed
)
{
    OpcUa_UInt32 uTimerNumber = (OpcUa_UInt32)(size_t)pvCallbackData;

    OpcUa_ReferenceParameter (msecElapsed);
    /*OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "OneShotTimerCallback\n");*/
    ++a_TimerCallbacks [uTimerNumber];
    OpcUa_Timer_Delete (&hTimer);
    return OpcUa_Good;
}


OpcUa_StatusCode
OPCUA_DLLCALL
KillCallback (
    OpcUa_Void* pvCallbackData,
    OpcUa_Timer hTimer,
    OpcUa_UInt32 msecElapsed
)
{
    OpcUa_UInt32 uTimerNumber = (OpcUa_UInt32)(size_t)pvCallbackData;

    OpcUa_ReferenceParameter (hTimer);
    OpcUa_ReferenceParameter (msecElapsed);

    /*OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "KillCallback\n");*/
    ++a_KillCallbacks [uTimerNumber];
    return OpcUa_Good;
}

OpcUa_StatusCode OpcUa_TimerTest_Create()
{
    SNAPSHOT_TYPE s1, s2, s3;

    CASE_ID (IDBASE);
    /* Create a timer with an invalid handle pointer. */
    /* (hTimer = OpcUa_Null) */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_Timer_Create (OpcUa_Null, 300, TimerCallback, KillCallback, 0), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_TimerTest_Delete ()
{
    SNAPSHOT_TYPE s1, s2, s3;

    CASE_ID (IDBASE+1);
    /* Call delete with an invalid handle pointer. */
    /* (a_phTimer = OpcUa_Null) */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_Timer_Delete (OpcUa_Null), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


#ifdef OPCUA_TIMER_TEST_INVALID_HANDLES
    CASE_ID (IDBASE+2);
    /* Call delete with an invalid handle. */
    /* (*a_phTimer = OpcUa_Null) */
    hTimer = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_Timer_Delete (&hTimer), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();
#endif


#ifdef OPCUA_TIMER_TEST_INVALID_HANDLES
    CASE_ID (IDBASE+3);
    /* Call delete with an invalid handle. */
    /* (*a_phTimer = 0xFFFFFFFF) */
    hTimer = 0xFFFFFFFF;
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_Timer_Delete (&hTimer), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();
#endif

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_TimerTest_CreateDelete ()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_Timer hTimer;
    unsigned uTimerIndex;
    unsigned uSleepCount;
    unsigned uExpectedCount;

    CASE_ID (IDBASE+4);
    /* Create a timer with a short interval, verify that it runs, */
    /* delete it, and verify the kill callback runs. */
    hTimer = OpcUa_Null;
    InitializeTimerCounts ();
    MEM_SNAPSHOT (&s1);
    /*OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Test 255 ms timer within 500 ms interval.\n");*/
    ASSERT_GOOD (OpcUa_Timer_Create (&hTimer, 255, TimerCallback, KillCallback, 0));
    OpcUa_TimerTest_SleepForHalfSecond ();
    /*OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Verify callbacks: a_TimerCallbacks[0]=%i\n", a_TimerCallbacks[0]);*/
    ASSERT (1 == a_TimerCallbacks [0]);
    ASSERT_GOOD (OpcUa_Timer_Delete (&hTimer));
    ASSERT (1 == a_KillCallbacks [0]);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE+5);
    /* Create a timer no timer callback (fpTimerCallback = OpcUa_Null), */
    /* delete it, and verify the kill callback runs. */
    hTimer = OpcUa_Null;
    InitializeTimerCounts ();
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_Timer_Create (&hTimer, 300, OpcUa_Null, KillCallback, 0));
    OpcUa_TimerTest_SleepForHalfSecond ();
    ASSERT (0 == a_TimerCallbacks [0]);
    ASSERT_GOOD (OpcUa_Timer_Delete (&hTimer));
    ASSERT (1 == a_KillCallbacks [0]);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE+6);
    /* Create a timer with no kill callback. (fpKillCallback = OpcUa_Null), */
    /* verify that it runs, and delete it */
    hTimer = OpcUa_Null;
    InitializeTimerCounts ();
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_Timer_Create (&hTimer, 300, TimerCallback, OpcUa_Null, 0));
    OpcUa_TimerTest_SleepForHalfSecond ();
    ASSERT (1 == a_TimerCallbacks [0]);
    ASSERT_GOOD (OpcUa_Timer_Delete (&hTimer));
    ASSERT (0 == a_KillCallbacks [0]);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE+7);
    /* Create OPCUA_P_TIMER_NO_OF_TIMERS timers with short intervals, verify that they run, */
    /* delete them, and verify the kill callbacks run. */
    InitializeTimerCounts ();
    MEM_SNAPSHOT (&s1);
    for (uTimerIndex = 0; uTimerIndex < OPCUA_P_TIMER_NO_OF_TIMERS; ++uTimerIndex) {
        ASSERT_GOOD (OpcUa_Timer_Create (&a_hTimers [uTimerIndex], 300, TimerCallback, KillCallback, (OpcUa_Void*)(size_t)uTimerIndex));
    }
    OpcUa_TimerTest_SleepForHalfSecond ();
    for (uTimerIndex = 0; uTimerIndex < OPCUA_P_TIMER_NO_OF_TIMERS; ++uTimerIndex) {
        ASSERT_GOOD (OpcUa_Timer_Delete (&a_hTimers [uTimerIndex]));
    }
    for (uTimerIndex = 0; uTimerIndex < OPCUA_P_TIMER_NO_OF_TIMERS; ++uTimerIndex) {
        ASSERT (1 == a_TimerCallbacks [uTimerIndex]);
        ASSERT (1 == a_KillCallbacks [uTimerIndex]);
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE+8);
    /* Create OPCUA_P_TIMER_NO_OF_TIMERS + 1 timers with short intervals, verify that */
    /* creation fails on the last timer, and delete them. */
    InitializeTimerCounts ();
    MEM_SNAPSHOT (&s1);
    for (uTimerIndex = 0; uTimerIndex < OPCUA_P_TIMER_NO_OF_TIMERS; ++uTimerIndex) {
        ASSERT_GOOD (OpcUa_Timer_Create (&a_hTimers [uTimerIndex], 300, TimerCallback, KillCallback, (OpcUa_Void*)(size_t)uTimerIndex));
    }
    ASSERT_STATUS (OpcUa_Timer_Create (&a_hTimers [OPCUA_P_TIMER_NO_OF_TIMERS], 300, TimerCallback, KillCallback, (OpcUa_Void*)OPCUA_P_TIMER_NO_OF_TIMERS), OpcUa_BadResourceUnavailable);
    OpcUa_TimerTest_SleepForHalfSecond ();
    for (uTimerIndex = 0; uTimerIndex < OPCUA_P_TIMER_NO_OF_TIMERS; ++uTimerIndex) {
        ASSERT_GOOD (OpcUa_Timer_Delete (&a_hTimers [uTimerIndex]));
    }
    for (uTimerIndex = 0; uTimerIndex < OPCUA_P_TIMER_NO_OF_TIMERS; ++uTimerIndex) {
        ASSERT (1 == a_TimerCallbacks [uTimerIndex]);
        ASSERT (1 == a_KillCallbacks [uTimerIndex]);
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE+9);
    /* Create a timer with a zero interval, verify that it runs multiple times, */
    /* and delete it. */
    hTimer = OpcUa_Null;
    InitializeTimerCounts ();
    MEM_SNAPSHOT (&s1);
#if 0
    ASSERT_GOOD (OpcUa_Timer_Create (&hTimer, 0, TimerCallback, KillCallback, 0));
    SleepForHalfSecond ();
    ASSERT (a_TimerCallbacks [0] > 1);
    ASSERT_GOOD (OpcUa_Timer_Delete (&hTimer));
    ASSERT (1 == a_KillCallbacks [0]);
#else
    ASSERT_STATUS(OpcUa_Timer_Create (&hTimer, 0, TimerCallback, KillCallback, 0), OpcUa_BadInvalidArgument);
#endif
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE+10);
    /* Create a timer with a very long interval, verify that it doesn't run, */
    /* and delete it. */
    hTimer = OpcUa_Null;
    InitializeTimerCounts ();
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_Timer_Create (&hTimer, 0x0FFFFFFF, TimerCallback, KillCallback, 0));
    OpcUa_TimerTest_SleepForHalfSecond ();
    ASSERT (0 == a_TimerCallbacks [0]);
    ASSERT_GOOD (OpcUa_Timer_Delete (&hTimer));
    ASSERT (1 == a_KillCallbacks [0]);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


#ifdef OPCUA_TIMER_TEST_INVALID_HANDLES
    CASE_ID (IDBASE+11);
    /* Create a timer with a short interval, verify that it runs, */
    /* and call delete on it twice. */
    hTimer = OpcUa_Null;
    InitializeTimerCounts ();
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_Timer_Create (&a_hTimers [0], 300, TimerCallback, KillCallback, 0));
    hTimer = a_hTimers [0];
    SleepForHalfSecond ();
    ASSERT (1 == a_TimerCallbacks [0]);
    ASSERT_GOOD (OpcUa_Timer_Delete (&a_hTimer [0]));
    ASSERT (1 == a_KillCallbacks [0]);
    ASSERT (OpcUa_Good != OpcUa_Timer_Delete (&hTimer));
    ASSERT (1 == a_KillCallbacks [0]);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();
#endif

    /* The following tests may fail if too much timers are used simultanously.
       (remember: this are low precision timers)
    */
    /* Number should not be lower than OPCUA_P_TIMER_NO_OF_TIMERS */
    #define NO_OF_TIMERS_TO_TEST 5

    #define INTERVAL_BASE (200)
    /* Calculate how many times the timer callback routine has been expected to run */
    /* The formula is:  Number of seconds asleep - precision / timer interval */
    #define EXPECTED_COUNT(SLEEP_NUM,TIMER_NUM) ((500 * (SLEEP_NUM + 1) - 100) / (INTERVAL_BASE * (TIMER_NUM + 1)))

    CASE_ID (IDBASE+12);
    /* Create NO_OF_TIMERS_TO_TEST timers with different intervals, verify that they run */
    /* the proper number of times, and delete them. */
    InitializeTimerCounts ();
    MEM_SNAPSHOT (&s1);

    for (uTimerIndex = 0; uTimerIndex < NO_OF_TIMERS_TO_TEST; ++uTimerIndex)
    {
        /* first timer every 150 msec, second every 300 msec and so on to 1350 */
        ASSERT_GOOD (OpcUa_Timer_Create (&a_hTimers [uTimerIndex], INTERVAL_BASE * (uTimerIndex + 1), TimerCallback, KillCallback, (OpcUa_Void*)(size_t)uTimerIndex));
    }

    /* for sleep count */
    for (uSleepCount = 0; uSleepCount < NO_OF_TIMERS_TO_TEST; ++uSleepCount)
    {
        OpcUa_TimerTest_SleepForHalfSecond ();

        /* for every timer */
        for (uTimerIndex = 0; uTimerIndex < NO_OF_TIMERS_TO_TEST; ++uTimerIndex)
        {
            uExpectedCount = EXPECTED_COUNT (uSleepCount, uTimerIndex);

            if (uExpectedCount > 0)
            {
                /* if expected count is non 0, accept interval +-1 */
                ASSERT((uExpectedCount - 1) <= a_TimerCallbacks [uTimerIndex] && (uExpectedCount + 1) >= a_TimerCallbacks [uTimerIndex]);
            }
            else
            {
                /* if expected count is 0, accept 0 and 1 */
                ASSERT(0 == a_TimerCallbacks [uTimerIndex] || 1 == a_TimerCallbacks [uTimerIndex]);
            };
        }
    }

    for (uTimerIndex = 0; uTimerIndex < NO_OF_TIMERS_TO_TEST; ++uTimerIndex)
    {
        ASSERT_GOOD (OpcUa_Timer_Delete (&a_hTimers [uTimerIndex]));
    }

    for (uTimerIndex = 0; uTimerIndex < NO_OF_TIMERS_TO_TEST; ++uTimerIndex)
    {
        ASSERT (1 == a_KillCallbacks [uTimerIndex]);
    }

    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE+13);
    /* Create OPCUA_P_TIMER_NO_OF_TIMERS one-shot timers with different intervals, and verify that they run */
    /* and are deleted. (Create one-shot by calling Delete during the callback) */
    InitializeTimerCounts ();
    MEM_SNAPSHOT (&s1);
    for (uTimerIndex = 0; uTimerIndex < NO_OF_TIMERS_TO_TEST; ++uTimerIndex)
    {
        ASSERT_GOOD (OpcUa_Timer_Create (&a_hTimers [uTimerIndex], INTERVAL_BASE * (uTimerIndex + 1), OneShotTimerCallback, KillCallback, (OpcUa_Void*)(size_t)uTimerIndex));
    }

    /* sleep one and a half second */
    OpcUa_TimerTest_SleepForHalfSecond ();
    OpcUa_TimerTest_SleepForHalfSecond ();
    OpcUa_TimerTest_SleepForHalfSecond ();

    for (uSleepCount = 0; uSleepCount < NO_OF_TIMERS_TO_TEST; ++uSleepCount)
    {
        for (uTimerIndex = 0; uTimerIndex < NO_OF_TIMERS_TO_TEST; ++uTimerIndex)
        {
            /* Timer should have fired once, so count should be exactly one */
            ASSERT (a_TimerCallbacks [uTimerIndex] == 1);
            if(a_TimerCallbacks [uTimerIndex] != 1)
            {
                OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Timer at index %u is %u", uTimerIndex, a_TimerCallbacks[uTimerIndex]);
            }
        }
    }

    /* shutdown time */
    OpcUa_TimerTest_SleepForHalfSecond ();

    /* At this point, all timers should have been destroyed by the call to */
    /* OpcUa_Timer_Delete in the OneShotTimerCallback routine */
    for (uTimerIndex = 0; uTimerIndex < NO_OF_TIMERS_TO_TEST; ++uTimerIndex)
    {
        ASSERT (1 == a_KillCallbacks [uTimerIndex]);
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}
