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
/*#  include <conio.h>*/
#elif defined(__linux__)
#  include <stdlib.h>
#  include <sys/select.h>
#endif

#include "sockettest.h"
#include "unittest.h"
#include <opcua_core.h>
#include <opcua_p_socket_interface.h>
#include <opcua_p_datetime.h>
#include <opcua_p_memory.h>
#include <opcua_memory.h>
#include <opcua_semaphore.h>
#include <opcua_platformdefs.h>

#include <opcua_p_socket.h>

#if !OPCUA_MULTITHREADED
/* this test requires internal socket structure in single thread configuration */
#include <opcua_p_socket_internal.h>
#endif

typedef struct _OpcUa_SocketTest_CallbackInfo
{
    OpcUa_UInt16        caseid;
    OpcUa_Void*         data;
    OpcUa_StatusCode    uStatus;
    OpcUa_Socket        hClientSocket;
    OpcUa_Socket        hServerSocket;
} OpcUa_SocketTest_CallbackInfo;

/* forward declarations of test cases */
OpcUa_StatusCode OpcUa_SocketTest_TestCreate();
OpcUa_StatusCode OpcUa_SocketTest_TestCreateClient();
OpcUa_StatusCode OpcUa_SocketTest_TestCreateServer();
OpcUa_StatusCode OpcUa_SocketTest_TestWrite();
/* TODO: add test cases here */

/* Test cases used definitions */
#define OPCUA_INVALID_SOCKET_FLAG                       8

/* HINT: To test TLS transport, the protocol identifier must be set to https! */
#define OPCUA_SOCKETTEST_TLS_SERVER_URL                 "https://127.0.0.1:3840"
#define OPCUA_SOCKETTEST_TLS_SERVER_URL_WITH_WRONG_PORT "https://127.0.0.1:5000"

/* TODO: - Query OPCUA_P_SOCKETMANAGER_SUPPORT_SSL
         - Add tests for - FAIL: connecting with plain client to TLS server.
                         - FAIL: connecting with TLS to plain servers.
                         - OK:   every test should be run once with each URL.
*/

#define OPCUA_SOCKETTEST_SERVER_URL                     "opc.tcp://127.0.0.1:3840"
#define OPCUA_SOCKETTEST_SERVER_URL_WITH_WRONG_PORT     "opc.tcp://127.0.0.1:5000"
#define OPCUA_SOCKETTEST_BUFFER_SIZE                    1024
#define OPCUA_SOCKETTEST_SEM_TIMEOUT                    5000
#define OPCUA_SOCKETTEST_NUMBEROFSOCKETSFORSERVER       10
#define OPCUA_SOCKETTEST_NUMBEROFSOCKETSFORCLIENT       3

/* Global variables. */
OpcUa_Byte*     OpcUa_SocketTest_g_1kByteBuffer         = OpcUa_Null;
OpcUa_Byte*     OpcUa_SocketTest_g_1MByteBuffer         = OpcUa_Null;
OpcUa_Byte*     OpcUa_SocketTest_g_ReceiveBuffer        = OpcUa_Null;
OpcUa_UInt32    OpcUa_SocketTest_g_uTotalBytesSent      = 0;
OpcUa_UInt32    OpcUa_SocketTest_g_uTotalBytesReceived  = 0;
OpcUa_Byte*     OpcUa_SocketTest_g_pByte                = OpcUa_Null;

#if !OPCUA_MULTITHREADED
typedef struct _OpcUa_SocketTest_Event
{
    OpcUa_UInt32    uiCurrent;
    OpcUa_UInt32    uiMax;
} OpcUa_SocketTest_Event;
#endif /* !OPCUA_MULTITHREADED */

/** Helper OpcUa_SocketTest_CreateEvent */
OpcUa_StatusCode OpcUa_SocketTest_CreateEvent(  OpcUa_Handle*   a_phEvent,
                                                OpcUa_UInt32    a_uInitalValue,
                                                OpcUa_UInt32    a_uMaxRange)
{
#if OPCUA_MULTITHREADED

    return OpcUa_Semaphore_Create(a_phEvent, (OpcUa_UInt32)a_uInitalValue, (OpcUa_UInt32)a_uMaxRange);

#else /* OPCUA_MULTITHREADED */

    OpcUa_SocketTest_Event* pEvent = (OpcUa_SocketTest_Event*)OpcUa_Null;

    OpcUa_ReturnErrorIfArgumentNull(a_phEvent);
    *a_phEvent = OpcUa_Null;

    pEvent = (OpcUa_SocketTest_Event*)OpcUa_Alloc(sizeof(OpcUa_SocketTest_Event));
    OpcUa_ReturnErrorIfAllocFailed(pEvent);

    pEvent->uiMax       = a_uMaxRange;
    pEvent->uiCurrent   = a_uInitalValue;

    *a_phEvent = pEvent;

    return OpcUa_Good;

#endif /* OPCUA_MULTITHREADED */
}

/** Helper OpcUa_SocketTest_WaitForEvent */
OpcUa_Void OpcUa_SocketTest_DeleteEvent(  OpcUa_Handle*   a_phEvent)
{
#if OPCUA_MULTITHREADED
    OpcUa_Semaphore_Delete(a_phEvent);
#else /* OPCUA_MULTITHREADED */

    if(a_phEvent == OpcUa_Null)
    {
        return;
    }

    if(*a_phEvent == OpcUa_Null)
    {
        return;
    }

    OpcUa_Free(*a_phEvent);
    *a_phEvent = OpcUa_Null;

#endif /* OPCUA_MULTITHREADED */
}

/** Helper OpcUa_SocketTest_WaitForEvent */
OpcUa_StatusCode OpcUa_SocketTest_WaitForEvent( OpcUa_Handle    a_hEvent,
                                                OpcUa_UInt32    a_msecTimeout)
{
#if OPCUA_MULTITHREADED

    return OpcUa_Semaphore_TimedWait(a_hEvent, a_msecTimeout);

#else /* OPCUA_MULTITHREADED */

    OpcUa_SocketTest_Event* pEvent          = (OpcUa_SocketTest_Event*)a_hEvent;
    OpcUa_TimeVal           Start;
    OpcUa_TimeVal           End;
    OpcUa_UInt32            TotalWaitTime   = (OpcUa_UInt32)a_msecTimeout;
    OpcUa_UInt32            WaitTime        = TotalWaitTime;
    OpcUa_UInt32            Diff;
    OpcUa_UInt32            DiffMS;
    OpcUa_StatusCode        uStatus         = OpcUa_Good;

    OpcUa_ReturnErrorIfArgumentNull(a_hEvent);

    OpcUa_DateTime_GetTimeOfDay(&Start);

    OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "OpcUa_SocketTest_WaitForEvent: Wait start!\n");

    /* wait as long as time is left and no resource is available */
    /* in other words: wait until time is over or a resource is available */
    while(WaitTime > 0 && pEvent->uiCurrent == 0)
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

    if(WaitTime > 0)
    {
        pEvent->uiCurrent--;
        OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "OpcUa_SocketTest_WaitForEvent: Wait success!\n");
    }
    else
    {
        /* timeout */
        OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "OpcUa_SocketTest_WaitForEvent: Wait timeout!\n");
        uStatus = OpcUa_GoodNonCriticalTimeout;
    }

    return uStatus;

#endif /* OPCUA_MULTITHREADED */
}

