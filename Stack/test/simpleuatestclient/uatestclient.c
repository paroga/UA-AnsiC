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

/*********************************************************************************************/
/*****************     A simple UA test client based on the Ansi C Stack     *****************/
/*********************************************************************************************/

/*********************************************************************************************/
/***********************                     Config                   ************************/
/*********************************************************************************************/
/* !0 == active, 0 == inactive */
/* use visual studio debug heap */
#define UATESTCLIENT_USE_CRTDBG                             0
/* use visual leak detector for visual studio */
#define UATESTCLIENT_USE_VISUAL_LEAK_DETECTOR               0
/* client signs the messages */
#define UATESTCLIENT_USE_SIGNING                            1
/* client encrypts the messages */
#define UATESTCLIENT_USE_ENCRYPTION                         1
/* use the synchronous api - only possible when multithreading is supported */
#define UATESTCLIENT_USE_SYNC_API                           1
/* standard timeout for connect process */
#define UATESTCLIENT_TIMEOUT                                OPCUA_INFINITE
/* URL of the server */
#define UATESTCLIENT_SERVER_URL                             "opc.tcp://localhost"
/* Transport profile used by the client */
#define UATESTCLIENT_TRANSPORT_PROFILE_URI                  OpcUa_TransportProfile_UaTcp
/* wait for user input after shutting down */
#define UATESTCLIENT_WAIT_FOR_USER_INPUT                    1
/* how often the request should be repeated */
#define UATESTCLIENT_NO_OF_REQUESTS                         1
/* set to 1 if the client should stop when a request finishes with error. */
#define UATESTCLIENT_BREAK_ON_ERROR                         1
/* set to 1 to use the content of UaTestClient_g_HugeCharArray instead of the string Hello for requests. */
#define UATESTCLIENT_LARGE_REQUEST                          0
/* defines how often the connect-requests-disconnect cycle is executed before this application terminates. */
#define UATESTCLIENT_NUMBER_OF_CONNECTS                     1
/* the used trace level */
#define UATESTCLIENT_TRACE_LEVEL                            OPCUA_TRACE_OUTPUT_LEVEL_DEBUG
/* defines whether Win32 PKI is used or OpenSSL */
#define UATESTCLIENT_USEWIN32PKI                            0
/* select request variant content (string or UINT32 array) */
#define UATESTCLIENT_STRING_REQUEST                         1
/* use selfsigned certificate as application certificate */
#define UATESTCLIENT_SELFSIGNED                             1

#if UATESTCLIENT_LARGE_REQUEST
    /* define the size in bytes of the large requests body (count of elements in UaTestClient_g_HugeCharArray). */
    #define UATESTCLIENT_LARGE_BODY                         4194304
#endif /* UATESTCLIENT_LARGE_REQUEST */

#if !UATESTCLIENT_STRING_REQUEST
    /* define the number of elements in the UInt32 array  (count of elements in UaTestClient_g_UInt32Array). */
    #define UATESTCLIENT_UINT32_ARRAY_SIZE                  1000
#endif /* UATESTCLIENT_STRING_REQUEST */

/* PKI Locations - this may need to be changed with other platform layers! */
#if UATESTCLIENT_USEWIN32PKI
#define UATESTCLIENT_CERTIFICATE_TRUST_LIST_LOCATION                "UA Applications"
#define UATESTCLIENT_CERTIFICATE_REVOCATION_LIST_LOCATION           "UA Applications"
#define UATESTCLIENT_ISSUER_CERTIFICATE_TRUST_LIST_LOCATION         "UA Certificate Authorities"
#define UATESTCLIENT_ISSUER_CERTIFICATE_REVOCATION_LIST_LOCATION    "UA Certificate Authorities"
#define UATESTCLIENT_CLIENT_CERTIFICATE_LOCATION                    L"UA Sample Client"
#define UATESTCLIENT_CLIENT_PRIVATE_KEY_LOCATION                    (OpcUa_CharA*)L"UA Sample Client"
#define UATESTCLIENT_SERVER_CERTIFICATE_LOCATION                    L"UA Sample Server"
#else /* UATESTCLIENT_USEWIN32PKI */
#if 1 /* UA TCP */

#if UATESTCLIENT_SELFSIGNED
#define UATESTCLIENT_CERTIFICATE_TRUST_LIST_LOCATION                "../PKI/certs/"
#define UATESTCLIENT_CERTIFICATE_REVOCATION_LIST_LOCATION           "../PKI/crl/"
#define UATESTCLIENT_ISSUER_CERTIFICATE_TRUST_LIST_LOCATION         "../PKI/issuers/"
#define UATESTCLIENT_ISSUER_CERTIFICATE_REVOCATION_LIST_LOCATION    "../PKI/crl/"
#define UATESTCLIENT_CLIENT_CERTIFICATE_LOCATION                    "../PKI/certs/selfsigned.der"
#define UATESTCLIENT_CLIENT_PRIVATE_KEY_LOCATION                    "../PKI/private/selfsignedkey.pem"
#else /* UATESTCLIENT_SELFSIGNED */
#define UATESTCLIENT_CERTIFICATE_TRUST_LIST_LOCATION                "../PKI/certs/"
#define UATESTCLIENT_CERTIFICATE_REVOCATION_LIST_LOCATION           "../PKI/crl/"
#define UATESTCLIENT_ISSUER_CERTIFICATE_TRUST_LIST_LOCATION         "../PKI/issuers/"
#define UATESTCLIENT_ISSUER_CERTIFICATE_REVOCATION_LIST_LOCATION    "../PKI/crl/"
#define UATESTCLIENT_CLIENT_CA_CERTIFICATE_LOCATION                 "../PKI/ca/rootca.der"
#define UATESTCLIENT_CLIENT_CERTIFICATE_LOCATION                    "../PKI/certs/rootcasigned.der"
#define UATESTCLIENT_CLIENT_PRIVATE_KEY_LOCATION                    "../PKI/private/rootcasignedkey.pem"
#endif /* UATESTCLIENT_SELFSIGNED */

#define UATESTCLIENT_SERVER_CERTIFICATE_LOCATION                    "../PKI/certs/selfsigned.der"

#else /* HTTPS */

#define UATESTCLIENT_CERTIFICATE_TRUST_LIST_LOCATION                "../PKI/CA/certs/"
#define UATESTCLIENT_CERTIFICATE_REVOCATION_LIST_LOCATION           "../PKI/CA/crl/"
#define UATESTCLIENT_ISSUER_CERTIFICATE_TRUST_LIST_LOCATION         "../PKI/CA/issuers/"
#define UATESTCLIENT_ISSUER_CERTIFICATE_REVOCATION_LIST_LOCATION    "../PKI/CA/crl/"
#define UATESTCLIENT_CLIENT_CERTIFICATE_LOCATION                    OpcUa_Null
#define UATESTCLIENT_CLIENT_PRIVATE_KEY_LOCATION                    OpcUa_Null
#define UATESTCLIENT_SERVER_CERTIFICATE_LOCATION                    OpcUa_Null
#endif
#endif /* UATESTCLIENT_USEWIN32PKI */

