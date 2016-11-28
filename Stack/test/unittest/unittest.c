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

#include "unittest.h"

#ifdef _WIN32
# pragma warning(disable:4996) /* safe_functions */
# include <windows.h>
# include <stdio.h>
# include <time.h>
# include <stdarg.h>
#else
# include <time.h>
# include <string.h>
#if OPCUA_USE_SYNCHRONISATION
pthread_mutex_t cs = PTHREAD_MUTEX_INITIALIZER;
#endif /* OPCUA_USE_SYNCHRONISATION */
#endif

/* TESTS */
#include <guidtest.h>
#include <mutextest.h>
#include <threadtest.h>
#include <semaphoretest.h>
#include <sockettest.h>
#include <memorytest.h>
#include <listtest.h>
#include <utilitiestest.h>
#include <guidtest.h>
#include <datetimetest.h>
#include <stringtest.h>
#include <threadpooltest.h>
#include <timertest.h>
#include <buffertest.h>
#include <securitytest.h>

OpcUa_Handle g_pPortLayerHandle;
OpcUa_ProxyStubConfiguration g_pProxyStubConfiguration;

/* system function prototypes */
#if 0
int  printf( const char *Format, ... );
int  getchar( void );
int  vprintf( const char *Format, va_list ArgList );
#endif

/** Global exit code. Returns the number of failed cases. */
int g_nExitCode = 0;
/** Global test case id. Holds the actual test case number. */
int g_idTestCase = 0;
/** Log only errors to have a better overview. */
OpcUa_Boolean g_bLogOnlyErrors = OpcUa_False;
/** Trace timestamps for each log entry. */
OpcUa_Boolean g_bLogTimeStamps = OpcUa_False;

/** Main test method of unit test */
OpcUa_StatusCode run()
{
    OpcUa_StatusCode uStatus;

    /* set alloc number to debug memory leaks */
    SET_BREAK_ALLOC( 0 );

    g_pProxyStubConfiguration.uProxyStub_Trace_Level                = OPCUA_TRACE_OUTPUT_LEVEL_WARNING;
    g_pProxyStubConfiguration.iSerializer_MaxAlloc                  = -1;
    g_pProxyStubConfiguration.iSerializer_MaxStringLength           = -1;
    g_pProxyStubConfiguration.iSerializer_MaxByteStringLength       = -1;
    g_pProxyStubConfiguration.iSerializer_MaxArrayLength            = -1;
    g_pProxyStubConfiguration.iSerializer_MaxMessageSize            = -1;
    g_pProxyStubConfiguration.iSerializer_MaxRecursionDepth         = -1;
    g_pProxyStubConfiguration.bSecureListener_ThreadPool_Enabled    = OpcUa_False;
    g_pProxyStubConfiguration.iSecureListener_ThreadPool_MinThreads = -1;
    g_pProxyStubConfiguration.iSecureListener_ThreadPool_MaxThreads = -1;
    g_pProxyStubConfiguration.iSecureListener_ThreadPool_MaxJobs    = -1;
    g_pProxyStubConfiguration.bSecureListener_ThreadPool_BlockOnAdd = OpcUa_True;
    g_pProxyStubConfiguration.uSecureListener_ThreadPool_Timeout    = OPCUA_INFINITE;
    g_pProxyStubConfiguration.iTcpListener_DefaultChunkSize         = -1;
    g_pProxyStubConfiguration.iTcpConnection_DefaultChunkSize       = -1;
    g_pProxyStubConfiguration.iTcpTransport_MaxMessageLength        = -1;
    g_pProxyStubConfiguration.iTcpTransport_MaxChunkCount           = -1;
    g_pProxyStubConfiguration.bTcpListener_ClientThreadsEnabled     = OpcUa_False;
    g_pProxyStubConfiguration.bTcpStream_ExpectWriteToBlock         = OpcUa_False;

#if !OPCUA_USE_STATIC_PLATFORM_INTERFACE

    uStatus = OpcUa_P_Initialize( &g_pPortLayerHandle );
    OpcUa_ReturnErrorIfBad( uStatus );

    uStatus = OpcUa_ProxyStub_Initialize( g_pPortLayerHandle,
                                          &g_pProxyStubConfiguration);
#else

    uStatus = OpcUa_P_Initialize();
    OpcUa_ReturnErrorIfBad( uStatus );

    uStatus = OpcUa_ProxyStub_Initialize(&g_pProxyStubConfiguration);

#endif /* !OPCUA_USE_STATIC_PLATFORM_INTERFACE */

    OpcUa_ReturnErrorIfBad( uStatus );

    /*****************************************************************************/
    /* START OF TESTS                                                            */

    /* run all tests */

#if 01
    OpcUa_SemaphoreTest_Run();
#endif
#if 01
    OpcUa_MutexTest_Run();
#endif
#if 01
    OpcUa_ThreadTest_Run();
#endif
#if 01
    OpcUa_ThreadPoolTest_Run();
#endif
#if 01
    OpcUa_BufferTest_Run();
#endif
#if 01
    OpcUa_MemoryTest_Run();
#endif
#if 01
    OpcUa_GuidTest_Run();
#endif
#if 01
    OpcUa_DateTimeTest_Run();
#endif
#if 01
    OpcUa_ListTest_Run();
#endif
#if 01
    OpcUa_UtilitiesTest_Run();
#endif
#if 0
    OpcUa_TimerTest_Run();
#endif
#if 01
    OpcUa_StringTest_Run();
#endif
#if 0
    OpcUa_SecurityTest_Run();
#endif
#if 01
    OpcUa_SocketTest_Run();
#endif

/* END OF TESTS                                                              */
/*****************************************************************************/

#if OPCUA_TRACE_ENABLE
/*  OpcUa_Trace_Clear(); */
#endif

    OpcUa_ProxyStub_Clear();

#if !OPCUA_USE_STATIC_PLATFORM_INTERFACE
    OpcUa_P_Clean( &g_pPortLayerHandle );
#else
    OpcUa_P_Clean();
#endif /* !OPCUA_USE_STATIC_PLATFORM_INTERFACE */

    /* All memory should be cleared now if no test FAILED. */
    DUMP_MEMORY_LEAKS;

    return OpcUa_Good;
}