/** Helper OpcUa_SocketTest_TriggerEvent */
OpcUa_StatusCode OpcUa_SocketTest_TriggerEvent( OpcUa_Handle    a_hEvent,
                                                OpcUa_UInt32    a_uiCount)
{
#if OPCUA_MULTITHREADED

    return OpcUa_Semaphore_Post(    (OpcUa_Semaphore)a_hEvent,
                                    a_uiCount);

#else /* OPCUA_MULTITHREADED */

    OpcUa_SocketTest_Event* pEvent = (OpcUa_SocketTest_Event*)a_hEvent;

    OpcUa_ReturnErrorIfArgumentNull(a_hEvent);

    /* check if requested amount of resources to release is available */
    if(pEvent->uiMax - pEvent->uiCurrent >= a_uiCount)
    {
        /* release the given number of resources */
        pEvent->uiCurrent += a_uiCount;
        return OpcUa_Good;
    }
    else
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "OpcUa_SocketTest_TriggerEvent: Invalid call!\n");
        return OpcUa_Bad;
    }

#endif /* OPCUA_MULTITHREADED */
}

/** Helper OpcUa_SocketTest_Wait */
OpcUa_Void OpcUa_SocketTest_Wait(OpcUa_UInt32   a_msecToWait)
{
#if OPCUA_MULTITHREADED

    OpcUa_Thread_Sleep(a_msecToWait);

#else /* OPCUA_MULTITHREADED */

    OpcUa_TimeVal   Start;
    OpcUa_TimeVal   End;
    OpcUa_Int32     TotalWaitTime   = (OpcUa_Int32)a_msecToWait;
    OpcUa_Int32     WaitTime        = TotalWaitTime;
    OpcUa_Int32     Diff;
    OpcUa_Int32     DiffMS;

    OpcUa_DateTime_GetTimeOfDay(&Start);

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

    return;

#endif /* OPCUA_MULTITHREADED */
}

/** Callback functions */
OpcUa_StatusCode Client_SocketCallback(OpcUa_Socket pSocket, OpcUa_UInt32 SocketEvent, OpcUa_Void* pUserData, OpcUa_UInt16 PortNumber, OpcUa_Boolean IsSSL)
{
    OpcUa_StringA                   strEvent    = OpcUa_Null;
    OpcUa_SocketTest_CallbackInfo*  cbinfo      = (OpcUa_SocketTest_CallbackInfo*) pUserData;
    OpcUa_StatusCode                uStatus     = OpcUa_Good;

    OpcUa_ReferenceParameter(IsSSL);
    OpcUa_ReferenceParameter(PortNumber);
    OpcUa_ReferenceParameter(pSocket);

    switch(SocketEvent)
    {
    case OPCUA_SOCKET_NO_EVENT:
        {
            strEvent = "NO_EVENT";
            break;
        }
    case OPCUA_SOCKET_READ_EVENT:
        {
            strEvent = "READ_EVENT";
            break;
        }
    case OPCUA_SOCKET_WRITE_EVENT:
        {
            strEvent = "WRITE_EVENT";
            break;
        }
    case OPCUA_SOCKET_EXCEPT_EVENT:
        {
            strEvent = "EXCEPT_EVENT";
            break;
        }
    case OPCUA_SOCKET_TIMEOUT_EVENT:
        {
            strEvent = "TIMEOUT_EVENT";
            break;
        }
    case OPCUA_SOCKET_CLOSE_EVENT:
        {
            strEvent = "CLOSE_EVENT";
            break;
        }
    case OPCUA_SOCKET_CONNECT_EVENT:
        {
            strEvent = "CONNECT_EVENT";
            break;
        }
    case OPCUA_SOCKET_ACCEPT_EVENT:
        {
            strEvent = "ACCEPT_EVENT";
            break;
        }
    case OPCUA_SOCKET_SHUTDOWN_EVENT:
        {
            strEvent = "SHUTDOWN_EVENT";
            break;
        }
    case OPCUA_SOCKET_NEED_BUFFER_EVENT:
        {
            strEvent = "NEED_BUFFER_EVENT";
            break;
        }
    case OPCUA_SOCKET_FREE_BUFFER_EVENT:
        {
            strEvent = "FREE_BUFFER_EVENT";
            break;
        }
    default:
        {
            strEvent = "ERROR DEFAULT!";
            break;
        }
    }

    if (cbinfo != OpcUa_Null)
    {
        OpcUa_UInt16    caseid          = 0;
        OpcUa_Void*     data            = OpcUa_Null;
        OpcUa_Socket    client_socket   = OpcUa_Null;

        SYNCHRONIZED(caseid         = cbinfo->caseid;
                     data           = cbinfo->data;
                     client_socket  = cbinfo->hClientSocket);
        (void)client_socket;

        switch (caseid)
        {
        case 9008:
        case 9009:
            {
                OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "Client_SocketCallback: \"%s\" for case %u, socket %p.\n", strEvent, caseid, pSocket);

                switch(SocketEvent)
                {
                case OPCUA_SOCKET_CONNECT_EVENT:
                    {
                        /* expected result */
                        OpcUa_Handle* phEvent = data;
                        cbinfo->uStatus = OpcUa_Good;
                        OpcUa_SocketTest_TriggerEvent(*phEvent, (OpcUa_UInt32) 1);
                        break;
                    }
                case OPCUA_SOCKET_READ_EVENT:
                    {
                        /* process data */
                        OpcUa_StatusCode uStatus = OpcUa_Good;
                        OpcUa_UInt32 uBytesRead = 0;
                        OpcUa_Byte pBuffer[1024];

                        uStatus = OpcUa_Socket_Read(pSocket, pBuffer, 1024, &uBytesRead);

                        if(OpcUa_IsBad(uStatus))
                        {
                            OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Client_SocketCallback: Socket closed.\n");
                            OpcUa_Socket_Close(pSocket);
                        }

                        break;
                    }
                case OPCUA_SOCKET_EXCEPT_EVENT:
                    {
                        /* unexpected result; connect failed */
                        OpcUa_Handle* phEvent = data;
                        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Client_SocketCallback: Unexpected! Connect failed!.\n");
                        cbinfo->uStatus = OpcUa_BadCommunicationError;
                        OpcUa_SocketTest_TriggerEvent(*phEvent, (OpcUa_UInt32) 1);
                        break;
                    }
                default:
                    {
                        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Client_SocketCallback: WARNING! Unhandled event type.\n");
                    }
                }

                break;
            }
        case 9010:
            {
                OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "Client_SocketCallback: \"%s\" for case %u.\n", strEvent, caseid);

                switch(SocketEvent)
                {
                case OPCUA_SOCKET_EXCEPT_EVENT:
                    {
                        /* expected result */
                        OpcUa_Handle hEvent = *((OpcUa_Handle*) data);
                        OpcUa_SocketTest_TriggerEvent(hEvent, (OpcUa_UInt32) 1);
                        break;
                    }
                case OPCUA_SOCKET_CONNECT_EVENT:
                    {
                        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Client_SocketCallback: WARNING! Unexpected server available? Test result not valid.\n");
                        break;
                    }
                case OPCUA_SOCKET_READ_EVENT:
                    {
                        /* process data */
                        OpcUa_StatusCode uStatus = OpcUa_Good;
                        OpcUa_UInt32 uBytesRead = 0;
                        OpcUa_Byte pBuffer[1024];

                        uStatus = OpcUa_Socket_Read(pSocket, pBuffer, 1024, &uBytesRead);

                        if(OpcUa_IsBad(uStatus))
                        {
                            OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Client_SocketCallback: Case 9010. Read 0 bytes. Connection closed.\n");
                            OpcUa_Socket_Close(pSocket);
                        }
                        break;
                    }
                default:
                    {
                        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Client_SocketCallback: WARNING! Unhandled event type.\n");
                    }
                }

                break;
            }
        case 9021:
        case 9022:
            {
                OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "Client_SocketCallback: \"%s\" for case %u.\n", strEvent, caseid);

                switch(SocketEvent)
                {
                case OPCUA_SOCKET_CONNECT_EVENT:
                    {
                        /* ignored */
                        break;
                    }
#if !OPCUA_MULTITHREADED
                case OPCUA_SOCKET_WRITE_EVENT:
                    {
                        /* socket writable - send remaining data if necessary */
                        OpcUa_Int32     result  = 0;
                        OpcUa_UInt32    uiSize  = 0;
                        OpcUa_Byte*     pData   = OpcUa_Null;
                        OpcUa_UInt32    uiTotal = 0;

                        switch(caseid)
                        {
                            case 9021:
                                {
                                    pData   = &OpcUa_SocketTest_g_1kByteBuffer[OpcUa_SocketTest_g_uTotalBytesSent];
                                    uiTotal = OPCUA_SOCKETTEST_BUFFER_SIZE;
                                    break;
                                }
                            case 9022:
                                {
                                    pData   = &OpcUa_SocketTest_g_1MByteBuffer[OpcUa_SocketTest_g_uTotalBytesSent];
                                    uiTotal = 1024 * OPCUA_SOCKETTEST_BUFFER_SIZE;
                                    break;
                                }
                        }

                        uiSize  = uiTotal - OpcUa_SocketTest_g_uTotalBytesSent;

                        if(OpcUa_SocketTest_g_uTotalBytesSent < uiTotal)
                        {
                            result = OpcUa_Socket_Write(client_socket, pData, uiSize, OpcUa_False);

                            if(result != OPCUA_SOCKET_ERROR)
                            {
                                if(result < (OpcUa_Int32)uiSize)
                                {
                                    uStatus = OpcUa_GoodCallAgain;
                                    OpcUa_SocketTest_g_uTotalBytesSent += result;
                                }
                                else
                                {
                                    OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "All data sent!\n");
                                }
                            }
                            else
                            {
                                /* interrupt connection */
                                OpcUa_Socket_Close(client_socket);
                            }
                        }
                        break;
                    }
#endif /* OPCUA_MULTITHREADED */
                default:
                    {
                        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Client_SocketCallback: WARNING! Unhandled event type.\n");
                    }
                }
                break;
            }
        default:
            {
                if(SocketEvent == OPCUA_SOCKET_READ_EVENT)
                {
                    /* process data */
                    OpcUa_StatusCode uStatus = OpcUa_Good;
                    OpcUa_UInt32 uBytesRead = 0;
                    OpcUa_Byte pBuffer[1024];

                    uStatus = OpcUa_Socket_Read(pSocket, pBuffer, 1024, &uBytesRead);

                    if(OpcUa_IsBad(uStatus))
                    {
                        OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "Client_SocketCallback: Socket closed.\n");
                        OpcUa_Socket_Close(pSocket);
                    }
                }
            }
        }
    }
    else
    {
        /* OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "Client_SocketCallback: \"%s\" with empty cbinfo.\n", strEvent); */
        switch(SocketEvent)
        {
        case OPCUA_SOCKET_READ_EVENT:
            {
                /* process data */
                OpcUa_StatusCode uStatus = OpcUa_Good;
                OpcUa_UInt32 uBytesRead = 0;
                OpcUa_Byte pBuffer[1024];

                uStatus = OpcUa_Socket_Read(pSocket, pBuffer, 1024, &uBytesRead);

                if(OpcUa_IsBad(uStatus))
                {
                    OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "Client_SocketCallback: Socket closed.\n");
                    OpcUa_Socket_Close(pSocket);
                }
                break;
            }
        default:
            {
            }
        }
    }

    return uStatus;
}