/* configuration checks */
#if UATESTCLIENT_USE_SIGNING || UATESTCLIENT_USE_ENCRYPTION
    #define UATESTCLIENT_USE_SECURE_COMMUNICATION               1
#else
    #define UATESTCLIENT_USE_SECURE_COMMUNICATION               0
#endif

#if UATESTCLIENT_USE_ENCRYPTION
    #if UATESTCLIENT_USE_SIGNING
        /* encryption always includes signing - undef for evaluation only */
        #undef UATESTCLIENT_USE_SIGNING
    #endif
#endif

#if UATESTCLIENT_USE_SECURE_COMMUNICATION
    /* verify the servers certificate after loading, before connecting */
    #define UATESTCLIENT_VERIFY_SERVER_CERTIFICATE_LOCALLY  1
    /* verify the clients certificate after loading, before connecting */
    #define UATESTCLIENT_VERIFY_CLIENT_CERTIFICATE_LOCALLY  1
#endif

/* switch between security policies based on above configuration */
#if UATESTCLIENT_USE_ENCRYPTION
    #define UATESTCLIENT_SECURITY_POLICY                    OpcUa_SecurityPolicy_Basic128Rsa15
    #define UATESTCLIENT_SECURITY_POLICY_LENGTH             OpcUa_StrLenA(OpcUa_SecurityPolicy_Basic128Rsa15)
    #define UATESTCLIENT_SECURITY_MODE                      OpcUa_MessageSecurityMode_SignAndEncrypt
#endif

#if UATESTCLIENT_USE_SIGNING
    #define UATESTCLIENT_SECURITY_POLICY                    OpcUa_SecurityPolicy_Basic256
    #define UATESTCLIENT_SECURITY_POLICY_LENGTH             OpcUa_StrLenA(OpcUa_SecurityPolicy_Basic256)
    #define UATESTCLIENT_SECURITY_MODE                      OpcUa_MessageSecurityMode_Sign
#endif

#if !UATESTCLIENT_USE_ENCRYPTION && !UATESTCLIENT_USE_SIGNING
    #define UATESTCLIENT_SECURITY_POLICY                    OpcUa_SecurityPolicy_None
    #define UATESTCLIENT_SECURITY_POLICY_LENGTH             OpcUa_StrLenA(OpcUa_SecurityPolicy_None)
    #define UATESTCLIENT_SECURITY_MODE                      OpcUa_MessageSecurityMode_None
#endif

#if UATESTCLIENT_USE_CRTDBG
    #ifndef _WIN32
        #undef UATESTCLIENT_USE_CRTDBG
    #endif /* _WIN32  */
#endif /* UATESTCLIENT_USE_CRTDBG */

/*********************************************************************************************/
/***********************                     Header                   ************************/
/*********************************************************************************************/
/* system */
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#include <windows.h>
#include <conio.h>
#else
#include <sys/socket.h>
#endif /* _WIN32  */

#include <stdio.h>

#if UATESTCLIENT_USE_CRTDBG
#include <crtdbg.h>
#endif /* UATESTCLIENT_USE_CRTDBG */

/* vld */
#if UATESTCLIENT_USE_VISUAL_LEAK_DETECTOR
#include <vld.h>
#endif /* UATESTCLIENT_USE_VISUAL_LEAK_DETECTOR */

/* ProxyStub */
#include <opcua_clientproxy.h>
#include <opcua_memory.h>
#include <opcua_core.h>
#include <opcua_trace.h>
#include <opcua_string.h>

#if OPCUA_HAVE_HTTPS
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

/*********************************************************************************************/
/***********************                    Types                     ************************/
/*********************************************************************************************/
typedef struct _UaTestClient_CertificateLocations
{
    OpcUa_StringA CertificateRevocationListLocation;
    OpcUa_StringA CertificateTrustListLocation;
    OpcUa_StringA ClientCertificateLocation;
    OpcUa_StringA ClientPrivateKeyLocation;
    OpcUa_StringA ServerCertificateLocation;
} UaTestClient_CertificateLocations;

#if !UATESTCLIENT_USE_SYNC_API

typedef enum _eUaTestClient_State
{
    UaTestClient_e_Invalid,
    UaTestClient_e_Connecting,
    UaTestClient_e_Connected,
    UaTestClient_e_Request,
    UaTestClient_e_Disconnecting,
    UaTestClient_e_Disconnected
} UaTestClient_State;

typedef struct _UaTestClient_AsyncContext
{
    OpcUa_Channel       hChannel;
    UaTestClient_State  eState;
    OpcUa_UInt32        uRemainingNumberOfRequests;
    /* other stuff - like session info for example */
} UaTestClient_AsyncContext;

#endif /* !UATESTCLIENT_USE_SYNC_API */

/*********************************************************************************************/
/***********************                 Prototypes                   ************************/
/*********************************************************************************************/

#if !UATESTCLIENT_USE_SYNC_API
OpcUa_StatusCode UaTestClient_DoAsyncRequest(UaTestClient_AsyncContext* pAsyncContext);
#endif /* !UATESTCLIENT_USE_SYNC_API */

/*********************************************************************************************/
/***********************                  Globals                     ************************/
/*********************************************************************************************/
OpcUa_Handle                                UaTestClient_g_pPortLayerHandle          = OpcUa_Null;
OpcUa_StatusCode                            UaTestClient_g_uStatus                   = OpcUa_Good;

/* security configuration */
OpcUa_PKIProvider                           UaTestClient_g_PkiProvider               = {OpcUa_Null, OpcUa_Null, OpcUa_Null, OpcUa_Null, OpcUa_Null, OpcUa_Null, OpcUa_Null, OpcUa_Null, OpcUa_Null};
OpcUa_String*                               UaTestClient_g_pSecurityPolicy           = OpcUa_Null;
OpcUa_ByteString                            UaTestClient_g_ClientCertificate         = OPCUA_BYTESTRING_STATICINITIALIZER;
OpcUa_ByteString                            UaTestClient_g_ServerCertificate         = OPCUA_BYTESTRING_STATICINITIALIZER;
OpcUa_ByteString                            UaTestClient_g_ClientPrivateKey          = OPCUA_BYTESTRING_STATICINITIALIZER;
OpcUa_Channel_SecurityToken*                UaTestClient_g_pSecurityToken            = OpcUa_Null;
OpcUa_ProxyStubConfiguration                UaTestClient_g_pProxyStubConfiguration;
OpcUa_CertificateStoreConfiguration        UaTestClient_g_PkiConfig;

#if UATESTCLIENT_STRING_REQUEST

#if UATESTCLIENT_LARGE_REQUEST
OpcUa_CharA                                 UaTestClient_g_HugeCharArray[UATESTCLIENT_LARGE_BODY];
#endif /* UATESTCLIENT_LARGE_REQUEST */

