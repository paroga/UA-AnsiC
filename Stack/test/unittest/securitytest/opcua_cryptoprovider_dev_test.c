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
#include <opcua_datetime.h>
#include <opcua_crypto.h>
#include <opcua_cryptofactory.h>
#include <opcua_cryptoprovider_dev_test.h>
#include <opcua_securitytest_helperfunctions.h>

#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

#include "unittest.h"

extern OpcUa_StringA            sPlaintext;
extern OpcUa_StringA            sInitialVector;
extern OpcUa_StringA            sCertificateFile;
extern OpcUa_StringA            sPrivateKeyFile;

OpcUa_StatusCode        uStatus                 = 0;

/*============================================================================
 * OpcUa_Test_Generate0BitsKey
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_Generate0BitsKey()
{
    OpcUa_String            mySecurityPolicy;
    OpcUa_Key               serverNonce;

    OpcUa_CryptoProvider    myCryptoProvider;
    OpcUa_StatusCode        uStatus             = OpcUa_Good;

    printf("\nGenerate0BitsKey");

    /* Generate needed test objects */
#if 0
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
#else
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
#endif
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_Key_Initialize(&serverNonce);

    /* generate ServerNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                0,
                                                &serverNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    serverNonce.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(serverNonce.Key.Length*sizeof(OpcUa_Byte));

    /* generate ServerNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                0,
                                                &serverNonce);
    OpcUa_GotoErrorIfBad(uStatus);


    uStatus = OpcUa_CryptoProvider_Delete(&myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

        printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_Generate128BitsKey
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_Generate128BitsKey()
{
    printf("\nGenerate128BitsKey");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_Generate256BitsKey
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_Generate256BitsKey()
{
    OpcUa_String            mySecurityPolicy;
    OpcUa_Key               serverNonce;

    OpcUa_CryptoProvider    myCryptoProvider;
    OpcUa_StatusCode        uStatus             = OpcUa_Good;

    printf("\nGenerate256BitsKey");

    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_Key_Initialize(&serverNonce);

    /* generate ServerNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &serverNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    serverNonce.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(serverNonce.Key.Length*sizeof(OpcUa_Byte));

    /* generate ServerNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &serverNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_CryptoProvider_Delete(&myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

        printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_Generate512BitsKey
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_Generate512BitsKey()
{
    printf("\nGenerate512BitsKey");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_Derive0BitsKey
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_Derive0BitsKey()
{
    OpcUa_Key               derivedKey;
    OpcUa_String            mySecurityPolicy;
    OpcUa_CryptoProvider    myCryptoProvider;
    OpcUa_StatusCode        uStatus     = OpcUa_Good;
    OpcUa_ByteString        seed;
    OpcUa_ByteString        secret;
#if 0 /* debug vars */
    OpcUa_CharA             randomData1[]   = "asdfwertlkjhvgzt";
    OpcUa_CharA             randomData2[]   = "��lpnnuu7ssi2999";
#endif

    printf("\nDerive0BitsKey");

    OpcUa_Key_Initialize(&derivedKey);

    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    seed.Length = 32;
    seed.Data = (OpcUa_Byte*)OpcUa_Memory_Alloc(seed.Length*sizeof(OpcUa_Byte));

    secret.Length = 32;
    secret.Data = (OpcUa_Byte*)OpcUa_Memory_Alloc(secret.Length*sizeof(OpcUa_Byte));

    uStatus = myCryptoProvider.DeriveKey(&myCryptoProvider, secret, seed, 0, &derivedKey);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;

}

/*============================================================================
 * OpcUa_Test_Derive128BitsKey
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_Derive128BitsKey()
{
    printf("\nDerive128BitsKey");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_Derive256BitsKey
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_Derive256BitsKey()
{
    OpcUa_Key               derivedKey;
    OpcUa_String            mySecurityPolicy;
    OpcUa_CryptoProvider    myCryptoProvider;
    OpcUa_StatusCode        uStatus     = OpcUa_Good;
    OpcUa_ByteString        seed;
    OpcUa_ByteString        secret;
#if 0 /* debug vars */
    OpcUa_CharA             randomData1[]   = "asdfwertlkjhvgzt";
    OpcUa_CharA             randomData2[]   = "��lpnnuu7ssi2999";
