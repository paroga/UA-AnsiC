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

#ifdef _WIN32
#else
# include <unistd.h> /* usleep */
#endif

#include "semaphoretest.h"
#include <unittest.h>
#include <opcua_core.h>

#if OPCUA_MULTITHREADED

/* forward declarations of test cases */
OpcUa_StatusCode OpcUa_SemaphoreTest_CreateDelete();
OpcUa_StatusCode OpcUa_SemaphoreTest_Wait();
OpcUa_StatusCode OpcUa_SemaphoreTest_Post();
OpcUa_StatusCode OpcUa_SemaphoreTest_DeleteRace();
OpcUa_StatusCode OpcUa_SemaphoreTest_TimedWait();
/* TODO: add test cases here */

unsigned g_uSemaphoreTestCounter = 0;

/** Test Entry Point */
void OpcUa_SemaphoreTest_Run()
{
    INIT_LOCK;

    OpcUa_SemaphoreTest_CreateDelete();
    OpcUa_SemaphoreTest_Wait();
    OpcUa_SemaphoreTest_Post();
    OpcUa_SemaphoreTest_DeleteRace();
    OpcUa_SemaphoreTest_TimedWait();

    DELETE_LOCK;
}

OpcUa_StatusCode OpcUa_SemaphoreTest_CreateDelete()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_Semaphore Sem = OpcUa_Null;

    /** Test 8000 */
    /*  OpcUa_Semaphore_Create / OpcUa_Semaphore_Delete */
    /*  Create and Delete in normal usage */
    CASE_ID(8000);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(OpcUa_Semaphore_Create(&Sem, 0, OpcUa_Int32_Max));
    ASSERT(OpcUa_Null != Sem);
    OpcUa_Semaphore_Delete(&Sem);
    MEM_SNAPSHOT(&s2);
    /* check memory */
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /** Test 8001 */
    /*  OpcUa_Semaphore_Create */
    /*  Create with an out-of-range a_uMaxRange */
#if defined(_WIN32) && !defined(_ONTIME)
    CASE_ID(8001);
    Sem = OpcUa_Null;
    MEM_SNAPSHOT(&s1);
    ASSERT_STATUS(OpcUa_Semaphore_Create(&Sem, 0, OpcUa_UInt32_Max), OpcUa_BadInternalError);
    ASSERT(OpcUa_Null == Sem);
    MEM_SNAPSHOT(&s2);
    /* check memory */
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();
#endif
    /** Test 8002 */
    /*  OpcUa_Semaphore_Create */
    /*  Create with a zero a_uMaxRange */
    CASE_ID(8002);
    Sem = OpcUa_Null;
    MEM_SNAPSHOT(&s1);
    ASSERT_STATUS(OpcUa_Semaphore_Create(&Sem, 0, 0), OpcUa_BadInvalidArgument);
    ASSERT(OpcUa_Null == Sem);
    MEM_SNAPSHOT(&s2);
    /* check memory */
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /** Test 8003 */
    /*  OpcUa_Semaphore_Create */
    /*  Create with an a_uInitialValue larger than a_uMaxRange */
    CASE_ID(8003);
    Sem = OpcUa_Null;
    MEM_SNAPSHOT(&s1);
    ASSERT_STATUS(OpcUa_Semaphore_Create(&Sem, 11, 10), OpcUa_BadInvalidArgument);
    ASSERT(OpcUa_Null == Sem);
    MEM_SNAPSHOT(&s2);
    /* check memory */
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_SemaphoreTest_Wait()
{
    OpcUa_Semaphore Sem = OpcUa_Null;

    /** Test 8004 */
    /*  OpcUa_Semaphore_Wait */
    /*  Wait on a semaphore with a non-zero initial value */
    CASE_ID(8004);
    ASSERT_GOOD(OpcUa_Semaphore_Create(&Sem, 1, 10));
    ASSERT(OpcUa_Null != Sem);
    ASSERT_GOOD(OpcUa_Semaphore_Wait(Sem));
    OpcUa_Semaphore_Delete(&Sem);
    END_CASE();

    return OpcUa_Good;
}