#else /* UATESTCLIENT_STRING_REQUEST */

OpcUa_Int32                                 UaTestClient_g_UInt32ArrayLength        = UATESTCLIENT_UINT32_ARRAY_SIZE;
OpcUa_UInt32                                UaTestClient_g_UInt32Array[UATESTCLIENT_UINT32_ARRAY_SIZE];

#endif /* UATESTCLIENT_STRING_REQUEST */

/*********************************************************************************************/
/***********************               Internal Helpers               ************************/
/*********************************************************************************************/

/*===========================================================================================*/
/** @brief Configuration checks.                                                             */
/*===========================================================================================*/
#if UATESTCLIENT_USE_SYNC_API
    #if !OPCUA_MULTITHREADED
        #error Synchronous API only available in multithread configuration!
    #endif /* OPCUA_MULTITHREADED */
#endif /* UATESTCLIENT_USE_SYNC_API */

/*===========================================================================================*/
/** @brief Wait for x to be pressed.                                                         */
/*===========================================================================================*/
OpcUa_Boolean UaTestClient_CheckForKeypress()
{
    if(!_kbhit()){}else{if (_getch()=='x'){return 1;}}return 0;
}

/*===========================================================================================*/
/** @brief Sets a servers and clients certificate and private key.                           */
/*===========================================================================================*/
static OpcUa_StatusCode UaTestClient_InitializePKI()
{
    OpcUa_Handle        hCertificateStore   = OpcUa_Null;

OpcUa_InitializeStatus(OpcUa_Module_Server, "InitializePKI");

#if UATESTCLIENT_USE_SECURE_COMMUNICATION
#if UATESTCLIENT_USEWIN32PKI
    UaTestClient_g_PkiConfig.strPkiType                                 = OPCUA_P_PKI_TYPE_WIN32;
    UaTestClient_g_PkiConfig.uFlags                                     = OPCUA_P_PKI_WIN32_STORE_MACHINE;
#else  /* UATESTCLIENT_USEWIN32PKI */
    UaTestClient_g_PkiConfig.strPkiType                                 = OPCUA_P_PKI_TYPE_OPENSSL;
#endif /* UATESTCLIENT_USEWIN32PKI */
#else /* UATESTCLIENT_USE_SECURE_COMMUNICATION */
    UaTestClient_g_PkiConfig.strPkiType                                 = OPCUA_PKI_TYPE_NONE;
#endif

    UaTestClient_g_PkiConfig.strTrustedCertificateListLocation          = UATESTCLIENT_CERTIFICATE_TRUST_LIST_LOCATION;
    UaTestClient_g_PkiConfig.strRevokedCertificateListLocation          = UATESTCLIENT_CERTIFICATE_REVOCATION_LIST_LOCATION;
    UaTestClient_g_PkiConfig.strIssuerCertificateStoreLocation          = UATESTCLIENT_ISSUER_CERTIFICATE_TRUST_LIST_LOCATION;
    UaTestClient_g_PkiConfig.strRevokedIssuerCertificateListLocation    = UATESTCLIENT_ISSUER_CERTIFICATE_REVOCATION_LIST_LOCATION;

    /****************************** create PKI Config ******************************/
    uStatus = OpcUa_PKIProvider_Create(&UaTestClient_g_PkiConfig, &UaTestClient_g_PkiProvider);
    OpcUa_ReturnErrorIfBad(uStatus);

#if UATESTCLIENT_USE_SECURE_COMMUNICATION

    /* Open Certificate Store */
    uStatus = UaTestClient_g_PkiProvider.OpenCertificateStore(  &UaTestClient_g_PkiProvider,
                                                                &hCertificateStore);
    if(OpcUa_IsBad(uStatus))
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestClient_InitializePKI: Failed to open certificate store! (0x%08X)\n", uStatus);
        OpcUa_GotoError;
    }

#ifdef UATESTCLIENT_CLIENT_CERTIFICATE_LOCATION

#if UATESTCLIENT_SELFSIGNED

    /*** Get client certificate ***/
    uStatus = UaTestClient_g_PkiProvider.LoadCertificate(   &UaTestClient_g_PkiProvider,
                                                            UATESTCLIENT_CLIENT_CERTIFICATE_LOCATION,
                                                            hCertificateStore,
                                                            &UaTestClient_g_ClientCertificate);
    if(OpcUa_IsBad(uStatus))
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestClient_InitializePKI: Failed to load client certificate \"%s\"! (0x%08X)\n", UATESTCLIENT_CLIENT_CERTIFICATE_LOCATION, uStatus);
        OpcUa_GotoError;
    }

#if UATESTCLIENT_VERIFY_CLIENT_CERTIFICATE_LOCALLY
    {
        OpcUa_Int iValidationCode = 0;

        uStatus = UaTestClient_g_PkiProvider.ValidateCertificate(   &UaTestClient_g_PkiProvider,
                                                                    &UaTestClient_g_ClientCertificate,
                                                                    hCertificateStore,
                                                                    &iValidationCode);
        if(OpcUa_IsBad(uStatus))
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestClient_InitializePKI: Client certificate invalid!\n");
            return uStatus;
        }
    }
#endif /* UATESTCLIENT_VERIFY_CLIENT_CERTIFICATE_LOCALLY */

#else /* UATESTCLIENT_SELFSIGNED */

    /*** load CA certificate and append to client certificate ***/
    uStatus = UaTestClient_g_PkiProvider.LoadCertificate(   &UaTestClient_g_PkiProvider,
                                                            UATESTCLIENT_CLIENT_CERTIFICATE_LOCATION,
                                                            hCertificateStore,
                                                            &UaTestClient_g_ClientCertificate);

    if(OpcUa_IsBad(uStatus))
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestClient_InitializePKI: Failed to load client certificate \"%s\"! (0x%08X)\n", UATESTCLIENT_CLIENT_CERTIFICATE_LOCATION, uStatus);
        OpcUa_GotoError;
    }
    else
    {
        OpcUa_ByteString bsCertificate = OPCUA_BYTESTRING_STATICINITIALIZER;

#if UATESTCLIENT_VERIFY_CLIENT_CERTIFICATE_LOCALLY
        {
            OpcUa_Int iValidationCode = 0;

            uStatus = UaTestClient_g_PkiProvider.ValidateCertificate(   &UaTestClient_g_PkiProvider,
                                                                        &UaTestClient_g_ClientCertificate,
                                                                        hCertificateStore,
                                                                        &iValidationCode);
            if(OpcUa_IsBad(uStatus))
            {
                OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestClient_InitializePKI: Client certificate invalid!\n");
                return uStatus;
            }
        }
#endif /* UATESTCLIENT_VERIFY_CLIENT_CERTIFICATE_LOCALLY */

        uStatus = UaTestClient_g_PkiProvider.LoadCertificate(   &UaTestClient_g_PkiProvider,
                                                                UATESTCLIENT_CLIENT_CA_CERTIFICATE_LOCATION,
                                                                hCertificateStore,
                                                                &bsCertificate);

        if(OpcUa_IsBad(uStatus))
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestClient_InitializePKI: Failed to load CA certificate \"%s\"! (0x%08X)\n", UATESTCLIENT_CLIENT_CA_CERTIFICATE_LOCATION, uStatus);
            OpcUa_GotoError;
        }

        uStatus = OpcUa_ByteString_Concatenate( &bsCertificate,
                                                &UaTestClient_g_ClientCertificate,
                                                0);

        if(OpcUa_IsBad(uStatus))
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestClient_InitializePKI: Could not append client certificate! (0x%08X)\n", uStatus);
            OpcUa_GotoError;
        }

        OpcUa_ByteString_Clear(&bsCertificate);
    }