#endif

    printf("\nDerive256BitsKey");

    OpcUa_Key_Initialize(&derivedKey);

    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    seed.Length = 32;
    seed.Data = (OpcUa_Byte*)OpcUa_Memory_Alloc(seed.Length*sizeof(OpcUa_Byte));

    secret.Length = 32;
    secret.Data = (OpcUa_Byte*)OpcUa_Memory_Alloc(secret.Length*sizeof(OpcUa_Byte));

    uStatus = myCryptoProvider.DeriveKey(&myCryptoProvider, secret, seed, 32, &derivedKey);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;

}

/*============================================================================
 * OpcUa_Test_Derive512BitsKey
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_Derive512BitsKey()
{
    printf("\nDerive512BitsKey");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_Derive0BitsChannelKeyset
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_Derive0BitsChannelKeyset()
{

    OpcUa_String            mySecurityPolicy;

    OpcUa_CryptoProvider    myCryptoProvider;

    OpcUa_SecurityKeyset   clientKeyset;
    OpcUa_SecurityKeyset   serverKeyset;

    OpcUa_Key               serverNonce;
    OpcUa_Key               clientNonce;

    OpcUa_StatusCode        uStatus                     = OpcUa_Good;

    printf("\nDerive256BitsChannelKeyset");

    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_Key_Initialize(&serverNonce);

    /* generate ServerNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &serverNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    serverNonce.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(serverNonce.Key.Length*sizeof(OpcUa_Byte));

    /* generate ServerNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &serverNonce);
    OpcUa_GotoErrorIfBad(uStatus);


    OpcUa_Key_Initialize(&clientNonce);

    /* generate ClientNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &clientNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    clientNonce.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(clientNonce.Key.Length*sizeof(OpcUa_Byte));

    /* generate ClientNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &clientNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    /* derive keys */
    uStatus = myCryptoProvider.DeriveChannelKeysets(
                                                        &myCryptoProvider,
                                                        clientNonce.Key,
                                                        serverNonce.Key,
                                                        0,
                                                        &clientKeyset,
                                                        &serverKeyset);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_Derive128BitsChannelKeyset
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_Derive128BitsChannelKeyset()
{
    printf("\nDerive128BitsChannelKeyset");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_Derive256BitsChannelKeyset
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_Derive256BitsChannelKeyset()
{

    OpcUa_String            mySecurityPolicy;

    OpcUa_CryptoProvider    myCryptoProvider;

    OpcUa_SecurityKeyset   clientKeyset;
    OpcUa_SecurityKeyset   serverKeyset;

    OpcUa_Key               serverNonce;
    OpcUa_Key               clientNonce;

    OpcUa_StatusCode        uStatus                     = OpcUa_Good;

    printf("\nDerive256BitsChannelKeyset");

    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_Key_Initialize(&serverNonce);

    /* generate ServerNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &serverNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    serverNonce.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(serverNonce.Key.Length*sizeof(OpcUa_Byte));

    /* generate ServerNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &serverNonce);
    OpcUa_GotoErrorIfBad(uStatus);


    OpcUa_Key_Initialize(&clientNonce);

    /* generate ClientNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &clientNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    clientNonce.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(clientNonce.Key.Length*sizeof(OpcUa_Byte));

    /* generate ClientNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &clientNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    /* derive keys */
    uStatus = myCryptoProvider.DeriveChannelKeysets(
                                                        &myCryptoProvider,
                                                        clientNonce.Key,
                                                        serverNonce.Key,
                                                        serverNonce.Key.Length,
                                                        &clientKeyset,
                                                        &serverKeyset);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_Derive512BitsChannelKeyset
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_Derive512BitsChannelKeyset()
{
    printf("\nDerive512BitsChannelKeyset");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_Generate1024BitsRSAKeypair
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_Generate1024BitsRSAKeypair()
{
    OpcUa_String            mySecurityPolicy;
    OpcUa_Key               rsaPublicKey;
    OpcUa_Key               rsaPrivateKey;

    OpcUa_CryptoProvider    myCryptoProvider;
    OpcUa_StatusCode        uStatus             = OpcUa_Good;

    printf("\nGenerate1024BitsRSAKeypair");

    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myCryptoProvider.GenerateAsymmetricKeypair(   &myCryptoProvider,
                                                            OpcUa_Crypto_Rsa_Id,
                                                            1024,
                                                            &rsaPublicKey,
                                                            &rsaPrivateKey);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_CryptoProvider_Delete(&myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_Generate2048BitsRSAKeypair
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_Generate2048BitsRSAKeypair()
{
    OpcUa_String            mySecurityPolicy;
    OpcUa_Key               rsaPublicKey;
    OpcUa_Key               rsaPrivateKey;

    OpcUa_CryptoProvider    myCryptoProvider;
    OpcUa_StatusCode        uStatus             = OpcUa_Good;

    printf("\nGenerate2048BitsRSAKeypair");

    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myCryptoProvider.GenerateAsymmetricKeypair(   &myCryptoProvider,
                                                            OpcUa_Crypto_Rsa_Id,
                                                            2048,
                                                            &rsaPublicKey,
                                                            &rsaPrivateKey);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_CryptoProvider_Delete(&myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_GenerateECKeypair
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_GenerateECKeypair()
{
    OpcUa_String            mySecurityPolicy;
    OpcUa_Key               ecPublicKey;
    OpcUa_Key               ecPrivateKey;

    OpcUa_CryptoProvider    myCryptoProvider;
    OpcUa_StatusCode        uStatus             = OpcUa_Good;

    printf("\nGenerateECKeypair");

    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myCryptoProvider.GenerateAsymmetricKeypair(   &myCryptoProvider,
                                                            OpcUa_Crypto_Ec_Id,
                                                            1024,
                                                            &ecPublicKey,
                                                            &ecPrivateKey);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_CryptoProvider_Delete(&myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_GenerateSelfsignedRootCertificateWith1024BitsRSAKey
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_GenerateSelfsignedRootCertificateWith1024BitsRSAKey()
{
#if 0
    FILE*                   pCertFile;
#endif
    OpcUa_Certificate*      pCertificate;
    OpcUa_String            mySecurityPolicy;

    OpcUa_Key               rsaPublicKey;
    OpcUa_Key               rsaPrivateKey;

    OpcUa_CryptoProvider    myCryptoProvider;

    OpcUa_Crypto_Extension  extension[2];
    OpcUa_Crypto_NameEntry  nameEntries[6];

    OpcUa_DateTime          createdAt;
    OpcUa_DateTime          expiresAt;
    OpcUa_StatusCode        uStatus             = OpcUa_Good;

    printf("\nGenerateSelfsignedRootCertificateWith1024BitsRSAKey");

    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myCryptoProvider.GenerateAsymmetricKeypair(   &myCryptoProvider,
                                                            OpcUa_Crypto_Rsa_Id,
                                                            1024,
                                                            &rsaPublicKey,
                                                            &rsaPrivateKey);
    OpcUa_GotoErrorIfBad(uStatus);

    extension[0].key = "subjectAltName";
    extension[0].value = "DNS:uaserver.opcfoundation.org";

    extension[1].key = "basicConstraints";
    extension[1].value = "critical,CA:TRUE,pathlen:3";

    nameEntries[0].key = "countryName";
    nameEntries[0].value = "DE";

    nameEntries[1].key = "stateOrProvinceName";
    nameEntries[1].value = "BW";

    nameEntries[2].key = "localityName";
    nameEntries[2].value = "Ladenburg";

    nameEntries[3].key = "organizationName";
    nameEntries[3].value = "opcfoundation.org";

    nameEntries[4].key = "organizationalUnitName";
    nameEntries[4].value = "Unified Architecture";

    nameEntries[5].key = "commonName";
    nameEntries[5].value = "UA Untrusted Root";

    OpcUa_DateTime_GetDateTimeFromString("2007-12-16T12:00:00Z", &createdAt);
    OpcUa_DateTime_GetDateTimeFromString("2037-12-30T12:00:00Z", &expiresAt);

    uStatus = myCryptoProvider.CreateCertificate(   &myCryptoProvider,
                                                    11,
                                                    createdAt,
                                                    expiresAt,
                                                    nameEntries,
                                                    6,
                                                    rsaPublicKey,
                                                    extension,
                                                    2,
                                                    OPCUA_P_SHA_160,
                                                    OpcUa_Null,
                                                    rsaPrivateKey,
                                                    &pCertificate);
    OpcUa_GotoErrorIfBad(uStatus);

#if 0
    fopen_s(&pCertFile, "C:\\ss_12_cert.der", "w");
    if (pCertFile == NULL) {
      return OpcUa_Bad;
    }
    i2d_X509_fp(pCertFile, (X509*)pCertificate);

    fclose (pCertFile);
#endif

    uStatus = OpcUa_CryptoProvider_Delete(&myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_GenerateSelfsignedRootCertificateWith2048BitsRSAKey
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_GenerateSelfsignedRootCertificateWith2048BitsRSAKey()
{
#if 0
    FILE*                   pCertFile;
#endif
    OpcUa_Certificate*      pCertificate;
    OpcUa_String            mySecurityPolicy;

    OpcUa_Key               rsaPublicKey;
    OpcUa_Key               rsaPrivateKey;

    OpcUa_CryptoProvider    myCryptoProvider;

    OpcUa_Crypto_Extension  extension[2];
    OpcUa_Crypto_NameEntry  nameEntries[6];

    OpcUa_DateTime          createdAt;
    OpcUa_DateTime          expiresAt;
    OpcUa_StatusCode        uStatus             = OpcUa_Good;

    printf("\nGenerateSelfsignedRootCertificateWith2048BitsRSAKey");

    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myCryptoProvider.GenerateAsymmetricKeypair(   &myCryptoProvider,
                                                            OpcUa_Crypto_Rsa_Id,
                                                            2048,
                                                            &rsaPublicKey,
                                                            &rsaPrivateKey);
    OpcUa_GotoErrorIfBad(uStatus);

    extension[0].key = "subjectAltName";
    extension[0].value = "DNS:uaserver.opcfoundation.org";

    extension[1].key = "basicConstraints";
    extension[1].value = "critical,CA:TRUE,pathlen:3";

    nameEntries[0].key = "countryName";
    nameEntries[0].value = "DE";

    nameEntries[1].key = "stateOrProvinceName";
    nameEntries[1].value = "BW";

    nameEntries[2].key = "localityName";
    nameEntries[2].value = "Ladenburg";

    nameEntries[3].key = "organizationName";
    nameEntries[3].value = "opcfoundation.org";

    nameEntries[4].key = "organizationalUnitName";
    nameEntries[4].value = "Unified Architecture";

    nameEntries[5].key = "commonName";
    nameEntries[5].value = "UA Untrusted Root";

    OpcUa_DateTime_GetDateTimeFromString("2007-12-16T12:00:00Z", &createdAt);
    OpcUa_DateTime_GetDateTimeFromString("2007-12-30T12:00:00Z", &expiresAt);

    uStatus = myCryptoProvider.CreateCertificate(   &myCryptoProvider,
                                                    11,
                                                    createdAt,
                                                    expiresAt,
                                                    nameEntries,
                                                    6,
                                                    rsaPublicKey,
                                                    extension,
                                                    2,
                                                    OPCUA_P_SHA_160,
                                                    OpcUa_Null,
                                                    rsaPrivateKey,
                                                    &pCertificate);
    OpcUa_GotoErrorIfBad(uStatus);

#if 0
    fopen_s(&pCertFile, "c:\\ss_11_cert.der", "w");
    if (pCertFile == NULL) {
      return OpcUa_Bad;
    }
    i2d_X509_fp(pCertFile, (X509*)pCertificate);

    fclose (pCertFile);
#endif

    uStatus = OpcUa_CryptoProvider_Delete(&myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_GenerateSelfsignedRootCertificateWithECKey
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_GenerateSelfsignedRootCertificateWithECKey()
{
    printf("\nGenerateSelfsignedRootCertificateWithECKey");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_GetPrivateKeyFromDEREncodedX509Certificate
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_GetPrivateKeyFromDEREncodedX509Certificate()
{
    printf("\nGetPrivateKeyFromDEREncodedX509Certificate");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_GetPublicKeyFromDEREncodedX509Certificate
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_GetPublicKeyFromDEREncodedX509Certificate()
{
    OpcUa_String            mySecurityPolicy;
    OpcUa_ByteString        certificate;

    OpcUa_CryptoProvider    myCryptoProvider;

    OpcUa_Key               publicKey;

    printf("\nGetPublicKeyFromDEREncodedX509Certificate");
    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_TestHelper_GetDEREncodedCertificateFromFile(sCertificateFile, &certificate);
    OpcUa_GotoErrorIfBad(uStatus);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myCryptoProvider.GetPublicKeyFromCert(&myCryptoProvider, &certificate, OpcUa_Null, &publicKey);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_GetSignatureFromDEREncodedX509Certificate
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_GetSignatureFromDEREncodedX509Certificate()
{
    OpcUa_String            mySecurityPolicy;
    OpcUa_ByteString        certificate;

    OpcUa_CryptoProvider    myCryptoProvider;

    OpcUa_Signature        signature;

    printf("\nGetSignatureFromDEREncodedX509Certificate");
    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_TestHelper_GetDEREncodedCertificateFromFile(sCertificateFile, &certificate);
    OpcUa_GotoErrorIfBad(uStatus);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_Signature_Initialize(&signature);

    uStatus = myCryptoProvider.GetSignatureFromCert(&myCryptoProvider, &certificate, &signature);

    signature.Signature.Data = (OpcUa_Byte*)OpcUa_Alloc(signature.Signature.Length*sizeof(OpcUa_Byte));

    uStatus = myCryptoProvider.GetSignatureFromCert(&myCryptoProvider, &certificate, &signature);

    OpcUa_Signature_Clear(&signature);

    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

    OpcUa_Signature_Clear(&signature);

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_AsymmetricEncryptionWith1024BitsRSAKey
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_AsymmetricEncryptionWith1024BitsRSAKey()
{
    OpcUa_String            mySecurityPolicy;
    OpcUa_ByteString        certificate;

    OpcUa_CryptoProvider    myCryptoProvider;

    OpcUa_Key               publicKey;
    OpcUa_Key               privateKey;

    OpcUa_Byte*             pCiphertext                 = OpcUa_Null;
    OpcUa_UInt32            ciphertextLen               = 0;

    OpcUa_Byte*             pPlaintext                  = OpcUa_Null;
    OpcUa_UInt32            plaintextLen                = PLAINTEXT_LENGTH;

    OpcUa_Byte*             pDecryptedCiphertext        = OpcUa_Null;
    OpcUa_UInt32            decryptedCiphertextLen      = 0;
    OpcUa_StatusCode        uStatus                     = OpcUa_Good;

    printf("\nAsymmetricEncryptionWith1024BitsRSAKey");

    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_TestHelper_GetDEREncodedPrivateKeyFromFile(sPrivateKeyFile, &privateKey);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_TestHelper_GetDEREncodedCertificateFromFile(sCertificateFile, &certificate);
    OpcUa_GotoErrorIfBad(uStatus);

    pPlaintext = (OpcUa_Byte*)OpcUa_Alloc(plaintextLen*sizeof(OpcUa_Byte));
    OpcUa_MemCpy(pPlaintext, plaintextLen, sPlaintext, plaintextLen);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myCryptoProvider.GetPublicKeyFromCert(&myCryptoProvider, &certificate, OpcUa_Null, &publicKey);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myCryptoProvider.AsymmetricEncrypt(   &myCryptoProvider,
                                                    pPlaintext,
                                                    plaintextLen,
                                                    &publicKey,
                                                    OpcUa_Null,
                                                    &ciphertextLen);
    OpcUa_GotoErrorIfBad(uStatus);

    pCiphertext = (OpcUa_Byte*)OpcUa_Alloc(ciphertextLen*sizeof(OpcUa_Byte));

    uStatus = myCryptoProvider.AsymmetricEncrypt(   &myCryptoProvider,
                                                    pPlaintext,
                                                    plaintextLen,
                                                    &publicKey,
                                                    pCiphertext,
                                                    &ciphertextLen);
    OpcUa_GotoErrorIfBad(uStatus);


    uStatus = myCryptoProvider.AsymmetricDecrypt(   &myCryptoProvider,
                                                    pCiphertext,
                                                    ciphertextLen,
                                                    &privateKey,
                                                    OpcUa_Null,
                                                    &decryptedCiphertextLen);
    OpcUa_GotoErrorIfBad(uStatus);

    pDecryptedCiphertext = (OpcUa_Byte*)OpcUa_Alloc(decryptedCiphertextLen*sizeof(OpcUa_Byte));

    uStatus = myCryptoProvider.AsymmetricDecrypt(   &myCryptoProvider,
                                                    pCiphertext,
                                                    ciphertextLen,
                                                    &privateKey,
                                                    pDecryptedCiphertext,
                                                    &decryptedCiphertextLen);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_CryptoProvider_Delete(&myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_AsymmetricEncryptionWith2048BitsRSAKey
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_AsymmetricEncryptionWith2048BitsRSAKey()
{
    printf("\nAsymmetricEncryptionWith2048BitsRSAKey");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_AsymmetricSignatureWith1024BitsRSAKeyAndSHA1
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_AsymmetricSignatureWith1024BitsRSAKeyAndSHA1()
{
    OpcUa_String            mySecurityPolicy;
    OpcUa_ByteString        certificate;

    OpcUa_CryptoProvider    myCryptoProvider;

    OpcUa_Key               publicKey;
    OpcUa_Key               privateKey;


    OpcUa_ByteString        plaintext;
    OpcUa_ByteString        signature;


    OpcUa_StatusCode        uStatus                     = OpcUa_Good;

    printf("\nAsymmetricSignatureWith1024BitsRSAKeyAndSHA1");

    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_TestHelper_GetDEREncodedPrivateKeyFromFile(sPrivateKeyFile, &privateKey);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_TestHelper_GetDEREncodedCertificateFromFile(sCertificateFile, &certificate);
    OpcUa_GotoErrorIfBad(uStatus);

    plaintext.Length = PLAINTEXT_LENGTH;
    plaintext.Data = (OpcUa_Byte*)OpcUa_Alloc(plaintext.Length);
    OpcUa_MemCpy(plaintext.Data, plaintext.Length, sPlaintext, plaintext.Length);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myCryptoProvider.GetPublicKeyFromCert(&myCryptoProvider, &certificate, OpcUa_Null, &publicKey);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myCryptoProvider.AsymmetricSign(      &myCryptoProvider,
                                                    plaintext,
                                                    &privateKey,
                                                    &signature);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myCryptoProvider.AsymmetricVerify(    &myCryptoProvider,
                                                    plaintext,
                                                    &publicKey,
                                                    &signature);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_CryptoProvider_Delete(&myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_AsymmetricSignatureWith1024BitsRSAKeyAndSHA256
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_AsymmetricSignatureWith1024BitsRSAKeyAndSHA256()
{
    printf("\nAsymmetricSignatureWith1024BitsRSAKeyAndSHA256");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_AsymmetricSignatureWith2048BitsRSAKeyAndSHA1
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_AsymmetricSignatureWith2048BitsRSAKeyAndSHA1()
{
    printf("\nAsymmetricSignatureWith2048BitsRSAKeyAndSHA1");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_AsymmetricSignatureWith2048BitsRSAKeyAndSHA256
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_AsymmetricSignatureWith2048BitsRSAKeyAndSHA256()
{
    printf("\nAsymmetricSignatureWith2048BitsRSAKeyAndSHA256");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_SymmetricEncryptionWith128BitsAESKeyUsingCBC
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_SymmetricEncryptionWith128BitsAESKeyUsingCBC()
{
    OpcUa_String            mySecurityPolicy;

    OpcUa_CryptoProvider    myCryptoProvider;

    OpcUa_SecurityKeyset    clientKeyset;
    OpcUa_SecurityKeyset    serverKeyset;

    OpcUa_Key               serverNonce;
    OpcUa_Key               clientNonce;

    OpcUa_Byte*             pCiphertext                 = OpcUa_Null;
    OpcUa_UInt32            ciphertextLen               = 0;

    OpcUa_Byte*             pPlaintext                  = OpcUa_Null;
    OpcUa_Int32             plaintextLen                = PLAINTEXT_LENGTH;
    OpcUa_UInt32            newPlaintextLen             = 0;

    OpcUa_Byte*             pDecryptedCiphertext        = OpcUa_Null;

    OpcUa_ByteString        initialVector;
    OpcUa_StatusCode        uStatus                     = OpcUa_Good;

    printf("\nSymmetricEncryptionWith128BitsAESKeyUsingCBC");

    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    pPlaintext = (OpcUa_Byte*)OpcUa_Alloc(plaintextLen*sizeof(OpcUa_Byte));
    OpcUa_MemCpy(pPlaintext, plaintextLen, sPlaintext, plaintextLen);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_Key_Initialize(&serverNonce);

    /* generate ServerNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &serverNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    serverNonce.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(serverNonce.Key.Length*sizeof(OpcUa_Byte));

    /* generate ServerNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &serverNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_Key_Initialize(&clientNonce);

    /* generate ClientNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &clientNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    clientNonce.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(clientNonce.Key.Length*sizeof(OpcUa_Byte));

    /* generate ClientNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &clientNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    /* derive keys */
    uStatus = myCryptoProvider.DeriveChannelKeysets(
                                                        &myCryptoProvider,
                                                        clientNonce.Key,
                                                        serverNonce.Key,
                                                        serverNonce.Key.Length,
                                                        &clientKeyset,
                                                        &serverKeyset);
    OpcUa_GotoErrorIfBad(uStatus);

    initialVector.Length = clientKeyset.InitializationVector.Key.Length;
    initialVector.Data = (OpcUa_Byte*)OpcUa_Alloc(initialVector.Length*sizeof(OpcUa_Byte));
    OpcUa_MemCpy(initialVector.Data, initialVector.Length, clientKeyset.InitializationVector.Key.Data, initialVector.Length);

    /* encrypt message */
    uStatus = myCryptoProvider.SymmetricEncrypt(
                                                    &myCryptoProvider,
                                                    pPlaintext,
                                                    plaintextLen,
                                                    &clientKeyset.EncryptionKey,
                                                    initialVector.Data,
                                                    OpcUa_Null,
                                                    &ciphertextLen);
    OpcUa_GotoErrorIfBad(uStatus);

    pCiphertext = (OpcUa_Byte*)OpcUa_Alloc(ciphertextLen*sizeof(OpcUa_Byte));

    uStatus = myCryptoProvider.SymmetricEncrypt(
                                                    &myCryptoProvider,
                                                    pPlaintext,
                                                    plaintextLen,
                                                    &clientKeyset.EncryptionKey,
                                                    initialVector.Data,
                                                    pCiphertext,
                                                    &ciphertextLen);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_MemCpy(initialVector.Data, initialVector.Length, clientKeyset.InitializationVector.Key.Data, initialVector.Length);

    /* decrypt message */
    uStatus = myCryptoProvider.SymmetricDecrypt(
                                                    &myCryptoProvider,
                                                    pCiphertext,
                                                    ciphertextLen,
                                                    &clientKeyset.EncryptionKey,
                                                    initialVector.Data,
                                                    OpcUa_Null,
                                                    &newPlaintextLen);
    OpcUa_GotoErrorIfBad(uStatus);

    pDecryptedCiphertext = (OpcUa_Byte*)OpcUa_Alloc(newPlaintextLen*sizeof(OpcUa_Byte));

    uStatus = myCryptoProvider.SymmetricDecrypt(
                                                    &myCryptoProvider,
                                                    pCiphertext,
                                                    ciphertextLen,
                                                    &clientKeyset.EncryptionKey,
                                                    initialVector.Data,
                                                    pDecryptedCiphertext,
                                                    &newPlaintextLen);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_CryptoProvider_Delete(&myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_SymmetricEncryptionWith256BitsAESKeyUsingCBC
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_SymmetricEncryptionWith256BitsAESKeyUsingCBC()
{
    printf("\nSymmetricEncryptionWith256BitsAESKeyUsingCBC");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_SymmetricEncryptionWith512BitsAESKeyUsingCBC
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_SymmetricEncryptionWith512BitsAESKeyUsingCBC()
{
    printf("\nSymmetricEncryptionWith512BitsAESKeyUsingCBC");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_SymmetricSignatureWithHMAC-SHA128
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_SymmetricSignatureWithHMACAndSHA128()
{
    OpcUa_String            mySecurityPolicy;

    OpcUa_CryptoProvider    myCryptoProvider;

    OpcUa_SecurityKeyset    clientKeyset;
    OpcUa_SecurityKeyset    serverKeyset;

    OpcUa_Key               serverNonce;
    OpcUa_Key               clientNonce;

    OpcUa_Byte*             pPlaintext                  = OpcUa_Null;
    OpcUa_Int32             plaintextLen                = PLAINTEXT_LENGTH;


    OpcUa_ByteString        signature;
    OpcUa_StatusCode        uStatus                     = OpcUa_Good;

    printf("\nSymmetricSignatureWithHMACAndSHA128");

    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &mySecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    pPlaintext = (OpcUa_Byte*)OpcUa_Alloc(plaintextLen*sizeof(OpcUa_Byte));
    OpcUa_MemCpy(pPlaintext, plaintextLen, sPlaintext, plaintextLen);

    /* ToDo: OpcUa_SecurityPolicy will be changed to OpcUa_AlgorithmSuite */
    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&mySecurityPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_Key_Initialize(&serverNonce);

    /* generate ServerNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &serverNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    serverNonce.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(serverNonce.Key.Length*sizeof(OpcUa_Byte));

    /* generate ServerNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &serverNonce);
    OpcUa_GotoErrorIfBad(uStatus);


    OpcUa_Key_Initialize(&clientNonce);

    /* generate ClientNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &clientNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    clientNonce.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(clientNonce.Key.Length*sizeof(OpcUa_Byte));

    /* generate ClientNonce */
    uStatus = myCryptoProvider.GenerateKey(
                                                &myCryptoProvider,
                                                -1,
                                                &clientNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    /* derive keys */
    uStatus = myCryptoProvider.DeriveChannelKeysets(
                                                        &myCryptoProvider,
                                                        clientNonce.Key,
                                                        serverNonce.Key,
                                                        serverNonce.Key.Length,
                                                        &clientKeyset,
                                                        &serverKeyset);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = myCryptoProvider.DeriveChannelKeysets(
                                                        &myCryptoProvider,
                                                        clientNonce.Key,
                                                        serverNonce.Key,
                                                        serverNonce.Key.Length,
                                                        &clientKeyset,
                                                        &serverKeyset);
    OpcUa_GotoErrorIfBad(uStatus);

    /* sign message */
    uStatus = myCryptoProvider.SymmetricSign(
                                                &myCryptoProvider,
                                                pPlaintext,
                                                plaintextLen,
                                                &clientKeyset.SigningKey,
                                                &signature);
    OpcUa_GotoErrorIfBad(uStatus);

    /* verify message */
    uStatus = myCryptoProvider.SymmetricVerify(
                                                    &myCryptoProvider,
                                                    pPlaintext,
                                                    plaintextLen,
                                                    &clientKeyset.SigningKey,
                                                    &signature);

    uStatus = OpcUa_CryptoProvider_Delete(&myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

    printf("...failed!");

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_SymmetricSignatureWithHMAC-SHA256
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_SymmetricSignatureWithHMACAndSHA256()
{
    printf("\nSymmetricSignatureWithHMACAndSHA256");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_SymmetricSignatureWithHMAC-SHA512
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_SymmetricSignatureWithHMACAndSHA512()
{
    printf("\nSymmetricSignatureWithHMACAndSHA512");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}
