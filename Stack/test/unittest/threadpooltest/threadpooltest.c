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

#include "threadpooltest.h"
#include <unittest.h>
/* base */
#include <opcua.h>

/* core */
#include <opcua_core.h>
#include <opcua_mutex.h>
#include <opcua_semaphore.h>
#include <opcua_thread.h>
#include <opcua_list.h>

/* self */
#include <opcua_threadpool.h>

#ifndef _WIN32
# include <unistd.h>
#endif

#if OPCUA_MULTITHREADED

/* multiplier for OpcUa_ThreadPoolTest_SleepForHalfSecond */
#define OPCUA_THREADPOOLTEST_DELAY 2

#define sleepShutdownDelay 1000
#define IDBASE 12000

void OpcUa_ThreadPoolTest_SleepForHalfSecond (int iHowOften)
{
    OpcUa_Thread_Sleep(500 * iHowOften);
}

/* forward declarations of test cases */
OpcUa_StatusCode OpcUa_ThreadPoolTest_CreateDelete();
OpcUa_StatusCode OpcUa_ThreadPoolTest_AddJob();
OpcUa_StatusCode OpcUa_ThreadPoolTest_AddJobLimited();
/* TODO: add test cases here */

/** Test Entry Point */
void OpcUa_ThreadPoolTest_Run()
{
    INIT_LOCK;

    OpcUa_ThreadPoolTest_CreateDelete();
    OpcUa_ThreadPoolTest_AddJob();
    OpcUa_ThreadPoolTest_AddJobLimited();

    DELETE_LOCK;
}


OpcUa_StatusCode OpcUa_ThreadPoolTest_CreateDelete()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_ThreadPool ThreadPool = OpcUa_Null;

    CASE_ID (IDBASE);
    /* Create and delete a thread pool with a small number of static threads. */
    /* (uMinThreads = uMaxThreads) */
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_ThreadPool_Create (&ThreadPool, 5, 5, OPCUA_INFINITE, OpcUa_True, 0));
    ASSERT (OpcUa_Null != ThreadPool);
    OpcUa_ThreadPool_Delete (&ThreadPool);
    ASSERT (OpcUa_Null == ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

#if OPCUA_THREADPOOL_EXPANSION
    CASE_ID (IDBASE + 1);
    /* Create and delete a thread pool with a small number of static and dynamic threads. */
    /* (uMinThreads < uMaxThreads) */
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_ThreadPool_Create (&ThreadPool, 5, 10, OPCUA_INFINITE, OpcUa_True, 0));
    ASSERT (OpcUa_Null != ThreadPool);
    OpcUa_ThreadPool_Delete (&ThreadPool);
    ASSERT (OpcUa_Null == ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();
#endif /* OPCUA_THREADPOOL_EXPANSION */

#if OPCUA_THREADPOOL_EXPANSION
    CASE_ID (IDBASE + 2);
    /* Create and delete a thread pool with the maximum number of dynamic threads. */
    /* (uMinThreads = 1, uMaxThreads = OpcUa_UInt32_Max) */
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_ThreadPool_Create (&ThreadPool, 1, OpcUa_UInt32_Max, OPCUA_INFINITE, OpcUa_True, 0));
    ASSERT (OpcUa_Null != ThreadPool);
    OpcUa_ThreadPool_Delete (&ThreadPool);
    ASSERT (OpcUa_Null == ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();
#endif /* OPCUA_THREADPOOL_EXPANSION */

    CASE_ID (IDBASE + 3);
    /* Create with an invalid phThreadPool (phThreadPool = OpcUa_Null) */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_ThreadPool_Create (OpcUa_Null, 5, 5, OPCUA_INFINITE, OpcUa_True, 0), OpcUa_BadInvalidArgument);
    ASSERT (OpcUa_Null == ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 4);
    /* Create with uMinThreads == 0 */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_ThreadPool_Create (&ThreadPool, 0, 0, OPCUA_INFINITE, OpcUa_True, 0), OpcUa_BadInvalidArgument);
    ASSERT (OpcUa_Null == ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 5);
    /* Create with uMaxThreads < uMinThreads */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_ThreadPool_Create (&ThreadPool, 10, 1, OPCUA_INFINITE, OpcUa_True, 0), OpcUa_BadInvalidArgument);
    ASSERT (OpcUa_Null == ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 6);
    /* Delete with an invalid phThreadPool (phThreadPool = OpcUa_Null) */
    MEM_SNAPSHOT (&s1);
    OpcUa_ThreadPool_Delete (OpcUa_Null);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 7);
    /* Delete with an invalid phThreadPool (*phThreadPool = OpcUa_Null) */
    MEM_SNAPSHOT (&s1);
    ASSERT (OpcUa_Null == ThreadPool);
    OpcUa_ThreadPool_Delete (&ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}