#endif /* UATESTCLIENT_SELFSIGNED */

#endif /* UATESTCLIENT_CLIENT_CERTIFICATE_LOCATION */

#ifdef UATESTCLIENT_SERVER_CERTIFICATE_LOCATION
    /*** Get server certificate ***/
    uStatus = UaTestClient_g_PkiProvider.LoadCertificate(   &UaTestClient_g_PkiProvider,
                                                            UATESTCLIENT_SERVER_CERTIFICATE_LOCATION,
                                                            hCertificateStore,
                                                            &UaTestClient_g_ServerCertificate);
    if(OpcUa_IsBad(uStatus))
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestClient_InitializePKI: Failed to load server certificate \"%s\"! (0x%08X)\n", UATESTCLIENT_SERVER_CERTIFICATE_LOCATION, uStatus);
        OpcUa_GotoError;
    }

#if UATESTCLIENT_VERIFY_SERVER_CERTIFICATE_LOCALLY
    {
        OpcUa_Int iValidationCode = 0;

        uStatus = UaTestClient_g_PkiProvider.ValidateCertificate(   &UaTestClient_g_PkiProvider,
                                                                    &UaTestClient_g_ServerCertificate,
                                                                    hCertificateStore,
                                                                    &iValidationCode);
        if(OpcUa_IsBad(uStatus))
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestClient_InitializePKI: Server certificate invalid!\n");
            return uStatus;
        }
    }
#endif /* UATESTCLIENT_VERIFY_SERVER_CERTIFICATE_LOCALLY */
#endif

#ifdef UATESTCLIENT_CLIENT_PRIVATE_KEY_LOCATION
    {
        /*** Get private key ***/
        uStatus = UaTestClient_g_PkiProvider.LoadPrivateKeyFromFile(UATESTCLIENT_CLIENT_PRIVATE_KEY_LOCATION,
                                                                    OpcUa_Crypto_Encoding_PEM,
                                                                    OpcUa_Null,
                                                                    &UaTestClient_g_ClientPrivateKey);
        if(OpcUa_IsBad(uStatus))
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestClient_InitializePKI: Failed to load client private key \"%s\"! (0x%08X)\n", UATESTCLIENT_CLIENT_PRIVATE_KEY_LOCATION, uStatus);
            OpcUa_GotoError;
        }
    }
#endif

    /* Close Certificate Store */
    UaTestClient_g_PkiProvider.CloseCertificateStore(   &UaTestClient_g_PkiProvider,
                                                        &hCertificateStore);

#endif /* UATESTCLIENT_USE_SECURE_COMMUNICATION */

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestClient_InitializePKI: Could not initialize client PKI.\n");

    if(hCertificateStore != OpcUa_Null)
    {
        UaTestClient_g_PkiProvider.CloseCertificateStore(   &UaTestClient_g_PkiProvider,
                                                            &hCertificateStore);
    }

OpcUa_FinishErrorHandling;
}

/*===========================================================================================*/
/** @brief Initializes the demo application.                                                 */
/*===========================================================================================*/
OpcUa_StatusCode UaTestClient_Initialize(OpcUa_Void)
{
    OpcUa_StatusCode uStatus = OpcUa_Good;

    UaTestClient_g_pProxyStubConfiguration.uProxyStub_Trace_Level                = UATESTCLIENT_TRACE_LEVEL;
    UaTestClient_g_pProxyStubConfiguration.iSerializer_MaxAlloc                  = -1;
    UaTestClient_g_pProxyStubConfiguration.iSerializer_MaxStringLength           = -1;
    UaTestClient_g_pProxyStubConfiguration.iSerializer_MaxByteStringLength       = -1;
    UaTestClient_g_pProxyStubConfiguration.iSerializer_MaxArrayLength            = -1;
    UaTestClient_g_pProxyStubConfiguration.iSerializer_MaxMessageSize            = -1;
    UaTestClient_g_pProxyStubConfiguration.bSecureListener_ThreadPool_Enabled    = OpcUa_False;
    UaTestClient_g_pProxyStubConfiguration.iSecureListener_ThreadPool_MinThreads = -1;
    UaTestClient_g_pProxyStubConfiguration.iSecureListener_ThreadPool_MaxThreads = -1;
    UaTestClient_g_pProxyStubConfiguration.iSecureListener_ThreadPool_MaxJobs    = -1;
    UaTestClient_g_pProxyStubConfiguration.bSecureListener_ThreadPool_BlockOnAdd = OpcUa_True;
    UaTestClient_g_pProxyStubConfiguration.uSecureListener_ThreadPool_Timeout    = OPCUA_INFINITE;
    UaTestClient_g_pProxyStubConfiguration.iTcpListener_DefaultChunkSize         = -1;
    UaTestClient_g_pProxyStubConfiguration.iTcpConnection_DefaultChunkSize       = -1;
    UaTestClient_g_pProxyStubConfiguration.iTcpTransport_MaxMessageLength        = -1;
    UaTestClient_g_pProxyStubConfiguration.iTcpTransport_MaxChunkCount           = -1;
    UaTestClient_g_pProxyStubConfiguration.bTcpListener_ClientThreadsEnabled     = OpcUa_False;
    UaTestClient_g_pProxyStubConfiguration.bTcpStream_ExpectWriteToBlock         = OpcUa_True;

    uStatus = OpcUa_P_Initialize(
#if !OPCUA_USE_STATIC_PLATFORM_INTERFACE
        &UaTestClient_g_pPortLayerHandle
#endif /* !OPCUA_USE_STATIC_PLATFORM_INTERFACE */
        );
    OpcUa_ReturnErrorIfBad(uStatus);

    uStatus = OpcUa_ProxyStub_Initialize(
#if !OPCUA_USE_STATIC_PLATFORM_INTERFACE
        UaTestClient_g_pPortLayerHandle,
#endif /* #if !OPCUA_USE_STATIC_PLATFORM_INTERFACE */
        &UaTestClient_g_pProxyStubConfiguration);
    OpcUa_ReturnErrorIfBad(uStatus);

    uStatus = OpcUa_String_CreateNewString( UATESTCLIENT_SECURITY_POLICY,
                                            UATESTCLIENT_SECURITY_POLICY_LENGTH,
                                            0,
                                            OpcUa_True,
                                            OpcUa_True,
                                            &UaTestClient_g_pSecurityPolicy);
    OpcUa_ReturnErrorIfBad(uStatus);

    uStatus = UaTestClient_InitializePKI();

#if UATESTCLIENT_LARGE_REQUEST
    OpcUa_MemSet(UaTestClient_g_HugeCharArray, 'A', sizeof(UaTestClient_g_HugeCharArray));
    UaTestClient_g_HugeCharArray[UATESTCLIENT_LARGE_BODY - 1] = '\0';
#endif /* UATESTCLIENT_LARGE_REQUEST */

    return uStatus;
}

