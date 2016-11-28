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
/*****************     A simple UA test server based on the Ansi C Stack     *****************/
/*********************************************************************************************/

/*********************************************************************************************/
/***********************                     Config                   ************************/
/*********************************************************************************************/
/* !0 == active, 0 == inactive */
/* use visual studio debug heap */
#define UATESTSERVER_USE_CRTDBG                             0
/* use visual studio debug heap */
#define UATESTSERVER_USE_VISUAL_LEAK_DETECTOR               0
/* support service calls over insecure communication channels */
#define UATESTSERVER_SUPPORT_NONSECURE_COMMUNICATION        1
/* activate support secure communication - no PKI locations required if 0 */
#define UATESTSERVER_SUPPORT_SECURE_COMMUNICATION           1
/* use the synchronous api */
#define UATESTSERVER_SYNC                                   1
/* wait for user input after shutting down */
#define UATESTSERVER_WAIT_FOR_USER_INPUT                    1
/* URL of the server */
#define UATESTSERVER_ENDPOINT_URL                           "opc.tcp://localhost"
/* Transport profile supported by the server */
#define UATESTSERVER_TRANSPORT_PROFILE_URI                  OpcUa_TransportProfile_UaTcp
/* set to 1 to use the content of UaTestServer_g_HugeCharArray instead of the string World for responses. */
#define UATESTSERVER_LARGE_RESPONSE                         0
/* the used trace level */
#define UATESTSERVER_TRACE_LEVEL                            OPCUA_TRACE_OUTPUT_LEVEL_DEBUG
/* defines whether Win32 PKI is used or OpenSSL */
#define UATESTSERVER_USEWIN32PKI                            0
/* print uastack version information */
#define UATESTSERVER_PRINT_VERSION                          1
/* use selfsigned certificate as application certificate */
#define UATESTSERVER_SELFSIGNED                             1

#if UATESTSERVER_LARGE_RESPONSE
    /* define the size in bytes of the large response body (count of elements in UaTestServer_g_HugeCharArray). */
    #define UATESTSERVER_LARGE_BODY                         4194304
#endif /* UATESTSERVER_LARGE_RESPONSE */

/* number of integers in the response array (used in response for uint32 array requests) */
#define UATESTSERVER_UINT32_ARRAY_SIZE                      1000

/* !0 if asynchronous service handling should be used */
#define UATESTSERVER_DORESPONSEASYNC                        0
#if UATESTSERVER_DORESPONSEASYNC
/* if response is sent asynchronously, delay the sending for this amount of milliseconds */
# define UATESTSERVER_ASYNCRESPONSEDELAY                    5
# if UATESTSERVER_SYNC
#   error UATESTSERVER_DORESPONSEASYNC does not allow UATESTSERVER_SYNC
# endif /* UATESTSERVER_SYNC */
#endif /* UATESTSERVER_DORESPONSEASYNC */

#if UATESTSERVER_SUPPORT_SECURE_COMMUNICATION

/* PKI Locations - this may need to be changed with other platform layers! */
#if UATESTSERVER_USEWIN32PKI

/* Personal folder of Store specified in PKIConfig; defines where the
   certificate (or the associated private key) to be loaded/exported
   is located. It also defines where a certificate to be saved/imported
   is stored.*/
# define UATESTSERVER_CERTIFICATE_TRUST_LIST_LOCATION             "UA Applications"
/* untrusted certificates folder in windows certificate store is used.*/
# define UATESTSERVER_CERTIFICATE_REVOCATION_LIST_LOCATION        "UA Applications"
# define UATESTSERVER_ISSUER_CERTIFICATE_TRUST_LIST_LOCATION      "UA Certificate Authorities"
# define UATESTSERVER_ISSUER_CERTIFICATE_REVOCATION_LIST_LOCATION "UA Certificate Authorities"
/* subject name of servercertificate */
# define UATESTSERVER_SERVER_CERTIFICATE_LOCATION                 L"UA Sample Server"
/* key is identified by server certificate in the store */
# define UATESTSERVER_SERVER_PRIVATE_KEY_LOCATION                 (OpcUa_CharA*)L"UA Sample Server"

#else /* UATESTSERVER_USEWIN32PKI */

#if UATESTSERVER_SELFSIGNED
# define UATESTSERVER_CERTIFICATE_TRUST_LIST_LOCATION             "../PKI/certs/"
# define UATESTSERVER_CERTIFICATE_REVOCATION_LIST_LOCATION        "../PKI/crl/"
# define UATESTSERVER_ISSUER_CERTIFICATE_TRUST_LIST_LOCATION      "../PKI/issuers/"
# define UATESTSERVER_ISSUER_CERTIFICATE_REVOCATION_LIST_LOCATION "../PKI/crl/"
# define UATESTSERVER_SERVER_CERTIFICATE_LOCATION                 "../PKI/certs/selfsigned.der"
# define UATESTSERVER_SERVER_PRIVATE_KEY_LOCATION                 "../PKI/private/selfsignedkey.pem"
#else /* UATESTSERVER_SELFSIGNED */
# define UATESTSERVER_CERTIFICATE_TRUST_LIST_LOCATION             "../PKI/certs/"
# define UATESTSERVER_CERTIFICATE_REVOCATION_LIST_LOCATION        "../PKI/crl/"
# define UATESTSERVER_ISSUER_CERTIFICATE_TRUST_LIST_LOCATION      "../PKI/issuers/"
# define UATESTSERVER_ISSUER_CERTIFICATE_REVOCATION_LIST_LOCATION "../PKI/crl/"
# define UATESTSERVER_SERVER_CA_CERTIFICATE_LOCATION              "../PKI/ca/rootca.der"
# define UATESTSERVER_SERVER_CERTIFICATE_LOCATION                 "../PKI/certs/rootcasigned.der"
# define UATESTSERVER_SERVER_PRIVATE_KEY_LOCATION                 "../PKI/private/rootcasignedkey.pem"
#endif /* UATESTSERVER_SELFSIGNED */
#endif /* UATESTSERVER_USEWIN32PKI */

#endif /* UATESTSERVER_SUPPORT_SECURE_COMMUNICATION */

#if UATESTSERVER_SUPPORT_SECURE_COMMUNICATION
    /* verify the servers certificate after loading, before connecting */
    #define UATESTSERVER_VERIFY_SERVER_CERTIFICATE_LOCALLY  1
#endif

#if UATESTSERVER_USE_CRTDBG
    #ifndef _WIN32
        #undef UATESTSERVER_USE_CRTDBG
    #endif /* _WIN32  */
#endif /* UATESTSERVER_USE_CRTDBG */

/*********************************************************************************************/
/***********************                     Header                   ************************/
/*********************************************************************************************/
/* system */
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <sys/socket.h>
#endif /* _WIN32  */

#include <stdio.h>

#if UATESTSERVER_USE_CRTDBG
#include <crtdbg.h>
#endif /* UATESTSERVER_USE_CRTDBG */

/* vld */
#if UATESTSERVER_USE_VISUAL_LEAK_DETECTOR
# include <vld.h>
#endif /* UaTestServer_USE_VISUAL_LEAK_DETECTOR */

/* serverstub (basic includes for implementing a server based on the stack) */
#include <opcua_serverstub.h>

/* extensions from the stack (get additional functionality) */
#include <opcua_thread.h>
#include <opcua_string.h>
#include <opcua_memory.h>
#include <opcua_core.h>
#include <opcua_guid.h>
#include <opcua_trace.h>

