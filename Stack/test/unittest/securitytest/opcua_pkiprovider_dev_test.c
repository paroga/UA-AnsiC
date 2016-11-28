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

#include <opcua.h>
#include <opcua_core.h>
#include <opcua_pki.h>
#include <opcua_p_pki.h>
#include <opcua_pkifactory.h>
#include <opcua_pkiprovider_dev_test.h>

/* for test output */
#include <stdio.h>

#define OpcUa_P_OpenSSL_CA_FILE         "..\\..\\PKI\\CA\\certs\\ua_untrusted_root.der"
//#define OpcUa_P_OpenSSL_CERT_FILE       "D:\\Projects\\OPC UA\\OPC UA PKI\\CA\\certs\\ua_untrusted_root.der"
//#define OpcUa_P_OpenSSL_CERT_FILE       "D:\\OPC UA\\OPC UA PKI\\CA\\newcerts\\07.pem"
#define OpcUa_P_OpenSSL_CERT_FILE       "..\\..\\PKI\\CA\\certs\\uaserver_untrustedRootCert.der"
/* can be read (Base64 encoded) */
//#define OpcUa_P_OpenSSL_CERT_FILE       "D:\\Projects\\OPC UA\\temp\\MSRoot_Base64.cer"

/* cannot be read (PKCS #7) */
//#define OpcUa_P_OpenSSL_CERT_FILE       "D:\\Projects\\OPC UA\\temp\\MSRoot_PKCS7.p7b"
/* cannot be read (DER encoded) */
//#define OpcUa_P_OpenSSL_CERT_FILE       "D:\\Projects\\OPC UA\\temp\\MSRoot_DER.cer"

#define OpcUa_P_OpenSSL_CA_FILE1        "..\\..\\PKI\\CA\\certs\\ua_untrusted_root1.der"
#define OpcUa_P_OpenSSL_CA_DIR          "..\\..\\PKI\\CA\\certs\\"
#define OpcUa_P_OpenSSL_STORE_DIR       "..\\..\\PKI\\CA\\"
#define OpcUa_P_OpenSSL_CLR_FILE        "..\\..\\PKI\\CA\\crl\\examplecrl.crl"

/*============================================================================
 * OpcUa_Test_OpenCertificateStore
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_OpenCertificateStore()
{
    OpcUa_P_OpenSSL_CertificateStore_Config*    pCertificateStoreCfg    = OpcUa_Null;
    OpcUa_PKIProvider                           myPKIProvider;
    OpcUa_Void*                                 pCertificateStore       = OpcUa_Null;
    OpcUa_StatusCode                            uStatus                 = OpcUa_Good;

    printf("\nOpenCertificateStore");

    pCertificateStoreCfg = (OpcUa_P_OpenSSL_CertificateStore_Config*)OpcUa_Alloc(sizeof(OpcUa_P_OpenSSL_CertificateStore_Config));

    pCertificateStoreCfg->PkiType                           = OpcUa_OpenSSL_PKI;
    pCertificateStoreCfg->CertificateRevocationListLocation = OpcUa_P_OpenSSL_CLR_FILE;
    pCertificateStoreCfg->CertificateTrustListLocation      = OpcUa_P_OpenSSL_CA_DIR;

    uStatus = OpcUa_PKIProvider_Create(pCertificateStoreCfg, &myPKIProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myPKIProvider.OpenCertificateStore(&myPKIProvider, &pCertificateStore);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_PKIProvider_Delete(&myPKIProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_CloseCertificateStore
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CloseCertificateStore()
{
    OpcUa_P_OpenSSL_CertificateStore_Config*    pCertificateStoreCfg    = OpcUa_Null;
    OpcUa_PKIProvider                           myPKIProvider;
    OpcUa_Void*                                 pCertificateStore       = OpcUa_Null;
    OpcUa_StatusCode                            uStatus                 = OpcUa_Good;

    printf("\nCloseCertificateStore");

    pCertificateStoreCfg = (OpcUa_P_OpenSSL_CertificateStore_Config*)OpcUa_Alloc(sizeof(OpcUa_P_OpenSSL_CertificateStore_Config));

    pCertificateStoreCfg->PkiType                           = OpcUa_OpenSSL_PKI;
    pCertificateStoreCfg->CertificateRevocationListLocation = OpcUa_P_OpenSSL_CLR_FILE;
    pCertificateStoreCfg->CertificateTrustListLocation      = OpcUa_P_OpenSSL_CA_DIR;

    uStatus = OpcUa_PKIProvider_Create(pCertificateStoreCfg, &myPKIProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myPKIProvider.OpenCertificateStore(&myPKIProvider, &pCertificateStore);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myPKIProvider.CloseCertificateStore(&myPKIProvider, &pCertificateStore);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_PKIProvider_Delete(&myPKIProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_LoadCertificateFromStore
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_LoadCertificateFromStore()
{
    OpcUa_P_OpenSSL_CertificateStore_Config*    pCertificateStoreCfg    = OpcUa_Null;
    OpcUa_PKIProvider                           myPKIProvider;
    OpcUa_ByteString                            Certificate             = {0, OpcUa_Null};
    OpcUa_Void*                                 pCertificateStore       = OpcUa_Null;
    OpcUa_StatusCode                            uStatus                 = OpcUa_Good;

    printf("\nLoadCertificateFromStore");

    pCertificateStoreCfg = (OpcUa_P_OpenSSL_CertificateStore_Config*)OpcUa_Alloc(sizeof(OpcUa_P_OpenSSL_CertificateStore_Config));

    pCertificateStoreCfg->PkiType                           = OpcUa_OpenSSL_PKI;
    pCertificateStoreCfg->CertificateRevocationListLocation = OpcUa_P_OpenSSL_CLR_FILE;
    pCertificateStoreCfg->CertificateTrustListLocation      = OpcUa_P_OpenSSL_CA_DIR;

    uStatus = OpcUa_PKIProvider_Create(pCertificateStoreCfg, &myPKIProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myPKIProvider.OpenCertificateStore(&myPKIProvider, &pCertificateStore);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myPKIProvider.LoadCertificate(&myPKIProvider, (OpcUa_Void*)OpcUa_P_OpenSSL_CERT_FILE, pCertificateStore, &Certificate);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myPKIProvider.CloseCertificateStore(&myPKIProvider, &pCertificateStore);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_PKIProvider_Delete(&myPKIProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_SaveCertificateIntoStore
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_SaveCertificateIntoStore()
{
    OpcUa_P_OpenSSL_CertificateStore_Config*    pCertificateStoreCfg    = OpcUa_Null;
    OpcUa_PKIProvider                           myPKIProvider;
#if 0 /* inactive code */
    OpcUa_ByteString*                           pCertificate            = OpcUa_Null;
