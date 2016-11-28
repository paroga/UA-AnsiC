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

#ifndef __UNIT_TEST_H__
#define __UNIT_TEST_H__

#include <opcua_proxystub.h>
#include <opcua_trace.h>

/* global exit code which returns the number of failed tests. */
extern int g_nExitCode;
/* global test case id */
extern int g_idTestCase;
/* Log only errors to have a better overview. */
extern OpcUa_Boolean g_bLogOnlyErrors;

/* log entry function */
void logentry(char *szFormat, ...);

/* should test cases be canceled when one test fails. */
#define CANCEL_TESTCASE_ON_ERROR

#ifdef CANCEL_TESTCASE_ON_ERROR
#  define RETURN return OpcUa_Bad;
#else
#  define RETURN
#endif

/* set actual test case id */
#define CASE_ID(x) g_idTestCase = x;
#define END_CASE() if (!g_bLogOnlyErrors) { logentry("CASE %08i FINISHED.\n", g_idTestCase); }

/* test macros */
#define ASSERT(x) if (!(x)) { logentry("CASE %08i FAILED (%s, %i): %s\n", g_idTestCase, __FILE__, __LINE__, #x); g_nExitCode++; RETURN; }
#define ASSERT_GOOD(x) {OpcUa_StatusCode uTempStatus = OpcUa_Good; if (!OpcUa_IsGood(x)) { logentry("CASE %08i FAILED (%s, %i): %s; (Status:0x%08X)\n", g_idTestCase, __FILE__, __LINE__, #x, uTempStatus); g_nExitCode++; RETURN; }}
#define ASSERT_STATUS(x, status) {OpcUa_StatusCode uTempStatus = OpcUa_Good; if (((uTempStatus = x) & 0xffff0000) != status) { logentry("CASE %08i FAILED (%s, %i): %s; (Status:0x%08X)\n", g_idTestCase, __FILE__, __LINE__, #x, uTempStatus); g_nExitCode++; RETURN; }}
#define ASSERT_NULL(x) if (x != OpcUa_Null) { logentry("CASE %08i FAILED (%s, %i): %s\n", g_idTestCase, __FILE__, __LINE__, #x); g_nExitCode++; RETURN; }
#ifdef _WIN32
#define WAIT(x) Sleep(x)
#else
#define WAIT(x)
#endif

/* memory leak detection */
#ifdef _WIN32

#  define _CRTDBG_MAP_ALLOC
#  include <stdlib.h>
# ifdef _MSC_VER
#  include <crtdbg.h>
# endif /* _MSC_VER */
#  include <winsock2.h>
#  include <windows.h>
# if _DEBUG && defined(_MSC_VER)
#  define SET_BREAK_ALLOC(x) _CrtSetBreakAlloc(x)
#  define DUMP_MEMORY_LEAKS _CrtDumpMemoryLeaks()
#  define SNAPSHOT_TYPE _CrtMemState
#  define MEM_SNAPSHOT(s) _CrtMemCheckpoint(s)
#  define CHECK_SNAPSHOTS(diff, a, b) if (_CrtMemDifference(diff, a, b)) { logentry("CASE %08i MEMORY LEAK FOUND (%s, %i)\n", g_idTestCase, __FILE__, __LINE__); _CrtMemDumpStatistics(diff); }
# else /* _DEBUG */
#  define SET_BREAK_ALLOC(x)
#  define DUMP_MEMORY_LEAKS
#  define SNAPSHOT_TYPE OpcUa_Int
#  define MEM_SNAPSHOT(s) (void)(s)
#  define CHECK_SNAPSHOTS(diff, a, b) (void)(diff)
# endif /* _DEBUG */

#else /* _WIN32 */

#  define SET_BREAK_ALLOC(x)
#  define DUMP_MEMORY_LEAKS
#  define SNAPSHOT_TYPE int
#  define MEM_SNAPSHOT(s) (void)(s)
#  define CHECK_SNAPSHOTS(diff, a, b) (void)(diff)

#endif

/* test synchronisation */
#if OPCUA_USE_SYNCHRONISATION
# ifdef _WIN32
CRITICAL_SECTION cs;
#  define INIT_LOCK InitializeCriticalSection(&cs);
#  define DELETE_LOCK DeleteCriticalSection(&cs);
#  define LOCK EnterCriticalSection(&cs);
#  define UNLOCK LeaveCriticalSection(&cs);
#  define SYNCHRONIZED(x) EnterCriticalSection(&cs); x; LeaveCriticalSection(&cs)
# else /* _WIN32 */
#  include <pthread.h>
extern pthread_mutex_t cs;
#  define INIT_LOCK pthread_mutex_init(&cs, NULL)
#  define DELETE_LOCK pthread_mutex_destroy(&cs)
#  define LOCK pthread_mutex_lock(&cs)
#  define UNLOCK pthread_mutex_unlock(&cs)
#  define SYNCHRONIZED(x) LOCK; x; UNLOCK;
# endif
#else /* OPCUA_USE_SYNCHRONISATION */
#  define INIT_LOCK
#  define DELETE_LOCK
#  define LOCK
#  define UNLOCK
#  define SYNCHRONIZED(x) x;
#endif /* OPCUA_USE_SYNCHRONISATION */

#endif /* __UNIT_TEST_H__ */