OpcUa_Void
IncrementCounter (
    OpcUa_Void* lpParameter
)
{
    OpcUa_Semaphore Sem = (OpcUa_Semaphore*)lpParameter;
    OpcUa_Semaphore_Wait(Sem);
    SYNCHRONIZED(++g_uSemaphoreTestCounter);
}

OpcUa_StatusCode OpcUa_SemaphoreTest_Post()
{
    OpcUa_Semaphore Sem = OpcUa_Null;
    int iIndex;
    OpcUa_Thread hThreads[] = {OpcUa_Null, OpcUa_Null, OpcUa_Null, OpcUa_Null, OpcUa_Null};

    /** Test 8005 */
    /*  OpcUa_Semaphore_Post */
    /*  Post with a zero a_uReleaseCount */

    CASE_ID(8005);
    ASSERT_GOOD(OpcUa_Semaphore_Create(&Sem, 1, 10));
    ASSERT(OpcUa_Null != Sem);
    ASSERT_STATUS(OpcUa_Semaphore_Post(Sem, 0), OpcUa_BadInvalidArgument);
    OpcUa_Semaphore_Delete(&Sem);
    END_CASE();

    /** Test 8006 */
    /*  OpcUa_Semaphore_Post */
    /*  Post with an out-of-range a_uReleaseCount */
#if defined(_WIN32) && !defined(_ONTIME)
    CASE_ID(8006);
    Sem = OpcUa_Null;
    ASSERT_GOOD(OpcUa_Semaphore_Create(&Sem, 1, 10));
    ASSERT(OpcUa_Null != Sem);
    ASSERT_STATUS(OpcUa_Semaphore_Post(Sem, 10), OpcUa_BadTooManyPosts);
    OpcUa_Semaphore_Delete(&Sem);
    END_CASE();
#else
    /* this case makes no sense, there is no max range in POSIX */
#endif

    /** Test 8007 */
    /*  OpcUa_Semaphore_Post */
    /*  Post with semaphore count + a_uReleaseCount > a_uMaxRange */
#if defined(_WIN32) && !defined(_ONTIME)
    CASE_ID(8007);
    Sem = OpcUa_Null;
    ASSERT_GOOD(OpcUa_Semaphore_Create(&Sem, 9, 10));
    ASSERT(OpcUa_Null != Sem);
    /* erzeugt keinen Fehler: sondern neue Resourcen!!! */
    ASSERT_STATUS(OpcUa_Semaphore_Post(Sem, 2), OpcUa_BadTooManyPosts);
    OpcUa_Semaphore_Delete(&Sem);
    END_CASE();
#else
    /* this case makes no sense, there is no max range in POSIX */
#endif

    /** Test 8008 */
    /*  OpcUa_Semaphore_Post */
    /*  Post in normal usage */
    /*
        This test requires the use of threads.  OpcUa_Threads are not available for use
        since their unit tests run after the semaphore's.  As a result, platform-specific threads
        will be used for this test.
    */
    CASE_ID(8008);
    Sem = OpcUa_Null;
    ASSERT_GOOD(OpcUa_Semaphore_Create(&Sem, 0, 10));
    ASSERT(OpcUa_Null != Sem);
    g_uSemaphoreTestCounter = 0;

    /* Create system worker threads that wait on the semaphore */
    for (iIndex = 0; iIndex < 5; ++iIndex)
    {
        ASSERT_GOOD(OpcUa_Thread_Create(&hThreads[iIndex], IncrementCounter, Sem));
        ASSERT_GOOD(OpcUa_Thread_Start(hThreads[iIndex]));
    }
    ASSERT_GOOD(OpcUa_Semaphore_Post(Sem, 5));
    OpcUa_Thread_Sleep(1000);
    ASSERT(5 == g_uSemaphoreTestCounter);
    for (iIndex = 0; iIndex < 5; ++iIndex)
    {
        ASSERT_GOOD(OpcUa_Thread_WaitForShutdown(hThreads[iIndex], OPCUA_INFINITE));
        OpcUa_Thread_Delete(&hThreads[iIndex]);
    }
    OpcUa_Semaphore_Delete(&Sem);
    END_CASE();

    return OpcUa_Good;
}