#endif
    OpcUa_Void*                                 pCertificateStore       = OpcUa_Null;
    OpcUa_StatusCode                            uStatus                 = OpcUa_Good;

    printf("\nSaveCertificateIntoStore");

    pCertificateStoreCfg = (OpcUa_P_OpenSSL_CertificateStore_Config*)OpcUa_Alloc(sizeof(OpcUa_P_OpenSSL_CertificateStore_Config));

    pCertificateStoreCfg->PkiType                           = OpcUa_OpenSSL_PKI;
    pCertificateStoreCfg->CertificateRevocationListLocation = OpcUa_P_OpenSSL_CLR_FILE;
    pCertificateStoreCfg->CertificateTrustListLocation      = OpcUa_P_OpenSSL_CA_DIR;

    uStatus = OpcUa_PKIProvider_Create(pCertificateStoreCfg, &myPKIProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myPKIProvider.OpenCertificateStore(&myPKIProvider, &pCertificateStore);
    OpcUa_GotoErrorIfBad(uStatus);

#if 0
    uStatus = myPKIProvider.SaveCertificate(&myPKIProvider, pCertificate, pCertificateStore, (OpcUa_Void*)OpcUa_P_OpenSSL_CA_FILE1);
    OpcUa_GotoErrorIfBad(uStatus);
#endif

    uStatus = myPKIProvider.CloseCertificateStore(&myPKIProvider, &pCertificateStore);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_PKIProvider_Delete(&myPKIProvider);
    OpcUa_GotoErrorIfBad(uStatus);
#if 1
    printf("...not implemented!");
    uStatus = OpcUa_BadNotImplemented;

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
#else
    printf("...passed!");

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;

#endif
}


OpcUa_StatusCode OpcUa_Test_ValidateCertificate()
{
    OpcUa_P_OpenSSL_CertificateStore_Config*    pCertificateStoreCfg    = OpcUa_Null;
    OpcUa_PKIProvider                           myPKIProvider;
    OpcUa_Void*                                 pCertificateStore       = OpcUa_Null;
#if 0 /* inactive code */
    OpcUa_ByteString*                           pCertificate            = OpcUa_Null;
    OpcUa_Int                                   validationCode          = -1;
#endif
    OpcUa_StatusCode                            uStatus                 = OpcUa_Good;

    printf("\nValidateCertificate");

    pCertificateStoreCfg = (OpcUa_P_OpenSSL_CertificateStore_Config*)OpcUa_Alloc(sizeof(OpcUa_P_OpenSSL_CertificateStore_Config));

    pCertificateStoreCfg->PkiType                           = OpcUa_OpenSSL_PKI;
    pCertificateStoreCfg->CertificateRevocationListLocation = OpcUa_P_OpenSSL_CLR_FILE;
    pCertificateStoreCfg->CertificateTrustListLocation      = OpcUa_P_OpenSSL_CA_DIR;

    uStatus = OpcUa_PKIProvider_Create(pCertificateStoreCfg, &myPKIProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myPKIProvider.OpenCertificateStore(&myPKIProvider, &pCertificateStore);
    OpcUa_GotoErrorIfBad(uStatus);

    /* ToDo: get certificate */

#if 0
    uStatus = myPKIProvider.ValidateCertificate(&myPKIProvider, pCertificate, pCertificateStore, &validationCode);
#endif

    uStatus = myPKIProvider.CloseCertificateStore(&myPKIProvider, &pCertificateStore);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_PKIProvider_Delete(&myPKIProvider);
    OpcUa_GotoErrorIfBad(uStatus);
#if 1
    printf("...not implemented!");
    uStatus = OpcUa_BadNotImplemented;

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

        printf("...failed!");

OpcUa_FinishErrorHandling;

#else
    printf("...passed!");

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;

#endif
}