OpcUa_StatusCode Server_SocketCallback(OpcUa_Socket pSocket, OpcUa_UInt32 SocketEvent, OpcUa_Void* pUserData, OpcUa_UInt16 PortNumber, OpcUa_Boolean IsSSL)
{
    OpcUa_StringA                   strEvent    = OpcUa_Null;
    OpcUa_SocketTest_CallbackInfo*  cbinfo      = (OpcUa_SocketTest_CallbackInfo*) pUserData;

    OpcUa_ReferenceParameter(PortNumber);
    OpcUa_ReferenceParameter(IsSSL);

    switch(SocketEvent)
    {
    case OPCUA_SOCKET_NO_EVENT:
        {
            strEvent = "NO_EVENT";
            break;
        }
    case OPCUA_SOCKET_READ_EVENT:
        {
            strEvent = "READ_EVENT";
            break;
        }
    case OPCUA_SOCKET_WRITE_EVENT:
        {
            strEvent = "WRITE_EVENT";
            break;
        }
    case OPCUA_SOCKET_EXCEPT_EVENT:
        {
            strEvent = "EXCEPT_EVENT";
            break;
        }
    case OPCUA_SOCKET_TIMEOUT_EVENT:
        {
            strEvent = "TIMEOUT_EVENT";
            break;
        }
    case OPCUA_SOCKET_CLOSE_EVENT:
        {
            strEvent = "CLOSE_EVENT";
            break;
        }
    case OPCUA_SOCKET_CONNECT_EVENT:
        {
            strEvent = "CONNECT_EVENT";
            break;
        }
    case OPCUA_SOCKET_ACCEPT_EVENT:
        {
            strEvent = "ACCEPT_EVENT";
            break;
        }
    case OPCUA_SOCKET_SHUTDOWN_EVENT:
        {
            strEvent = "SHUTDOWN_EVENT";
            break;
        }
    case OPCUA_SOCKET_NEED_BUFFER_EVENT:
        {
            strEvent = "NEED_BUFFER_EVENT";
            break;
        }
    case OPCUA_SOCKET_FREE_BUFFER_EVENT:
        {
            strEvent = "FREE_BUFFER_EVENT";
            break;
        }
    default:
        {
            strEvent = "ERROR DEFAULT!";
            break;
        }
    }

    if (cbinfo != OpcUa_Null)
    {
        OpcUa_UInt16    caseid  = 0;
        OpcUa_Void*     data    = OpcUa_Null;

        SYNCHRONIZED(caseid = cbinfo->caseid;
                     data   = cbinfo->data);

        switch (caseid)
        {
        case 9016:
            {
                OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "Server_SocketCallback: \"%s\" for case %u.\n", strEvent, caseid);

                switch(SocketEvent)
                {
                case OPCUA_SOCKET_ACCEPT_EVENT:
                {
                    OpcUa_Handle hEvent = *((OpcUa_Handle*) data);
                    OpcUa_SocketTest_TriggerEvent(hEvent, (OpcUa_UInt32) 1);
                    break;
                }
                case OPCUA_SOCKET_READ_EVENT:
                    {
                        /* process data */
                        OpcUa_StatusCode uStatus = OpcUa_Good;
                        OpcUa_UInt32 uBytesRead = 0;
                        OpcUa_Byte pBuffer[1024];

                        uStatus = OpcUa_Socket_Read(pSocket, pBuffer, 1024, &uBytesRead);

                        if(OpcUa_IsBad(uStatus))
                        {
                            OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "Server_SocketCallback: Socket closed.\n");
                            OpcUa_Socket_Close(pSocket);
                        }
                        break;
                    }
                default:
                    {
                        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Server_SocketCallback: WARNING! Unhandled event type.\n");
                    }
                }
                break;
            }
        case 9021:
            {
                OpcUa_Handle hEvent     = *((OpcUa_Handle*) data);
                OpcUa_StatusCode status = OpcUa_Good;
                OpcUa_UInt32 bytesread  = 0;

                OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "Server_SocketCallback: \"%s\" for case %u.\n", strEvent, caseid);

                switch(SocketEvent)
                {
                case OPCUA_SOCKET_READ_EVENT:
                    {
                        if(OpcUa_SocketTest_g_uTotalBytesReceived == 0)
                        {
                            OpcUa_SocketTest_g_pByte = OpcUa_SocketTest_g_ReceiveBuffer;
                        }

                        status = OpcUa_Socket_Read( pSocket, OpcUa_SocketTest_g_pByte, OPCUA_SOCKETTEST_BUFFER_SIZE, &bytesread);

                        if(OpcUa_IsGood(status))
                        {
                            OpcUa_SocketTest_g_uTotalBytesReceived += bytesread;

                            if(OpcUa_SocketTest_g_uTotalBytesReceived < OpcUa_SocketTest_g_uTotalBytesSent)
                            {
                                OpcUa_SocketTest_g_pByte += bytesread;
                            }
                            else
                            {
                                if(OpcUa_MemCmp(OpcUa_SocketTest_g_ReceiveBuffer, OpcUa_SocketTest_g_1kByteBuffer, OpcUa_SocketTest_g_uTotalBytesSent) == 0)
                                {
                                    /* expected result */
                                    OpcUa_SocketTest_TriggerEvent(hEvent, (OpcUa_UInt32) 1);
                                }
                                else
                                {
                                    OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Data corrupt!\n");
                                }
                            }
                        }
                        else
                        {
                            OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "Server_SocketCallback: Socket closed.\n");
                            OpcUa_Socket_Close(pSocket);
                        }

                        break;
                    }
                case OPCUA_SOCKET_ACCEPT_EVENT:
                    {
                        /* ignored */
                        OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "Accept.\n");
                        break;
                    }
                }
                break;
            }
        case 9022:
            {
                OpcUa_Handle hEvent     = *((OpcUa_Handle*) data);
                OpcUa_StatusCode status = OpcUa_Good;
                OpcUa_UInt32 bytesread  = 0;

                switch(SocketEvent)
                {
                case OPCUA_SOCKET_READ_EVENT:
                    {
                        if(OpcUa_SocketTest_g_uTotalBytesReceived == 0)
                        {
                            OpcUa_SocketTest_g_pByte = OpcUa_SocketTest_g_ReceiveBuffer;
                        }
                        status = OpcUa_Socket_Read(pSocket, OpcUa_SocketTest_g_pByte, 1024 * OPCUA_SOCKETTEST_BUFFER_SIZE, &bytesread);
                        if (OpcUa_IsGood(status))
                        {
                            OpcUa_SocketTest_g_uTotalBytesReceived += bytesread;
                            if(OpcUa_SocketTest_g_uTotalBytesReceived < OpcUa_SocketTest_g_uTotalBytesSent)
                            {
                                OpcUa_SocketTest_g_pByte += bytesread;
                            }
                            else
                            {
                                if (OpcUa_MemCmp(OpcUa_SocketTest_g_ReceiveBuffer, OpcUa_SocketTest_g_1MByteBuffer, OpcUa_SocketTest_g_uTotalBytesSent) == 0)
                                {
                                    OpcUa_SocketTest_TriggerEvent(hEvent, (OpcUa_UInt32) 1);
                                }
                                else
                                {
                                    OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Data corrupt!\n");
                                }
                            }
                        }
                        break;
                    }
                case OPCUA_SOCKET_ACCEPT_EVENT:
                    {
                        /* ignored */
                        OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "Accept.\n");
                        break;
                    }
                }
                break;
            }
        default:
            {
                switch(SocketEvent)
                {
                case OPCUA_SOCKET_READ_EVENT:
                    {
                        /* unhandled read event */
                        OpcUa_UInt32 bytesread  = 0;
                        OpcUa_Byte pBuffer[1024] = {0};
                        OpcUa_StatusCode status = OpcUa_Good;
                        status = OpcUa_Socket_Read(pSocket, pBuffer, 1024, &bytesread);
                        if(status == OpcUa_BadDisconnect)
                        {
                            /* some client disconnected */
                        }
                        else if(OpcUa_IsBad(status))
                        {
                            exit(-1);
                        }
                        while(bytesread == OPCUA_SOCKETTEST_BUFFER_SIZE)
                        {
                            bytesread = 0;
                            status = OpcUa_Socket_Read(pSocket, pBuffer, 1024, &bytesread);
                            if(OpcUa_IsBad(status))
                            {
                                exit(-1);
                            }
                        }
                        break;
                    }
                case OPCUA_SOCKET_ACCEPT_EVENT:
                    {
                        break;
                    }
                default:
                    {
                        exit(-1);
                    }
                }
            }
        }
    }
    else
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "Server_SocketCallback: \"%s\" with empty cbinfo.\n", strEvent);
        switch(SocketEvent)
        {
        case OPCUA_SOCKET_READ_EVENT:
            {
                /* process data */
                OpcUa_StatusCode uStatus = OpcUa_Good;
                OpcUa_UInt32 uBytesRead = 0;
                OpcUa_Byte pBuffer[1024];

                uStatus = OpcUa_Socket_Read(pSocket, pBuffer, 1024, &uBytesRead);

                if(OpcUa_IsBad(uStatus))
                {
                    OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "Server_SocketCallback: Socket closed.\n");
                    OpcUa_Socket_Close(pSocket);
                }
                break;
            }
        default:
            {
            }
        }
    }

    return OpcUa_Good;
}