/*********************************************************************************************/
/***********************                 Prototypes                   ************************/
/*********************************************************************************************/
/** @brief Manual asynchronous service handler. */
OpcUa_StatusCode UaTestServer_BeginTestStack(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType);

/** @brief Synchronous service handler. */
OpcUa_StatusCode UaTestServer_TestStack(
    OpcUa_Endpoint        hEndpoint,
    OpcUa_Handle          hContext,
    OpcUa_RequestHeader*  pRequestHeader,
    OpcUa_UInt32          nTestId,
    OpcUa_UInt32          nIteration,
    OpcUa_Variant*        pInput,
    OpcUa_ResponseHeader* pResponseHeader,
    OpcUa_Variant*        pOutput);

/*********************************************************************************************/
/***********************                    Types                     ************************/
/*********************************************************************************************/
typedef struct _UaTestServer_AsyncRequestContext
{
    OpcUa_Endpoint        hEndpoint;
    OpcUa_Handle          hContext;
    OpcUa_Void*           pRequest;
    OpcUa_EncodeableType  RequestType;
} UaTestServer_AsyncRequestContext;

/*********************************************************************************************/
/***********************                  Globals                     ************************/
/*********************************************************************************************/
OpcUa_Int32                                 UaTestServer_g_intDummy                      = 555;
OpcUa_Handle                                UaTestServer_g_PlatformLayerHandle           = OpcUa_Null;
OpcUa_Boolean                               UaTestServer_g_gbStopServer                  = OpcUa_False;
OpcUa_UInt32                                UaTestServer_g_uiShutdownBlocked             = 0;
#if OPCUA_USE_SYNCHRONISATION
OpcUa_Mutex                                 UaTestServer_g_hShutdownFlagMutex            = OpcUa_Null;
#endif /* OPCUA_USE_SYNCHRONISATION */

OpcUa_PKIProvider                           UaTestServer_g_PkiProvider                   = {OpcUa_Null, OpcUa_Null, OpcUa_Null, OpcUa_Null, OpcUa_Null, OpcUa_Null, OpcUa_Null, OpcUa_Null, OpcUa_Null};

/* security configuration */
#if UATESTSERVER_SUPPORT_SECURE_COMMUNICATION
OpcUa_Key                                   UaTestServer_g_ServerPrivateKey              = {OpcUa_Crypto_KeyType_Rsa_Private, {0, OpcUa_Null}, OpcUa_Null};
OpcUa_ByteString                            UaTestServer_g_ServerCertificate             = OPCUA_BYTESTRING_STATICINITIALIZER;
#endif /* UATESTSERVER_SUPPORT_SECURE_COMMUNICATION */

OpcUa_UInt32                                UaTestServer_g_uNoOfSecurityPolicies         = 0;
OpcUa_CertificateStoreConfiguration         UaTestServer_g_PkiConfig                     = {OPCUA_PKI_TYPE_NONE, OpcUa_Null, OpcUa_Null, OpcUa_Null, OpcUa_Null, 0, OpcUa_Null};
OpcUa_Endpoint_SecurityPolicyConfiguration* UaTestServer_g_pSecurityPolicyConfigurations = OpcUa_Null;
OpcUa_ProxyStubConfiguration                UaTestServer_g_pProxyStubConfiguration;

#if UATESTSERVER_LARGE_RESPONSE
OpcUa_CharA                                 UaTestServer_g_HugeCharArray[UATESTSERVER_LARGE_BODY];
#endif /* #if UATESTSERVER_LARGE_RESPONSE */

/*********************************************************************************************/
/***********************            Service Declarations              ************************/
/*********************************************************************************************/
/** @brief The testservice. */
OpcUa_ServiceType UaTestServer_TestStackService  =
{
    OpcUaId_TestStackRequest,
    &OpcUa_TestStackResponse_EncodeableType,
#if UATESTSERVER_SYNC
    (OpcUa_PfnBeginInvokeService*)OpcUa_Server_BeginTestStack,
    (OpcUa_PfnInvokeService*)UaTestServer_TestStack
#else
    UaTestServer_BeginTestStack,
    OpcUa_Null
#endif
};

/** @brief All supported services. */
OpcUa_ServiceType*  UaTestServer_SupportedServices[] =
{
    &UaTestServer_TestStackService,
    OpcUa_Null
};

/*********************************************************************************************/
/***********************               Internal Helpers               ************************/
/*********************************************************************************************/

/*===========================================================================================*/
/** @brief Wait for x to be pressed.                                                         */
/*===========================================================================================*/
OpcUa_Boolean UaTestServer_CheckForKeypress()
{
    if(!_kbhit()){}else{if (_getch()=='x'){return 1;}}return 0;
}

/*===========================================================================================*/
/** @brief Part of initialization process. Fill the security policies.                       */
/*===========================================================================================*/
static OpcUa_StatusCode UaTestServer_CreateSecurityPolicies(OpcUa_Void)
{
    OpcUa_UInt32 uIndex = 0;

#if UATESTSERVER_SUPPORT_NONSECURE_COMMUNICATION
    ++UaTestServer_g_uNoOfSecurityPolicies;
#endif

#if UATESTSERVER_SUPPORT_SECURE_COMMUNICATION
    UaTestServer_g_uNoOfSecurityPolicies += 2;
#endif

    /* allocate and initialize policy configurations */
    UaTestServer_g_pSecurityPolicyConfigurations = (OpcUa_Endpoint_SecurityPolicyConfiguration*)OpcUa_Alloc(sizeof(OpcUa_Endpoint_SecurityPolicyConfiguration) * UaTestServer_g_uNoOfSecurityPolicies);
    OpcUa_ReturnErrorIfAllocFailed(UaTestServer_g_pSecurityPolicyConfigurations);
    for(uIndex = 0; uIndex < UaTestServer_g_uNoOfSecurityPolicies; uIndex++)
    {
        OpcUa_String_Initialize(&((UaTestServer_g_pSecurityPolicyConfigurations[uIndex]).sSecurityPolicy));
        UaTestServer_g_pSecurityPolicyConfigurations[uIndex].pbsClientCertificate = OpcUa_Null;
    }

    uIndex = 0;

#if UATESTSERVER_SUPPORT_NONSECURE_COMMUNICATION
    OpcUa_String_AttachCopy(  &(UaTestServer_g_pSecurityPolicyConfigurations[uIndex].sSecurityPolicy),
                              OpcUa_SecurityPolicy_None);
    UaTestServer_g_pSecurityPolicyConfigurations[uIndex].uMessageSecurityModes = OPCUA_ENDPOINT_MESSAGESECURITYMODE_NONE;
    ++uIndex;
#endif

#if UATESTSERVER_SUPPORT_SECURE_COMMUNICATION
    OpcUa_String_AttachCopy(  &(UaTestServer_g_pSecurityPolicyConfigurations[uIndex].sSecurityPolicy),
                              OpcUa_SecurityPolicy_Basic128Rsa15);
    UaTestServer_g_pSecurityPolicyConfigurations[uIndex].uMessageSecurityModes = OPCUA_ENDPOINT_MESSAGESECURITYMODE_SIGNANDENCRYPT;
    ++uIndex;

    OpcUa_String_AttachCopy(  &(UaTestServer_g_pSecurityPolicyConfigurations[uIndex].sSecurityPolicy),
                              OpcUa_SecurityPolicy_Basic256);
    UaTestServer_g_pSecurityPolicyConfigurations[uIndex].uMessageSecurityModes = OPCUA_ENDPOINT_MESSAGESECURITYMODE_SIGN;
#endif /* UATESTSERVER_SUPPORT_SECURE_COMMUNICATION */

    return OpcUa_Good;
}

