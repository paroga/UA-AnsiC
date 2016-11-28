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
#include <opcua_pkiprovider_test.h>

/* for test output */
#include <stdio.h>
#include <securitytest.h>
#include "unittest.h"

/*============================================================================
 * OpcUa_Test_PKIByteStringDelete
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_PKIByteStringDelete(OpcUa_ByteString **pByte, OpcUa_Boolean bAssert)
{
    if(!pByte)
    {
        return OpcUa_BadInvalidArgument;
    }

    if(bAssert)
    {
        ASSERT(*pByte);
    }

    if(*pByte)
    {
        if(bAssert)
        {
            ASSERT((*pByte)->Data);
        }

        if((*pByte)->Data)
        {
            OpcUa_Free((*pByte)->Data);
            (*pByte)->Data = OpcUa_Null;
        }

        OpcUa_Free(*pByte);
        *pByte = OpcUa_Null;
    }

    return OpcUa_Good;
}

/*============================================================================
 * OpcUa_Test_PKIByteStringClear
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_PKIByteStringClear(OpcUa_ByteString *pByte, OpcUa_Boolean bAssert)
{
    if(pByte)
    {
        if (bAssert)
        {
            ASSERT(pByte->Data);
        }

        if(pByte->Data)
        {
            OpcUa_Free(pByte->Data);
            pByte->Data = OpcUa_Null;
        }

        pByte->Length = -1;
    }

    return OpcUa_Good;
}

/*============================================================================
 * OpcUa_Test_PKIProvider_OpenCertificateStore
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_PKIProvider_OpenCertificateStore(OpcUa_CertificateStoreConfiguration* a_pStoreConfig)
{
    OpcUa_PKIProvider   myPKIProvider;
    SNAPSHOT_TYPE       s1, s2, s3;
    OpcUa_Void*         pCertificateStore = OpcUa_Null;

OpcUa_InitializeStatus(OpcUa_Module_TestModule, "OpcUa_Test_PKIProvider_OpenCertificateStore");

    uStatus = OpcUa_PKIProvider_Create(a_pStoreConfig, &myPKIProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    /* empty comment */
    /* Basic test - all parameters are good */
    /* empty comment */
    CASE_ID(39500);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(myPKIProvider.OpenCertificateStore(&myPKIProvider, &pCertificateStore));
    ASSERT(pCertificateStore);
    END_CASE();

    /* empty comment */
    /* Basic test - all parameters are good */
    /* empty comment */
    CASE_ID(39501);
    ASSERT_GOOD(myPKIProvider.CloseCertificateStore(&myPKIProvider, &pCertificateStore));
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(39502);
    ASSERT_STATUS(myPKIProvider.OpenCertificateStore(OpcUa_Null, &pCertificateStore), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Certificate Store */
    /* empty comment */
    CASE_ID(39503);
    ASSERT_STATUS(myPKIProvider.OpenCertificateStore(&myPKIProvider, OpcUa_Null), OpcUa_BadInvalidArgument);
    END_CASE();

    OpcUa_PKIProvider_Delete(&myPKIProvider);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_PKIProvider_CloseCertificateStore
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_PKIProvider_CloseCertificateStore(OpcUa_CertificateStoreConfiguration* a_pStoreConfig)
{
    OpcUa_PKIProvider   myPKIProvider;
    OpcUa_Void*         pCertificateStore = OpcUa_Null;

OpcUa_InitializeStatus(OpcUa_Module_TestModule, "OpcUa_Test_PKIProvider_CloseCertificateStore");

    uStatus = OpcUa_PKIProvider_Create(a_pStoreConfig, &myPKIProvider);
    OpcUa_GotoErrorIfBad(uStatus);
    uStatus = myPKIProvider.OpenCertificateStore(&myPKIProvider, &pCertificateStore);
    OpcUa_GotoErrorIfBad(uStatus);

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(39504);
    ASSERT_STATUS(myPKIProvider.CloseCertificateStore(OpcUa_Null, &pCertificateStore), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Certificate Store */
    /* empty comment */
    CASE_ID(39505);
    ASSERT_STATUS(myPKIProvider.CloseCertificateStore(&myPKIProvider, OpcUa_Null), OpcUa_BadInvalidArgument);
    END_CASE();

    myPKIProvider.CloseCertificateStore(&myPKIProvider, &pCertificateStore);
    OpcUa_PKIProvider_Delete(&myPKIProvider);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_PKIProvider_LoadCertificate
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_PKIProvider_LoadCertificate(OpcUa_CertificateStoreConfiguration* a_pStoreConfig)
{
    OpcUa_PKIProvider   myPKIProvider;
    SNAPSHOT_TYPE       s1, s2, s3;
    OpcUa_Void*         pCertificateStore = OpcUa_Null;
    OpcUa_ByteString    Certificate       = OPCUA_BYTESTRING_STATICINITIALIZER;

OpcUa_InitializeStatus(OpcUa_Module_TestModule, "OpcUa_Test_PKIProvider_LoadCertificate");

    uStatus = OpcUa_PKIProvider_Create(a_pStoreConfig, &myPKIProvider);
    OpcUa_GotoErrorIfBad(uStatus);
    uStatus = myPKIProvider.OpenCertificateStore(&myPKIProvider, &pCertificateStore);
    OpcUa_GotoErrorIfBad(uStatus);

    /* empty comment */
    /* Basic test - All Parameters Good */
    /* empty comment */
    CASE_ID(39506);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(myPKIProvider.LoadCertificate(&myPKIProvider, (OpcUa_Void*)OPCUA_SECURITY_TEST_CERT_FILE, pCertificateStore, &Certificate));
    ASSERT(&Certificate.Data);
    OpcUa_Test_PKIByteStringClear(&Certificate, OpcUa_True);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(39507);
    ASSERT_STATUS(myPKIProvider.LoadCertificate(OpcUa_Null, (OpcUa_Void*)OPCUA_SECURITY_TEST_CERT_FILE, pCertificateStore, &Certificate), OpcUa_BadInvalidArgument);
    OpcUa_Test_PKIByteStringClear(&Certificate, OpcUa_False);
    END_CASE();

    /* empty comment */
    /* Basic test - Null File */
    /* empty comment */
    CASE_ID(39508);
    ASSERT_STATUS(myPKIProvider.LoadCertificate(&myPKIProvider, OpcUa_Null, pCertificateStore, &Certificate), OpcUa_BadInvalidArgument);
    OpcUa_Test_PKIByteStringClear(&Certificate, OpcUa_False);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Certificate Store */
    /* empty comment */
    CASE_ID(39509);
    ASSERT_STATUS(myPKIProvider.LoadCertificate(&myPKIProvider, (OpcUa_Void*)OPCUA_SECURITY_TEST_CERT_FILE, OpcUa_Null, &Certificate), OpcUa_BadInvalidArgument);
    OpcUa_Test_PKIByteStringClear(&Certificate, OpcUa_False);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Certificate Pointer */
    /* empty comment */
    CASE_ID(39510);
    ASSERT_STATUS(myPKIProvider.LoadCertificate(&myPKIProvider, (OpcUa_Void*)OPCUA_SECURITY_TEST_CERT_FILE, pCertificateStore, OpcUa_Null), OpcUa_BadInvalidArgument);
    /*OpcUa_ByteString_Clear(&Certificate);*/
    END_CASE();

    myPKIProvider.CloseCertificateStore(&myPKIProvider, &pCertificateStore);
    OpcUa_PKIProvider_Delete(&myPKIProvider);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_PKIProvider_ValidateCertificate
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_PKIProvider_ValidateCertificate(OpcUa_CertificateStoreConfiguration* a_pStoreConfig)
{
    OpcUa_PKIProvider   myPKIProvider;
    SNAPSHOT_TYPE       s1, s2, s3;
    OpcUa_Void*         pCertificateStore   = OpcUa_Null;
    OpcUa_ByteString    Certificate         = OPCUA_BYTESTRING_STATICINITIALIZER;
    OpcUa_Int           iValidationCode     = 0;

OpcUa_InitializeStatus(OpcUa_Module_TestModule, "OpcUa_Test_PKIProvider_ValidateCertificate");

    uStatus = OpcUa_PKIProvider_Create(a_pStoreConfig, &myPKIProvider);
    OpcUa_GotoErrorIfBad(uStatus);
    uStatus = myPKIProvider.OpenCertificateStore(&myPKIProvider, &pCertificateStore);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myPKIProvider.LoadCertificate(&myPKIProvider, (OpcUa_Void*)OPCUA_SECURITY_TEST_CERT_FILE, pCertificateStore, &Certificate);
    if(uStatus != OpcUa_Good)
    {
        myPKIProvider.CloseCertificateStore(&myPKIProvider, &pCertificateStore);
        OpcUa_GotoErrorIfBad(uStatus);
    }

    /* empty comment */
    /* Basic test - All Parameters Good */
    /* empty comment */
    CASE_ID(39515);
    myPKIProvider.ValidateCertificate(&myPKIProvider, &Certificate, pCertificateStore, &iValidationCode);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(myPKIProvider.ValidateCertificate(&myPKIProvider, &Certificate, pCertificateStore, &iValidationCode));
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(39516);
    ASSERT_STATUS(myPKIProvider.ValidateCertificate(OpcUa_Null, &Certificate, pCertificateStore, &iValidationCode), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Certificate */
    /* empty comment */
    CASE_ID(39517);
    ASSERT_STATUS(myPKIProvider.ValidateCertificate(&myPKIProvider, OpcUa_Null, pCertificateStore, &iValidationCode), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Certificate Store */
    /* empty comment */
    CASE_ID(39518);
    ASSERT_STATUS(myPKIProvider.ValidateCertificate(&myPKIProvider, &Certificate, OpcUa_Null, &iValidationCode), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Validation Code */
    /* empty comment */
    CASE_ID(39519);
    ASSERT_STATUS(myPKIProvider.ValidateCertificate(&myPKIProvider, &Certificate, pCertificateStore, OpcUa_Null), OpcUa_BadInvalidArgument);
    END_CASE();

    OpcUa_Test_PKIByteStringClear(&Certificate, OpcUa_False);
    myPKIProvider.CloseCertificateStore(&myPKIProvider, &pCertificateStore);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_PKIProvider_SaveCertificate
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_PKIProvider_SaveCertificate(OpcUa_CertificateStoreConfiguration* a_pStoreConfig)
{
/* *** Note: The OpcUa_P_OpenSSL_PKI_SaveCertificate() calls d2i_X509() which is corrupting the */
/*     buffer used by the Certificate structure - Which means we can't reuse it, and we can't */
/*     free the buffer as the CRT will crash. */
/* empty comment */
    OpcUa_PKIProvider   myPKIProvider;
    SNAPSHOT_TYPE       s1, s2, s3;
    OpcUa_Void*         pCertificateStore   = OpcUa_Null;
    OpcUa_ByteString    Certificate         = OPCUA_BYTESTRING_STATICINITIALIZER;

OpcUa_InitializeStatus(OpcUa_Module_TestModule, "OpcUa_Test_PKIProvider_SaveCertificate");

    uStatus = OpcUa_PKIProvider_Create(a_pStoreConfig, &myPKIProvider);
    OpcUa_GotoErrorIfBad(uStatus);
    uStatus = myPKIProvider.OpenCertificateStore(&myPKIProvider, &pCertificateStore);
    OpcUa_GotoErrorIfBad(uStatus);
    uStatus = myPKIProvider.LoadCertificate(&myPKIProvider, (OpcUa_Void*)OPCUA_SECURITY_TEST_CERT_FILE, pCertificateStore, &Certificate);
    OpcUa_GotoErrorIfBad(uStatus);

    /* empty comment */
    /* Basic test - All Parameters Good */
    /* empty comment */
    CASE_ID(39511);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(myPKIProvider.SaveCertificate(&myPKIProvider, &Certificate, pCertificateStore, (OpcUa_Void*)OPCUA_SECURITY_TEST_CERT_FILE_SAVE));
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(39512);
    ASSERT_STATUS(myPKIProvider.SaveCertificate(OpcUa_Null, &Certificate, pCertificateStore, (OpcUa_Void*)OPCUA_SECURITY_TEST_CERT_FILE_SAVE), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Certificate */
    /* empty comment */
    CASE_ID(39512);
    ASSERT_STATUS(myPKIProvider.SaveCertificate(&myPKIProvider, OpcUa_Null, pCertificateStore, (OpcUa_Void*)OPCUA_SECURITY_TEST_CERT_FILE_SAVE), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Certificate Store */
    /* empty comment */
    CASE_ID(39513);
    ASSERT_STATUS(myPKIProvider.SaveCertificate(&myPKIProvider, &Certificate, OpcUa_Null, (OpcUa_Void*)OPCUA_SECURITY_TEST_CERT_FILE_SAVE), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null File */
    /* empty comment */
    CASE_ID(39514);
    ASSERT_STATUS(myPKIProvider.SaveCertificate(&myPKIProvider, &Certificate, pCertificateStore, OpcUa_Null), OpcUa_BadInvalidArgument);
    END_CASE();

    OpcUa_Test_PKIByteStringClear(&Certificate, OpcUa_False);
    myPKIProvider.CloseCertificateStore(&myPKIProvider, &pCertificateStore);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_Bad_PKIProvider
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_PKIProvider(OpcUa_StringA sConfig)
{
    OpcUa_CertificateStoreConfiguration CertificateStoreCfg;

OpcUa_InitializeStatus(OpcUa_Module_TestModule, "Test_PKIProvider");

    OpcUa_ReferenceParameter(sConfig);
    /* printf("\nTest PKIProvider - 39500\n"); */

    OpcUa_CertificateStoreConfiguration_Initialize(&CertificateStoreCfg);
    CertificateStoreCfg.strPkiType                        = OPCUA_P_PKI_TYPE_OPENSSL;
    CertificateStoreCfg.strRevokedCertificateListLocation = OPCUA_SECURITY_TEST_CLR_DIR;
    CertificateStoreCfg.strTrustedCertificateListLocation = OPCUA_SECURITY_TEST_CA_DIR;

    OpcUa_Test_PKIProvider_OpenCertificateStore(&CertificateStoreCfg);
    OpcUa_Test_PKIProvider_CloseCertificateStore(&CertificateStoreCfg);
    OpcUa_Test_PKIProvider_LoadCertificate(&CertificateStoreCfg);
    OpcUa_Test_PKIProvider_ValidateCertificate(&CertificateStoreCfg);
    OpcUa_Test_PKIProvider_SaveCertificate(&CertificateStoreCfg);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}