/** Test Entry Point */
void OpcUa_SocketTest_Run()
{
    INIT_LOCK;

    OpcUa_SocketTest_TestCreate();
    OpcUa_SocketTest_TestCreateClient();
    OpcUa_SocketTest_TestCreateServer();
    OpcUa_SocketTest_TestWrite();

    DELETE_LOCK;
    /* TODO: add test cases here */
}

/** Test OpcUa_SocketManager_Create */
OpcUa_StatusCode OpcUa_SocketTest_TestCreate()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_SocketManager manager = OpcUa_Null;
    OpcUa_StatusCode status     = OpcUa_Good;

    /* CASE 9000 */
    /* Good case */
    /* Create socket manager, delete it, check for memory leaks. */
    CASE_ID(9000);
    MEM_SNAPSHOT(&s1);
    status = OpcUa_SocketManager_Create(&manager, OPCUA_SOCKETTEST_NUMBEROFSOCKETSFORSERVER, OPCUA_SOCKET_NO_FLAG);
    ASSERT_GOOD(status);
    if (OpcUa_IsGood(status))
    {
        OpcUa_SocketManager_Delete(&manager);
        ASSERT_NULL(manager);
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9001 */
    /* Bad case */
    /* Request more sockets than allowed by the socket manager. */
    CASE_ID(9001);
    MEM_SNAPSHOT(&s1);
    status = OpcUa_SocketManager_Create(&manager, OPCUA_P_SOCKETMANAGER_NUMBEROFSOCKETS + 1, OPCUA_SOCKET_NO_FLAG);
    ASSERT_STATUS(status, OpcUa_BadInvalidArgument);
    if (OpcUa_IsGood(status))
    {
        OpcUa_SocketManager_Delete(&manager);
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9002 */
    /* Good case */
    /* Request 0 sockets. */
    CASE_ID(9002);
    MEM_SNAPSHOT(&s1);
    status = OpcUa_SocketManager_Create(&manager, 0, OPCUA_SOCKET_NO_FLAG);
    ASSERT_GOOD(status);
    if (OpcUa_IsGood(status))
    {
        OpcUa_SocketManager_Delete(&manager);
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9003 */
    /* Bad case */
    /* Pass invalid socket flag. */
    CASE_ID(9003);
    MEM_SNAPSHOT(&s1);
    status = OpcUa_SocketManager_Create(&manager, OPCUA_SOCKETTEST_NUMBEROFSOCKETSFORSERVER, OPCUA_INVALID_SOCKET_FLAG);
    ASSERT_STATUS(status, OpcUa_BadInvalidArgument);
    if (OpcUa_IsGood(status))
    {
        OpcUa_SocketManager_Delete(&manager);
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}

/** Test OpcUa_SocketManager_CreateClient*/
OpcUa_StatusCode OpcUa_SocketTest_TestCreateClient()
{
    SNAPSHOT_TYPE s1, s2, s3;

#if OPCUA_MULTITHREADED
    OpcUa_SocketManager             clientSocketManager         = OpcUa_Null;
    OpcUa_SocketManager             serverSocketManager         = OpcUa_Null;
#endif /* OPCUA_MULTITHREADED */
    OpcUa_SocketManager             tempManager                 = OpcUa_Null;
    OpcUa_Socket                    client_socket               = OpcUa_Null;
    OpcUa_Socket                    server_socket               = OpcUa_Null;
    OpcUa_Handle                    hEvent                      = OpcUa_Null;
    OpcUa_StatusCode                uStatus                     = OpcUa_Good;
    OpcUa_Int                       i                           = 0;
    OpcUa_Socket                    socketArray[OPCUA_P_SOCKETMANAGER_NUMBEROFSOCKETS];
    OpcUa_StatusCode                statusArray[OPCUA_P_SOCKETMANAGER_NUMBEROFSOCKETS];
    OpcUa_SocketTest_CallbackInfo   cbinfo8;
    OpcUa_SocketTest_CallbackInfo   cbinfo10;

#if OPCUA_MULTITHREADED
    /* create the socket managers for the client and server sockets */
    uStatus = OpcUa_SocketManager_Create(
        &clientSocketManager,
        OPCUA_SOCKETTEST_NUMBEROFSOCKETSFORCLIENT,
        OPCUA_SOCKET_NO_FLAG);

    if(!OpcUa_IsGood(uStatus))
    {
        logentry("Preparation for OpcUa_SocketTest_TestCreateClient FAILED. Could not create client socketmanager -> 0x%08X. Aborting test run.\n", uStatus);
        return uStatus;
    }

    uStatus = OpcUa_SocketManager_Create(
        &serverSocketManager,
        OPCUA_SOCKETTEST_NUMBEROFSOCKETSFORSERVER,
        OPCUA_SOCKET_NO_FLAG);

    if(!OpcUa_IsGood(uStatus))
    {
        logentry("Preparation for OpcUa_SocketTest_TestCreateClient FAILED. Could not create server socketmanager -> 0x%08X. Aborting test run.\n", uStatus);
        return uStatus;
    }

#endif /* OPCUA_MULTITHREADED */

    /* create the server socket used by the connect tests */
    uStatus = OpcUa_SocketManager_CreateServer(
#if OPCUA_MULTITHREADED
        serverSocketManager,
#else /* OPCUA_MULTITHREADED */
        OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
        OPCUA_SOCKETTEST_SERVER_URL,
        OpcUa_True,
        Server_SocketCallback,
        (OpcUa_Void*)OpcUa_Null,
        &server_socket);

    if(!OpcUa_IsGood(uStatus))
    {
        logentry("Preparation for OpcUa_SocketTest_TestCreateClient FAILED. Could not create server socket -> 0x%08X. Aborting test run.\n", uStatus);
        return uStatus;
    }

    /* give server some time to start up - waiting for open event would be better ... */
    OpcUa_SocketTest_Wait(500);

    /* create a semaphore used for fetching connect events and time outs. */
    OpcUa_SocketTest_CreateEvent(&hEvent, (OpcUa_UInt32) 0, (OpcUa_UInt32) 1);

    /* CASE 9004 */
    /* Bad case */
    /* Pass null pointer for storing created socket. */
    CASE_ID(9004);
    MEM_SNAPSHOT(&s1);
    statusArray[0] = OpcUa_SocketManager_CreateClient(
#if OPCUA_MULTITHREADED
        clientSocketManager,
#else /* OPCUA_MULTITHREADED */
        OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
        OPCUA_SOCKETTEST_SERVER_URL,
        (OpcUa_UInt16)0,
        Client_SocketCallback,
        (OpcUa_Void*)OpcUa_Null,
        (OpcUa_Socket*)OpcUa_Null);

    ASSERT_STATUS(statusArray[0], OpcUa_BadInvalidArgument);
    if (OpcUa_IsGood(statusArray[0]))
    {
        OpcUa_Socket_Close(client_socket);
        client_socket = OpcUa_Null;
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9005 */
    /* Bad case */
    /* Pass invalid remote address. */
    CASE_ID(9005);
    MEM_SNAPSHOT(&s1);
    statusArray[0] = OpcUa_SocketManager_CreateClient(
#if OPCUA_MULTITHREADED
        clientSocketManager,
#else /* OPCUA_MULTITHREADED */
        OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
        (OpcUa_StringA)OpcUa_Null,
        (OpcUa_UInt16)0,
        Client_SocketCallback,
        (OpcUa_Void*)OpcUa_Null,
        &client_socket);

    ASSERT_STATUS(statusArray[0], OpcUa_BadInvalidArgument);
    if (OpcUa_IsGood(statusArray[0]))
    {
        OpcUa_Socket_Close(client_socket);
        client_socket = OpcUa_Null;
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9006 */
    /* Good case */
    /* Pass null pointer to callback function. */
    CASE_ID(9006);
    MEM_SNAPSHOT(&s1);
    statusArray[0] = OpcUa_SocketManager_CreateClient(
#if OPCUA_MULTITHREADED
        clientSocketManager,
#else /* OPCUA_MULTITHREADED */
        OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
        OPCUA_SOCKETTEST_SERVER_URL,
        (OpcUa_UInt16)0,
        (OpcUa_Socket_EventCallback)OpcUa_Null,
        (OpcUa_Void*)OpcUa_Null,
        &client_socket);

    ASSERT_GOOD(statusArray[0]);
    if (OpcUa_IsGood(statusArray[0]))
    {
        OpcUa_Socket_Close(client_socket);
        client_socket = OpcUa_Null;
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9007 */
    /* Good case */
    /* Create socket, close socket, check for memory leaks. */
    CASE_ID(9007);
    MEM_SNAPSHOT(&s1);
    statusArray[0] = OpcUa_SocketManager_CreateClient(
#if OPCUA_MULTITHREADED
        clientSocketManager,
#else /* OPCUA_MULTITHREADED */
        OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
        OPCUA_SOCKETTEST_SERVER_URL,
        (OpcUa_UInt16)0,
        Client_SocketCallback,
        (OpcUa_Void*)OpcUa_Null,
        &client_socket);

    ASSERT_GOOD(statusArray[0]);
    if (OpcUa_IsGood(statusArray[0]))
    {
        OpcUa_Socket_Close(client_socket);
        client_socket = OpcUa_Null;
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9008 */
    /* Good case */
    /* Create client socket, wait for OPCUA_SOCKET_CONNECT_EVENT. */
    CASE_ID(9008);
    MEM_SNAPSHOT(&s1);
    SYNCHRONIZED(cbinfo8.caseid  = 9008;
                 cbinfo8.data    = (OpcUa_Void*) &hEvent;
                 cbinfo8.uStatus = OpcUa_BadWaitingForResponse);

    statusArray[0] = OpcUa_SocketManager_CreateClient(
#if OPCUA_MULTITHREADED
        clientSocketManager,
#else /* OPCUA_MULTITHREADED */
        OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
        OPCUA_SOCKETTEST_SERVER_URL,
        (OpcUa_UInt16)0,
        Client_SocketCallback,
        (OpcUa_Void*)&cbinfo8,
        &client_socket);

    ASSERT_GOOD(statusArray[0]);
    if (OpcUa_IsGood(statusArray[0]))
    {
        OpcUa_StatusCode status = OpcUa_Good;
        status = OpcUa_SocketTest_WaitForEvent(hEvent, OPCUA_INFINITE);
        ASSERT_STATUS(status, OpcUa_Good);
        ASSERT_STATUS(cbinfo8.uStatus, OpcUa_Good);
        OpcUa_Socket_Close(client_socket);
        client_socket = OpcUa_Null;
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9010 */
    /* Bad case */
    /* Connect to the wrong port. Check for error code or wait for OPCUA_SOCKET_EXCEPT_EVENT. */
    CASE_ID(9010);
    MEM_SNAPSHOT(&s1);
    SYNCHRONIZED(cbinfo10.caseid = 9010;
                 cbinfo10.data   = (OpcUa_Void*) &hEvent);

    statusArray[0] = OpcUa_SocketManager_CreateClient(
#if OPCUA_MULTITHREADED
        clientSocketManager,
#else /* OPCUA_MULTITHREADED */
        OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
        OPCUA_SOCKETTEST_SERVER_URL_WITH_WRONG_PORT,
        (OpcUa_UInt16)0,
        Client_SocketCallback,
        (OpcUa_Void*)&cbinfo10,
        &client_socket);

    if (OpcUa_IsGood(statusArray[0]))
    {
        OpcUa_StatusCode status = OpcUa_Good;
        status = OpcUa_SocketTest_WaitForEvent(hEvent, OPCUA_SOCKETTEST_SEM_TIMEOUT);
        ASSERT_STATUS(status, OpcUa_Good);
        OpcUa_Socket_Close(client_socket);
        client_socket = OpcUa_Null;
    }
    else
    {
        ASSERT_STATUS(statusArray[0], OpcUa_BadCommunicationError);
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9011 */
    /* Bad case */
    /* Create more sockets than allowed by the socket manager. */
    CASE_ID(9011);
    MEM_SNAPSHOT(&s1);

    /* create a socket manager, that is able to host 5 sockets */
    ASSERT_GOOD(OpcUa_SocketManager_Create(&tempManager, (OpcUa_UInt32)5, OPCUA_SOCKET_NO_FLAG));

    /* create the 5 valid (client) sockets */
    for (i = 0; i < 5; i++)
    {
        statusArray[i] = OpcUa_SocketManager_CreateClient(  tempManager,
                                                            OPCUA_SOCKETTEST_SERVER_URL,
                                                            0,
                                                            Client_SocketCallback,
                                                            (OpcUa_Void*)OpcUa_Null,
                                                            &socketArray[i]);
        ASSERT_GOOD(statusArray[i]);
    }

    /* create an additional sixth socket, that should not be able to be created by the socket manager. */
    statusArray[5] = OpcUa_SocketManager_CreateClient(  tempManager,
                                                        OPCUA_SOCKETTEST_SERVER_URL,
                                                        (OpcUa_UInt16)0,
                                                        Client_SocketCallback,
                                                        (OpcUa_Void*)OpcUa_Null,
                                                        &client_socket);
    ASSERT_STATUS(statusArray[5], OpcUa_BadMaxConnectionsReached);

    /* close all sockets (the sixth one only if it was created successfully), and delete the socket manager. */
    for (i = 0; i < 5; i++)
    {
        if (OpcUa_IsGood(statusArray[i]))
        {
            OpcUa_Socket_Close(socketArray[i]);
            socketArray[i] = OpcUa_Null;
        }
    }
    if (OpcUa_IsGood(statusArray[5]))
    {
        OpcUa_Socket_Close(client_socket);
        client_socket = OpcUa_Null;
    }
    OpcUa_SocketManager_Delete(&tempManager);
    tempManager = OpcUa_Null;
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    OpcUa_Socket_Close(server_socket);

#if OPCUA_MULTITHREADED
    OpcUa_SocketManager_Delete(&clientSocketManager);
    OpcUa_SocketManager_Delete(&serverSocketManager);
#endif

    OpcUa_SocketTest_DeleteEvent(&hEvent);

    return OpcUa_Good;
}

/** Test OpcUa_SocketManager_CreateServer*/
OpcUa_StatusCode OpcUa_SocketTest_TestCreateServer()
{
    SNAPSHOT_TYPE s1, s2, s3;
#if OPCUA_MULTITHREADED
    OpcUa_SocketManager             serverSocketManager = OpcUa_Null;
    OpcUa_SocketManager             clientSocketManager = OpcUa_Null;
#endif /* OPCUA_MULTITHREADED */
    OpcUa_SocketTest_CallbackInfo   cbinfo16;
    OpcUa_Handle                    hEvent = OpcUa_Null;
    OpcUa_StatusCode                statusArray[OPCUA_P_SOCKETMANAGER_NUMBEROFSOCKETS];
    OpcUa_Socket                    socketArray[OPCUA_P_SOCKETMANAGER_NUMBEROFSOCKETS];

#if OPCUA_MULTITHREADED
    OpcUa_SocketManager_Create(&serverSocketManager, OPCUA_SOCKETTEST_NUMBEROFSOCKETSFORSERVER, OPCUA_SOCKET_NO_FLAG);
    OpcUa_SocketManager_Create(&clientSocketManager, OPCUA_SOCKETTEST_NUMBEROFSOCKETSFORCLIENT, OPCUA_SOCKET_NO_FLAG);
#endif /* OPCUA_MULTITHREADED */

    OpcUa_SocketTest_CreateEvent(&hEvent, (OpcUa_UInt32) 0, (OpcUa_UInt32) 1);

    /* CASE 9013 */
    /* Bad case */
    /* Pass null pointer for storing created socket. */
    CASE_ID(9013);
    MEM_SNAPSHOT(&s1);
    statusArray[0] = OpcUa_SocketManager_CreateServer(
#if OPCUA_MULTITHREADED
        serverSocketManager,
#else /* OPCUA_MULTITHREADED */
        OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
        OPCUA_SOCKETTEST_SERVER_URL,
        OpcUa_True,
        Server_SocketCallback,
        (OpcUa_Void*)OpcUa_Null,
        (OpcUa_Socket*)OpcUa_Null);

    ASSERT_STATUS(statusArray[0], OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9014 */
    /* Good case */
    /* Pass a null pointer to the callback function. */
    CASE_ID(9014);
    MEM_SNAPSHOT(&s1);
    statusArray[0] = OpcUa_SocketManager_CreateServer(
#if OPCUA_MULTITHREADED
        serverSocketManager,
#else /* OPCUA_MULTITHREADED */
        OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
        OPCUA_SOCKETTEST_SERVER_URL,
        OpcUa_True,
        (OpcUa_Socket_EventCallback)OpcUa_Null,
        (OpcUa_Void*)OpcUa_Null,
        &socketArray[0]);

    ASSERT_GOOD(statusArray[0]);
    if (OpcUa_IsGood(statusArray[0]))
    {
        OpcUa_Socket_Close(socketArray[0]);
        socketArray[0] = OpcUa_Null;
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9015 */
    /* Good case */
    /* Create socket, close it, check for memory leaks. */
    CASE_ID(9015);
    MEM_SNAPSHOT(&s1);
    statusArray[0] = OpcUa_SocketManager_CreateServer(
#if OPCUA_MULTITHREADED
        serverSocketManager,
#else /* OPCUA_MULTITHREADED */
        OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
        OPCUA_SOCKETTEST_SERVER_URL,
        OpcUa_True,
        Server_SocketCallback,
        (OpcUa_Void*)OpcUa_Null,
        &socketArray[0]);

    ASSERT_GOOD(statusArray[0]);
    /* give server some time to start up - waiting for open event would be better ... */
    OpcUa_SocketTest_Wait(500);
    if (OpcUa_IsGood(statusArray[0]))
    {
        OpcUa_Socket_Close(socketArray[0]);
        socketArray[0] = OpcUa_Null;
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9016 */
    /* Good case */
    /* Create server socket, connect a client socket to it, wait for OPCUA_SOCKET_ACCEPT_EVENT. */
    CASE_ID(9016);
    MEM_SNAPSHOT(&s1);
    SYNCHRONIZED(cbinfo16.caseid = 9016;
                 cbinfo16.data   = (OpcUa_Void*) &hEvent);
    statusArray[0] = OpcUa_SocketManager_CreateServer(
#if OPCUA_MULTITHREADED
        serverSocketManager,
#else /* OPCUA_MULTITHREADED */
        OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
        OPCUA_SOCKETTEST_SERVER_URL,
        OpcUa_True,
        Server_SocketCallback,
        (OpcUa_Void*)&cbinfo16,
        &socketArray[0]);

    ASSERT_GOOD(statusArray[0]);
    /* give server some time to start up - waiting for open event would be better ... */
    OpcUa_SocketTest_Wait(1000);
    if (OpcUa_IsGood(statusArray[0]))
    {
        statusArray[1] = OpcUa_SocketManager_CreateClient(
#if OPCUA_MULTITHREADED
            clientSocketManager,
#else /* OPCUA_MULTITHREADED */
            OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
            OPCUA_SOCKETTEST_SERVER_URL,
            (OpcUa_UInt16)0,
            (OpcUa_Socket_EventCallback)OpcUa_Null,
            (OpcUa_Void*)OpcUa_Null,
            &socketArray[1]);

        ASSERT_GOOD(statusArray[1]);
        if (OpcUa_IsGood(statusArray[1]))
        {
            OpcUa_StatusCode status = OpcUa_Good;
            status = OpcUa_SocketTest_WaitForEvent(hEvent, OPCUA_SOCKETTEST_SEM_TIMEOUT);
            ASSERT_STATUS(status, OpcUa_Good);
            OpcUa_Socket_Close(socketArray[1]);
            socketArray[1] = OpcUa_Null;
        }
        OpcUa_Socket_Close(socketArray[0]);
        socketArray[0] = OpcUa_Null;
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9017 */
    /* Bad case */
    /* Bind two sockets to the same port. */
    CASE_ID(9017);
    MEM_SNAPSHOT(&s1);
    statusArray[0] = OpcUa_SocketManager_CreateServer(
#if OPCUA_MULTITHREADED
        serverSocketManager,
#else /* OPCUA_MULTITHREADED */
        OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
        OPCUA_SOCKETTEST_SERVER_URL,
        OpcUa_True,
        Server_SocketCallback,
        (OpcUa_Void*)OpcUa_Null,
        &socketArray[0]);

    ASSERT_GOOD(statusArray[0]);
    /* give server some time to start up - waiting for open event would be better ... */
    OpcUa_SocketTest_Wait(500);
    if (OpcUa_IsGood(statusArray[0]))
    {
        statusArray[1] = OpcUa_SocketManager_CreateServer(
#if OPCUA_MULTITHREADED
            serverSocketManager,
#else /* OPCUA_MULTITHREADED */
        OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
            OPCUA_SOCKETTEST_SERVER_URL,
            OpcUa_True,
            Server_SocketCallback,
            (OpcUa_Void*)OpcUa_Null,
            &socketArray[1]);

        ASSERT_STATUS(statusArray[1], OpcUa_BadCommunicationError);
        if (OpcUa_IsGood(statusArray[1]))
        {
            OpcUa_Socket_Close(socketArray[1]);
            socketArray[1] = OpcUa_Null;
        }
        OpcUa_Socket_Close(socketArray[0]);
        socketArray[0] = OpcUa_Null;
    }
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

#if OPCUA_MULTITHREADED
    OpcUa_SocketManager_Delete(&clientSocketManager);
    OpcUa_SocketManager_Delete(&serverSocketManager);
    clientSocketManager = OpcUa_Null;
    serverSocketManager = OpcUa_Null;
#endif /* OPCUA_MULTITHREADED */

    OpcUa_SocketTest_DeleteEvent(&hEvent);

    return OpcUa_Good;
}

/** Test OpcUa_Socket_Write*/
OpcUa_StatusCode OpcUa_SocketTest_TestWrite()
{
    SNAPSHOT_TYPE s1, s2, s3;
#if OPCUA_MULTITHREADED
    OpcUa_SocketManager manager = OpcUa_Null;
#endif /* OPCUA_MULTITHREADED */
    OpcUa_Handle        hEvent  = OpcUa_Null;
    OpcUa_Socket        client_socket  = OpcUa_Null;
    OpcUa_Socket        server_socket  = OpcUa_Null;
    OpcUa_Socket        bogus_socket   = OpcUa_Null;
    OpcUa_SocketTest_CallbackInfo cbinfo;
    OpcUa_Byte          tempbuf[] = "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";
    OpcUa_Byte*         pByte = OpcUa_Null;
    /*OpcUa_StatusCode  status = OpcUa_Good;*/
    OpcUa_Int32         result = 0;
    OpcUa_Int           i = 0;

    OpcUa_MemSet(&cbinfo, 0, sizeof(OpcUa_SocketTest_CallbackInfo));

    OpcUa_SocketTest_CreateEvent(&hEvent, (OpcUa_UInt32) 0, (OpcUa_UInt32) 1);

#if OPCUA_MULTITHREADED
    OpcUa_SocketManager_Create(&manager, OPCUA_SOCKETTEST_NUMBEROFSOCKETSFORSERVER, OPCUA_SOCKET_NO_FLAG);
#endif /* OPCUA_MULTITHREADED */

    OpcUa_SocketManager_CreateServer(
#if OPCUA_MULTITHREADED
        manager,
#else /* OPCUA_MULTITHREADED */
        OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
        OPCUA_SOCKETTEST_SERVER_URL,
        OpcUa_True,
        Server_SocketCallback,
        (OpcUa_Void*)&cbinfo,
        &server_socket);

    OpcUa_SocketManager_CreateClient(
#if OPCUA_MULTITHREADED
        manager,
#else /* OPCUA_MULTITHREADED */
        OpcUa_Null,
#endif /* OPCUA_MULTITHREADED */
        OPCUA_SOCKETTEST_SERVER_URL,
        (OpcUa_UInt16)0,
        Client_SocketCallback,
        (OpcUa_Void*)&cbinfo,
        &client_socket);

    OpcUa_SocketTest_g_1kByteBuffer     = OpcUa_Alloc(OPCUA_SOCKETTEST_BUFFER_SIZE);
    OpcUa_SocketTest_g_1MByteBuffer     = OpcUa_Alloc(1024 * OPCUA_SOCKETTEST_BUFFER_SIZE);
    OpcUa_SocketTest_g_ReceiveBuffer    = OpcUa_Alloc(1024 * OPCUA_SOCKETTEST_BUFFER_SIZE);

    OpcUa_MemSet(OpcUa_SocketTest_g_1kByteBuffer, 0, OPCUA_SOCKETTEST_BUFFER_SIZE);
    OpcUa_MemSet(OpcUa_SocketTest_g_1MByteBuffer, 0, 1024 * OPCUA_SOCKETTEST_BUFFER_SIZE);
    OpcUa_MemSet(OpcUa_SocketTest_g_ReceiveBuffer, 0, 1024 * OPCUA_SOCKETTEST_BUFFER_SIZE);

    pByte = OpcUa_SocketTest_g_1kByteBuffer;
    for (i = 0; i < OPCUA_SOCKETTEST_BUFFER_SIZE / 64; i++)
    {
        OpcUa_MemCpy(pByte, 64, tempbuf, 64);
        pByte += 64;
    }

    pByte = OpcUa_SocketTest_g_1MByteBuffer;
    for (i = 0; i < 1024 * OPCUA_SOCKETTEST_BUFFER_SIZE / 64; i++)
    {
        OpcUa_MemCpy(pByte, 64, tempbuf, 64);
        pByte += 64;
    }

    /* CASE 9018 */
    /* Bad case */
    /* Write to uninitialized socket. */
    CASE_ID(9018);
    MEM_SNAPSHOT(&s1);
    result = OpcUa_Socket_Write(bogus_socket, OpcUa_SocketTest_g_1kByteBuffer, OPCUA_SOCKETTEST_BUFFER_SIZE, OpcUa_True);
    ASSERT_STATUS(result, OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9019 */
    /* Bad case */
    /* Use invalid buffer. */
    CASE_ID(9019);
    MEM_SNAPSHOT(&s1);
    result = OpcUa_Socket_Write(client_socket, OpcUa_Null, OPCUA_SOCKETTEST_BUFFER_SIZE, OpcUa_False);
    ASSERT(result == OPCUA_SOCKET_ERROR);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9020 */
    /* Bad case */
    /* Write with buffer size = 0. */
    CASE_ID(9020);
    MEM_SNAPSHOT(&s1);
    result = OpcUa_Socket_Write(client_socket, OpcUa_SocketTest_g_1kByteBuffer, 0, OpcUa_False);
    ASSERT(result == OPCUA_SOCKET_ERROR);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9021 */
    /* Good case */
    /* Write 1 KB and check if data is received. */
    CASE_ID(9021);
    MEM_SNAPSHOT(&s1);
    SYNCHRONIZED(   cbinfo.caseid           = 9021;
                    cbinfo.data             = (OpcUa_Void*)&hEvent;
                    cbinfo.uStatus          = OpcUa_BadWaitingForResponse;
                    cbinfo.hClientSocket    = client_socket;
                    cbinfo.hServerSocket    = server_socket);

    OpcUa_SocketTest_g_uTotalBytesReceived = 0;
    OpcUa_MemSet(OpcUa_SocketTest_g_ReceiveBuffer, 0, OPCUA_SOCKETTEST_BUFFER_SIZE);

    /* initial write */
    LOCK;
    result = OpcUa_Socket_Write(client_socket, OpcUa_SocketTest_g_1kByteBuffer, OPCUA_SOCKETTEST_BUFFER_SIZE, OpcUa_False);
    if(result != OPCUA_SOCKET_ERROR)
    {
        OpcUa_StatusCode status = OpcUa_Good;
        OpcUa_SocketTest_g_uTotalBytesSent = result;
        UNLOCK;
        status = OpcUa_SocketTest_WaitForEvent(hEvent, OPCUA_SOCKETTEST_SEM_TIMEOUT);
        ASSERT_STATUS(status, OpcUa_Good);
    }
    else
    {
        UNLOCK;
        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "9021: initial send failed\n");
    }

    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* CASE 9022 */
    /* Good case */
    /* Write 1 MB and check if data is received. */
    CASE_ID(9022);
    MEM_SNAPSHOT(&s1);
    SYNCHRONIZED(   cbinfo.caseid           = 9022;
                    cbinfo.data             = (OpcUa_Void*)&hEvent;
                    cbinfo.uStatus          = OpcUa_BadWaitingForResponse;
                    cbinfo.hClientSocket    = client_socket;
                    cbinfo.hServerSocket    = server_socket);

    OpcUa_SocketTest_g_uTotalBytesReceived = 0;
    OpcUa_MemSet(OpcUa_SocketTest_g_ReceiveBuffer, 0, 1024 * OPCUA_SOCKETTEST_BUFFER_SIZE);

    LOCK;
    result = OpcUa_Socket_Write(client_socket, OpcUa_SocketTest_g_1MByteBuffer, 1024 * OPCUA_SOCKETTEST_BUFFER_SIZE, OpcUa_False);
    if(result != OPCUA_SOCKET_ERROR)
    {
        OpcUa_StatusCode status = OpcUa_Good;
        OpcUa_SocketTest_g_uTotalBytesSent = result;
        UNLOCK;
        status = OpcUa_SocketTest_WaitForEvent(hEvent, OPCUA_SOCKETTEST_SEM_TIMEOUT);
        ASSERT_STATUS(status, OpcUa_Good);
    }
    else
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "9022: sent %i bytes\n", result);
    }

    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    OpcUa_SocketTest_DeleteEvent(&hEvent);
    OpcUa_Socket_Close(client_socket);
    OpcUa_Socket_Close(server_socket);

#if OPCUA_MULTITHREADED
    OpcUa_SocketManager_Delete(&manager);
#endif /* OPCUA_MULTITHREADED */

    OpcUa_Free(OpcUa_SocketTest_g_1kByteBuffer);
    OpcUa_Free(OpcUa_SocketTest_g_1MByteBuffer);
    OpcUa_Free(OpcUa_SocketTest_g_ReceiveBuffer);

    OpcUa_SocketTest_g_1kByteBuffer = OpcUa_Null;
    OpcUa_SocketTest_g_1MByteBuffer = OpcUa_Null;
    OpcUa_SocketTest_g_ReceiveBuffer = OpcUa_Null;

    return OpcUa_Good;
}