/*===========================================================================================*/
/** @brief Sets a servers certificate and private key.                                       */
/*===========================================================================================*/
static OpcUa_StatusCode UaTestServer_InitializePKI()
{
#if UATESTSERVER_SUPPORT_SECURE_COMMUNICATION
    OpcUa_Handle hCertificateStore = OpcUa_Null;
#endif /* UATESTSERVER_SUPPORT_SECURE_COMMUNICATION */

OpcUa_InitializeStatus(OpcUa_Module_Server, "InitializePKI");

    OpcUa_MemSet(&UaTestServer_g_PkiProvider, 0, sizeof(OpcUa_PKIProvider));
    OpcUa_MemSet(&UaTestServer_g_PkiConfig,   0, sizeof(OpcUa_CertificateStoreConfiguration));

#if UATESTSERVER_SUPPORT_SECURE_COMMUNICATION

    /****************************** create PKI Config ******************************/
#if UATESTSERVER_USEWIN32PKI
    UaTestServer_g_PkiConfig.strPkiType                                 = OPCUA_P_PKI_TYPE_WIN32;
    UaTestServer_g_PkiConfig.uFlags                                     = OPCUA_P_PKI_WIN32_STORE_MACHINE;
#else /* UATESTSERVER_ */
    UaTestServer_g_PkiConfig.strPkiType                                 = OPCUA_P_PKI_TYPE_OPENSSL;
#endif /* UATESTSERVER_ */

    UaTestServer_g_PkiConfig.strTrustedCertificateListLocation          = UATESTSERVER_CERTIFICATE_TRUST_LIST_LOCATION;
    UaTestServer_g_PkiConfig.strRevokedCertificateListLocation          = UATESTSERVER_CERTIFICATE_REVOCATION_LIST_LOCATION;
    UaTestServer_g_PkiConfig.strIssuerCertificateStoreLocation          = UATESTSERVER_ISSUER_CERTIFICATE_TRUST_LIST_LOCATION;
    UaTestServer_g_PkiConfig.strRevokedIssuerCertificateListLocation    = UATESTSERVER_ISSUER_CERTIFICATE_REVOCATION_LIST_LOCATION;

    /* create application pki provider */
    uStatus = OpcUa_PKIProvider_Create(&UaTestServer_g_PkiConfig, &UaTestServer_g_PkiProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    /* open certificate store */
    uStatus = UaTestServer_g_PkiProvider.OpenCertificateStore(  &UaTestServer_g_PkiProvider,
                                                                &hCertificateStore);
    if(OpcUa_IsBad(uStatus))
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestServer_InitializePKI: Failed to open certificate store! (0x%08X)\n", uStatus);
        OpcUa_GotoError;
    }

#if UATESTSERVER_SELFSIGNED
    /*** load server certificate ***/
    uStatus = UaTestServer_g_PkiProvider.LoadCertificate(   &UaTestServer_g_PkiProvider,
                                                            UATESTSERVER_SERVER_CERTIFICATE_LOCATION,
                                                            hCertificateStore,
                                                            &UaTestServer_g_ServerCertificate);

    if(OpcUa_IsBad(uStatus))
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestServer_InitializePKI: Failed to load server certificate \"%s\"! (0x%08X)\n", UATESTSERVER_SERVER_CERTIFICATE_LOCATION, uStatus);
        OpcUa_GotoError;
    }
#else
    /*** load server certificate and append CA certificate ***/
    uStatus = UaTestServer_g_PkiProvider.LoadCertificate(   &UaTestServer_g_PkiProvider,
                                                            UATESTSERVER_SERVER_CERTIFICATE_LOCATION,
                                                            hCertificateStore,
                                                            &UaTestServer_g_ServerCertificate);

    if(OpcUa_IsBad(uStatus))
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestServer_InitializePKI: Failed to load server certificate \"%s\"! (0x%08X)\n", UATESTSERVER_SERVER_CERTIFICATE_LOCATION, uStatus);
        OpcUa_GotoError;
    }
    else
    {
        OpcUa_ByteString bsCertificate = OPCUA_BYTESTRING_STATICINITIALIZER;

#if UATESTSERVER_VERIFY_SERVER_CERTIFICATE_LOCALLY
        {
            OpcUa_Int iValidationCode = 0;

            uStatus = UaTestServer_g_PkiProvider.ValidateCertificate(   &UaTestServer_g_PkiProvider,
                                                                        &UaTestServer_g_ServerCertificate,
                                                                        hCertificateStore,
                                                                        &iValidationCode);
            if(OpcUa_IsBad(uStatus))
            {
                OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestServer_InitializePKI: Server certificate invalid!\n");
                return uStatus;
            }
        }
#endif /* UATESTSERVER_VERIFY_SERVER_CERTIFICATE_LOCALLY */

        uStatus = UaTestServer_g_PkiProvider.LoadCertificate(   &UaTestServer_g_PkiProvider,
                                                                UATESTSERVER_SERVER_CA_CERTIFICATE_LOCATION,
                                                                hCertificateStore,
                                                                &bsCertificate);

        if(OpcUa_IsBad(uStatus))
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestServer_InitializePKI: Failed to load CA certificate \"%s\"! (0x%08X)\n", UATESTSERVER_SERVER_CA_CERTIFICATE_LOCATION, uStatus);
            OpcUa_GotoError;
        }

        uStatus = OpcUa_ByteString_Concatenate( &bsCertificate,
                                                &UaTestServer_g_ServerCertificate,
                                                0);

        OpcUa_ByteString_Clear(&bsCertificate);

        if(OpcUa_IsBad(uStatus))
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestServer_InitializePKI: Could not append server certificate! (0x%08X)\n", uStatus);
            OpcUa_GotoError;
        }
    }
#endif

    /*** get server private key ***/
    uStatus = UaTestServer_g_PkiProvider.LoadPrivateKeyFromFile(UATESTSERVER_SERVER_PRIVATE_KEY_LOCATION,
                                                                OpcUa_Crypto_Encoding_PEM,
                                                                OpcUa_Null,
                                                                &UaTestServer_g_ServerPrivateKey.Key);

    if(OpcUa_IsBad(uStatus))
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestServer_InitializePKI: Failed to load server private key \"%s\"! (0x%08X)\n", UATESTSERVER_SERVER_PRIVATE_KEY_LOCATION, uStatus);
        OpcUa_GotoError;
    }

    /* close certificate store */
    UaTestServer_g_PkiProvider.CloseCertificateStore(   &UaTestServer_g_PkiProvider,
                                                        &hCertificateStore);

#else /* UATESTSERVER_SUPPORT_SECURE_COMMUNICATION */

    /****************************** create PKI Config ******************************/
    UaTestServer_g_PkiConfig.strPkiType                             = OPCUA_PKI_TYPE_NONE;
    UaTestServer_g_PkiConfig.strTrustedCertificateListLocation      = OpcUa_Null;
    UaTestServer_g_PkiConfig.strRevokedCertificateListLocation      = OpcUa_Null;
    UaTestServer_g_PkiConfig.strIssuerCertificateStoreLocation      = OpcUa_Null;
    UaTestServer_g_PkiConfig.strRevokedIssuerCertificateListLocation= OpcUa_Null;

#endif /* UATESTSERVER_SUPPORT_SECURE_COMMUNICATION */

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestServer_InitializePKI: Could not initialize server PKI.\n");

#if UATESTSERVER_SUPPORT_SECURE_COMMUNICATION
    if(hCertificateStore != OpcUa_Null)
    {
        UaTestServer_g_PkiProvider.CloseCertificateStore(   &UaTestServer_g_PkiProvider,
                                                            &hCertificateStore);
    }