/*===========================================================================================*/
/** @brief Cleans up all security ressources from the demo application.                      */
/*===========================================================================================*/
OpcUa_Void UaTestClient_SecurityClear(OpcUa_Void)
{
    OpcUa_String_Delete(&UaTestClient_g_pSecurityPolicy);

    OpcUa_ByteString_Clear(&UaTestClient_g_ClientPrivateKey);
    OpcUa_ByteString_Clear(&UaTestClient_g_ClientCertificate);
    OpcUa_ByteString_Clear(&UaTestClient_g_ServerCertificate);

    /* delete PKI provider */
    OpcUa_PKIProvider_Delete(&UaTestClient_g_PkiProvider);
}

/*===========================================================================================*/
/** @brief Cleans up all ressources from the demo application.                               */
/*===========================================================================================*/
OpcUa_Void UaTestClient_Clean(OpcUa_Void)
{
    /* clear pki and security policies */
    UaTestClient_SecurityClear();

    OpcUa_ProxyStub_Clear();

    OpcUa_P_Clean(
#if !OPCUA_USE_STATIC_PLATFORM_INTERFACE
        &UaTestClient_g_pPortLayerHandle
#endif /* !OPCUA_USE_STATIC_PLATFORM_INTERFACE */
        );
}

/*===========================================================================================*/
/** @brief Receives events from the channel.                                                 */
/*===========================================================================================*/
OpcUa_StatusCode UaTestClient_ChannelCallback(  OpcUa_Channel                   a_hChannel,
                                                OpcUa_Void*                     a_pvCallbackData,
                                                OpcUa_Channel_Event             a_eEvent,
                                                OpcUa_StatusCode                a_uStatus,
                                                OpcUa_Channel_SecurityToken*    a_pSecurityToken)
{
OpcUa_InitializeStatus(OpcUa_Module_Client, "ChannelCallback");

    OpcUa_ReferenceParameter(a_hChannel);
    OpcUa_ReferenceParameter(a_pvCallbackData);
    OpcUa_ReferenceParameter(a_uStatus);
    OpcUa_ReferenceParameter(a_pSecurityToken);

    switch(a_eEvent)
    {
    case eOpcUa_Channel_Event_Connected:
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "UaTestClient_ChannelCallback: Channel has been established.\n");
            break;
        }
    case eOpcUa_Channel_Event_Disconnected:
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "UaTestClient_ChannelCallback: Channel has been lost.\n");
            break;
        }
    case eOpcUa_Channel_Event_Renewed:
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "UaTestClient_ChannelCallback: Channel has been renewed.\n");
            break;
        }
#if OPCUA_HAVE_HTTPS
    /* only used for SSL connections */
    case eOpcUa_Channel_Event_VerifyCertificate:
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM,
                        "UaTestClient_ChannelCallback: UaTestClient_ChannelCallback: Certificate validation returned 0x%08X.\n"
                        "received certificate: %i:%8.8s \n",
                        a_uStatus,
                        a_pSecurityToken->SecurityToken.HttpsSecurityToken.bsServerCertificate.Length,
                        a_pSecurityToken->SecurityToken.HttpsSecurityToken.bsServerCertificate.Data);
            uStatus = OpcUa_BadContinue; /* accept certificate */
            break;
        }
#endif /* OPCUA_HAVE_HTTPS */
    default:
        {
        }
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*********************************************************************************************/
/****************************     Main Service Call Chains     *******************************/
/*********************************************************************************************/

#if UATESTCLIENT_USE_SYNC_API
/*===========================================================================================*/
/** @brief Connect and call the TestStackService by using the sync/blocking API.             */
/*===========================================================================================*/
OpcUa_StatusCode UaTestClient_DoSyncRequest(OpcUa_Channel a_hChannel)
{
    OpcUa_RequestHeader     RequestHeader;
    OpcUa_Variant           pSign;

    OpcUa_ResponseHeader    ResponseHeader;
    OpcUa_Variant           pCounterSign;

    OpcUa_UInt32            uiRequestRepetitions = 0;

    OpcUa_UInt32            uiConnectTick = 0;
    OpcUa_UInt32            uiDisconnectTick = 0;

OpcUa_InitializeStatus(OpcUa_Module_Client, "DoSyncRequest");

    OpcUa_RequestHeader_Initialize(&RequestHeader);
    OpcUa_ResponseHeader_Initialize(&ResponseHeader);
    OpcUa_Variant_Initialize(&pSign);
    OpcUa_Variant_Initialize(&pCounterSign);

    /************************************************************************************/

    /* sync connect */
    uStatus = OpcUa_Channel_Connect(    a_hChannel,
                                        UATESTCLIENT_SERVER_URL,
                                        UATESTCLIENT_TRANSPORT_PROFILE_URI,
                                        UaTestClient_ChannelCallback,
                                        OpcUa_Null,
#if UATESTCLIENT_USE_SECURE_COMMUNICATION
                                        &UaTestClient_g_ClientCertificate,
                                        &UaTestClient_g_ClientPrivateKey,
                                        &UaTestClient_g_ServerCertificate,
#else
                                        OpcUa_Null,
                                        OpcUa_Null,
                                        OpcUa_Null,
#endif
                                        &UaTestClient_g_PkiConfig,
                                        UaTestClient_g_pSecurityPolicy,
                                        OPCUA_SECURITYTOKEN_LIFETIME_MAX,
                                        UATESTCLIENT_SECURITY_MODE,
                                        &UaTestClient_g_pSecurityToken,
                                        UATESTCLIENT_TIMEOUT); /* network timeout */

    /* check for common errors */
    if(OpcUa_IsBad(uStatus))
    {
        OpcUa_StringA sError = OpcUa_Null;

        switch(uStatus)
        {
        case OpcUa_BadTimeout:
            {
                sError = "OpcUa_BadTimeout";
                break;
            }
        case OpcUa_BadCommunicationError:
            {
                sError = "OpcUa_BadCommunicationError";
                break;
            }
        case OpcUa_BadConnectionClosed:
            {
                sError = "OpcUa_BadConnectionClosed";
                break;
            }
        case OpcUa_BadCertificateInvalid:
            {
                sError = "OpcUa_BadCertificateInvalid";
                break;
            }
        case OpcUa_BadCertificateTimeInvalid:
            {
                sError = "OpcUa_BadCertificateTimeInvalid";
                break;
            }
        case OpcUa_BadCertificateRevoked:
            {
                sError = "OpcUa_BadCertificateRevoked";
                break;
            }
        case OpcUa_BadCertificateUntrusted:
            {
                sError = "OpcUa_BadCertificateUntrusted";
                break;
            }
        case OpcUa_BadCertificateIssuerRevocationUnknown:
            {
                sError = "OpcUa_BadCertificateIssuerRevocationUnknown";
                break;
            }
        case OpcUa_BadConnectionRejected:
            {
                sError = "OpcUa_BadConnectionRejected";
                break;
            }
        case OpcUa_BadFileNotFound:
            {
                sError = "OpcUa_BadFileNotFound";
                break;
            }
        case OpcUa_BadSecurityConfig:
            {
                sError = "OpcUa_BadSecurityConfig";
                break;
            }
        case OpcUa_BadInternalError:
            {
                sError = "OpcUa_BadInternalError";
                break;
            }
        case OpcUa_BadHostUnknown:
            {
                sError = "OpcUa_BadHostUnknown";
                break;
            }
        default:
            {
                sError = "unknown";
            }
        }

        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "* Connect failed with 0x%08X (%s)!\n", uStatus, sError);
        OpcUa_GotoErrorIfBad(uStatus);
    }

    OpcUa_GotoErrorIfBad(uStatus);

    uiConnectTick = OpcUa_GetTickCount();

    /************************************************************************************/
    OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "**** Starting with %u requests ****\n", UATESTCLIENT_NO_OF_REQUESTS);

    for(uiRequestRepetitions = 0; uiRequestRepetitions < UATESTCLIENT_NO_OF_REQUESTS; uiRequestRepetitions++)
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "**** Request No. %u ****\n", uiRequestRepetitions);

