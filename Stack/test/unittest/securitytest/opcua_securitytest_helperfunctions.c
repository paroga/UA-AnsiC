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

/* platform specific */
#include <stdio.h>

#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

#include <securitytest.h>
#include <opcua_securitytest_helperfunctions.h>

/*** HELPER FUNCTIONS ***/

/*============================================================================
 * OpcUa_TestHelper_GenerateLowSecurityCryptoPolicy
 *===========================================================================*/
OpcUa_StatusCode OpcUa_TestHelper_CleanupCryptoPolicy(OpcUa_String* a_pCryptoPolicy)
{
    OpcUa_ReferenceParameter(a_pCryptoPolicy);
    return(OpcUa_Good);
}

/*============================================================================
 * OpcUa_TestHelper_GenerateLowSecurityCryptoPolicy
 *===========================================================================*/
OpcUa_StatusCode OpcUa_TestHelper_GenerateLowSecurityCryptoPolicy(OpcUa_StringA a_pUri, OpcUa_String* a_pCryptoPolicy)
{
    OpcUa_StatusCode uStatus    = OpcUa_Good;

    OpcUa_ReturnErrorIfArgumentNull(a_pUri);
    OpcUa_ReturnErrorIfArgumentNull(a_pCryptoPolicy);

    uStatus = OpcUa_BadNotImplemented;

    return uStatus;
}

/*============================================================================
 * OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy
 *===========================================================================*/
OpcUa_StatusCode OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_StringA a_pUri, OpcUa_String* a_pCryptoPolicy)
{
    OpcUa_StatusCode uStatus    = OpcUa_Good;

    OpcUa_ReturnErrorIfArgumentNull(a_pUri);
    OpcUa_ReturnErrorIfArgumentNull(a_pCryptoPolicy);

    OpcUa_String_Initialize(a_pCryptoPolicy);

    /*** Generate SecurityPolicy ***/
    uStatus = OpcUa_String_AttachToString(  a_pUri,
                                            OpcUa_StrLenA(a_pUri),
                                            0,
                                            OpcUa_False,
                                            OpcUa_False,
                                            a_pCryptoPolicy);

    return uStatus;
}

/*============================================================================
 * OpcUa_TestHelper_GenerateHighSecurityCryptoPolicy
 *===========================================================================*/
OpcUa_StatusCode OpcUa_TestHelper_GenerateHighSecurityCryptoPolicy(OpcUa_StringA a_pUri, OpcUa_String* a_pCryptoPolicy)
{
    OpcUa_StatusCode uStatus    = OpcUa_Good;

    OpcUa_ReturnErrorIfArgumentNull(a_pUri);
    OpcUa_ReturnErrorIfArgumentNull(a_pCryptoPolicy);

    uStatus = OpcUa_BadNotImplemented;

    return uStatus;
}

/*============================================================================
 * OpcUa_TestHelper_GenerateNoSecurityCryptoPolicy
 *===========================================================================*/
OpcUa_StatusCode OpcUa_TestHelper_GenerateNoSecurityCryptoPolicy(OpcUa_StringA a_pUri, OpcUa_String* a_pCryptoPolicy)
{
    OpcUa_StatusCode uStatus    = OpcUa_Good;

    OpcUa_ReturnErrorIfArgumentNull(a_pUri);
    OpcUa_ReturnErrorIfArgumentNull(a_pCryptoPolicy);

    OpcUa_String_Initialize(a_pCryptoPolicy);

    /*** Generate SecurityPolicy ***/
    uStatus = OpcUa_String_AttachToString(  OpcUa_SecurityPolicy_Basic128Rsa15,
                                            OpcUa_StrLenA(OpcUa_SecurityPolicy_Basic128Rsa15),
                                            0,
                                            OpcUa_False,
                                            OpcUa_False,
                                            a_pCryptoPolicy);

    return uStatus;
}

/*============================================================================
 * OpcUa_TestHelper_GetDEREncodedPrivateKeyFromFile
 *===========================================================================*/
/* ToDo: These functions will move into the PKI interface of OPC UA */
OpcUa_StatusCode OpcUa_TestHelper_GetDEREncodedPrivateKeyFromFile(  OpcUa_StringA   a_sPrivateKeyFile,
                                                                    OpcUa_Key*      a_pPrivateKey)
{
    OpcUa_CertificateStoreConfiguration CertificateStoreCfg;
    OpcUa_PKIProvider                    myPKIProvider;

OpcUa_InitializeStatus(OpcUa_Module_TestModule, "TestHelper_GetDEREncodedPrivateKeyFromFile");

    OpcUa_CertificateStoreConfiguration_Initialize(&CertificateStoreCfg);
    CertificateStoreCfg.strPkiType                         = OPCUA_P_PKI_TYPE_OPENSSL;
    CertificateStoreCfg.strRevokedCertificateListLocation  = OPCUA_SECURITY_TEST_CLR_DIR;
    CertificateStoreCfg.strTrustedCertificateListLocation  = OPCUA_SECURITY_TEST_CA_DIR;

    OpcUa_Key_Initialize(a_pPrivateKey);

    uStatus = OpcUa_PKIProvider_Create(&CertificateStoreCfg, &myPKIProvider);
    if(OpcUa_IsBad(uStatus))
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Could not create PKI provider.\n");
        return uStatus;
    }

    uStatus = myPKIProvider.LoadPrivateKeyFromFile( a_sPrivateKeyFile,
                                                    OpcUa_Crypto_Encoding_PEM,
                                                    OpcUa_Null,
                                                   &a_pPrivateKey->Key);
    if(OpcUa_IsBad(uStatus))
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Could not load private key from file %s.\n", a_sPrivateKeyFile);
        OpcUa_GotoError;
    }

    a_pPrivateKey->Type = OpcUa_Crypto_KeyType_Rsa_Private;
    OpcUa_PKIProvider_Delete(&myPKIProvider);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    OpcUa_Key_Clear(a_pPrivateKey);
    OpcUa_PKIProvider_Delete(&myPKIProvider);

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_TestHelper_GetDEREncodedCertificateFromFile
 *===========================================================================*/
OpcUa_StatusCode OpcUa_TestHelper_GetDEREncodedCertificateFromFile(OpcUa_StringA a_sCertificateFile, OpcUa_ByteString* a_pCertificate)
{
    OpcUa_StatusCode    uStatus     = OpcUa_Good;

#if 1 /* TODO: replace this implementation with something platform independent (use PKIProvider API) */

    OpcUa_Byte*         buf         = NULL;
    OpcUa_Byte*         p           = NULL;
    FILE*               pfCert      = NULL;
    X509*               pTmpCert    = NULL;

    /*** Get certificate ***/
    pfCert = fopen(a_sCertificateFile, "r");
    if(pfCert == NULL)
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Could not open %s\n", a_sCertificateFile);
        return OpcUa_Bad;
    }

    pTmpCert = d2i_X509_fp(pfCert, (X509**)OpcUa_Null);
    fclose(pfCert);

    a_pCertificate->Length = i2d_X509(pTmpCert, NULL);
    buf = (OpcUa_Byte*)OpcUa_Alloc(a_pCertificate->Length);
    p = buf;
    i2d_X509(pTmpCert, &p);

    a_pCertificate->Data = buf;

    X509_free(pTmpCert);

#else

    /* place platform independent implementation here! */

#endif

    return uStatus;
}