#endif /* UATESTSERVER_SUPPORT_SECURE_COMMUNICATION */

OpcUa_FinishErrorHandling;
}

/*===========================================================================================*/
/** @brief Initializes the demo application.                                                 */
/*===========================================================================================*/
OpcUa_StatusCode UaTestServer_Initialize(OpcUa_Void)
{
#if UATESTSERVER_PRINT_VERSION
    OpcUa_StringA strVersion = OpcUa_Null;
#endif /* UATESTSERVER_PRINT_VERSION */

OpcUa_InitializeStatus(OpcUa_Module_Server, "Initialize");

    UaTestServer_g_pProxyStubConfiguration.uProxyStub_Trace_Level                = UATESTSERVER_TRACE_LEVEL;
    UaTestServer_g_pProxyStubConfiguration.iSerializer_MaxAlloc                  = -1;
    UaTestServer_g_pProxyStubConfiguration.iSerializer_MaxStringLength           = -1;
    UaTestServer_g_pProxyStubConfiguration.iSerializer_MaxByteStringLength       = -1;
    UaTestServer_g_pProxyStubConfiguration.iSerializer_MaxArrayLength            = -1;
    UaTestServer_g_pProxyStubConfiguration.iSerializer_MaxMessageSize            = -1;
    UaTestServer_g_pProxyStubConfiguration.bSecureListener_ThreadPool_Enabled    = OpcUa_False;
    UaTestServer_g_pProxyStubConfiguration.iSecureListener_ThreadPool_MinThreads = -1;
    UaTestServer_g_pProxyStubConfiguration.iSecureListener_ThreadPool_MaxThreads = -1;
    UaTestServer_g_pProxyStubConfiguration.iSecureListener_ThreadPool_MaxJobs    = -1;
    UaTestServer_g_pProxyStubConfiguration.bSecureListener_ThreadPool_BlockOnAdd = OpcUa_True;
    UaTestServer_g_pProxyStubConfiguration.uSecureListener_ThreadPool_Timeout    = OPCUA_INFINITE;
    UaTestServer_g_pProxyStubConfiguration.iTcpListener_DefaultChunkSize         = -1;
    UaTestServer_g_pProxyStubConfiguration.iTcpConnection_DefaultChunkSize       = -1;
    UaTestServer_g_pProxyStubConfiguration.iTcpTransport_MaxMessageLength        = -1;
    UaTestServer_g_pProxyStubConfiguration.iTcpTransport_MaxChunkCount           = -1;
    UaTestServer_g_pProxyStubConfiguration.bTcpListener_ClientThreadsEnabled     = OpcUa_False;
    UaTestServer_g_pProxyStubConfiguration.bTcpStream_ExpectWriteToBlock         = OpcUa_True;

    /* initialize platform layer */
    uStatus = OpcUa_P_Initialize(
#if !OPCUA_USE_STATIC_PLATFORM_INTERFACE
        &UaTestServer_g_PlatformLayerHandle
#endif /* !OPCUA_USE_STATIC_PLATFORM_INTERFACE */
        );
    OpcUa_GotoErrorIfBad(uStatus);

    /* initialize stack */
    uStatus = OpcUa_ProxyStub_Initialize(
#if !OPCUA_USE_STATIC_PLATFORM_INTERFACE
        UaTestServer_g_PlatformLayerHandle,
#endif /* #if !OPCUA_USE_STATIC_PLATFORM_INTERFACE */
        &UaTestServer_g_pProxyStubConfiguration);
    OpcUa_GotoErrorIfBad(uStatus);

#if UATESTSERVER_PRINT_VERSION

    /* get version string information */

    strVersion = OpcUa_P_GetVersion();
    OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "\nOpcUa_P_GetVersion:\n%s\n", strVersion);

    strVersion = OpcUa_P_GetConfigString();
    OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "\nOpcUa_P_GetConfigString:\n%s\n", strVersion);

    strVersion = OpcUa_ProxyStub_GetVersion();
    OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "\nOpcUa_ProxyStub_GetVersion:\n%s\n", strVersion);

    strVersion = OpcUa_ProxyStub_GetStaticConfigString();
    OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "\nOpcUa_ProxyStub_GetStaticConfigString:\n%s\n", strVersion);

    strVersion = OpcUa_ProxyStub_GetConfigString();
    OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "\nOpcUa_ProxyStub_GetConfigString:\n%s\n", strVersion);

#endif /* UATESTSERVER_PRINT_VERSION */

    /* initialize security configuration */
    uStatus = UaTestServer_InitializePKI();
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = UaTestServer_CreateSecurityPolicies();
    OpcUa_GotoErrorIfBad(uStatus);

#if UATESTSERVER_LARGE_RESPONSE
    OpcUa_MemSet(UaTestServer_g_HugeCharArray, 'B', sizeof(UaTestServer_g_HugeCharArray));
    UaTestServer_g_HugeCharArray[UATESTSERVER_LARGE_BODY - 1] = '\0';
#endif /* UATESTSERVER_LARGE_RESPONSE */

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*===========================================================================================*/
/** @brief Cleans up all security ressources from the demo application.                      */
/*===========================================================================================*/
static OpcUa_Void UaTestServer_SecurityClear(OpcUa_Void)
{
    OpcUa_UInt32 uIndex = 0;

    /* clean up security policies */
    if(UaTestServer_g_pSecurityPolicyConfigurations != OpcUa_Null)
    {
        for(uIndex = 0; uIndex < UaTestServer_g_uNoOfSecurityPolicies; uIndex++)
        {
            OpcUa_String_Clear(&(UaTestServer_g_pSecurityPolicyConfigurations[uIndex].sSecurityPolicy));
            UaTestServer_g_pSecurityPolicyConfigurations[uIndex].pbsClientCertificate = OpcUa_Null;
        }

        OpcUa_Free(UaTestServer_g_pSecurityPolicyConfigurations);
    }

#if UATESTSERVER_SUPPORT_SECURE_COMMUNICATION
    OpcUa_ByteString_Clear(&UaTestServer_g_ServerPrivateKey.Key);
    OpcUa_ByteString_Clear(&UaTestServer_g_ServerCertificate);

    /* delete PKI provider */
    OpcUa_PKIProvider_Delete(&UaTestServer_g_PkiProvider);
#endif /* UATESTSERVER_SUPPORT_SECURE_COMMUNICATION */
}

/*===========================================================================================*/
/** @brief Cleans up all ressources from the demo application.                               */
/*===========================================================================================*/
OpcUa_Void UaTestServer_Clear(OpcUa_Void)
{
    UaTestServer_SecurityClear();

    OpcUa_ProxyStub_Clear();
    OpcUa_P_Clean(
#if !OPCUA_USE_STATIC_PLATFORM_INTERFACE
        &UaTestServer_g_PlatformLayerHandle
#endif /* !OPCUA_USE_STATIC_PLATFORM_INTERFACE */
        );
}

