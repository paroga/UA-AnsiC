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

#include "mutextest.h"
#include <unittest.h>
#include <opcua_core.h>

#ifndef _WIN32
# include <unistd.h>
#endif

#if OPCUA_MULTITHREADED

/* forward declarations of test cases */
OpcUa_StatusCode OpcUa_MutexTest_CreateDelete();
OpcUa_StatusCode OpcUa_MutexTest_LockUnlock();
/* TODO: add test cases here */

unsigned g_uMutexLockCount = 0;
unsigned g_uThreadResultCount = 0;

/** Test Entry Point */
void OpcUa_MutexTest_Run()
{
    OpcUa_MutexTest_CreateDelete();
    OpcUa_MutexTest_LockUnlock();
}

OpcUa_StatusCode OpcUa_MutexTest_CreateDelete()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_Mutex Mutex = OpcUa_Null;

    /** Test 7000 */
    /*  OpcUa_Mutex_Create / OpcUa_Mutex_Delete */
    /*  Create and Delete a mutex in normal usage */
    CASE_ID(7000);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(OpcUa_Mutex_Create(&Mutex));
    ASSERT(OpcUa_Null != Mutex);
    OpcUa_Mutex_Delete(&Mutex);
    MEM_SNAPSHOT(&s2);
    /* check memory */
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /** Test 7001 */
    /*  OpcUa_Mutex_Create */
    /*  Call Create with an OpcUa_Null pointer */
    CASE_ID(7001);
    MEM_SNAPSHOT(&s1);
    ASSERT_STATUS(OpcUa_Mutex_Create(OpcUa_Null), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    /* check memory */
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /** Test 7002 */
    /*  OpcUa_Mutex_Delete */
    /*  Call Delete with an OpcUa_Null pointer */
    CASE_ID(7002);
    /* This should not generate an exception */
    OpcUa_Mutex_Delete(OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

OpcUa_Void
LockMutex (
    OpcUa_Void* lpParameter
)
{
    OpcUa_Mutex Mutex = (OpcUa_Mutex)lpParameter;
    OpcUa_Mutex_Lock(Mutex);
    g_uThreadResultCount = g_uMutexLockCount;
    OpcUa_Mutex_Unlock(Mutex);
}

OpcUa_Void
UnownedMutex (
    OpcUa_Void* lpParameter
)
{
    OpcUa_Mutex Mutex = (OpcUa_Mutex)lpParameter;
    /* The unlock should fail so the thread will block when lock is */
    /* called.  When the main thread calls unlock on the mutex, this */
    /* thread will grab the mutex, make a copy of the current lock count */
    /* unlock the mutex, then return */
    OpcUa_Mutex_Unlock(Mutex);
    OpcUa_Mutex_Lock(Mutex);
    g_uThreadResultCount = g_uMutexLockCount;
    OpcUa_Mutex_Unlock(Mutex);
}

OpcUa_StatusCode OpcUa_MutexTest_LockUnlock()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_Mutex Mutex = OpcUa_Null;
    int iIndex;
    OpcUa_Thread hThread = NULL;

    /** Test 7003 */
    /*  OpcUa_Mutex_Lock / OpcUa_Mutex_Unlock */
    /*  Call Lock then Unlock from the same thread */
    CASE_ID(7003);
    ASSERT_GOOD(OpcUa_Mutex_Create(&Mutex));
    ASSERT(OpcUa_Null != Mutex);
    /* Thread should not get blocked on the mutex when Lock is called */
    OpcUa_Mutex_Lock(Mutex);
    OpcUa_Mutex_Unlock(Mutex);
    OpcUa_Mutex_Delete(&Mutex);
    END_CASE();

#if 0 /* Deactivating testcase, because desired behavior is not clear. This just should not be done. */
    /** Test 7004 */
    /*  OpcUa_Mutex_Lock / OpcUa_Mutex_Delete */
    /*  Delete a locked mutex */
    CASE_ID(7004);
    Mutex = OpcUa_Null;
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(OpcUa_Mutex_Create(&Mutex));
    ASSERT(OpcUa_Null != Mutex);
    OpcUa_Mutex_Lock(Mutex);
    OpcUa_Mutex_Delete(&Mutex);
    MEM_SNAPSHOT(&s2);
    /* check memory */
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();
#else
    /* unused */
    MEM_SNAPSHOT(&s1);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
#endif

    /** Test 7005 */
    /*  OpcUa_Mutex_Lock / OpcUa_Mutex_Unlock */
    /*  Thread B waits on mutex locked in Thread A */
    /*
        This test requires the use of threads.  OpcUa_Threads are not available for use
        since their unit tests run after the mutex's.  As a result, platform-specific threads
        will be used for this test.
    */
    CASE_ID(7005);
    Mutex = OpcUa_Null;
    ASSERT_GOOD(OpcUa_Mutex_Create(&Mutex));
    ASSERT(OpcUa_Null != Mutex);
    OpcUa_Mutex_Lock(Mutex);
    g_uMutexLockCount = 1;
    g_uThreadResultCount = 0xFFFFFFFF;
    ASSERT_GOOD(OpcUa_Thread_Create(&hThread, LockMutex, Mutex));
    ASSERT_GOOD(OpcUa_Thread_Start(hThread));
    /* The worker thread should block on the mutex created by main thread */
    /* and not be able to modify g_uThreadResultCount.  So the count should be 0xFFFFFFFF */
    ASSERT(0xFFFFFFFF == g_uThreadResultCount);
    --g_uMutexLockCount;
    OpcUa_Mutex_Unlock(Mutex);
    OpcUa_Thread_Sleep(100);
    ASSERT(0 == g_uThreadResultCount);
    OpcUa_Mutex_Delete(&Mutex);
    ASSERT_GOOD(OpcUa_Thread_WaitForShutdown(hThread, OPCUA_INFINITE));
    OpcUa_Thread_Delete(&hThread);
    END_CASE();

    /** Test 7006 */
    /*  OpcUa_Mutex_Lock / OpcUa_Mutex_Unlock */
    /*  Thread B waits on mutex locked multiple times in Thread A */
    /*
        This test requires the use of threads.  OpcUa_Threads are not available for use
        since their unit tests run after the mutex's.  As a result, platform-specific threads
        will be used for this test.
    */
    CASE_ID(7006);
    Mutex = OpcUa_Null;
    ASSERT_GOOD(OpcUa_Mutex_Create(&Mutex));
    ASSERT(OpcUa_Null != Mutex);
    for (iIndex = 0; iIndex < 5; ++iIndex)
    {
        OpcUa_Mutex_Lock(Mutex);
    }
    g_uMutexLockCount = 5;
    g_uThreadResultCount = 0xFFFFFFFF;
    ASSERT_GOOD(OpcUa_Thread_Create(&hThread, LockMutex, Mutex));
    ASSERT_GOOD(OpcUa_Thread_Start(hThread));
    OpcUa_Thread_Sleep(100);
    for (iIndex = 0; iIndex < 5; ++iIndex)
    {
        /* The worker thread should block on the mutex created by main thread */
        /* and not be able to modify g_uThreadResultCount until Unlock has been */
        /* called enough times.  So the count should be 0xFFFFFFFF */
        ASSERT(0xFFFFFFFF == g_uThreadResultCount);
        --g_uMutexLockCount;
        OpcUa_Mutex_Unlock(Mutex);
        OpcUa_Thread_Sleep(100);
    }
    ASSERT(0 == g_uThreadResultCount);
    OpcUa_Mutex_Delete(&Mutex);
    ASSERT_GOOD(OpcUa_Thread_WaitForShutdown(hThread, OPCUA_INFINITE));
    OpcUa_Thread_Delete(&hThread);
    END_CASE();

#ifdef windows_does_support_correct_mutex_implementation
   /** Test 7007 */
    /*  OpcUa_Mutex_Lock / OpcUa_Mutex_Unlock */
    /*  Thread B tries to unlock mutex locked in Thread A */
    /*
        This test requires the use of threads.  OpcUa_Threads are not available for use
        since their unit tests run after the mutex's.  As a result, platform-specific threads
        will be used for this test.
    */
    CASE_ID(7007);
    Mutex = OpcUa_Null;
    ASSERT_GOOD(OpcUa_Mutex_Create(&Mutex));
    ASSERT(OpcUa_Null != Mutex);
    OpcUa_Mutex_Lock(Mutex);
    g_uMutexLockCount = 1;
    g_uThreadResultCount = 0xFFFFFFFF;
    ASSERT_GOOD(OpcUa_Thread_Create(&hThread, UnownedMutex, Mutex));
    ASSERT_GOOD(OpcUa_Thread_Start(hThread));
    OpcUa_Thread_Sleep(100);
    /* If worker thread was able to unlock mutex created by main thread */
    /* g_uThreadResultCount would be 1.  It should not be able to do this */
    /* so the count should be 0xFFFFFFFF */
    ASSERT(0xFFFFFFFF == g_uThreadResultCount);
    --g_uMutexLockCount;
    OpcUa_Mutex_Unlock(Mutex);
    OpcUa_Thread_Sleep(100);
    ASSERT(0 == g_uThreadResultCount);
    OpcUa_Mutex_Delete(&Mutex);
    ASSERT_GOOD(OpcUa_Thread_WaitForShutdown(hThread, OPCUA_INFINITE));
    OpcUa_Thread_Delete(&hThread);
    END_CASE();
#endif /* windows_does_support_correct_mutex_implementation */

    return OpcUa_Good;
}

#endif /* OPCUA_MULTITHREADED */