/** prints usage of unit test */
void usage()
{
    printf( "OPC UA ANSI C Stack Unit Test V1.0\n"
#ifdef _WIN32
            "Usage: unitest.exe [--help|--batch|--log-only-errors|--log-timestamps]\n"
#else
            "Usage: ./unitest [--help|--batch|--log-only-errors|--log-timestamps]\n"
#endif
            "\n"
            "Examples:\n"
            "  unittest --help\n"
            "  unittest --batch\n"
            "\n"
            " Main operation mode:\n"
            "\n"
            "  If all tests have PASSED the unitest returns exit code 0,\n"
            "  otherwise the number of FAILED test cases.\n"
            "  By default unittest doesn't exit after test execution and waits for user input to exit.\n"
            "\n"
            " Options:\n"
            "  --help             prints this message\n"
            "  --batch            disables waiting after test execution to continue batch processing\n"
            "  --log-only-errors  disables writing success messages, so that errors don't get lost\n"
            "  --log-timestamps   writes a timestamp of each log entry\n"
            "  --test             specifies what test to run. Use --test help to get a list of available test cases.\n"
            "\n"
            "Have a nice day.\n" );
}

/** application's main entry point */
int main( int argc, char *argv[] )
{
    OpcUa_Boolean bWaitWhenFinished = OpcUa_True;
    time_t t;
    struct tm *ts;
    int i = 0;

#ifdef MSVC
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    /*_CrtSetBreakAlloc(2499);*/
#endif

    /* simple command line parsing */
    while ( ++i < argc )
    {
        if ( strcmp( argv[i], "--help" ) == 0 )
        {
            usage();
            return 0;
        }
        else if ( strcmp( argv[i], "--batch" ) == 0 )
        {
            bWaitWhenFinished = OpcUa_False;
        }
        else if ( strcmp( argv[i], "--log-only-errors" ) == 0 )
        {
            g_bLogOnlyErrors = OpcUa_True;
        }
        else if ( strcmp( argv[i], "--log-timestamps" ) == 0 )
        {
            g_bLogTimeStamps = OpcUa_True;
        }
    }

    /* output unit test header */
    t = time( NULL );
    ts = localtime( &t );
    printf( "OPC Ua Unit Test:\n" );
    printf( "Time: %02i:%02i:%02i\n", ts->tm_hour, ts->tm_min, ts->tm_sec );
    printf( "Date: %04i-%02i-%02i\n", ts->tm_year + 1900, ts->tm_mon + 1, ts->tm_mday );
    printf( "\n" );

    printf("Starting test...\n");
    /* run the test */
    run();
    printf("Finished test.\n");

    /* check result */
    if ( g_nExitCode == 0 )
    {
        printf( "All tests PASSED.\n" );
    }
    else
    {
        printf( "%i test case(s) FAILED.\n", g_nExitCode );
    }

    /* wait before exit, that's nice for debugging */
    if ( bWaitWhenFinished )
    {
        printf( "Press Enter to exit.\n" );
        getchar();
    }

    /* return exit code for batch processing */
    return g_nExitCode;
}

void logentry( char *szFormat, ... )
{
    va_list list;
    time_t t;
    struct tm *ts;

    if ( g_bLogTimeStamps )
    {
        /* TODO: change to gettimeofday to get also msec (windows doesn't support that -> use opc portability layer) */
        t = time( NULL );
        ts = localtime( &t );
        printf( "%02i:%02i:%02i ", ts->tm_hour, ts->tm_min, ts->tm_sec );
    }

    va_start( list, szFormat );
    vprintf( szFormat, list );
    va_end( list );
}