/*===========================================================================================*/
/** @brief Set shutdown flag and wait for all threads to leave the block.                    */
/*===========================================================================================*/
OpcUa_Void UaTestServer_SetAndWaitShutdown(OpcUa_Void)
{
#if OPCUA_USE_SYNCHRONISATION
    OpcUa_Mutex_Lock(UaTestServer_g_hShutdownFlagMutex);
#endif /* OPCUA_USE_SYNCHRONISATION */

    UaTestServer_g_gbStopServer = OpcUa_True;
    while(UaTestServer_g_uiShutdownBlocked > 0)
    {

#if OPCUA_USE_SYNCHRONISATION
        OpcUa_Mutex_Unlock(UaTestServer_g_hShutdownFlagMutex);
#endif /* OPCUA_USE_SYNCHRONISATION */

#if OPCUA_MULTITHREADED
        OpcUa_Thread_Sleep(5);
#endif /* OPCUA_MULTITHREADED */

#if OPCUA_USE_SYNCHRONISATION
        OpcUa_Mutex_Lock(UaTestServer_g_hShutdownFlagMutex);
#endif /* OPCUA_USE_SYNCHRONISATION */

    }

#if OPCUA_USE_SYNCHRONISATION
    OpcUa_Mutex_Unlock(UaTestServer_g_hShutdownFlagMutex);
#endif /* OPCUA_USE_SYNCHRONISATION */
}

/*********************************************************************************************/
/***********************     Stack Callbacks and Service Handlers     ************************/
/*********************************************************************************************/

/*===========================================================================================*/
/** @brief Finish request by sending a service fault.                                        */
/*===========================================================================================*/
OpcUa_StatusCode UaTestServer_SendFault(    OpcUa_RequestHeader*    a_pRequestHeader,
                                            OpcUa_ResponseHeader*   a_pResponseHeader,
                                            OpcUa_Endpoint          a_hEndpoint,
                                            OpcUa_Handle*           a_phContext,
                                            OpcUa_StatusCode        a_uStatus)
{
    OpcUa_Void*             pFault              = OpcUa_Null;
    OpcUa_EncodeableType*   pFaultType          = OpcUa_Null;
    OpcUa_DiagnosticInfo*   pServiceDiagnostics = OpcUa_Null;
    OpcUa_Int32             nNoOfStringTable    = 0;
    OpcUa_Int32*            pNoOfStringTable    = &nNoOfStringTable;
    OpcUa_String**          pStringTable        = OpcUa_Null;

OpcUa_InitializeStatus(OpcUa_Module_Server, "SendFault");

    /* check if response already started */
    if(a_pResponseHeader != OpcUa_Null)
    {
        pServiceDiagnostics = &a_pResponseHeader->ServiceDiagnostics;
        pNoOfStringTable    = &a_pResponseHeader->NoOfStringTable;
        pStringTable        = &a_pResponseHeader->StringTable;
    }
    else
    {
        OpcUa_Void*             pResponse           = OpcUa_Null;
        OpcUa_EncodeableType*   pResponseType       = OpcUa_Null;

        /* create new response */
        uStatus = OpcUa_Endpoint_BeginSendResponse(  a_hEndpoint,
                                                    *a_phContext,
                                                    &pResponse,
                                                    &pResponseType);
        OpcUa_ReturnErrorIfBad(uStatus);

        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);
    }

    /* create a fault */
    uStatus = OpcUa_ServerApi_CreateFault(  a_pRequestHeader,
                                            a_uStatus,
                                            pServiceDiagnostics,
                                            pNoOfStringTable,
                                            pStringTable,
                                            &pFault,
                                            &pFaultType);

    if(OpcUa_IsGood(uStatus))
    {
        /* service fault created */
        OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "UaTestServer_SendFault: Sending service fault!\n");

        /* send service fault response */
        uStatus = OpcUa_Endpoint_EndSendResponse(   a_hEndpoint,
                                                    a_phContext,
                                                    OpcUa_Good,
                                                    pFault,
                                                    pFaultType);

        OpcUa_EncodeableObject_Delete(pFaultType, (OpcUa_Void**)&pFault);

        OpcUa_GotoErrorIfBad(uStatus);
    }
    else
    {
        /* creation of fault failed */
        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestServer_SendFault: Can not send service fault!\n");

        /* finish response */
        uStatus = OpcUa_Endpoint_CancelSendResponse(    a_hEndpoint,
                                                        uStatus,
                                                        OpcUa_String_FromCString("UaTestServer_SendFault: Could not send service fault!"),
                                                        a_phContext);
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*===========================================================================================*/
/** @brief Get notifications about secure channel events.                                    */
/*===========================================================================================*/
OpcUa_StatusCode UaTestServer_EndpointCallback(
    OpcUa_Endpoint          a_hEndpoint,
    OpcUa_Void*             a_pCallbackData,
    OpcUa_Endpoint_Event    a_eEvent,
    OpcUa_StatusCode        a_uStatus,
    OpcUa_UInt32            a_uSecureChannelId,
    OpcUa_Handle*           a_phRawRequestContext,
    OpcUa_ByteString*       a_pbsClientCertificate,
    OpcUa_String*           a_pSecurityPolicy,
    OpcUa_UInt16            a_uSecurityMode,
    OpcUa_UInt32            a_uRequestedLifetime)
{
OpcUa_InitializeStatus(OpcUa_Module_Server, "EndpointCallback");

    OpcUa_ReferenceParameter(a_hEndpoint);
    OpcUa_ReferenceParameter(a_pCallbackData);
    OpcUa_ReferenceParameter(a_uSecureChannelId);
    OpcUa_ReferenceParameter(a_phRawRequestContext);
    OpcUa_ReferenceParameter(a_pSecurityPolicy);
    OpcUa_ReferenceParameter(a_pbsClientCertificate);
    OpcUa_ReferenceParameter(a_uSecurityMode);
    OpcUa_ReferenceParameter(a_uStatus);
    OpcUa_ReferenceParameter(a_uRequestedLifetime);

    switch(a_eEvent)
    {
    case eOpcUa_Endpoint_Event_SecureChannelOpened:
        {
            OpcUa_String sPeerInfo = OPCUA_STRING_STATICINITIALIZER;

            if(OpcUa_IsGood(OpcUa_Endpoint_GetPeerInfoBySecureChannelId( a_hEndpoint, a_uSecureChannelId, &sPeerInfo)))
            {
                OpcUa_Trace(    OPCUA_TRACE_LEVEL_SYSTEM,
                                "UaTestServer_EndpointCallback: SecureChannel %i opened with %s in mode %u status 0x%08X from %*.*s!\n",
                                a_uSecureChannelId,
                                (a_pSecurityPolicy)?OpcUa_String_GetRawString(a_pSecurityPolicy):"(not provided)",
                                a_uSecurityMode,
                                a_uStatus,
                                OpcUa_String_StrLen(&sPeerInfo),
                                OpcUa_String_StrLen(&sPeerInfo),
                                OpcUa_String_GetRawString(&sPeerInfo));
            }
            else
            {
                OpcUa_Trace(    OPCUA_TRACE_LEVEL_SYSTEM,
                                "UaTestServer_EndpointCallback: SecureChannel %i opened with %s in mode %u status 0x%08X!\n",
                                a_uSecureChannelId,
                                (a_pSecurityPolicy)?OpcUa_String_GetRawString(a_pSecurityPolicy):"(not provided)",
                                a_uSecurityMode,
                                a_uStatus);
            }

            OpcUa_String_Clear(&sPeerInfo);

            break;
        }
    case eOpcUa_Endpoint_Event_SecureChannelClosed:
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "UaTestServer_EndpointCallback: SecureChannel %i closed with status 0x%08X!\n", a_uSecureChannelId, a_uStatus);
            break;
        }
    case eOpcUa_Endpoint_Event_SecureChannelRenewed:
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "UaTestServer_EndpointCallback: SecureChannel %i renewed!\n", a_uSecureChannelId);
            break;
        }
    case eOpcUa_Endpoint_Event_UnsupportedServiceRequested:
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "UaTestServer_EndpointCallback: SecureChannel %i received request for unsupported service!\n", a_uSecureChannelId);
            break;
        }
    case eOpcUa_Endpoint_Event_DecoderError:
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "UaTestServer_EndpointCallback: SecureChannel %i received a request that could not be decoded! (0x%08X)\n", a_uSecureChannelId, a_uStatus);
            break;
        }
    case eOpcUa_Endpoint_Event_SecureChannelOpenVerifyCertificate:
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "UaTestServer_EndpointCallback: Verification of certificate validation requested! (open, 0x%08X)\n", a_uStatus);
            break;
        }
    case eOpcUa_Endpoint_Event_SecureChannelRenewVerifyCertificate:
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "UaTestServer_EndpointCallback: Verification of certificate validation requested! (renew, 0x%08X)\n", a_uStatus);
            break;
        }
    case eOpcUa_Endpoint_Event_Invalid:
    default:
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "UaTestServer_EndpointCallback: Unknown Endpoint event!\n");
            break;
        }
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

