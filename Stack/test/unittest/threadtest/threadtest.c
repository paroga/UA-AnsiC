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

#include "threadtest.h"
#include <unittest.h>
#include <opcua_thread.h>

#ifndef _WIN32
# include <unistd.h>
#endif

#if OPCUA_MULTITHREADED

#define sleepShutdownDelay 1000

/* forward declarations of test cases */
OpcUa_StatusCode OpcUa_ThreadTest_CreateDelete();
OpcUa_StatusCode OpcUa_ThreadTest_StartExit();
OpcUa_StatusCode OpcUa_ThreadTest_Sleep();
OpcUa_StatusCode OpcUa_ThreadTest_WaitForShutdown();
/* TODO: add test cases here */

unsigned g_uThreadExitedFlag = 0;

/** Test Entry Point */
void OpcUa_ThreadTest_Run()
{
    OpcUa_ThreadTest_CreateDelete();
    OpcUa_ThreadTest_StartExit();
    OpcUa_ThreadTest_Sleep();
    OpcUa_ThreadTest_WaitForShutdown();
}

OpcUa_Void DummyThreadMain(OpcUa_Void* pArguments)
{
    (void)pArguments;
}

OpcUa_StatusCode OpcUa_ThreadTest_CreateDelete()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_Thread Thread = OpcUa_Null;

    /** Test 11000 */
    /*  OpcUa_Thread_Create / OpcUa_Thread_Delete */
    /*  Create and Delete a thread in normal usage */
    CASE_ID(11000);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(OpcUa_Thread_Create(    &Thread,
                                        DummyThreadMain,
                                        OpcUa_Null));
    ASSERT(OpcUa_Null != Thread);
    OpcUa_Thread_Delete(&Thread);
    ASSERT(OpcUa_Null == Thread);
    MEM_SNAPSHOT(&s2);
    /* check memory */
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /** Test 11001 */
    /*  OpcUa_Thread_Create */
    /*  Call Create with an OpcUa_Null pointer */
    CASE_ID(11001);
    MEM_SNAPSHOT(&s1);
    ASSERT_STATUS(  OpcUa_Thread_Create(OpcUa_Null,
                                        DummyThreadMain,
                                        OpcUa_Null),
                    OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    /* check memory */
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /** Test 11002 */
    /*  OpcUa_Thread_Delete */
    /*  Call Delete with an OpcUa_Null pointer */
    CASE_ID(11002);
    /* This should not generate an exception */
    OpcUa_Thread_Delete(OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

OpcUa_Void SetExitFlag(OpcUa_Void* pArguments)
{
#if 0
    OpcUa_Thread Thread = OpcUa_Null;
    Thread = (OpcUa_Thread)pArguments;
#endif

    OpcUa_ReferenceParameter(pArguments);
    g_uThreadExitedFlag = 1;
}



OpcUa_StatusCode OpcUa_ThreadTest_StartExit()
{
    OpcUa_Thread Thread = OpcUa_Null;

    /** Test 11003 */
    /*  OpcUa_Thread_Start / OpcUa_Thread_Exit */
    /*  Start a thread and verify that it ran. */
    CASE_ID(11003);
    ASSERT_GOOD(    OpcUa_Thread_Create(&Thread,        /* Thread Handle   */
                                        SetExitFlag,    /* Thread Function */
                                        OpcUa_Null));   /* Thread Argument */
    ASSERT(OpcUa_Null != Thread);
    g_uThreadExitedFlag = 0;

    ASSERT_GOOD(OpcUa_Thread_Start(Thread));

    /* Give the worker thread a chance to run */
    OpcUa_Thread_Sleep(500);

    ASSERT_GOOD(OpcUa_Thread_WaitForShutdown(Thread, OPCUA_INFINITE));
    ASSERT(1 == g_uThreadExitedFlag);
    OpcUa_Thread_Delete(&Thread);
    ASSERT(OpcUa_Null == Thread);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_Void  SleepThenSetExitFlag(OpcUa_Void* pArgument)
{
    /* generates a warning when /Wp64 is set */
    /* with this method the cast below fails on 16-bit systems */
    OpcUa_UInt32 uSleepTime = (OpcUa_UInt32)(size_t)(pArgument);

    OpcUa_Thread_Sleep (uSleepTime);
    g_uThreadExitedFlag = 1;
}


OpcUa_StatusCode OpcUa_ThreadTest_Sleep()
{
    int iSecondsWaited = 0;
    OpcUa_Thread Thread = OpcUa_Null;

    /** Test 11004 */
    /*  OpcUa_Thread_Sleep */
    /*  Call sleep in a thread */
    CASE_ID(11004);
    ASSERT_GOOD(OpcUa_Thread_Create(    &Thread,
                                        SleepThenSetExitFlag,
                                        (OpcUa_Void*)4900));

    ASSERT(OpcUa_Null != Thread);
    g_uThreadExitedFlag = 0;

    ASSERT_GOOD(OpcUa_Thread_Start(Thread));

    for(iSecondsWaited = 1; iSecondsWaited < 15; ++iSecondsWaited)
    {
        OpcUa_Thread_Sleep(1000);
        if (1 == g_uThreadExitedFlag)
        {
            break;
        }
    }
    ASSERT(5 == iSecondsWaited);
    OpcUa_Thread_Delete(&Thread);
    ASSERT(OpcUa_Null == Thread);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_Void SleepThenShutdown(OpcUa_Thread pArguments)
{
#if 0
    OpcUa_Thread Thread = OpcUa_Null;
    Thread = (OpcUa_Thread)pArguments;
#endif

    OpcUa_ReferenceParameter(pArguments);

    OpcUa_Thread_Sleep(sleepShutdownDelay);
    ++g_uThreadExitedFlag;
}


OpcUa_Void WaitForThreadShutdown(OpcUa_Void* pArgument)
{
    OpcUa_Thread ShutdownThread = (OpcUa_Thread)pArgument;

    OpcUa_Thread_WaitForShutdown(ShutdownThread, OPCUA_INFINITE);
    ++g_uThreadExitedFlag;
}


OpcUa_StatusCode OpcUa_ThreadTest_WaitForShutdown()
{
    int iIndex;
    OpcUa_Thread Thread = OpcUa_Null;
    OpcUa_Thread Threads [6];

    /** Test 11005 */
    /*  OpcUa_Thread_WaitForShutdown */
    /*  Call WaitForShutdown in normal usage. */
    CASE_ID(11005);
    ASSERT_GOOD(OpcUa_Thread_Create(    &Thread,
                                        SleepThenSetExitFlag,
                                        (OpcUa_Void*)1500));
    ASSERT(OpcUa_Null != Thread);
    g_uThreadExitedFlag = 0;

    ASSERT_GOOD(OpcUa_Thread_Start(Thread));
    ASSERT_GOOD(OpcUa_Thread_WaitForShutdown(Thread,5000));
    ASSERT(1 == g_uThreadExitedFlag);
    OpcUa_Thread_Delete(&Thread);
    ASSERT(OpcUa_Null == Thread);
    END_CASE();

    /** Test 11006 */
    /*  OpcUa_Thread_WaitForShutdown */
    /*  Call WaitForShutdown with a zero timeout. */
    CASE_ID(11006);
    ASSERT_GOOD(OpcUa_Thread_Create(    &Thread,
                                        SleepThenSetExitFlag,
                                        (OpcUa_Void*)2500));
    ASSERT(OpcUa_Null != Thread);
    g_uThreadExitedFlag = 0;

    ASSERT_GOOD(OpcUa_Thread_Start(Thread));
    ASSERT_STATUS(OpcUa_Thread_WaitForShutdown(Thread,0), OpcUa_GoodNonCriticalTimeout);
    ASSERT(0 == g_uThreadExitedFlag);
    /* Now give thread a chance to finish */
    ASSERT_GOOD(OpcUa_Thread_WaitForShutdown(Thread,5000));
    ASSERT(1 == g_uThreadExitedFlag);
    OpcUa_Thread_Delete(&Thread);
    ASSERT(OpcUa_Null == Thread);
    END_CASE();

    /** Test 11007 */
    /*  OpcUa_Thread_WaitForShutdown */
    /*  Call WaitForShutdown with an OPCUA_INFINITE timeout. */
    CASE_ID(11007);
    ASSERT_GOOD(OpcUa_Thread_Create(    &Thread,
                                        SleepThenSetExitFlag,
                                        (OpcUa_Void*)4500));
    ASSERT(OpcUa_Null != Thread);
    g_uThreadExitedFlag = 0;

    ASSERT_GOOD(OpcUa_Thread_Start(Thread));
    ASSERT_GOOD(OpcUa_Thread_WaitForShutdown(Thread,OPCUA_INFINITE));
    ASSERT(1 == g_uThreadExitedFlag);
    OpcUa_Thread_Delete(&Thread);
    ASSERT(OpcUa_Null == Thread);
    END_CASE();

    /** Test 11008 */
    /*  OpcUa_Thread_WaitForShutdown */
    /*  Multiple threads all wait for the same thread to shutdown by calling WaitForShutdown */
    CASE_ID(11008);
    g_uThreadExitedFlag = 0;
    for (iIndex = 0; iIndex < 6; ++iIndex)
    {
        if (0 == iIndex)
        {
            /* create the thread, the other 5 are waiting for shutdown. */
            ASSERT_GOOD(OpcUa_Thread_Create(    &Threads[iIndex],
            SleepThenShutdown, /* Thread 0 is the thread whose shutdown will be waited on */
            OpcUa_Null));
            ASSERT(OpcUa_Null != Threads [iIndex]);
        }
        else
        {
            /* create the 5 threads, that are waiting on the first one. */
            ASSERT_GOOD(OpcUa_Thread_Create(    &Threads[iIndex],
                                                WaitForThreadShutdown, /* The rest of the threads will be waiting on Thread 0 */
                                                (OpcUa_Void*)Threads[0])); /* Pass thread to call WaitForShutdown on */
            ASSERT(OpcUa_Null != Threads [iIndex]);
        }

        /* start the previously created thread */
        ASSERT_GOOD(OpcUa_Thread_Start(Threads[iIndex]));
    }
    OpcUa_Thread_Sleep(sleepShutdownDelay + 1000);

    /* All threads didn't wake up and exit as expected when thread 0 exited */
    /* If this assert fails, there will be some threads that are blocked forever */
    ASSERT(6 == g_uThreadExitedFlag);
    for(iIndex = 0; iIndex < 6; ++iIndex)
    {
        OpcUa_Thread_WaitForShutdown(Threads[iIndex], OPCUA_INFINITE);
        OpcUa_Thread_Delete(&Threads[iIndex]);
        ASSERT(OpcUa_Null == Threads [iIndex]);
    }
    END_CASE();

    return OpcUa_Good;
}

#endif /* OPCUA_MULTITHREADED */