#if UATESTCLIENT_STRING_REQUEST

        pSign.Datatype = OpcUaType_String;
        pSign.ArrayType = OpcUa_VariantArrayType_Scalar;

#if UATESTCLIENT_LARGE_REQUEST
        uStatus = OpcUa_String_AttachToString(  (OpcUa_StringA)UaTestClient_g_HugeCharArray,
                                                sizeof(UaTestClient_g_HugeCharArray),
                                                0,
                                                OpcUa_False,
                                                OpcUa_False,
                                                &pSign.Value.String);
#else
        uStatus = OpcUa_String_AttachToString(  "Hello",
                                                sizeof("Hello"),
                                                0,
                                                OpcUa_False,
                                                OpcUa_False,
                                                &pSign.Value.String);
#endif

#else /* UATESTCLIENT_STRING_REQUEST */

        pSign.Datatype = OpcUaType_UInt32;
        pSign.ArrayType = OpcUa_VariantArrayType_Array;

        pSign.Value.Array.Length            = UaTestClient_g_UInt32ArrayLength;
        pSign.Value.Array.Value.UInt32Array = UaTestClient_g_UInt32Array;

#endif /* UATESTCLIENT_STRING_REQUEST */

        RequestHeader.TimeoutHint   = UATESTCLIENT_TIMEOUT;
        RequestHeader.Timestamp     = OpcUa_DateTime_UtcNow();

        uStatus = OpcUa_ClientApi_TestStack(    a_hChannel,
                                                &RequestHeader,
                                                0,
                                                0,
                                                &pSign,
                                                &ResponseHeader,
                                                &pCounterSign);

        if(OpcUa_IsBad(uStatus))
        {
            if(OpcUa_IsEqual(OpcUa_BadTimeout))
            {
                OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "*-* Timeout *-*\n");
            }
            else
            {
                OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Service Invoke failed: 0x%X\r\n", uStatus);
            }

#if UATESTCLIENT_BREAK_ON_ERROR

            OpcUa_Variant_Clear(&pCounterSign);
            OpcUa_ResponseHeader_Clear(&ResponseHeader);

            break;
#endif
        }
        else
        {
            if(OpcUa_IsBad(ResponseHeader.ServiceResult))
            {
                OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Service Invoke failed: 0x%X\r\n", ResponseHeader.ServiceResult);

                OpcUa_Variant_Clear(&pCounterSign);
                OpcUa_ResponseHeader_Clear(&ResponseHeader);
            }
            else
            {
                if (pCounterSign.Datatype == OpcUaType_String && pCounterSign.ArrayType == OpcUa_VariantArrayType_Scalar)
                {
                    OpcUa_UInt32 uLen = OpcUa_String_StrLen(&(pCounterSign.Value.String));

                    if(uLen < 50)
                    {
                        OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "**** %u. TestStack Response=%*.*s ****\r\n", uiRequestRepetitions, uLen, uLen, OpcUa_String_GetRawString(&(pCounterSign.Value.String)));
                    }
                    else
                    {
                        OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "**** %u. TestStack Response too large to print! ****\r\n", uiRequestRepetitions);
                    }
                }
            }
        }

        OpcUa_Variant_Clear(&pCounterSign);
        OpcUa_ResponseHeader_Clear(&ResponseHeader);
    }
    /************************************************************************************/

    uiDisconnectTick = OpcUa_GetTickCount();

    OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "**** Disconnecting after %u requests and %u milliseconds. ****\n", uiRequestRepetitions, uiDisconnectTick - uiConnectTick);
    OpcUa_Channel_Disconnect(a_hChannel);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    OpcUa_Variant_Clear(&pCounterSign);
    OpcUa_Channel_Disconnect(a_hChannel);

OpcUa_FinishErrorHandling;
}

#else /* UATESTCLIENT_USE_SYNC_API */