#if UATESTSERVER_SYNC

/*===========================================================================================*/
/** @brief Synchronous service handler.                                                      */
/*===========================================================================================*/
OpcUa_StatusCode UaTestServer_TestStack(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_RequestHeader*  a_pRequestHeader,
    OpcUa_UInt32          a_nTestId,
    OpcUa_UInt32          a_nIteration,
    OpcUa_Variant*        a_pInput,
    OpcUa_ResponseHeader* a_pResponseHeader,
    OpcUa_Variant*        a_pOutput)
{
    OpcUa_UInt32 uSecureChannelId   = 0;
    OpcUa_String sPeerInfo          = OPCUA_STRING_STATICINITIALIZER;

OpcUa_InitializeStatus(OpcUa_Module_Server, "TestStack");

    OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "**** TestStack called! ****\n");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pInput);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pOutput);

    OpcUa_ReferenceParameter(a_nTestId);
    OpcUa_ReferenceParameter(a_nIteration);

    OpcUa_Variant_Initialize(a_pOutput);

    uStatus = OpcUa_Endpoint_GetMessageSecureChannelId(a_hEndpoint, a_hContext, &uSecureChannelId);
    OpcUa_ReturnErrorIfBad(uStatus);

    OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "**** Channel %u! ****\n", uSecureChannelId);

    OpcUa_Endpoint_GetPeerInfoFromContext( a_hEndpoint, a_hContext, &sPeerInfo);

    OpcUa_Trace(    OPCUA_TRACE_LEVEL_INFO,
                    "**** From address %*.*s! ****\n",
                    OpcUa_String_StrLen(&sPeerInfo),
                    OpcUa_String_StrLen(&sPeerInfo),
                    OpcUa_String_GetRawString(&sPeerInfo));

    OpcUa_String_Clear(&sPeerInfo);

    if(     a_pInput->Datatype  == OpcUaType_String
        &&  a_pInput->ArrayType == OpcUa_VariantArrayType_Scalar)
    {
        OpcUa_UInt32 uLen = OpcUa_String_StrLen(&(a_pInput->Value.String));

        if(uLen < 50)
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "**** TestStack Request=%*.*s ****\r\n", uLen, uLen, OpcUa_String_GetRawString(&(a_pInput->Value.String)));
        }
        else
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "**** TestStack Request too large to print! ****\r\n");
        }

        /* set response */
        a_pOutput->Datatype = OpcUaType_String;
        a_pOutput->ArrayType = OpcUa_VariantArrayType_Scalar; /* not really needed, its default */

#if UATESTSERVER_LARGE_RESPONSE
        uStatus = OpcUa_String_AttachToString(
            (OpcUa_StringA)UaTestServer_g_HugeCharArray,
            sizeof(UaTestServer_g_HugeCharArray),
            0,
            OpcUa_False,
            OpcUa_False,
            &a_pOutput->Value.String);
#else
        uStatus = OpcUa_String_AttachToString(
            "World",
            5,
            0,
            OpcUa_True,
            OpcUa_True,
            &a_pOutput->Value.String);
#endif
    }
    else if(    a_pInput->Datatype  == OpcUaType_UInt32
            &&  a_pInput->ArrayType == OpcUa_VariantArrayType_Array)
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "**** TestStack Request has type UInt32 array! ****\r\n");

        /* set response */
        a_pOutput->Datatype = OpcUaType_UInt32;
        a_pOutput->ArrayType = OpcUa_VariantArrayType_Array;

        a_pOutput->Value.Array.Length            = UATESTSERVER_UINT32_ARRAY_SIZE;

        /* values not defined */
        a_pOutput->Value.Array.Value.UInt32Array = (OpcUa_UInt32*)OpcUa_Alloc(sizeof(OpcUa_UInt32)*UATESTSERVER_UINT32_ARRAY_SIZE);
    }

    OpcUa_GotoErrorIfBad(uStatus);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    /* nothing to do */

OpcUa_FinishErrorHandling;
}

#else /* UATESTSERVER_SYNC */

#if UATESTSERVER_DORESPONSEASYNC

/*===========================================================================================*/
/** @brief Check if application is currently being shut down and block if not.               */
/*===========================================================================================*/
OpcUa_Boolean UaTestServer_IsAndBlockShutdown(OpcUa_Void)
{
    OpcUa_Boolean bReturn;

    OpcUa_Mutex_Lock(UaTestServer_g_hShutdownFlagMutex);
    UaTestServer_g_uiShutdownBlocked++;
    bReturn = UaTestServer_g_gbStopServer;
    OpcUa_Mutex_Unlock(UaTestServer_g_hShutdownFlagMutex);

    return bReturn;
}

/*===========================================================================================*/
/** @brief Leave shutdown block for this thread.                                             */
/*===========================================================================================*/
OpcUa_Void UaTestServer_UnblockShutdown(OpcUa_Void)
{
    OpcUa_Mutex_Lock(UaTestServer_g_hShutdownFlagMutex);
    UaTestServer_g_uiShutdownBlocked--;
    OpcUa_Mutex_Unlock(UaTestServer_g_hShutdownFlagMutex);
}