OpcUa_Semaphore OpcUa_ThreadPoolTest_g_StaticSem;
OpcUa_Semaphore OpcUa_ThreadPoolTest_g_DynamicSem;

OpcUa_UInt32 OpcUa_ThreadPoolTest_g_JobCount;

OpcUa_Void OpcUa_ThreadPoolTest_JobFunc (OpcUa_Void* pArgument)
{
    OpcUa_Semaphore* pSem = (OpcUa_Semaphore*)pArgument;
    OpcUa_Semaphore_Wait (*pSem);
    SYNCHRONIZED(++OpcUa_ThreadPoolTest_g_JobCount);
}

OpcUa_StatusCode OpcUa_ThreadPoolTest_AddJob()
{
    int i;
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_ThreadPool ThreadPool = OpcUa_Null;

    /* Create semaphores that will be used in later tests */
    ASSERT_GOOD (OpcUa_Semaphore_Create (&OpcUa_ThreadPoolTest_g_StaticSem, 0, OpcUa_Int32_Max));
    ASSERT_GOOD (OpcUa_Semaphore_Create (&OpcUa_ThreadPoolTest_g_DynamicSem, 0, OpcUa_Int32_Max));

    CASE_ID (IDBASE + 8);
    /* AddJob with invalid phThreadPool (hThreadPool = OpcUa_Null) */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_ThreadPool_AddJob (OpcUa_Null, OpcUa_ThreadPoolTest_JobFunc, OpcUa_Null), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 9);
    /* AddJob with invalid pFunction (pFunction = OpcUa_Null) */
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_ThreadPool_Create (&ThreadPool, 5, 5, OPCUA_INFINITE, OpcUa_True, 0));
    ASSERT (OpcUa_Null != ThreadPool);
    ASSERT_STATUS (OpcUa_ThreadPool_AddJob (ThreadPool, OpcUa_Null, OpcUa_Null), OpcUa_BadInvalidArgument);
    OpcUa_ThreadPool_Delete (&ThreadPool);
    ASSERT (OpcUa_Null == ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 10);
    /* Multiple AddJobs so only static threads can handle them. */
    /* (Num Jobs <= uMinThreads) */
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_ThreadPool_Create (&ThreadPool, 5, 5, OPCUA_INFINITE, OpcUa_True, 0));
    ASSERT (OpcUa_Null != ThreadPool);
    OpcUa_ThreadPoolTest_g_JobCount = 0;
    for (i = 0; i < 5; ++i) {
        /* Queue up the jobs */
        ASSERT_GOOD (OpcUa_ThreadPool_AddJob (ThreadPool, OpcUa_ThreadPoolTest_JobFunc, &OpcUa_ThreadPoolTest_g_StaticSem));
    }
    /* Release all the worker threads */
    OpcUa_Semaphore_Post (OpcUa_ThreadPoolTest_g_StaticSem, 5);
    OpcUa_ThreadPoolTest_SleepForHalfSecond (OPCUA_THREADPOOLTEST_DELAY);
    ASSERT (5 == OpcUa_ThreadPoolTest_g_JobCount);
    while (OpcUa_Good == OpcUa_Semaphore_TimedWait (OpcUa_ThreadPoolTest_g_StaticSem, 0));    /* Drain the semaphore */
    OpcUa_ThreadPool_Delete (&ThreadPool);
    ASSERT (OpcUa_Null == ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

#if OPCUA_THREADPOOL_EXPANSION
    CASE_ID (IDBASE + 11);
    /* Multiple AddJobs so only dynamic threads can handle them. */
    /* (Block static threads, Num Dynamic Jobs <= uMaxThreads - uMinThreads) */
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_ThreadPool_Create (&ThreadPool, 1, 6, OPCUA_INFINITE, OpcUa_True, 0));
    ASSERT (OpcUa_Null != ThreadPool);
    OpcUa_ThreadPoolTest_g_JobCount = 0;
    /* Queue the static job */
    ASSERT_GOOD (OpcUa_ThreadPool_AddJob (ThreadPool, OpcUa_ThreadPoolTest_JobFunc, &OpcUa_ThreadPoolTest_g_StaticSem));
    for (i = 0; i < 5; ++i) {
        /* Queue up the dynamic jobs */
        ASSERT_GOOD (OpcUa_ThreadPool_AddJob (ThreadPool, OpcUa_ThreadPoolTest_JobFunc, &OpcUa_ThreadPoolTest_g_DynamicSem));
    }
    /* Release the dynamic threads */
    OpcUa_Semaphore_Post (OpcUa_ThreadPoolTest_g_DynamicSem, 5);
    OpcUa_ThreadPoolTest_SleepForHalfSecond ();
    ASSERT (5 == OpcUa_ThreadPoolTest_g_JobCount);
    /* Release the static thread */
    OpcUa_Semaphore_Post (OpcUa_ThreadPoolTest_g_StaticSem, 1);
    OpcUa_ThreadPoolTest_SleepForHalfSecond ();
    ASSERT (6 == OpcUa_ThreadPoolTest_g_JobCount);
    while (OpcUa_Good == OpcUa_Semaphore_TimedWait (OpcUa_ThreadPoolTest_g_StaticSem, OPCUA_INFINITE));    /* Drain the semaphore */
    while (OpcUa_Good == OpcUa_Semaphore_TimedWait (OpcUa_ThreadPoolTest_g_DynamicSem, OPCUA_INFINITE));    /* Drain the semaphore */
    OpcUa_ThreadPool_Delete (&ThreadPool);
    ASSERT (OpcUa_Null == ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();
#endif /* OPCUA_THREADPOOL_EXPANSION */

    CASE_ID (IDBASE + 12);
    /* Overloaded AddJobs so only static threads can handle them. */
    /* ((uMaxThreads = uMinThreads) < Num Jobs) */
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_ThreadPool_Create (&ThreadPool, 5, 5, OPCUA_INFINITE, OpcUa_True, 0));
    ASSERT (OpcUa_Null != ThreadPool);
    OpcUa_ThreadPoolTest_g_JobCount = 0;
    for (i = 0; i < 20; ++i) {
        /* Queue up the jobs */
        ASSERT_GOOD (OpcUa_ThreadPool_AddJob (ThreadPool, OpcUa_ThreadPoolTest_JobFunc, &OpcUa_ThreadPoolTest_g_StaticSem));
    }
    /* Release all the worker threads */
    OpcUa_Semaphore_Post (OpcUa_ThreadPoolTest_g_StaticSem, 20);
    OpcUa_ThreadPoolTest_SleepForHalfSecond (OPCUA_THREADPOOLTEST_DELAY);
    ASSERT (20 == OpcUa_ThreadPoolTest_g_JobCount);
    while (OpcUa_Good == OpcUa_Semaphore_TimedWait (OpcUa_ThreadPoolTest_g_StaticSem, 0)); /* Drain the semaphore */
    OpcUa_ThreadPool_Delete (&ThreadPool);
    ASSERT (OpcUa_Null == ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

#if OPCUA_THREADPOOL_EXPANSION
    CASE_ID (IDBASE + 13);
    /* Overloaded AddJobs so only dynamic threads can handle them. */
    /* (Block static threads, (uMaxThreads = uMinThreads) < Num Jobs) */
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_ThreadPool_Create (&ThreadPool, 1, 6, OPCUA_INFINITE, OpcUa_True, 0));
    ASSERT (OpcUa_Null != ThreadPool);
    OpcUa_ThreadPoolTest_g_JobCount = 0;
    /* Queue the static job */
    ASSERT_GOOD (OpcUa_ThreadPool_AddJob (ThreadPool, OpcUa_ThreadPoolTest_JobFunc, &OpcUa_ThreadPoolTest_g_StaticSem));
    for (i = 0; i < 20; ++i) {
        /* Queue up the dynamic jobs */
        ASSERT_GOOD (OpcUa_ThreadPool_AddJob (ThreadPool, OpcUa_ThreadPoolTest_JobFunc, &OpcUa_ThreadPoolTest_g_DynamicSem));
    }
    /* Release the dynamic threads */
    OpcUa_Semaphore_Post (OpcUa_ThreadPoolTest_g_DynamicSem, 20);
    OpcUa_ThreadPoolTest_SleepForHalfSecond ();
    ASSERT (20 == OpcUa_ThreadPoolTest_g_JobCount);
    /* Release the static thread */
    OpcUa_Semaphore_Post (OpcUa_ThreadPoolTest_g_StaticSem, 1);
    OpcUa_ThreadPoolTest_SleepForHalfSecond ();
    ASSERT (21 == OpcUa_ThreadPoolTest_g_JobCount);
    while (OpcUa_Good == OpcUa_Semaphore_TimedWait (OpcUa_ThreadPoolTest_g_StaticSem, 0));    /* Drain the semaphore */
    while (OpcUa_Good == OpcUa_Semaphore_TimedWait (OpcUa_ThreadPoolTest_g_DynamicSem, 0));    /* Drain the semaphore */
    OpcUa_ThreadPool_Delete (&ThreadPool);
    ASSERT (OpcUa_Null == ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();
#endif /* OPCUA_THREADPOOL_EXPANSION */

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_ThreadPoolTest_AddJobLimited()
{
    int i;
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_ThreadPool ThreadPool = OpcUa_Null;

    /* following tests work with a max job setting of 10, 5 static threads, no expansion and nonblocking. */
    CASE_ID (IDBASE + 14);
    /* Multiple AddJobs for static. */
    /* (Num Jobs <= uMinThreads) */
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_ThreadPool_Create (&ThreadPool, 5, 5, 10, OpcUa_False, 0));
    ASSERT (OpcUa_Null != ThreadPool);
    OpcUa_ThreadPoolTest_g_JobCount = 0;
    for (i = 0; i < 5; ++i) {
        /* Queue up the jobs */
        ASSERT_GOOD (OpcUa_ThreadPool_AddJob (ThreadPool, OpcUa_ThreadPoolTest_JobFunc, &OpcUa_ThreadPoolTest_g_StaticSem));
    }
    /* Release all the worker threads */
    OpcUa_Semaphore_Post (OpcUa_ThreadPoolTest_g_StaticSem, 5);
    OpcUa_ThreadPoolTest_SleepForHalfSecond (OPCUA_THREADPOOLTEST_DELAY);
    ASSERT (5 == OpcUa_ThreadPoolTest_g_JobCount);
    while (OpcUa_Good == OpcUa_Semaphore_TimedWait (OpcUa_ThreadPoolTest_g_StaticSem, 0));    /* Drain the semaphore */
    OpcUa_ThreadPool_Delete (&ThreadPool);
    ASSERT (OpcUa_Null == ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 15);
    /* Overloaded AddJobs for static threads. 10+ threads must be rejected with OpcUa_BadWouldBlock by the pool. */
    /* ((uMaxThreads = uMinThreads) < Num Jobs) */
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_ThreadPool_Create (&ThreadPool, 5, 5, 10, OpcUa_False, 0));
    ASSERT (OpcUa_Null != ThreadPool);
    OpcUa_ThreadPoolTest_g_JobCount = 0;
    for (i = 0; i < 10; ++i) {
        /* Queue up the jobs */
        ASSERT_GOOD (OpcUa_ThreadPool_AddJob (ThreadPool, OpcUa_ThreadPoolTest_JobFunc, &OpcUa_ThreadPoolTest_g_StaticSem));
    }
    for (i = 0; i < 10; ++i) {
        /* Queue up the jobs */
        ASSERT_STATUS (OpcUa_ThreadPool_AddJob (ThreadPool, OpcUa_ThreadPoolTest_JobFunc, &OpcUa_ThreadPoolTest_g_StaticSem), OpcUa_BadWouldBlock);
    }
    /* Release all the worker threads */
    OpcUa_Semaphore_Post (OpcUa_ThreadPoolTest_g_StaticSem, 20);
    OpcUa_ThreadPoolTest_SleepForHalfSecond (OPCUA_THREADPOOLTEST_DELAY);
    ASSERT (10 == OpcUa_ThreadPoolTest_g_JobCount);
    while (OpcUa_Good == OpcUa_Semaphore_TimedWait (OpcUa_ThreadPoolTest_g_StaticSem, 0)); /* Drain the semaphore */
    OpcUa_ThreadPool_Delete (&ThreadPool);
    ASSERT (OpcUa_Null == ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();



    /* following tests work with a max job setting of 10, 5 static threads, no expansion and blocking. */

    CASE_ID (IDBASE + 16);
    /* Multiple AddJobs for static. */
    /* (Num Jobs <= uMinThreads) */
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_ThreadPool_Create (&ThreadPool, 5, 5, 10, OpcUa_True, 0));
    ASSERT (OpcUa_Null != ThreadPool);
    OpcUa_ThreadPoolTest_g_JobCount = 0;
    for (i = 0; i < 5; ++i) {
        /* Queue up the jobs */
        ASSERT_GOOD (OpcUa_ThreadPool_AddJob (ThreadPool, OpcUa_ThreadPoolTest_JobFunc, &OpcUa_ThreadPoolTest_g_StaticSem));
    }
    /* Release all the worker threads */
    OpcUa_Semaphore_Post (OpcUa_ThreadPoolTest_g_StaticSem, 5);
    OpcUa_ThreadPoolTest_SleepForHalfSecond (OPCUA_THREADPOOLTEST_DELAY);
    ASSERT (5 == OpcUa_ThreadPoolTest_g_JobCount);
    while (OpcUa_Good == OpcUa_Semaphore_TimedWait (OpcUa_ThreadPoolTest_g_StaticSem, 0));    /* Drain the semaphore */
    OpcUa_ThreadPool_Delete (&ThreadPool);
    ASSERT (OpcUa_Null == ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 17);
    /* Overloaded AddJobs for static threads. 10+ threads must be accepted by blocking in the function call. */
    /* ((uMaxThreads = uMinThreads) < Num Jobs) */
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_ThreadPool_Create (&ThreadPool, 5, 5, 10, OpcUa_True, OPCUA_INFINITE));
    ASSERT (OpcUa_Null != ThreadPool);
    OpcUa_ThreadPoolTest_g_JobCount = 0;
    for (i = 0; i < 10; ++i) {
        /* Queue up the jobs */
        ASSERT_GOOD (OpcUa_ThreadPool_AddJob (ThreadPool, OpcUa_ThreadPoolTest_JobFunc, &OpcUa_ThreadPoolTest_g_StaticSem));
    }
    /* Release all the worker threads */
    OpcUa_Semaphore_Post (OpcUa_ThreadPoolTest_g_StaticSem, 10);
    for (i = 0; i < 10; ++i) {
        /* Queue up the jobs */
        ASSERT_GOOD (OpcUa_ThreadPool_AddJob (ThreadPool, OpcUa_ThreadPoolTest_JobFunc, &OpcUa_ThreadPoolTest_g_StaticSem));
    }
    /* Release all the worker threads */
    OpcUa_Semaphore_Post (OpcUa_ThreadPoolTest_g_StaticSem, 10);
    OpcUa_ThreadPoolTest_SleepForHalfSecond (OPCUA_THREADPOOLTEST_DELAY);
    ASSERT (20 == OpcUa_ThreadPoolTest_g_JobCount);
    while (OpcUa_Good == OpcUa_Semaphore_TimedWait (OpcUa_ThreadPoolTest_g_StaticSem, 0)); /* Drain the semaphore */
    OpcUa_ThreadPool_Delete (&ThreadPool);
    ASSERT (OpcUa_Null == ThreadPool);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}

#endif /* OPCUA_MULTITHREADED */