/*===========================================================================================*/
/** @brief Receive asynchronous notifications about connection state changes in the channel. */
/*===========================================================================================*/
OpcUa_StatusCode UaTestClient_ConnectionStateChanged(   OpcUa_Channel                a_hChannel,
                                                        OpcUa_Void*                  a_pCallbackData,
                                                        OpcUa_Channel_Event          a_eEvent,
                                                        OpcUa_StatusCode             a_uStatus,
                                                        OpcUa_Channel_SecurityToken* a_pSecurityToken)
{
    UaTestClient_AsyncContext*  pAsyncContext   = (UaTestClient_AsyncContext*)a_pCallbackData;
    OpcUa_StringA               sEvent          = OpcUa_Null;

OpcUa_InitializeStatus(OpcUa_Module_Client, "ConnectionStateChanged");

    OpcUa_ReferenceParameter(a_hChannel);
    OpcUa_ReferenceParameter(a_pSecurityToken);

    UaTestClient_g_uStatus = a_uStatus;

    switch(a_eEvent)
    {
    case eOpcUa_Channel_Event_Connected:
        {
            sEvent = "Connected";
            break;
        }
    case eOpcUa_Channel_Event_Disconnected:
        {
            sEvent = "Disconnected";
            break;
        }
    default:
        {
            sEvent = "Unkown";
        }
    }

    /* check for common errors */
    if(OpcUa_IsBad(a_uStatus))
    {
        OpcUa_StringA sError = OpcUa_Null;

        switch(uStatus)
        {
        case OpcUa_BadTimeout:
            {
                sError = "OpcUa_BadTimeout";
                break;
            }
        case OpcUa_BadCommunicationError:
            {
                sError = "OpcUa_BadCommunicationError";
                break;
            }
        case OpcUa_BadConnectionClosed:
            {
                sError = "OpcUa_BadConnectionClosed";
                break;
            }
        case OpcUa_BadCertificateInvalid:
            {
                sError = "OpcUa_BadCertificateInvalid";
                break;
            }
        case OpcUa_BadCertificateTimeInvalid:
            {
                sError = "OpcUa_BadCertificateTimeInvalid";
                break;
            }
        case OpcUa_BadCertificateRevoked:
            {
                sError = "OpcUa_BadCertificateRevoked";
                break;
            }
        case OpcUa_BadCertificateUntrusted:
            {
                sError = "OpcUa_BadCertificateUntrusted";
                break;
            }
        case OpcUa_BadCertificateIssuerRevocationUnknown:
            {
                sError = "OpcUa_BadCertificateIssuerRevocationUnknown";
                break;
            }
        default:
            {
                sError = "unknown";
            }
        }

        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "**** Connection reports event %s with status 0x%08X (%s)!\n", sEvent, uStatus, sError);
    }
    else
    {
        switch(pAsyncContext->eState)
        {
        case UaTestClient_e_Connecting:
            {
                /* connect done */
                pAsyncContext->eState = UaTestClient_e_Connected;

                /* call service */
                UaTestClient_DoAsyncRequest(pAsyncContext);
                break;
            }
        case UaTestClient_e_Disconnecting:
            {
                /* disconnect done */
                pAsyncContext->eState = UaTestClient_e_Disconnected;
                OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "**** Disconnected! Press x to quit! (0x%08X)!\n", uStatus);
                break;
            }
        case UaTestClient_e_Connected:
        case UaTestClient_e_Request:
        case UaTestClient_e_Disconnected:
        default:
            {
                /* invalid state */
            }
        }
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*===========================================================================================*/
/** @brief Start the disconnect process via the asynchronous API.                            */
/*===========================================================================================*/
OpcUa_StatusCode UaTestClient_DoAsyncDisconnect(UaTestClient_AsyncContext* a_pAsyncContext)
{
OpcUa_InitializeStatus(OpcUa_Module_Client, "DoAsyncDisconnect");

    a_pAsyncContext->eState = UaTestClient_e_Disconnecting;

    /* async disconnect */
    uStatus = OpcUa_Channel_BeginDisconnect(a_pAsyncContext->hChannel,
                                            UaTestClient_ConnectionStateChanged,
                                            (OpcUa_Void*)a_pAsyncContext);
    OpcUa_GotoErrorIfBad(uStatus);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*===========================================================================================*/
/** @brief Gets called by the stack if a response is available or the request timed out.     */
/*===========================================================================================*/
OpcUa_StatusCode UaTestClient_AsyncCallback(OpcUa_Channel           a_hChannel,
                                            OpcUa_Void*             a_pResponse,
                                            OpcUa_EncodeableType*   a_pResponseType,
                                            OpcUa_Void*             a_pCallbackData,
                                            OpcUa_StatusCode        a_uStatus)
{
    OpcUa_TestStackResponse*    pResponse       = (OpcUa_TestStackResponse*)a_pResponse;
    UaTestClient_AsyncContext*  pAsyncContext   = (UaTestClient_AsyncContext*)a_pCallbackData;

OpcUa_InitializeStatus(OpcUa_Module_Client, "UaTestClient_AsyncCallback");

    OpcUa_ReferenceParameter(a_hChannel);
    OpcUa_ReferenceParameter(a_pCallbackData);

    if(OpcUa_IsBad(a_uStatus))
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "Service Invoke failed: 0x%X\r\n", a_uStatus);
        uStatus = a_uStatus;
        OpcUa_GotoError;
    }

    if(a_pResponseType->TypeId == OpcUaId_ServiceFault)
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "Service Invoke failed: 0x%X\r\n", pResponse->ResponseHeader.ServiceResult);
        uStatus = a_uStatus;
        OpcUa_GotoError;
    }

    if(pResponse->Output.Datatype == OpcUaType_String && pResponse->Output.ArrayType == OpcUa_VariantArrayType_Scalar)
    {

        OpcUa_UInt32 uLen = OpcUa_String_StrLen(&(pResponse->Output.Value.String));

        if(uLen < 50)
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "**** TestStack Response=%*.*s ****\r\n", uLen, uLen, OpcUa_String_GetRawString(&(pResponse->Output.Value.String)));
        }
        else
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "**** TestStack Response too large to print! ****\r\n");
        }
    }

    OpcUa_EncodeableObject_Delete(a_pResponseType, &pResponse);

    --pAsyncContext->uRemainingNumberOfRequests;

    if(pAsyncContext->uRemainingNumberOfRequests <= 0)
    {
        UaTestClient_DoAsyncDisconnect(pAsyncContext);
    }
    else
    {
        uStatus = UaTestClient_DoAsyncRequest(pAsyncContext);
        if(OpcUa_IsBad(uStatus))
        {
            UaTestClient_DoAsyncDisconnect(pAsyncContext);
            uStatus = OpcUa_Good;
        }
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    OpcUa_EncodeableObject_Delete(a_pResponseType, &pResponse);
    UaTestClient_g_uStatus = uStatus;

OpcUa_FinishErrorHandling;
}

/*===========================================================================================*/
/** @brief Call the TestStackService via asynchronous API                                    */
/*===========================================================================================*/
OpcUa_StatusCode UaTestClient_DoAsyncRequest(UaTestClient_AsyncContext* a_pAsyncContext)
{
    OpcUa_RequestHeader         RequestHeader;
    OpcUa_Variant               pSign;

OpcUa_InitializeStatus(OpcUa_Module_Client, "DoAsyncRequest");

    OpcUa_RequestHeader_Initialize(&RequestHeader);
    OpcUa_Variant_Initialize(&pSign);

    OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "**** Requesting ****\n");

    pSign.Datatype = OpcUaType_String;
    pSign.ArrayType = OpcUa_VariantArrayType_Scalar;

#if UATESTCLIENT_LARGE_REQUEST
    uStatus = OpcUa_String_AttachToString(  (OpcUa_StringA)UaTestClient_g_HugeCharArray,
                                            sizeof(UaTestClient_g_HugeCharArray),
                                            0,
                                            OpcUa_False,
                                            OpcUa_False,
                                            &pSign.Value.String);
#else
    uStatus = OpcUa_String_AttachToString(  "Hello",
                                            sizeof("Hello"),
                                            0,
                                            OpcUa_True,
                                            OpcUa_False,
                                            &pSign.Value.String);