/*===========================================================================================*/
/** @brief Notification about timer deletion.                                                */
/*===========================================================================================*/
OpcUa_StatusCode OPCUA_DLLCALL UaTestServer_DoAsyncResponseTimerKillCB( OpcUa_Void*     a_pvCallbackData,
                                                                        OpcUa_Timer     a_hTimer,
                                                                        OpcUa_UInt32    a_msecElapsed)
{
    UaTestServer_AsyncRequestContext* pAsyncRequestContext = (UaTestServer_AsyncRequestContext*)a_pvCallbackData;

OpcUa_InitializeStatus(OpcUa_Module_Server, "DoAsyncResponseTimerKillCB");

    OpcUa_ReferenceParameter(a_hTimer);
    OpcUa_ReferenceParameter(a_msecElapsed);

    OpcUa_EncodeableObject_Delete(&pAsyncRequestContext->RequestType, (OpcUa_Void**)&pAsyncRequestContext->pRequest);
    OpcUa_Free(a_pvCallbackData);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*===========================================================================================*/
/** @brief Finishes processing of a TestStack service request, triggered by timer.           */
/*===========================================================================================*/
OpcUa_StatusCode OPCUA_DLLCALL UaTestServer_DoAsyncResponseTimerCB( OpcUa_Void*     a_pvCallbackData,
                                                                    OpcUa_Timer     a_hAsyncResponseTimer,
                                                                    OpcUa_UInt32    a_msecElapsed)
{
    UaTestServer_AsyncRequestContext* pAsyncRequestContext = (UaTestServer_AsyncRequestContext*)a_pvCallbackData;
    OpcUa_TestStackResponse*          pResponse            = OpcUa_Null;
    OpcUa_EncodeableType*             pResponseType        = OpcUa_Null;

OpcUa_InitializeStatus(OpcUa_Module_Server, "DoAsyncResponseTimerCB");

    OpcUa_ReferenceParameter(a_msecElapsed);

    /* Check if shut down has been initiated and cancel response. */
    if(UaTestServer_IsAndBlockShutdown() != OpcUa_False)
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "******************** Async response is being cancelled! *************************\n");

        /* send the response */
        OpcUa_Endpoint_CancelSendResponse( pAsyncRequestContext->hEndpoint,
                                           OpcUa_Good,
                                           OpcUa_Null,
                                           &pAsyncRequestContext->hContext);
    }
    else
    {
        /* create a context to use for sending a response */
        uStatus = OpcUa_Endpoint_BeginSendResponse(pAsyncRequestContext->hEndpoint, pAsyncRequestContext->hContext, (OpcUa_Void**)&pResponse, &pResponseType);
        OpcUa_GotoErrorIfBad(uStatus);

        /* set response */
        pResponse->Output.Datatype = OpcUaType_String;
        pResponse->Output.ArrayType = OpcUa_VariantArrayType_Scalar; /* not really needed, its default */

#if UATESTSERVER_LARGE_RESPONSE
        uStatus = OpcUa_String_AttachToString(  (OpcUa_StringA)UaTestServer_g_HugeCharArray,
                                                sizeof(UaTestServer_g_HugeCharArray),
                                                0,
                                                OpcUa_False,
                                                OpcUa_False,
                                                &pResponse->Output.Value.String);
#else
        uStatus = OpcUa_String_AttachToString(  "World",
                                                5,
                                                0,
                                                OpcUa_True,
                                                OpcUa_True,
                                                &pResponse->Output.Value.String);
#endif

        OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "********************** Async response about to be sent! *************************\n");

        /* send the response */
        uStatus = OpcUa_Endpoint_EndSendResponse(   pAsyncRequestContext->hEndpoint,
                                                    &pAsyncRequestContext->hContext,
                                                    OpcUa_Good,
                                                    pResponse,
                                                    pResponseType);

        if(OpcUa_IsBad(uStatus))
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestServer_DoAsyncResponseTimerCB: Response could not be sent! Status 0x%08X!\n", uStatus);
        }
        else
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "************************** Async response sent! *****************************\n");
        }
    }

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);
    OpcUa_Timer_Delete(&a_hAsyncResponseTimer);

    UaTestServer_UnblockShutdown();

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    UaTestServer_SendFault(pAsyncRequestContext->pRequest, (OpcUa_ResponseHeader*)pResponse, pAsyncRequestContext->hEndpoint, &pAsyncRequestContext->hContext, uStatus);

    UaTestServer_UnblockShutdown();

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);
    OpcUa_Timer_Delete(&a_hAsyncResponseTimer);

OpcUa_FinishErrorHandling;
}
#endif /* UATESTSERVER_DORESPONSEASYNC */

/*===========================================================================================*/
/** @brief Begins processing of a TestStack service request.                                 */
/*===========================================================================================*/
OpcUa_StatusCode UaTestServer_BeginTestStack(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_TestStackRequest*                     pRequest                        = OpcUa_Null;

    /* securechannel information */
    OpcUa_UInt32                                uSecureChannelId                = 0;
    OpcUa_Endpoint_SecurityPolicyConfiguration  SecurityPolicyConfiguration;

#if UATESTSERVER_DORESPONSEASYNC
    UaTestServer_AsyncRequestContext*           pAsyncRequestContext            = OpcUa_Null;
    OpcUa_Timer                                 hAsyncResponseTimer             = OpcUa_Null;
#else /* UATESTSERVER_DORESPONSEASYNC */
    OpcUa_TestStackResponse*                    pResponse                       = OpcUa_Null;
    OpcUa_EncodeableType*                       pResponseType                   = OpcUa_Null;
#endif /* UATESTSERVER_DORESPONSEASYNC */

OpcUa_InitializeStatus(OpcUa_Module_Server, "BeginTestStack");

    OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "********************** BeginTestStack called! *************************\n");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_MemSet(&SecurityPolicyConfiguration, 0, sizeof(OpcUa_Endpoint_SecurityPolicyConfiguration));

    /* get the securechannel id */
    uStatus = OpcUa_Endpoint_GetMessageSecureChannelId( a_hEndpoint,
                                                        a_hContext,
                                                        &uSecureChannelId);
    OpcUa_ReturnErrorIfBad(uStatus);

    /* get the security policy */
    uStatus = OpcUa_Endpoint_GetMessageSecureChannelSecurityPolicy( a_hEndpoint,
                                                                    a_hContext,
                                                                    &SecurityPolicyConfiguration);
    OpcUa_ReturnErrorIfBad(uStatus);

    /* print the securechannel information */
    OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "********************** Channel ID %u! **********************\n", uSecureChannelId);
    if(!OpcUa_String_IsEmpty(&SecurityPolicyConfiguration.sSecurityPolicy) && !OpcUa_String_IsNull(&SecurityPolicyConfiguration.sSecurityPolicy))
    {
        OpcUa_UInt32 uLength     = 0;
        OpcUa_CharA* pPolicyName = OpcUa_Null;

        pPolicyName = OpcUa_String_GetRawString(&SecurityPolicyConfiguration.sSecurityPolicy);
        uLength     = OpcUa_String_StrLen(&SecurityPolicyConfiguration.sSecurityPolicy);

        OpcUa_Trace(    OPCUA_TRACE_LEVEL_INFO,
                        "********************** Security Policy URI \"%*.*s\"! **********************\n",
                        uLength,
                        uLength,
                        pPolicyName);
        OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "********************** Security Mode %u! **********************\n", SecurityPolicyConfiguration.uMessageSecurityModes);

        OpcUa_String_Clear(&SecurityPolicyConfiguration.sSecurityPolicy);
    }

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_TestStackRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_TestStackRequest*)*a_ppRequest;

    if(     pRequest->Input.Datatype  == OpcUaType_String
        &&  pRequest->Input.ArrayType == OpcUa_VariantArrayType_Scalar)
    {
        OpcUa_UInt32 uLen = OpcUa_String_StrLen(&(pRequest->Input.Value.String));

        if(uLen < 50)
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "**** TestStack Request=%*.*s ****\r\n", uLen, uLen, OpcUa_String_GetRawString(&(pRequest->Input.Value.String)));
        }
        else
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "**** TestStack Request too large to print! ****\r\n");
        }
    }

#if UATESTSERVER_DORESPONSEASYNC

    pAsyncRequestContext = (UaTestServer_AsyncRequestContext*)OpcUa_Alloc(sizeof(UaTestServer_AsyncRequestContext));
    OpcUa_GotoErrorIfAllocFailed(pAsyncRequestContext);
    OpcUa_MemSet(pAsyncRequestContext, 0, sizeof(UaTestServer_AsyncRequestContext));

    pAsyncRequestContext->hContext                  = a_hContext;
    pAsyncRequestContext->hEndpoint                 = a_hEndpoint;
    pAsyncRequestContext->pRequest                  = *a_ppRequest;
    OpcUa_MemCpy(&pAsyncRequestContext->RequestType, sizeof(OpcUa_EncodeableType), a_pRequestType, sizeof(OpcUa_EncodeableType));
    pAsyncRequestContext->RequestType.NamespaceUri  = OpcUa_Null;
    pAsyncRequestContext->RequestType.TypeName      = OpcUa_Null;


    *a_ppRequest = OpcUa_Null;

    OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "********************** Starting async response task with %u ms delay! *************************\n", UATESTSERVER_ASYNCRESPONSEDELAY);

    uStatus = OpcUa_Timer_Create(   &hAsyncResponseTimer,
                                    UATESTSERVER_ASYNCRESPONSEDELAY,
                                    UaTestServer_DoAsyncResponseTimerCB,
                                    UaTestServer_DoAsyncResponseTimerKillCB,
                                    (OpcUa_Void*)pAsyncRequestContext);