OpcUa_Void
DelayedPost (
    OpcUa_Void* lpParameter
)
{
    OpcUa_Semaphore Sem = (OpcUa_Semaphore*)lpParameter;
    /* Give the main thread a chance to wait on the semaphore */
    OpcUa_Thread_Sleep(250);
    OpcUa_Semaphore_Post(Sem, 1);
}

OpcUa_StatusCode OpcUa_SemaphoreTest_DeleteRace()
{
    /** Test 8009 */
    /*  OpcUa_Semaphore_Delete */
    /*  Delete threading race condition */
    /*
        This test requires the use of threads.  OpcUa_Threads are not available for use
        since their unit tests run after the semaphore's.  As a result, platform-specific threads
        will be used for this test.
    */
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_Semaphore Sem = OpcUa_Null;
    OpcUa_Thread hThread = OpcUa_Null;

    CASE_ID(8009);
    MEM_SNAPSHOT(&s1);
    /* create sem with all resources occupied */
    ASSERT_GOOD(OpcUa_Semaphore_Create(&Sem, 0, 1)); /* 0 = number of free resources */
    ASSERT(OpcUa_Null != Sem);
    /* Create a low priority system worker thread that will set the semaphore */
    ASSERT_GOOD(OpcUa_Thread_Create(&hThread, DelayedPost, Sem));
    ASSERT_GOOD(OpcUa_Thread_Start(hThread));
    /* acquire one resource */
    ASSERT_GOOD(OpcUa_Semaphore_Wait(Sem));
    /* release the resource, before deleting */
    OpcUa_Semaphore_Post(Sem, 1);
    /* delete sem */
    OpcUa_Semaphore_Delete(&Sem);
    ASSERT_GOOD(OpcUa_Thread_WaitForShutdown(hThread, OPCUA_INFINITE));
    OpcUa_Thread_Delete(&hThread);
    /* Give the worker thread a chance to complete and release any memory it may have */
    OpcUa_Thread_Sleep(1);
    MEM_SNAPSHOT(&s2);
    /* check memory */
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_SemaphoreTest_TimedWait()
{
    OpcUa_Semaphore Sem = OpcUa_Null;
    OpcUa_Thread hThread = OpcUa_Null;

    /** Test 8010 */
    /*  OpcUa_Semaphore_TimedWait */
    /*  Normal TimedWait on a semaphore */
    /*
        This test requires the use of threads.  OpcUa_Threads are not available for use
        since their unit tests run after the semaphore's.  As a result, platform-specific threads
        will be used for this test.
    */
    CASE_ID(8010);
    /* create sem with all resources occupied */
    ASSERT_GOOD(OpcUa_Semaphore_Create(&Sem, 0, 1)); /* 0 = number of free resources */
    ASSERT(OpcUa_Null != Sem);
    /* Create a system worker thread that will set the semaphore */
    ASSERT_GOOD(OpcUa_Thread_Create(&hThread, DelayedPost, Sem));
    ASSERT_GOOD(OpcUa_Thread_Start(hThread));
    /* acquire one resource */
    ASSERT_GOOD(OpcUa_Semaphore_TimedWait(Sem, 1000));
    /* release the resource, before deleting */
    OpcUa_Semaphore_Post(Sem, 1);
    /* delete sem */
    OpcUa_Semaphore_Delete(&Sem);
    ASSERT_GOOD(OpcUa_Thread_WaitForShutdown(hThread, OPCUA_INFINITE));
    OpcUa_Thread_Delete(&hThread);
    END_CASE();

    /** Test 8011 */
    /*  OpcUa_Semaphore_TimedWait */
    /*  Timed out on a TimedWait */
    CASE_ID(8011);
    Sem = OpcUa_Null;
    ASSERT_GOOD(OpcUa_Semaphore_Create(&Sem, 0, 1));
    ASSERT(OpcUa_Null != Sem);
    ASSERT_STATUS(OpcUa_Semaphore_TimedWait(Sem, 300), OpcUa_GoodNonCriticalTimeout);
    /* release the resource, before deleting */
    OpcUa_Semaphore_Post(Sem, 1);
    /* delete sem */
    OpcUa_Semaphore_Delete(&Sem);
    END_CASE();

    return OpcUa_Good;
}

#endif /* OPCUA_MULTITHREADED */