#endif

    RequestHeader.TimeoutHint   = UATESTCLIENT_TIMEOUT;
    RequestHeader.Timestamp     = OpcUa_DateTime_UtcNow();

    a_pAsyncContext->eState = UaTestClient_e_Request;

    /* async request */
    uStatus = OpcUa_ClientApi_BeginTestStack(   a_pAsyncContext->hChannel,
                                                &RequestHeader,
                                                1,
                                                UATESTCLIENT_TIMEOUT,
                                                &pSign,
                                                UaTestClient_AsyncCallback,
                                                (OpcUa_Void*)a_pAsyncContext);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_Variant_Clear(&pSign);
    OpcUa_RequestHeader_Clear(&RequestHeader);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    OpcUa_Variant_Clear(&pSign);
    OpcUa_RequestHeader_Clear(&RequestHeader);

OpcUa_FinishErrorHandling;
}

/*===========================================================================================*/
/** @brief Start the connect process via the asynchronous API.                               */
/*===========================================================================================*/
OpcUa_StatusCode UaTestClient_DoAsyncConnect(UaTestClient_AsyncContext* a_pAsyncContext)
{
OpcUa_InitializeStatus(OpcUa_Module_Client, "DoAsyncConnect");

    a_pAsyncContext->eState = UaTestClient_e_Connecting;

    /* async connect */
    uStatus = OpcUa_Channel_BeginConnect(   a_pAsyncContext->hChannel,
                                            UATESTCLIENT_SERVER_URL,
                                            UATESTCLIENT_TRANSPORT_PROFILE_URI,
                                            &UaTestClient_g_ClientCertificate,
                                            &UaTestClient_g_ClientPrivateKey,
                                            &UaTestClient_g_ServerCertificate,
                                            &UaTestClient_g_PkiConfig,
                                            UaTestClient_g_pSecurityPolicy,
                                            0, /* security token lifetime - temporarily unused */
                                            UATESTCLIENT_SECURITY_MODE,
                                            UATESTCLIENT_TIMEOUT,
                                            UaTestClient_ConnectionStateChanged,
                                            (OpcUa_Void*)a_pAsyncContext);
    OpcUa_GotoErrorIfBad(uStatus);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*===========================================================================================*/
/** @brief Connect and call the TestStackService by using the async/callback API.            */
/*===========================================================================================*/
OpcUa_StatusCode UaTestClient_DoAsync(OpcUa_Channel a_hChannel)
{
    UaTestClient_AsyncContext*  pAsyncContext = OpcUa_Null;

OpcUa_InitializeStatus(OpcUa_Module_Client, "DoAsync");

    pAsyncContext = (UaTestClient_AsyncContext*)OpcUa_Alloc(sizeof(UaTestClient_AsyncContext));
    OpcUa_GotoErrorIfAllocFailed(pAsyncContext);
    OpcUa_MemSet(pAsyncContext, 0, sizeof(UaTestClient_AsyncContext));

    pAsyncContext->hChannel = a_hChannel;
    pAsyncContext->uRemainingNumberOfRequests = UATESTCLIENT_NO_OF_REQUESTS;

    uStatus = UaTestClient_DoAsyncConnect(pAsyncContext);

    /******************************************************************************/
    /* Wait for user command to terminate the client thread.                      */
    /* While looping here, server is active.                                      */
    /* When in SingleThread config, we need to drive the socket event             */
    /* loop to keep message and timer processing active.                          */
    /******************************************************************************/
    /**/ while(!UaTestClient_CheckForKeypress())                                /**/
    /**/ {                                                                      /**/
#if OPCUA_MULTITHREADED                                                         /**/
    /**/    OpcUa_Thread_Sleep(100);                                            /**/
#else /* OPCUA_MULTITHREADED */                                                 /**/
    /**/    uStatus = OpcUa_SocketManager_Loop(OpcUa_Null, 100, OpcUa_True);    /**/
    /**/    if(OpcUa_IsBad(uStatus))                                            /**/
    /**/    {                                                                   /**/
    /**/        break;                                                          /**/
    /**/    }                                                                   /**/
#endif /* OPCUA_MULTITHREADED */                                                /**/
    /**/ }                                                                      /**/
    /******************************************************************************/

    OpcUa_Free(pAsyncContext);

    uStatus = UaTestClient_g_uStatus;

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

#endif /* UATESTCLIENT_USE_SYNC_API */

/*********************************************************************************************/
/***********************        Application Main Entry Point          ************************/
/*********************************************************************************************/

/*===========================================================================================*/
/** @brief Main entry function.                                                              */
/*===========================================================================================*/
int main(int argc, char* argv[])
{
    OpcUa_StatusCode        uStatus     = OpcUa_Good;
    OpcUa_Channel           hChannel    = OpcUa_Null;
    OpcUa_UInt              uConnect    = 0;

#if UATESTCLIENT_USE_CRTDBG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    /*_CrtSetBreakAlloc(905);*/
#endif

    OpcUa_ReferenceParameter(argc);
    OpcUa_ReferenceParameter(argv);

    uStatus = UaTestClient_Initialize();
    OpcUa_GotoErrorIfBad(uStatus);

    for(uConnect = 0; uConnect < UATESTCLIENT_NUMBER_OF_CONNECTS; uConnect++)
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "**** Connect No. %u ****\n", uConnect);

        uStatus = OpcUa_Channel_Create( &hChannel,
                                        OpcUa_Channel_SerializerType_Binary);
#if UATESTCLIENT_BREAK_ON_ERROR
        OpcUa_GotoErrorIfBad(uStatus);
#else
        if(OpcUa_IsBad(uStatus))
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "main: Channel creation error 0x%08X during connect %u.\n", uStatus, uConnect);
        }
#endif

        if(OpcUa_IsGood(uStatus))
        {
#if UATESTCLIENT_USE_SYNC_API
            uStatus = UaTestClient_DoSyncRequest(hChannel);
#else
            uStatus = UaTestClient_DoAsync(hChannel);
#endif

            OpcUa_Channel_Delete(&hChannel);
#if UATESTCLIENT_BREAK_ON_ERROR
            OpcUa_GotoErrorIfBad(uStatus);
#else
            if(OpcUa_IsBad(uStatus))
            {
                OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "main: Connect/Request error 0x%08X during connect %u.\n", uStatus, uConnect);
            }
#endif
        }

        uStatus = OpcUa_Good;
    }

    UaTestClient_Clean();

#if UATESTCLIENT_WAIT_FOR_USER_INPUT
    printf("Shutdown complete!\nPress enter to exit!\n");
    getchar();
#endif

    return 0;

Error:

    OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "**** Error during connect No. %u ****\n", uConnect);

    if(hChannel != OpcUa_Null)
    {
        OpcUa_Channel_Delete(&hChannel);
    }

    UaTestClient_Clean();

#if UATESTCLIENT_WAIT_FOR_USER_INPUT
    printf("Shutdown complete!\nPress enter to exit!\n");
    getchar();
#endif

    return uStatus;
}

/*********************************************************************************************/
/***********************                End Of File                   ************************/
/*********************************************************************************************/