#else /* UATESTSERVER_DORESPONSEASYNC */

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* set response */
    pResponse->Output.Datatype = OpcUaType_String;
    pResponse->Output.ArrayType = OpcUa_VariantArrayType_Scalar; /* not really needed, its default */

#if UATESTSERVER_LARGE_RESPONSE
    uStatus = OpcUa_String_AttachToString(  (OpcUa_StringA)UaTestServer_g_HugeCharArray,
                                            sizeof(UaTestServer_g_HugeCharArray),
                                            0,
                                            OpcUa_False,
                                            OpcUa_False,
                                            &pResponse->Output.Value.String);
#else
    uStatus = OpcUa_String_AttachToString(  "World",
                                            5,
                                            0,
                                            OpcUa_True,
                                            OpcUa_True,
                                            &pResponse->Output.Value.String);
#endif

    /* send the response immediately */
    uStatus = OpcUa_Endpoint_EndSendResponse(   a_hEndpoint,
                                                &a_hContext,
                                                OpcUa_Good,
                                                pResponse,
                                                pResponseType);

    if(OpcUa_IsBad(uStatus))
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "UaTestServer_BeginTestStack: Response could not be sent! Status 0x%08X\n", uStatus);

        /* cleanup resources */
        OpcUa_Endpoint_CancelSendResponse(  a_hEndpoint,
                                            uStatus,
                                            OpcUa_Null,
                                            &a_hContext);
        uStatus = OpcUa_Good;
    }

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

#endif /* UATESTSERVER_DORESPONSEASYNC */

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    OpcUa_Trace(OPCUA_TRACE_LEVEL_INFO, "********************** BeginTestStack failed! *************************\n");

    /* send an error response */
#if !UATESTSERVER_DORESPONSEASYNC
    UaTestServer_SendFault(*a_ppRequest, ((pResponse)?(&pResponse->ResponseHeader):(OpcUa_Null)), a_hEndpoint, &a_hContext, uStatus);
#else /* !UATESTSERVER_DORESPONSEASYNC */
    UaTestServer_SendFault(*a_ppRequest, OpcUa_Null, a_hEndpoint, &a_hContext, uStatus);
#endif /* !UATESTSERVER_DORESPONSEASYNC */
    uStatus = OpcUa_Good; /* call finished */

#if !UATESTSERVER_DORESPONSEASYNC
    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);
#endif /* !UATESTSERVER_DORESPONSEASYNC */

OpcUa_FinishErrorHandling;
}

#endif /* UATESTSERVER_SYNC */

/*********************************************************************************************/
/***********************            Server Main Function              ************************/
/*********************************************************************************************/

/*===========================================================================================*/
/** @brief Endpoint handling.                                                                */
/*===========================================================================================*/
OpcUa_StatusCode UaTestServer_Serve(OpcUa_Void)
{
    OpcUa_Endpoint      hEndpoint   = OpcUa_Null;
    OpcUa_StringA       sUrl        = UATESTSERVER_ENDPOINT_URL;

OpcUa_InitializeStatus(OpcUa_Module_Server, "Serve");

    UaTestServer_TestStackService.ResponseType = &OpcUa_TestStackResponse_EncodeableType;

    /* initialize endpoint */
    uStatus = OpcUa_Endpoint_Create(    &hEndpoint,
                                        OpcUa_Endpoint_SerializerType_Binary,
                                        UaTestServer_SupportedServices);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "********************** Starting Server! *************************\n");

    /* open endpoint */
    uStatus = OpcUa_Endpoint_Open(  hEndpoint,                                      /* Endpoint Instance        */
                                    sUrl,                                           /* Endpoint URL             */
                                    UATESTSERVER_TRANSPORT_PROFILE_URI,             /* The transport profile    */
                                    UaTestServer_EndpointCallback,                  /* Endpoint Callback        */
                                    OpcUa_Null,                                     /* Endpoint Callback Data   */
#if UATESTSERVER_SUPPORT_SECURE_COMMUNICATION
                                    &UaTestServer_g_ServerCertificate,              /* Server Certificate       */
                                    &UaTestServer_g_ServerPrivateKey,               /* Private Key              */
#else /* UATESTSERVER_SUPPORT_SECURE_COMMUNICATION */
                                    OpcUa_Null,                                     /* Server Certificate       */
                                    OpcUa_Null,                                     /* Private Key              */
#endif /* UATESTSERVER_SUPPORT_SECURE_COMMUNICATION */
                                    &UaTestServer_g_PkiConfig,                      /* PKI Configuration        */
                                    UaTestServer_g_uNoOfSecurityPolicies,           /* NoOf SecurityPolicies    */
                                    UaTestServer_g_pSecurityPolicyConfigurations);  /* SecurityPolicies         */
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "********************** Server started! *************************\n");

    /******************************************************************************/
    /* Wait for user command to terminate the server thread.                      */
    /* While looping here, server is active.                                      */
    /* When in SingleThread config, we need to drive the socket event             */
    /* loop to keep message and timer processing active.                          */
    /******************************************************************************/
    /**/ while(!UaTestServer_CheckForKeypress())                                /**/
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

    OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "********************** Stopping Server! ************************\n");

    /* wait for other threads to stop */
    UaTestServer_SetAndWaitShutdown();

    /* close endpoint */
    uStatus = OpcUa_Endpoint_Close(hEndpoint);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_Trace(OPCUA_TRACE_LEVEL_SYSTEM, "********************** Server stopped! *************************\n");

    /* Clean up */
    OpcUa_Endpoint_Delete(&hEndpoint);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    /* Clean up comm */
    OpcUa_Endpoint_Delete(&hEndpoint);

OpcUa_FinishErrorHandling;
}

/*********************************************************************************************/
/***********************        Application Main Entry Point          ************************/
/*********************************************************************************************/

/*===========================================================================================*/
/** @brief Main entry function.                                                              */
/*===========================================================================================*/
int main(void)
{
    OpcUa_StatusCode    uStatus     = OpcUa_Good;

#if UATESTSERVER_USE_CRTDBG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    /*_CrtSetBreakAlloc(2499);*/
#endif

    /* Initialize Stack */
    uStatus = UaTestServer_Initialize();
    if(OpcUa_IsBad(uStatus))
    {
        printf("Could not initialize application!\n");
        OpcUa_GotoError;
    }

    uStatus = UaTestServer_Serve();

    /* Clean up Base */
    UaTestServer_Clear();

#if UATESTSERVER_WAIT_FOR_USER_INPUT
    printf("Shutdown complete!\nPress enter to exit!\n");
    getchar();
#endif

    return (int)uStatus;

Error:

#if UATESTSERVER_WAIT_FOR_USER_INPUT
    printf("Couldn't start server!\nPress enter to exit!\n");
    getchar();
#endif

    /* Clean up Base */
    UaTestServer_Clear();

    return (int)uStatus;
}

/*********************************************************************************************/
/***********************                End Of File                   ************************/
/*********************************************************************************************/
