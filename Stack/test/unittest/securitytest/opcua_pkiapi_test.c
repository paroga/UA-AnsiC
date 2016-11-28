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
#include <opcua_p_pki.h>
#include <opcua_pkiapi_test.h>

#include <securitytest.h>

/* for test output */
#include <stdio.h>

#if 0 /* CHZ: does not test abstract provider API but specific OpenSSL implementation */

OpcUa_StringA             sClientCert       = "..\\..\\PKI\\CA\\certs\\uaclient_untrustedRootCert.der";
OpcUa_StringA             sPEMCert          = "..\\..\\PKI\\CA\\certs\\ua_untrusted_root1.pem";
OpcUa_StringA             sPrivateKey       = "..\\..\\PKI\\CA\\private\\uaclient_privateKey.pem";

/*============================================================================
 * OpcUa_Test_LoadCertificateFromFileDER
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_LoadCertificateFromFileDER()
{
    OpcUa_StatusCode    uStatus             = OpcUa_Good;
    OpcUa_ByteString    clientCertificate;

    printf("\nLoadCertificateFromFile");

    uStatus = OpcUa_P_OpenSSL_X509_LoadFromFile(sClientCert, OpcUa_Crypto_Encoding_DER, OpcUa_Null, &clientCertificate);

    OpcUa_GotoErrorIfBad(uStatus);

    /* check whether bytestring is big enough for a valid certificate (100 is just an estimation!)*/
    if(clientCertificate.Length > 100)
        printf("...passed!");

    OpcUa_GotoErrorIfBad(uStatus);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

        printf("...failed [error handling block]!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_LoadCertificateFromFilePEM
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_LoadCertificateFromFilePEM()
{
    OpcUa_StatusCode    uStatus             = OpcUa_Good;
    OpcUa_ByteString    clientCertificate;

    printf("\nLoadCertificateFromFile");

    uStatus = OpcUa_P_OpenSSL_X509_LoadFromFile(sPEMCert, OpcUa_Crypto_Encoding_PEM, OpcUa_Null, &clientCertificate);

    OpcUa_GotoErrorIfBad(uStatus);

    /* check whether bytestring is big enough for a valid certificate (100 is just an estimation!)*/
    if(clientCertificate.Length > 100)
        printf("...passed!");


    OpcUa_GotoErrorIfBad(uStatus);

    return uStatus;

OpcUa_BeginErrorHandling;

        printf("...failed [error handling block]!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_LoadPrivateKeyFromFilePEM
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_LoadPrivateKeyFromFilePEM()
{
    OpcUa_StatusCode    uStatus             = OpcUa_Good;
    OpcUa_ByteString   ClientPrivateKey     = {0, OpcUa_Null};

    printf("\nLoadPrivateKeyFromFile");

    uStatus = OpcUa_P_OpenSSL_RSA_LoadPrivateKeyFromFile(sPrivateKey, OpcUa_Crypto_Encoding_PEM, OpcUa_Null, &ClientPrivateKey);

    OpcUa_GotoErrorIfBad(uStatus);

    /* check whether bytestring is big enough for a valid certificate (100 is just an estimation!)*/
    if(ClientPrivateKey.Length > 100)
        printf("...passed!");


OpcUa_BeginErrorHandling;

        printf("...failed [error handling block]!");

OpcUa_FinishErrorHandling;
}
#endif
