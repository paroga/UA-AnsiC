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
#include <opcua_cryptoprovider_test.h>
#include <opcua_securitytest_helperfunctions.h>

#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

#include <securitytest.h>

#include "unittest.h"

OpcUa_StringA           sPlaintext              = OPCUA_SECURITY_TEST_PLAINTEXT;
OpcUa_StringA           sInitialVector          = OPCUA_SECURITY_TEST_INITIALVECTOR;
OpcUa_StringA           sPKCS12CertPassword     = OPCUA_SECURITY_TEST_PKCS12CERTPASSWORD;
OpcUa_StringA           sPKCS12Cert             = OPCUA_SECURITY_TEST_PKCS12CERT;
OpcUa_StringA           sCertificateFile        = OPCUA_SECURITY_TEST_CERTIFICATEFILE;
OpcUa_StringA           sPrivateKeyFile         = OPCUA_SECURITY_TEST_PRIVATEKEYFILE;
OpcUa_CharA             randomData1[]           = OPCUA_SECURITY_TEST_RANDOMDATA1;
OpcUa_CharA             randomData2[]           = OPCUA_SECURITY_TEST_RANDOMDATA2;

typedef struct {
    OpcUa_String            myCryptoPolicy;
    OpcUa_CryptoProvider    myCryptoProvider;
    OpcUa_Key               pServerNonce;
    OpcUa_Key               pClientNonce;
    OpcUa_Key               pDerivedKey;
    OpcUa_Key               pRsaPublicKey;
    OpcUa_Key               pRsaPrivateKey;
    OpcUa_Key               pCertKey;
    OpcUa_Key               pPublicKey;
    OpcUa_Key               privateKey;
    OpcUa_Key               emptyKey;
    OpcUa_ByteString        seed;
    OpcUa_ByteString        secret;
    OpcUa_ByteString        empty;

    OpcUa_ByteString        pCertificateData;

    OpcUa_StringA           CertificateFile; /* TODO: Set this! */
    OpcUa_SecurityKeyset    pClientKeyset;
    OpcUa_SecurityKeyset    pServerKeyset;
    OpcUa_Crypto_Extension  extension[2];
    OpcUa_Crypto_NameEntry  nameEntries[6];
    OpcUa_DateTime          createdAt;
    OpcUa_DateTime          expiresAt;
    OpcUa_DateTime          emptyAt;
    OpcUa_Certificate       pCertificate;
    OpcUa_Signature         pSignature;
    OpcUa_Byte*             pCiphertext;
    OpcUa_UInt32             ciphertextLen;
    OpcUa_Byte*             pPlaintext;
    OpcUa_UInt32             plaintextLen;
    OpcUa_Byte*             pDecryptedCiphertext;
    OpcUa_UInt32             decryptedCiphertextLen;
    OpcUa_ByteString        initialVector;
    OpcUa_ByteString        pByteSignature;
} TESTCRYPTOPROVIDERDATA;

/*============================================================================
 * OpcUa_Test_CryptoByteSignatureCleanup
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoByteStringCleanup(OpcUa_ByteString *pByte, OpcUa_Boolean bAssert)
{
    if(!pByte)
    {
        return OpcUa_BadInvalidArgument;
    }

    if(bAssert)
    {
        ASSERT(pByte->Data);
    }

    if(pByte->Data)
    {
        OpcUa_Free(pByte->Data);
        pByte->Data = OpcUa_Null;
    }

    return OpcUa_Good;
}

/*============================================================================
 * OpcUa_Test_CryptoSignatureCleanup
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoSignatureCleanup(OpcUa_Signature *pSign, OpcUa_Boolean bAssert)
{
    if(!pSign)
    {
        return OpcUa_BadInvalidArgument;
    }

    if(bAssert)
    {
        ASSERT(pSign->Signature.Data);
    }

    if(pSign->Signature.Data)
    {
        OpcUa_Signature_Clear(pSign);
    }

    return OpcUa_Good;
}

/*============================================================================
 * OpcUa_Test_CryptoKeyCleanup
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoKeyCleanup(OpcUa_Key *pKey, OpcUa_Boolean bAssert)
{
    if(!pKey)
    {
        return OpcUa_BadInvalidArgument;
    }

    if(bAssert)
    {
        ASSERT(pKey->Key.Data);
    }

    if(pKey->Key.Data)
    {
        OpcUa_Key_Clear(pKey);
    }

    return OpcUa_Good;
}

/*============================================================================
 * OpcUa_Test_CryptoKeysetCleanup
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoKeysetCleanup(OpcUa_SecurityKeyset *pKey, OpcUa_Boolean bAssert)
{
    if(!pKey)
    {
        return OpcUa_BadInvalidArgument;
    }

    if(bAssert)
    {
        ASSERT(pKey->SigningKey.Key.Data);
    }

    if(pKey->SigningKey.Key.Data)
    {
        OpcUa_Free(pKey->SigningKey.Key.Data);
        pKey->SigningKey.Key.Data = OpcUa_Null;
    }

    if(bAssert)
    {
        ASSERT(pKey->EncryptionKey.Key.Data);
    }

    if(pKey->EncryptionKey.Key.Data)
    {
        OpcUa_Free(pKey->EncryptionKey.Key.Data);
        pKey->EncryptionKey.Key.Data = OpcUa_Null;
    }

    if(bAssert)
    {
        ASSERT(pKey->InitializationVector.Key.Data);
    }

    if(pKey->InitializationVector.Key.Data)
    {
        OpcUa_Free(pKey->InitializationVector.Key.Data);
        pKey->InitializationVector.Key.Data = OpcUa_Null;
    }

    return OpcUa_Good;
}

/*============================================================================
 * OpcUa_Test_CryptoProvider_GenerateKey
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoProvider_GenerateKey(TESTCRYPTOPROVIDERDATA *td)
{
    SNAPSHOT_TYPE s1, s2, s3;

    /* empty comment */
    /* Basic test - Generate a 256 bit key */
    /* empty comment */
    CASE_ID(17500);
    MEM_SNAPSHOT(&s1);
    OpcUa_Key_Initialize(&td->pServerNonce);
    ASSERT_GOOD(td->myCryptoProvider.GenerateKey(&td->myCryptoProvider, -1, &td->pServerNonce));
    td->pServerNonce.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pServerNonce.Key.Length*sizeof(OpcUa_Byte));
    ASSERT_GOOD(td->myCryptoProvider.GenerateKey(&td->myCryptoProvider, -1, &td->pServerNonce));
    OpcUa_Test_CryptoKeyCleanup(&td->pServerNonce, OpcUa_False);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17501);
    OpcUa_Key_Initialize(&td->pServerNonce);
    ASSERT_STATUS(td->myCryptoProvider.GenerateKey( OpcUa_Null, -1,&td->pServerNonce), OpcUa_BadInvalidArgument);
    OpcUa_Test_CryptoKeyCleanup(&td->pServerNonce, OpcUa_False);
    END_CASE();

    /* empty comment */
    /* Basic test - Zero Length */
    /* empty comment */
    CASE_ID(17502);
    OpcUa_Key_Initialize(&td->pServerNonce);
    ASSERT_STATUS(td->myCryptoProvider.GenerateKey( &td->myCryptoProvider, 0, &td->pServerNonce), OpcUa_BadInvalidArgument);
    OpcUa_Test_CryptoKeyCleanup(&td->pServerNonce, OpcUa_False);
    END_CASE();

    /* empty comment */
    /* Basic test - Very Large Key Length */
    /* empty comment */
    CASE_ID(17503);
    OpcUa_Key_Initialize(&td->pServerNonce);
    ASSERT_STATUS(td->myCryptoProvider.GenerateKey( &td->myCryptoProvider, 0x80000, &td->pServerNonce), OpcUa_BadInvalidArgument);
    OpcUa_Test_CryptoKeyCleanup(&td->pServerNonce, OpcUa_False);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Key */
    /* empty comment */
    CASE_ID(17505);
    OpcUa_Key_Initialize(&td->pServerNonce);
    ASSERT_STATUS(td->myCryptoProvider.GenerateKey( &td->myCryptoProvider, -1, OpcUa_Null), OpcUa_BadInvalidArgument);
    OpcUa_Test_CryptoKeyCleanup(&td->pServerNonce, OpcUa_False);
    END_CASE();

    return OpcUa_Good;
}

/*============================================================================
 * OpcUa_Test_CryptoProvider_DeriveKey
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoProvider_DeriveKey(TESTCRYPTOPROVIDERDATA *td)
{
    SNAPSHOT_TYPE s1, s2, s3;

    /* empty comment */
    /* Basic test - Generate a 256 bit key */
    /* empty comment */
    CASE_ID(17506);
    MEM_SNAPSHOT(&s1);
    OpcUa_Key_Initialize(&td->pDerivedKey);
    ASSERT_GOOD(td->myCryptoProvider.DeriveKey(&td->myCryptoProvider, td->secret, td->seed, -1, &td->pDerivedKey));
    td->pDerivedKey.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pDerivedKey.Key.Length*sizeof(OpcUa_Byte));
    ASSERT_GOOD(td->myCryptoProvider.DeriveKey(&td->myCryptoProvider, td->secret, td->seed, -1, &td->pDerivedKey));
    OpcUa_Test_CryptoKeyCleanup(&td->pDerivedKey, OpcUa_True);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17507);
    OpcUa_Key_Initialize(&td->pDerivedKey);
    ASSERT_STATUS(td->myCryptoProvider.DeriveKey(OpcUa_Null, td->secret, td->seed, -1, &td->pDerivedKey), OpcUa_BadInvalidArgument);
    OpcUa_Test_CryptoKeyCleanup(&td->pDerivedKey, OpcUa_False);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Secret */
    /* empty comment */
    CASE_ID(17508);
    OpcUa_Key_Initialize(&td->pDerivedKey);
    ASSERT_STATUS(td->myCryptoProvider.DeriveKey(&td->myCryptoProvider, td->empty, td->seed, -1, &td->pDerivedKey), OpcUa_BadInvalidArgument);
    OpcUa_Test_CryptoKeyCleanup(&td->pDerivedKey, OpcUa_False);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Seed */
    /* empty comment */
    CASE_ID(17509);
    OpcUa_Key_Initialize(&td->pDerivedKey);
    ASSERT_STATUS(td->myCryptoProvider.DeriveKey(&td->myCryptoProvider, td->secret, td->empty, -1, &td->pDerivedKey), OpcUa_BadInvalidArgument);
    OpcUa_Test_CryptoKeyCleanup(&td->pDerivedKey, OpcUa_False);
    END_CASE();

    /* empty comment */
    /* Basic test - Zero Key Length */
    /* empty comment */
    CASE_ID(17510);
    OpcUa_Key_Initialize(&td->pDerivedKey);
    ASSERT_STATUS(td->myCryptoProvider.DeriveKey(&td->myCryptoProvider, td->secret, td->seed, 0, &td->pDerivedKey), OpcUa_BadInvalidArgument);
    OpcUa_Test_CryptoKeyCleanup(&td->pDerivedKey, OpcUa_False);
    END_CASE();

    /* empty comment */
    /* Basic test - Very Large Key Length */
    /* empty comment */
    CASE_ID(17511);
    OpcUa_Key_Initialize(&td->pDerivedKey);
    ASSERT_STATUS(td->myCryptoProvider.DeriveKey(&td->myCryptoProvider, td->secret, td->seed, 0x80000, &td->pDerivedKey), OpcUa_BadInvalidArgument);
    OpcUa_Test_CryptoKeyCleanup(&td->pDerivedKey, OpcUa_False);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Key */
    /* empty comment */
    CASE_ID(17513);
    ASSERT_STATUS(td->myCryptoProvider.DeriveKey(&td->myCryptoProvider, td->secret, td->seed, -1, OpcUa_Null), OpcUa_BadInvalidArgument);
    OpcUa_Test_CryptoKeyCleanup(&td->pDerivedKey, OpcUa_False);
    END_CASE();

    return(OpcUa_Good);
}

/*============================================================================
 * OpcUa_Test_CryptoProvider_DeriveChannelKeysets
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoProvider_DeriveChannelKeysets(TESTCRYPTOPROVIDERDATA *td)
{
    SNAPSHOT_TYPE           s1, s2, s3;

    /* create fake nonces - type of handle prevents free */
    td->myCryptoProvider.GenerateKey(&td->myCryptoProvider, -1, &td->pServerNonce);
    td->myCryptoProvider.GenerateKey(&td->myCryptoProvider, -1, &td->pClientNonce);
    td->pServerNonce.Type           = OpcUa_Crypto_KeyType_Random_Handle;
    td->pServerNonce.fpClearHandle  = OpcUa_Null;
    td->pServerNonce.Key.Data       = (OpcUa_Byte*)randomData1;
    td->pClientNonce.Type           = OpcUa_Crypto_KeyType_Random_Handle;
    td->pClientNonce.fpClearHandle  = OpcUa_Null;
    td->pClientNonce.Key.Data       = (OpcUa_Byte*)randomData2;

    /* empty comment */
    /* Basic test - Generate a 256 bit key */
    /* empty comment */
    CASE_ID(17514);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(td->myCryptoProvider.DeriveChannelKeysets(&td->myCryptoProvider, td->pClientNonce.Key, td->pServerNonce.Key, td->pServerNonce.Key.Length, &td->pClientKeyset, &td->pServerKeyset));
    td->pClientKeyset.EncryptionKey.Key.Data        = (OpcUa_Byte*)OpcUa_Alloc(td->pClientKeyset.EncryptionKey.Key.Length*sizeof(OpcUa_Byte));
    td->pClientKeyset.InitializationVector.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pClientKeyset.InitializationVector.Key.Length*sizeof(OpcUa_Byte));
    td->pClientKeyset.SigningKey.Key.Data           = (OpcUa_Byte*)OpcUa_Alloc(td->pClientKeyset.SigningKey.Key.Length*sizeof(OpcUa_Byte));
    td->pServerKeyset.EncryptionKey.Key.Data        = (OpcUa_Byte*)OpcUa_Alloc(td->pServerKeyset.EncryptionKey.Key.Length*sizeof(OpcUa_Byte));
    td->pServerKeyset.InitializationVector.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pServerKeyset.InitializationVector.Key.Length*sizeof(OpcUa_Byte));
    td->pServerKeyset.SigningKey.Key.Data           = (OpcUa_Byte*)OpcUa_Alloc(td->pServerKeyset.SigningKey.Key.Length*sizeof(OpcUa_Byte));
    ASSERT_GOOD(td->myCryptoProvider.DeriveChannelKeysets(&td->myCryptoProvider, td->pClientNonce.Key, td->pServerNonce.Key, td->pServerNonce.Key.Length, &td->pClientKeyset, &td->pServerKeyset));
    OpcUa_Test_CryptoKeysetCleanup(&td->pClientKeyset, OpcUa_True);
    OpcUa_Test_CryptoKeysetCleanup(&td->pServerKeyset, OpcUa_True);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17515);
    ASSERT_STATUS(td->myCryptoProvider.DeriveChannelKeysets(OpcUa_Null, td->pClientNonce.Key, td->pServerNonce.Key, td->pServerNonce.Key.Length, &td->pClientKeyset, &td->pServerKeyset), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Client Key */
    /* empty comment */
    CASE_ID(17516);
    ASSERT_STATUS(td->myCryptoProvider.DeriveChannelKeysets(&td->myCryptoProvider, td->empty, td->pServerNonce.Key, td->pServerNonce.Key.Length, &td->pClientKeyset, &td->pServerKeyset), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Server Key */
    /* empty comment */
    CASE_ID(17517);
    ASSERT_STATUS(td->myCryptoProvider.DeriveChannelKeysets(&td->myCryptoProvider, td->pClientNonce.Key, td->empty, td->pServerNonce.Key.Length, &td->pClientKeyset, &td->pServerKeyset), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Zero Length Server Key */
    /* empty comment */
    CASE_ID(17518);
    ASSERT_STATUS(td->myCryptoProvider.DeriveChannelKeysets(&td->myCryptoProvider, td->pClientNonce.Key, td->pServerNonce.Key, 0, &td->pClientKeyset, &td->pServerKeyset), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Very Large Length Server Key */
    /* empty comment */
    CASE_ID(17519);
    ASSERT_STATUS(td->myCryptoProvider.DeriveChannelKeysets(&td->myCryptoProvider, td->pClientNonce.Key, td->pServerNonce.Key, 0x80000, &td->pClientKeyset, &td->pServerKeyset), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Client KeySet */
    /* empty comment */
    CASE_ID(17521);
    ASSERT_STATUS(td->myCryptoProvider.DeriveChannelKeysets(&td->myCryptoProvider, td->pClientNonce.Key, td->pServerNonce.Key, td->pServerNonce.Key.Length, OpcUa_Null, &td->pServerKeyset), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Server KeySet */
    /* empty comment */
    CASE_ID(17522);
    ASSERT_STATUS(td->myCryptoProvider.DeriveChannelKeysets(&td->myCryptoProvider, td->pClientNonce.Key, td->pServerNonce.Key, td->pServerNonce.Key.Length, &td->pClientKeyset, OpcUa_Null), OpcUa_BadInvalidArgument);
    END_CASE();

    OpcUa_Test_CryptoKeyCleanup(&td->pClientNonce, OpcUa_False);
    OpcUa_Test_CryptoKeyCleanup(&td->pServerNonce, OpcUa_False);

    return(OpcUa_Good);
}

/*============================================================================
 * OpcUa_Test_CryptoProvider_GenerateAsymmetricKeypair
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoProvider_GenerateAsymmetricKeypair(TESTCRYPTOPROVIDERDATA *td)
{
    SNAPSHOT_TYPE s1, s2, s3;

    /* empty comment */
    /* Basic test - Generate 1024 bit key */
    /* empty comment */
    CASE_ID(17523);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(td->myCryptoProvider.GenerateAsymmetricKeypair( &td->myCryptoProvider, OpcUa_Crypto_Rsa_Id, 1024, &td->pRsaPublicKey, &td->pRsaPrivateKey));
    td->pRsaPublicKey.Key.Data = OpcUa_Alloc(td->pRsaPublicKey.Key.Length);
    td->pRsaPrivateKey.Key.Data = OpcUa_Alloc(td->pRsaPrivateKey.Key.Length);
    ASSERT_GOOD(td->myCryptoProvider.GenerateAsymmetricKeypair( &td->myCryptoProvider, OpcUa_Crypto_Rsa_Id, 1024, &td->pRsaPublicKey, &td->pRsaPrivateKey));
    OpcUa_Test_CryptoKeyCleanup(&td->pRsaPublicKey, OpcUa_True);
    OpcUa_Test_CryptoKeyCleanup(&td->pRsaPrivateKey, OpcUa_True);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17524);
    ASSERT_STATUS(td->myCryptoProvider.GenerateAsymmetricKeypair(OpcUa_Null, OpcUa_Crypto_Rsa_Id, 1024, &td->pRsaPublicKey, &td->pRsaPrivateKey), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Invalid ID */
    /* empty comment */
    CASE_ID(17525);
    ASSERT_STATUS(td->myCryptoProvider.GenerateAsymmetricKeypair(&td->myCryptoProvider, (OpcUa_UInt)-1, 1024, &td->pRsaPublicKey, &td->pRsaPrivateKey), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Zero Length Key */
    /* empty comment */
    CASE_ID(17526);
    ASSERT_STATUS(td->myCryptoProvider.GenerateAsymmetricKeypair(&td->myCryptoProvider, OpcUa_Crypto_Rsa_Id, 0, &td->pRsaPublicKey, &td->pRsaPrivateKey), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Very Large Length Key */
    /* empty comment */
    CASE_ID(17527);
    ASSERT_STATUS(td->myCryptoProvider.GenerateAsymmetricKeypair(&td->myCryptoProvider, OpcUa_Crypto_Rsa_Id, 0x80000, &td->pRsaPublicKey, &td->pRsaPrivateKey), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Public Key */
    /* empty comment */
    CASE_ID(17529);
    ASSERT_STATUS(td->myCryptoProvider.GenerateAsymmetricKeypair(&td->myCryptoProvider, OpcUa_Crypto_Rsa_Id, 1024, OpcUa_Null, &td->pRsaPrivateKey), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Private Key */
    /* empty comment */
    CASE_ID(17530);
    ASSERT_STATUS(td->myCryptoProvider.GenerateAsymmetricKeypair(&td->myCryptoProvider, OpcUa_Crypto_Rsa_Id, 1024, &td->pRsaPublicKey, OpcUa_Null), OpcUa_BadInvalidArgument);
    END_CASE();

    return(OpcUa_Good);
}

/*============================================================================
 * OpcUa_Test_CryptoProvider_CreateCertificate
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoProvider_CreateCertificate(TESTCRYPTOPROVIDERDATA *td)
{
    OpcUa_StatusCode        uStatus;
    SNAPSHOT_TYPE           s1, s2, s3;

    /* empty comment */
    /* Basic test - Normal Functionality */
    /* empty comment */
    CASE_ID(17531);
    ASSERT_GOOD(td->myCryptoProvider.GenerateAsymmetricKeypair( &td->myCryptoProvider, OpcUa_Crypto_Rsa_Id, 1024, &td->pRsaPublicKey, &td->pRsaPrivateKey));
    MEM_SNAPSHOT(&s1);

    td->extension[0].key = "subjectAltName";
    td->extension[0].value = "DNS:uaserver.opcfoundation.org";

    td->extension[1].key = "basicConstraints";
    td->extension[1].value = "critical,CA:TRUE,pathlen:3";

    td->nameEntries[0].key = "countryName";
    td->nameEntries[0].value = "CA";

    td->nameEntries[1].key = "stateOrProvinceName";
    td->nameEntries[1].value = "AB";

    td->nameEntries[2].key = "localityName";
    td->nameEntries[2].value = "Calgary";

    td->nameEntries[3].key = "organizationName";
    td->nameEntries[3].value = "opcfoundation.org";

    td->nameEntries[4].key = "organizationalUnitName";
    td->nameEntries[4].value = "Unified Architecture";

    td->nameEntries[5].key = "commonName";
    td->nameEntries[5].value = "UA Untrusted Root";

    OpcUa_DateTime_GetDateTimeFromString("2007-12-16T12:00:00Z", &td->createdAt);
    OpcUa_DateTime_GetDateTimeFromString("2037-12-30T12:00:00Z", &td->expiresAt);

    uStatus = td->myCryptoProvider.CreateCertificate(       &td->myCryptoProvider,
                                                            11,
                                                            td->createdAt,
                                                            td->expiresAt,
                                                            td->nameEntries,
                                                            6,
                                                            td->pRsaPublicKey,
                                                            td->extension,
                                                            2,
                                                            OPCUA_P_SHA_256,
                                                            OpcUa_Null,
                                                            td->pRsaPrivateKey,
                                                            &td->pCertificate);
    ASSERT_GOOD(uStatus);
    ASSERT(td->pCertificate);

    if(td->pCertificate != OpcUa_Null)
    {
        X509_free((X509*)td->pCertificate);
        td->pCertificate = OpcUa_Null;
    }

    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17532);
    ASSERT_STATUS(td->myCryptoProvider.CreateCertificate(   OpcUa_Null,
                                                            11,
                                                            td->createdAt,
                                                            td->expiresAt,
                                                            td->nameEntries,
                                                            6,
                                                            td->pRsaPublicKey,
                                                            td->extension,
                                                            2,
                                                            OPCUA_P_SHA_256,
                                                            OpcUa_Null,
                                                            td->pRsaPrivateKey,
                                                            &td->pCertificate)
    , OpcUa_BadInvalidArgument);

    if(td->pCertificate != OpcUa_Null)
    {
        X509_free((X509*)td->pCertificate);
        td->pCertificate = OpcUa_Null;
    }

    END_CASE();

    /* empty comment */
    /* Basic test - Invalid Serial Number */
    /* empty comment */
    CASE_ID(17533);
    ASSERT_STATUS(td->myCryptoProvider.CreateCertificate(   &td->myCryptoProvider,
                                                            -1,
                                                            td->createdAt,
                                                            td->expiresAt,
                                                            td->nameEntries,
                                                            6,
                                                            td->pRsaPublicKey,
                                                            td->extension,
                                                            2,
                                                            OPCUA_P_SHA_256,
                                                            OpcUa_Null,
                                                            td->pRsaPrivateKey,
                                                            &td->pCertificate)
    , OpcUa_BadInvalidArgument);

    if(td->pCertificate != OpcUa_Null)
    {
        X509_free((X509*)td->pCertificate);
        td->pCertificate = OpcUa_Null;
    }

    END_CASE();

    /* empty comment */
    /* Basic test - Null Start Date */
    /* empty comment */
    CASE_ID(17534);
    ASSERT_STATUS(td->myCryptoProvider.CreateCertificate(   &td->myCryptoProvider,
                                                            11,
                                                            td->emptyAt,
                                                            td->expiresAt,
                                                            td->nameEntries,
                                                            6,
                                                            td->pRsaPublicKey,
                                                            td->extension,
                                                            2,
                                                            OPCUA_P_SHA_256,
                                                            OpcUa_Null,
                                                            td->pRsaPrivateKey,
                                                            &td->pCertificate)
    , OpcUa_BadInvalidArgument);

    if(td->pCertificate != OpcUa_Null)
    {
        X509_free((X509*)td->pCertificate);
        td->pCertificate = OpcUa_Null;
    }

    END_CASE();

    /* empty comment */
    /* Basic test - Null End Date */
    /* empty comment */
    CASE_ID(17535);
    ASSERT_STATUS(td->myCryptoProvider.CreateCertificate(   &td->myCryptoProvider,
                                                            11,
                                                            td->createdAt,
                                                            td->emptyAt,
                                                            td->nameEntries,
                                                            6,
                                                            td->pRsaPublicKey,
                                                            td->extension,
                                                            2,
                                                            OPCUA_P_SHA_256,
                                                            OpcUa_Null,
                                                            td->pRsaPrivateKey,
                                                            &td->pCertificate)
    , OpcUa_BadInvalidArgument);

    if(td->pCertificate != OpcUa_Null)
    {
        X509_free((X509*)td->pCertificate);
        td->pCertificate = OpcUa_Null;
    }

    END_CASE();

    /* empty comment */
    /* Basic test - Null Name List */
    /* empty comment */
    CASE_ID(17536);
    ASSERT_STATUS(td->myCryptoProvider.CreateCertificate(   &td->myCryptoProvider,
                                                            11,
                                                            td->createdAt,
                                                            td->expiresAt,
                                                            OpcUa_Null,
                                                            6,
                                                            td->pRsaPublicKey,
                                                            td->extension,
                                                            2,
                                                            OPCUA_P_SHA_256,
                                                            OpcUa_Null,
                                                            td->pRsaPrivateKey,
                                                            &td->pCertificate)
    , OpcUa_BadInvalidArgument);

    if(td->pCertificate != OpcUa_Null)
    {
        X509_free((X509*)td->pCertificate);
        td->pCertificate = OpcUa_Null;
    }

    END_CASE();

    /* empty comment */
    /* Basic test - Null Public Key */
    /* empty comment */
    CASE_ID(17537);
    ASSERT_STATUS(td->myCryptoProvider.CreateCertificate(   &td->myCryptoProvider,
                                                            11,
                                                            td->createdAt,
                                                            td->expiresAt,
                                                            td->nameEntries,
                                                            (OpcUa_UInt)0,
                                                            td->emptyKey,
                                                            td->extension,
                                                            2,
                                                            OPCUA_P_SHA_256,
                                                            OpcUa_Null,
                                                            td->pRsaPrivateKey,
                                                            &td->pCertificate)
    , OpcUa_BadInvalidArgument);

    if(td->pCertificate != OpcUa_Null)
    {
        X509_free((X509*)td->pCertificate);
        td->pCertificate = OpcUa_Null;
    }

    END_CASE();

    /* empty comment */
    /* Basic test - Null Extensions */
    /* empty comment */
    CASE_ID(17538);
    ASSERT_STATUS(td->myCryptoProvider.CreateCertificate(   &td->myCryptoProvider,
                                                            11,
                                                            td->createdAt,
                                                            td->expiresAt,
                                                            td->nameEntries,
                                                            6,
                                                            td->pRsaPublicKey,
                                                            OpcUa_Null,
                                                            2,
                                                            OPCUA_P_SHA_256,
                                                            OpcUa_Null,
                                                            td->pRsaPrivateKey,
                                                            &td->pCertificate)
    , OpcUa_BadInvalidArgument);

    if(td->pCertificate != OpcUa_Null)
    {
        X509_free((X509*)td->pCertificate);
        td->pCertificate = OpcUa_Null;
    }

    END_CASE();

    /* empty comment */
    /* Basic test - Invalid Hash Algorithm */
    /* empty comment */
    CASE_ID(17540);
    ASSERT_STATUS(td->myCryptoProvider.CreateCertificate(   &td->myCryptoProvider,
                                                            11,
                                                            td->createdAt,
                                                            td->expiresAt,
                                                            td->nameEntries,
                                                            6,
                                                            td->pRsaPublicKey,
                                                            td->extension,
                                                            2,
                                                            (OpcUa_UInt)-1,
                                                            OpcUa_Null,
                                                            td->pRsaPrivateKey,
                                                            &td->pCertificate)
    , OpcUa_BadInvalidArgument);

    if(td->pCertificate != OpcUa_Null)
    {
        X509_free((X509*)td->pCertificate);
        td->pCertificate = OpcUa_Null;
    }

    END_CASE();

    /* empty comment */
    /* Basic test - Null Private key */
    /* empty comment */
    CASE_ID(17541);
    ASSERT_STATUS(td->myCryptoProvider.CreateCertificate(   &td->myCryptoProvider,
                                                            11,
                                                            td->createdAt,
                                                            td->expiresAt,
                                                            td->nameEntries,
                                                            6,
                                                            td->pRsaPublicKey,
                                                            td->extension,
                                                            2,
                                                            OPCUA_P_SHA_256,
                                                            OpcUa_Null,
                                                            td->emptyKey,
                                                            &td->pCertificate)
    , OpcUa_BadInvalidArgument);

    if(td->pCertificate != OpcUa_Null)
    {
        X509_free((X509*)td->pCertificate);
        td->pCertificate = OpcUa_Null;
    }

    END_CASE();

    /* empty comment */
    /* Basic test - Null Certicate Pointer */
    /* empty comment */
    CASE_ID(17542);
    ASSERT_STATUS(  td->myCryptoProvider.CreateCertificate( &td->myCryptoProvider,
                                                            11,
                                                            td->createdAt,
                                                            td->expiresAt,
                                                            td->nameEntries,
                                                            6,
                                                            td->pRsaPublicKey,
                                                            td->extension,
                                                            2,
                                                            OPCUA_P_SHA_256,
                                                            OpcUa_Null,
                                                            td->pRsaPrivateKey,
                                                            OpcUa_Null),
                    OpcUa_BadInvalidArgument);

    if(td->pCertificate != OpcUa_Null)
    {
        X509_free((X509*)td->pCertificate);
        td->pCertificate = OpcUa_Null;
    }

    END_CASE();

    /*** clean up ***/

    if(td->pCertificate != OpcUa_Null)
    {
        X509_free((X509*)td->pCertificate);
        td->pCertificate = OpcUa_Null;
    }

    OpcUa_Test_CryptoKeyCleanup(&td->pRsaPublicKey, OpcUa_True);
    OpcUa_Test_CryptoKeyCleanup(&td->pRsaPrivateKey, OpcUa_True);

    return(OpcUa_Good);
}

/*============================================================================
 * OpcUa_Test_CryptoProvider_GetPrivateKeyFromCert
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoProvider_GetPrivateKeyFromCert(TESTCRYPTOPROVIDERDATA *td)
{
    SNAPSHOT_TYPE           s1, s2, s3;

    /* empty comment */
    /* Basic test - Normal */
    /* empty comment */
    CASE_ID(17543);
    MEM_SNAPSHOT(&s1);

    OpcUa_Key_Initialize(&td->pCertKey);

    ASSERT_GOOD(td->myCryptoProvider.GetPrivateKeyFromCert( &td->myCryptoProvider,
                                                              td->CertificateFile,
                                                              "\0",
                                                              &td->pCertKey));

    td->pCertKey.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pCertKey.Key.Length*sizeof(OpcUa_Byte));

    ASSERT_GOOD(td->myCryptoProvider.GetPrivateKeyFromCert( &td->myCryptoProvider,
                                                              td->CertificateFile,
                                                              "\0",
                                                              &td->pCertKey));

    OpcUa_Test_CryptoKeyCleanup(&td->pCertKey, OpcUa_True);

    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17544);

    OpcUa_Key_Initialize(&td->pCertKey);

    ASSERT_STATUS(td->myCryptoProvider.GetPrivateKeyFromCert(   OpcUa_Null,
                                                                td->CertificateFile,
                                                                "\0",
                                                                &td->pCertKey)
    , OpcUa_BadInvalidArgument);

    OpcUa_Test_CryptoKeyCleanup(&td->pCertKey, OpcUa_False);

    END_CASE();

    /* empty comment */
    /* Basic test - Null Certificate */
    /* empty comment */
    CASE_ID(17545);

    OpcUa_Key_Initialize(&td->pCertKey);

    ASSERT_STATUS(td->myCryptoProvider.GetPrivateKeyFromCert(   &td->myCryptoProvider,
                                                                OpcUa_Null,
                                                                "\0",
                                                                &td->pCertKey)
    , OpcUa_BadInvalidArgument);

    OpcUa_Test_CryptoKeyCleanup(&td->pCertKey, OpcUa_False);

    END_CASE();

    /* empty comment */
    /* Basic test - Null Password */
    /* empty comment */
    CASE_ID(17546);

    OpcUa_Key_Initialize(&td->pCertKey);

    ASSERT_STATUS(td->myCryptoProvider.GetPrivateKeyFromCert(   &td->myCryptoProvider,
                                                                td->CertificateFile,
                                                                OpcUa_Null,
                                                                &td->pCertKey)
    , OpcUa_BadInvalidArgument);

    OpcUa_Test_CryptoKeyCleanup(&td->pCertKey, OpcUa_False);

    END_CASE();

    /* empty comment */
    /* Basic test - Null Key Pointer */
    /* empty comment */
    CASE_ID(17547);

    OpcUa_Key_Initialize(&td->pCertKey);

    ASSERT_STATUS(td->myCryptoProvider.GetPrivateKeyFromCert(   &td->myCryptoProvider,
                                                                td->CertificateFile,
                                                                "\0",
                                                                OpcUa_Null)
    , OpcUa_BadInvalidArgument);

    OpcUa_Test_CryptoKeyCleanup(&td->pCertKey, OpcUa_False);

    END_CASE();

    return(OpcUa_Good);
}

/*============================================================================
 * OpcUa_Test_CryptoProvider_GetPublicKeyFromCert
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoProvider_GetPublicKeyFromCert(TESTCRYPTOPROVIDERDATA *td)
{
    SNAPSHOT_TYPE           s1, s2, s3;

    /* empty comment */
    /* Basic test - Normal */
    /* empty comment */
    CASE_ID(17548);
    MEM_SNAPSHOT(&s1);

    OpcUa_Key_Initialize(&td->pCertKey);

    ASSERT_GOOD(td->myCryptoProvider.GetPublicKeyFromCert(  &td->myCryptoProvider,
                                                              &td->pCertificateData,
                                                              "\0",
                                                              &td->pCertKey));

    td->pCertKey.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pCertKey.Key.Length*sizeof(OpcUa_Byte));

    ASSERT_GOOD(td->myCryptoProvider.GetPublicKeyFromCert(  &td->myCryptoProvider,
                                                              &td->pCertificateData,
                                                              "\0",
                                                              &td->pCertKey));

    OpcUa_Test_CryptoKeyCleanup(&td->pCertKey, OpcUa_True);

    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17549);

    OpcUa_Key_Initialize(&td->pCertKey);

    ASSERT_STATUS(td->myCryptoProvider.GetPublicKeyFromCert(    OpcUa_Null,
                                                                &td->pCertificateData,
                                                                "\0",
                                                                &td->pCertKey)
    , OpcUa_BadInvalidArgument);

    OpcUa_Test_CryptoKeyCleanup(&td->pCertKey, OpcUa_False);

    END_CASE();

    /* empty comment */
    /* Basic test - Null Certificate */
    /* empty comment */
    CASE_ID(17550);

    OpcUa_Key_Initialize(&td->pCertKey);

    ASSERT_STATUS(td->myCryptoProvider.GetPublicKeyFromCert(    &td->myCryptoProvider,
                                                                OpcUa_Null,
                                                                "\0",
                                                                &td->pCertKey)
    , OpcUa_BadInvalidArgument);

    OpcUa_Test_CryptoKeyCleanup(&td->pCertKey, OpcUa_False);

    END_CASE();

    /* empty comment */
    /* Basic test - Null Password */
    /* empty comment */
    CASE_ID(17551);

    OpcUa_Key_Initialize(&td->pCertKey);

    ASSERT_STATUS(td->myCryptoProvider.GetPublicKeyFromCert(    &td->myCryptoProvider,
                                                                &td->pCertificateData,
                                                                OpcUa_Null,
                                                                &td->pCertKey)
    , OpcUa_Good);

    OpcUa_Test_CryptoKeyCleanup(&td->pCertKey, OpcUa_False);

    END_CASE();

    /* empty comment */
    /* Basic test - Null Key Pointer */
    /* empty comment */
    CASE_ID(17552);
/* empty comment */
/* *** Crashes - will be fixed later with another allocation model (2007-04-23, SHL) */
/* empty comment */

    OpcUa_Key_Initialize(&td->pCertKey);

    ASSERT_STATUS(td->myCryptoProvider.GetPublicKeyFromCert(    &td->myCryptoProvider,
                                                                &td->pCertificateData,
                                                                "\0",
                                                                OpcUa_Null)
    , OpcUa_BadInvalidArgument);

    OpcUa_Test_CryptoKeyCleanup(&td->pCertKey, OpcUa_False);

    END_CASE();

    return(OpcUa_Good);
}

/*============================================================================
 * OpcUa_Test_CryptoProvider_GetSignatureFromCert
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoProvider_GetSignatureFromCert(TESTCRYPTOPROVIDERDATA *td)
{
    SNAPSHOT_TYPE           s1, s2, s3;

/* empty comment */
/* GetSignatureFromCert */
/* empty comment */
    /* empty comment */
    /* Basic test - Normal */
    /* empty comment */
    CASE_ID(17553);
    MEM_SNAPSHOT(&s1);
    OpcUa_Signature_Initialize(&td->pSignature);
    ASSERT_GOOD(td->myCryptoProvider.GetSignatureFromCert(&td->myCryptoProvider, &td->pCertificateData, &td->pSignature));
    td->pSignature.Signature.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pSignature.Signature.Length*sizeof(OpcUa_Byte));
    ASSERT_GOOD(td->myCryptoProvider.GetSignatureFromCert( &td->myCryptoProvider, &td->pCertificateData, &td->pSignature));
    OpcUa_Test_CryptoSignatureCleanup(&td->pSignature, OpcUa_True);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17554);
    OpcUa_Signature_Initialize(&td->pSignature);
    ASSERT_STATUS(td->myCryptoProvider.GetSignatureFromCert(OpcUa_Null, &td->pCertificateData, &td->pSignature), OpcUa_BadInvalidArgument);
    OpcUa_Test_CryptoSignatureCleanup(&td->pSignature, OpcUa_False);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Certificate */
    /* empty comment */
    CASE_ID(17555);
    OpcUa_Signature_Initialize(&td->pSignature);
    ASSERT_STATUS(td->myCryptoProvider.GetSignatureFromCert(&td->myCryptoProvider, OpcUa_Null, &td->pSignature), OpcUa_BadInvalidArgument);
    OpcUa_Test_CryptoSignatureCleanup(&td->pSignature, OpcUa_False);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Key Pointer */
    /* empty comment */
    CASE_ID(17556);
    OpcUa_Signature_Initialize(&td->pSignature);
    ASSERT_STATUS(td->myCryptoProvider.GetSignatureFromCert(&td->myCryptoProvider, &td->pCertificateData, OpcUa_Null), OpcUa_BadInvalidArgument);
    OpcUa_Test_CryptoSignatureCleanup(&td->pSignature, OpcUa_False);
    END_CASE();

    /* clean up */
    if(td->pCertificateData.Data)
    {
        OpcUa_Free(td->pCertificateData.Data);
        td->pCertificateData.Data = OpcUa_Null;
    }

    OpcUa_Test_CryptoKeyCleanup(&td->pRsaPublicKey, OpcUa_False);
    OpcUa_Test_CryptoKeyCleanup(&td->pRsaPrivateKey, OpcUa_False);

    return(OpcUa_Good);
}

/*============================================================================
 * OpcUa_Test_CryptoProvider_SymmetricEncrypt
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoProvider_SymmetricEncrypt(TESTCRYPTOPROVIDERDATA *td)
{

    OpcUa_Byte*         pTempBuffer = OpcUa_Null;
    SNAPSHOT_TYPE       s1, s2, s3;

    /* copy initial vector for decryption, since it will be changed during encryption */
    pTempBuffer = (OpcUa_Byte*)OpcUa_Alloc(td->initialVector.Length*sizeof(OpcUa_Byte));
    OpcUa_MemCpy(pTempBuffer, td->initialVector.Length, td->initialVector.Data, td->initialVector.Length);

    /* empty comment */
    /* Basic test - Normal */
    /* empty comment */
    CASE_ID(17557);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(td->myCryptoProvider.SymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, td->initialVector.Data, OpcUa_Null, &td->ciphertextLen));
    if(td->ciphertextLen > 0){td->pCiphertext = (OpcUa_Byte*)OpcUa_Alloc(td->ciphertextLen);}
    OpcUa_MemSet(td->pCiphertext, 0xAA, td->ciphertextLen);
    ASSERT_GOOD(td->myCryptoProvider.SymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, pTempBuffer, td->pCiphertext, &td->ciphertextLen));
    if(td->pCiphertext){OpcUa_Free(td->pCiphertext);td->pCiphertext = OpcUa_Null;}
    td->ciphertextLen = 0;
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Normal - Check Decrypted Matches Original */
    /* empty comment */
    CASE_ID(17558);
    MEM_SNAPSHOT(&s1);
    OpcUa_MemCpy(pTempBuffer, td->initialVector.Length, td->initialVector.Data, td->initialVector.Length);

    ASSERT_GOOD(td->myCryptoProvider.SymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, pTempBuffer, OpcUa_Null, &td->ciphertextLen));
    if(td->ciphertextLen > 0){td->pCiphertext = (OpcUa_Byte*)OpcUa_Alloc(td->ciphertextLen*sizeof(OpcUa_Byte));}
    OpcUa_MemCpy(pTempBuffer, td->initialVector.Length, td->initialVector.Data, td->initialVector.Length);
    ASSERT_GOOD(td->myCryptoProvider.SymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, pTempBuffer, td->pCiphertext, &td->ciphertextLen));

    OpcUa_MemCpy(pTempBuffer, td->initialVector.Length, td->initialVector.Data, td->initialVector.Length);

    ASSERT_GOOD(td->myCryptoProvider.SymmetricDecrypt(&td->myCryptoProvider, td->pCiphertext, td->ciphertextLen, &td->pClientKeyset.EncryptionKey, pTempBuffer, OpcUa_Null, &td->decryptedCiphertextLen));
    if(td->decryptedCiphertextLen > 0){td->pDecryptedCiphertext = (OpcUa_Byte*)OpcUa_Alloc(td->decryptedCiphertextLen*sizeof(OpcUa_Byte));}
    OpcUa_MemCpy(pTempBuffer, td->initialVector.Length, td->initialVector.Data, td->initialVector.Length);
    ASSERT_GOOD(td->myCryptoProvider.SymmetricDecrypt(&td->myCryptoProvider, td->pCiphertext, td->ciphertextLen, &td->pClientKeyset.EncryptionKey, pTempBuffer, td->pDecryptedCiphertext, &td->decryptedCiphertextLen));

    ASSERT(td->pDecryptedCiphertext);
    if(td->pDecryptedCiphertext)
    {
      if(td->decryptedCiphertextLen != td->plaintextLen){ASSERT_STATUS(OpcUa_BadNoMatch, OpcUa_Good);}
      if(OpcUa_MemCmp(td->pDecryptedCiphertext, td->pPlaintext, td->plaintextLen) != 0){ASSERT_STATUS(OpcUa_BadNoMatch, OpcUa_Good);}
    }
    else{ASSERT_STATUS(OpcUa_BadNoMatch, OpcUa_Good);}

    if(td->pDecryptedCiphertext){OpcUa_Free(td->pDecryptedCiphertext);td->pDecryptedCiphertext = OpcUa_Null;}
    if (td->pCiphertext){OpcUa_Free(td->pCiphertext);td->pCiphertext = OpcUa_Null;}
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

/* empty comment */
/* SymmetricEncrypt */
/* empty comment */
    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17559);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricEncrypt(OpcUa_Null, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, pTempBuffer, OpcUa_Null, &td->ciphertextLen), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Plain Text */
    /* empty comment */
    CASE_ID(17560);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricEncrypt( &td->myCryptoProvider, OpcUa_Null, td->plaintextLen, &td->pClientKeyset.EncryptionKey, pTempBuffer, td->pCiphertext, &td->ciphertextLen), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Zero Length */
    /* empty comment */
    CASE_ID(17561);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, 0, &td->pClientKeyset.EncryptionKey, pTempBuffer, OpcUa_Null, &td->ciphertextLen), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Encryption Key */
    /* empty comment */
    CASE_ID(17563);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->emptyKey, pTempBuffer, OpcUa_Null, &td->ciphertextLen), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Init Data */
    /* empty comment */
    CASE_ID(17564); /* copy initial vector for decryption, since it will be changed during encryption */
    ASSERT_STATUS(td->myCryptoProvider.SymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, OpcUa_Null, td->pCiphertext, &td->ciphertextLen), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Target Buffer Length */
    /* empty comment */
    CASE_ID(17566);/* copy initial vector for decryption, since it will be changed during encryption */
    ASSERT_STATUS(td->myCryptoProvider.SymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, pTempBuffer, OpcUa_Null, OpcUa_Null), OpcUa_BadInvalidArgument);
    END_CASE();

#if 0 /* CHZ: not finished? */
/* empty comment */
/* SymmetricDecrypt */
/* empty comment */
    /* empty comment */
    /* Common Data For Decryption Tests */
    /* empty comment */

    /* ToDo:    Does not encrypt since encryption key is 32 bytes
                and the provider uses an algorithm with 16 bytes.
                ->should be the same
    */
    uStatus = td->myCryptoProvider.SymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, td->initialVector.Data, &td->pCiphertext, &td->ciphertextLen);

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17567);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricDecrypt(OpcUa_Null, td->pCiphertext, td->ciphertextLen, &td->pClientKeyset.EncryptionKey, td->initialVector.Data, &td->pDecryptedCiphertext, &td->decryptedCiphertextLen), OpcUa_BadInvalidArgument);
    if (td->pDecryptedCiphertext){OpcUa_Free(td->pDecryptedCiphertext); td->pDecryptedCiphertext = OpcUa_Null;}
    END_CASE();

    /* empty comment */
    /* Basic test - Null Encrypted Text */
    /* empty comment */
    CASE_ID(17568);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricDecrypt(&td->myCryptoProvider, OpcUa_Null, td->ciphertextLen, &td->pClientKeyset.EncryptionKey, td->initialVector.Data, &td->pDecryptedCiphertext, &td->decryptedCiphertextLen), OpcUa_BadInvalidArgument);
    if (td->pDecryptedCiphertext){OpcUa_Free(td->pDecryptedCiphertext); td->pDecryptedCiphertext = OpcUa_Null;
    END_CASE();

    /* empty comment */
    /* Basic test - Zero Encrypted Text Length */
    /* empty comment */
    CASE_ID(17569);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricDecrypt(&td->myCryptoProvider, td->pCiphertext, 0, &td->pClientKeyset.EncryptionKey, td->initialVector.Data, &td->pDecryptedCiphertext, &td->decryptedCiphertextLen), OpcUa_BadInvalidArgument);
    if (td->pDecryptedCiphertext){OpcUa_Free(td->pDecryptedCiphertext); td->pDecryptedCiphertext = OpcUa_Null;
    END_CASE();

    /* empty comment */
    /* Basic test - Negative Encrypted Text Length */
    /* empty comment */
    CASE_ID(17570);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricDecrypt(&td->myCryptoProvider, td->pCiphertext, -1, &td->pClientKeyset.EncryptionKey, td->initialVector.Data, &td->pDecryptedCiphertext, &td->decryptedCiphertextLen), OpcUa_BadInvalidArgument);
    if (td->pDecryptedCiphertext){OpcUa_Free(td->pDecryptedCiphertext);td->pDecryptedCiphertext = OpcUa_Null;}
    END_CASE();

    /* empty comment */
    /* Basic test - Null Decryption Key */
    /* empty comment */
    CASE_ID(17571);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricDecrypt(&td->myCryptoProvider, td->pCiphertext, td->ciphertextLen, &td->emptyKey, td->initialVector.Data, &td->pDecryptedCiphertext, &td->decryptedCiphertextLen), OpcUa_BadInvalidArgument);
    if (td->pDecryptedCiphertext){OpcUa_Free(td->pDecryptedCiphertext);td->pDecryptedCiphertext = OpcUa_Null;}
    END_CASE();

    /* empty comment */
    /* Basic test - Null Init Data */
    /* empty comment */
    CASE_ID(17572);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricDecrypt(&td->myCryptoProvider, td->pCiphertext, td->ciphertextLen, &td->pClientKeyset.EncryptionKey, OpcUa_Null, &td->pDecryptedCiphertext, &td->decryptedCiphertextLen), OpcUa_BadInvalidArgument);
    if (td->pDecryptedCiphertext){OpcUa_Free(td->pDecryptedCiphertext);td->pDecryptedCiphertext = OpcUa_Null;}
    END_CASE();

    /* empty comment */
    /* Basic test - Null Target Buffer Pointer */
    /* empty comment */
    CASE_ID(17573);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricDecrypt(&td->myCryptoProvider, td->pCiphertext, td->ciphertextLen, &td->pClientKeyset.EncryptionKey, td->initialVector.Data, OpcUa_Null, &td->decryptedCiphertextLen), OpcUa_BadInvalidArgument);
    if (td->pDecryptedCiphertext){OpcUa_Free(td->pDecryptedCiphertext); td->pDecryptedCiphertext = OpcUa_Null;
    END_CASE();

    /* empty comment */
    /* Basic test - Null Target Buffer Length */
    /* empty comment */
    CASE_ID(17574);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricDecrypt(&td->myCryptoProvider, td->pCiphertext, td->ciphertextLen, &td->pClientKeyset.EncryptionKey, td->initialVector.Data, &td->pDecryptedCiphertext, OpcUa_Null), OpcUa_BadInvalidArgument);
    if (td->pDecryptedCiphertext){OpcUa_Free(td->pDecryptedCiphertext); td->pDecryptedCiphertext = OpcUa_Null;
    END_CASE();
#endif /* CHZ: not finished? */

/* empty comment */
/* Cleanup Common Data */
/* empty comment */
    if(td->pCiphertext)
    {
        OpcUa_Free(td->pCiphertext);
        td->pCiphertext = OpcUa_Null;
    }

    if(pTempBuffer)
    {
        OpcUa_Free(pTempBuffer);
        pTempBuffer = OpcUa_Null;
    }

    return(OpcUa_Good);
}

/*============================================================================
 * OpcUa_Test_CryptoProvider_AsymmetricEncrypt
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoProvider_AsymmetricEncrypt(TESTCRYPTOPROVIDERDATA *td)
{
    OpcUa_StatusCode        uStatus         = OpcUa_Good;
    SNAPSHOT_TYPE           s1, s2, s3;

    /* empty comment */
    /* Basic test - Normal */
    /* empty comment */
    CASE_ID(17575);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(td->myCryptoProvider.AsymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pPublicKey, OpcUa_Null, &td->ciphertextLen));
    td->pCiphertext = (OpcUa_Byte*)OpcUa_Alloc(td->ciphertextLen*sizeof(OpcUa_Byte));
    ASSERT_GOOD(td->myCryptoProvider.AsymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pPublicKey, td->pCiphertext, &td->ciphertextLen));
    ASSERT(td->pCiphertext);
    if(td->pCiphertext){OpcUa_Free(td->pCiphertext);td->pCiphertext = OpcUa_Null;}
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Normal - Check Decrypted Matches Original */
    /* empty comment */
    CASE_ID(17576);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(td->myCryptoProvider.AsymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pPublicKey, OpcUa_Null, &td->ciphertextLen));
    td->pCiphertext = (OpcUa_Byte*)OpcUa_Alloc(td->ciphertextLen);
    ASSERT_GOOD(td->myCryptoProvider.AsymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pPublicKey, td->pCiphertext, &td->ciphertextLen));
    ASSERT_GOOD(td->myCryptoProvider.AsymmetricDecrypt(&td->myCryptoProvider, td->pCiphertext, td->ciphertextLen, &td->privateKey, OpcUa_Null, &td->decryptedCiphertextLen));
    td->pDecryptedCiphertext = (OpcUa_Byte*)OpcUa_Alloc(td->decryptedCiphertextLen);
    ASSERT_GOOD(td->myCryptoProvider.AsymmetricDecrypt(&td->myCryptoProvider, td->pCiphertext, td->ciphertextLen, &td->privateKey, td->pDecryptedCiphertext, &td->decryptedCiphertextLen));
    ASSERT(td->pDecryptedCiphertext);
    if(td->pDecryptedCiphertext)
    {
        if(td->decryptedCiphertextLen != td->plaintextLen){ASSERT_STATUS(OpcUa_BadNoMatch, OpcUa_Good);}
        if(OpcUa_MemCmp(td->pDecryptedCiphertext, td->pPlaintext, td->plaintextLen) != 0){ASSERT_STATUS(OpcUa_BadNoMatch, OpcUa_Good);}
    }
    else{ASSERT_STATUS(OpcUa_BadNoMatch, OpcUa_Good);}
    if(td->pDecryptedCiphertext){OpcUa_Free(td->pDecryptedCiphertext);td->pDecryptedCiphertext = OpcUa_Null;}
    if(td->pCiphertext){OpcUa_Free(td->pCiphertext);td->pCiphertext = OpcUa_Null;}
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

/* empty comment */
/* AsymmetricEncrypt */
/* empty comment */
    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17577);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricEncrypt(OpcUa_Null, td->pPlaintext, td->plaintextLen, &td->pPublicKey, td->pCiphertext, &td->ciphertextLen), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Plain Text */
    /* empty comment */
    CASE_ID(17578);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricEncrypt(&td->myCryptoProvider, OpcUa_Null, td->plaintextLen, &td->pPublicKey, td->pCiphertext, &td->ciphertextLen), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Zero Length */
    /* empty comment */
    CASE_ID(17579);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, 0, &td->pPublicKey, td->pCiphertext, &td->ciphertextLen), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Encryption Key */
    /* empty comment */
    CASE_ID(17581);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->emptyKey, td->pCiphertext, &td->ciphertextLen), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Target Buffer Length */
    /* empty comment */
    CASE_ID(17583);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pPublicKey, td->pCiphertext, OpcUa_Null), OpcUa_BadInvalidArgument);
    END_CASE();

/* empty comment */
/* AsymmetricDecrypt */
/* empty comment */
    /* empty comment */
    /* Common Data For Decryption Tests */
    /* empty comment */
    uStatus = td->myCryptoProvider.AsymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pPublicKey, OpcUa_Null, &td->ciphertextLen);
    ASSERT(OpcUa_IsGood(uStatus));
    td->pCiphertext = (OpcUa_Byte*)OpcUa_Alloc(td->ciphertextLen*sizeof(OpcUa_Byte));
    uStatus = td->myCryptoProvider.AsymmetricEncrypt(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pPublicKey, td->pCiphertext, &td->ciphertextLen);
    ASSERT(OpcUa_IsGood(uStatus));

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17584);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricDecrypt(OpcUa_Null, td->pCiphertext, td->ciphertextLen, &td->privateKey, td->pDecryptedCiphertext, &td->decryptedCiphertextLen), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Encrypted Text */
    /* empty comment */
    CASE_ID(17585);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricDecrypt(&td->myCryptoProvider, OpcUa_Null, td->ciphertextLen, &td->privateKey, td->pDecryptedCiphertext, &td->decryptedCiphertextLen), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Zero Encrypted Text Length */
    /* empty comment */
    CASE_ID(17586);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricDecrypt(&td->myCryptoProvider, td->pCiphertext, 0, &td->privateKey, td->pDecryptedCiphertext, &td->decryptedCiphertextLen), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Decryption Key */
    /* empty comment */
    CASE_ID(17588);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricDecrypt(&td->myCryptoProvider, td->pCiphertext, td->ciphertextLen, &td->emptyKey, td->pDecryptedCiphertext, &td->decryptedCiphertextLen), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Target Buffer Length */
    /* empty comment */
    CASE_ID(17590);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricDecrypt(&td->myCryptoProvider, td->pCiphertext, td->ciphertextLen, &td->privateKey, td->pDecryptedCiphertext, OpcUa_Null), OpcUa_BadInvalidArgument);
    END_CASE();


/* empty comment */
/* Cleanup Common Data */
/* empty comment */
    if(td->pCiphertext)
    {
        OpcUa_Free(td->pCiphertext);
        td->pCiphertext = OpcUa_Null;
    }

    return(OpcUa_Good);
}

/*============================================================================
 * OpcUa_Test_CryptoProvider_SymmetricSign
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoProvider_SymmetricSign(TESTCRYPTOPROVIDERDATA *td)
{
    OpcUa_StatusCode        uStatus;
    SNAPSHOT_TYPE           s1, s2, s3;

    /* empty comment */
    /* Basic test - Normal */
    /* empty comment */
    CASE_ID(17591);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(td->myCryptoProvider.SymmetricSign(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, &td->pByteSignature));
    if(td->pByteSignature.Length > 0){td->pByteSignature.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pByteSignature.Length*sizeof(OpcUa_Byte));}
    ASSERT_GOOD(td->myCryptoProvider.SymmetricSign(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, &td->pByteSignature));
    OpcUa_Test_CryptoByteStringCleanup(&td->pByteSignature, OpcUa_True);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Normal - Check Signature */
    /* empty comment */
    CASE_ID(17592);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(td->myCryptoProvider.SymmetricSign(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, &td->pByteSignature));
    if(td->pByteSignature.Length > 0){td->pByteSignature.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pByteSignature.Length*sizeof(OpcUa_Byte));}
    ASSERT_GOOD(td->myCryptoProvider.SymmetricSign(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, &td->pByteSignature));
    ASSERT_GOOD(td->myCryptoProvider.SymmetricVerify(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, &td->pByteSignature));
    OpcUa_Test_CryptoByteStringCleanup(&td->pByteSignature, OpcUa_False);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

/* empty comment */
/* SymmetricSign */
/* empty comment */
    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17593);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricSign(OpcUa_Null, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, &td->pByteSignature), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Plain Text */
    /* empty comment */
    CASE_ID(17594);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricSign(&td->myCryptoProvider, OpcUa_Null, td->plaintextLen, &td->pClientKeyset.EncryptionKey, &td->pByteSignature), OpcUa_BadInvalidArgument);
    OpcUa_Test_CryptoByteStringCleanup(&td->pByteSignature, OpcUa_False);
    END_CASE();

    /* empty comment */
    /* Basic test - Zero Length */
    /* empty comment */
    CASE_ID(17595);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricSign(&td->myCryptoProvider, td->pPlaintext, 0, &td->pClientKeyset.EncryptionKey, &td->pByteSignature), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Encryption Key */
    /* empty comment */
    CASE_ID(17597);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricSign(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->emptyKey, &td->pByteSignature), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Signature */
    /* empty comment */
    CASE_ID(17598);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricSign(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, OpcUa_Null), OpcUa_BadInvalidArgument);
    END_CASE();

/* empty comment */
/* SymmetricVerify */
/* empty comment */
    /* empty comment */
    /* Common Data For Decryption Tests */
    /* empty comment */
    uStatus = td->myCryptoProvider.SymmetricSign(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, &td->pByteSignature);
    ASSERT(OpcUa_IsGood(uStatus));
    if(td->pByteSignature.Length > 0){td->pByteSignature.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pByteSignature.Length*sizeof(OpcUa_Byte));}
    uStatus = td->myCryptoProvider.SymmetricSign(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, &td->pByteSignature);
    ASSERT(OpcUa_IsGood(uStatus));

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17599);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricVerify(OpcUa_Null, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, &td->pByteSignature), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Plain Text */
    /* empty comment */
    CASE_ID(17600);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricVerify(&td->myCryptoProvider, OpcUa_Null, td->plaintextLen, &td->pClientKeyset.EncryptionKey, &td->pByteSignature), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Zero length */
    /* empty comment */
    CASE_ID(17601);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricVerify(&td->myCryptoProvider, td->pPlaintext, 0, &td->pClientKeyset.EncryptionKey, &td->pByteSignature), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Decryption Key */
    /* empty comment */
    CASE_ID(17603);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricVerify(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->emptyKey, &td->pByteSignature), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Signature */
    /* empty comment */
    CASE_ID(17604);
    ASSERT_STATUS(td->myCryptoProvider.SymmetricVerify(&td->myCryptoProvider, td->pPlaintext, td->plaintextLen, &td->pClientKeyset.EncryptionKey, OpcUa_Null), OpcUa_BadInvalidArgument);
    END_CASE();

/* empty comment */
/* Cleanup Common Data */
/* empty comment */
    OpcUa_Test_CryptoByteStringCleanup(&td->pByteSignature, OpcUa_True);

    return(OpcUa_Good);
}

/*============================================================================
 * OpcUa_Test_CryptoProvider_AsymmetricSign
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoProvider_AsymmetricSign(TESTCRYPTOPROVIDERDATA *td)
{
    OpcUa_ByteString        pPlaintext = OPCUA_BYTESTRING_STATICINITIALIZER;
    OpcUa_StatusCode        uStatus = OpcUa_Good;
    SNAPSHOT_TYPE           s1, s2, s3;
    OpcUa_UInt32            uBits = 0;

    pPlaintext.Length = td->plaintextLen;
    pPlaintext.Data = td->pPlaintext;

    /* empty comment */
    /* Basic test - Normal */
    /* empty comment */
    CASE_ID(17605);
    MEM_SNAPSHOT(&s1);
    OpcUa_Crypto_GetAsymmetricKeyLength(&td->myCryptoProvider, td->pPublicKey, &uBits);
    td->pByteSignature.Length = uBits/8;
    td->pByteSignature.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pByteSignature.Length);
    ASSERT_GOOD(td->myCryptoProvider.AsymmetricSign(&td->myCryptoProvider, pPlaintext, &td->privateKey, &td->pByteSignature));
    OpcUa_Test_CryptoByteStringCleanup(&td->pByteSignature, OpcUa_True);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* empty comment */
    /* Basic test - Normal - Check Signature */
    /* empty comment */
    CASE_ID(17606);
    MEM_SNAPSHOT(&s1);
    OpcUa_Crypto_GetAsymmetricKeyLength(&td->myCryptoProvider, td->pPublicKey, &uBits);
    td->pByteSignature.Length = uBits/8;
    td->pByteSignature.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pByteSignature.Length);
    ASSERT_GOOD(td->myCryptoProvider.AsymmetricSign(&td->myCryptoProvider, pPlaintext, &td->privateKey, &td->pByteSignature));
    ASSERT_GOOD(td->myCryptoProvider.AsymmetricVerify(&td->myCryptoProvider, pPlaintext, &td->pPublicKey, &td->pByteSignature));
    OpcUa_Test_CryptoByteStringCleanup(&td->pByteSignature, OpcUa_False);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

/* empty comment */
/* AsymmetricSign */
/* empty comment */
    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17607);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricSign(OpcUa_Null, pPlaintext, &td->privateKey, &td->pByteSignature), OpcUa_BadInvalidArgument);
    OpcUa_Test_CryptoByteStringCleanup(&td->pByteSignature, OpcUa_False);
    END_CASE();

    /* empty comment */
    /* Basic test - Null In Plain Text ByteString */
    /* empty comment */
    CASE_ID(17608);
    pPlaintext.Data = OpcUa_Null;
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricSign(&td->myCryptoProvider, pPlaintext, &td->privateKey, &td->pByteSignature), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Encryption Key */
    /* empty comment */
    CASE_ID(17609);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricSign(&td->myCryptoProvider, pPlaintext, &td->emptyKey, &td->pByteSignature), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Signature Pointer */
    /* empty comment */
    CASE_ID(17610);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricSign(&td->myCryptoProvider, pPlaintext, &td->privateKey, OpcUa_Null), OpcUa_BadInvalidArgument);
    END_CASE();


/* empty comment */
/* AsymmetricVerify */
/* empty comment */
    /* empty comment */
    /* Common Data For Decryption Tests */
    /* empty comment */
    pPlaintext.Data = td->pPlaintext;
    OpcUa_Crypto_GetAsymmetricKeyLength(&td->myCryptoProvider, td->pPublicKey, &uBits);
    td->pByteSignature.Length = uBits/8;
    td->pByteSignature.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pByteSignature.Length);
    uStatus = td->myCryptoProvider.AsymmetricSign(&td->myCryptoProvider, pPlaintext, &td->privateKey, &td->pByteSignature);
    ASSERT(OpcUa_IsGood(uStatus));

    /* empty comment */
    /* Basic test - Null Provider */
    /* empty comment */
    CASE_ID(17611);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricVerify(OpcUa_Null, pPlaintext, &td->pPublicKey, &td->pByteSignature), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null In Plain Text ByteString */
    /* empty comment */
    CASE_ID(17612);
    pPlaintext.Data = OpcUa_Null;
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricVerify(&td->myCryptoProvider, pPlaintext, &td->pPublicKey, &td->pByteSignature), OpcUa_BadInvalidArgument);
    pPlaintext.Data = td->pPlaintext;
    END_CASE();

    /* empty comment */
    /* Basic test - Null Decryption Key */
    /* empty comment */
    CASE_ID(17613);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricVerify(&td->myCryptoProvider, pPlaintext, &td->emptyKey, &td->pByteSignature), OpcUa_BadInvalidArgument);
    END_CASE();

    /* empty comment */
    /* Basic test - Null Signature */
    /* empty comment */
    CASE_ID(17614);
    ASSERT_STATUS(td->myCryptoProvider.AsymmetricVerify(&td->myCryptoProvider, pPlaintext, &td->pPublicKey, OpcUa_Null), OpcUa_BadInvalidArgument);
    END_CASE();

/* empty comment */
/* Cleanup Common Data */
/* empty comment */
    OpcUa_Test_CryptoByteStringCleanup(&td->pByteSignature, OpcUa_True);

    return(OpcUa_Good);
}

/*============================================================================
 * OpcUa_Test_CryptoProvider
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoProvider(OpcUa_StringA sUri)
{
    OpcUa_StatusCode        uStatus;
    TESTCRYPTOPROVIDERDATA  TestData, *td;

    OpcUa_MemSet(&TestData, 0, sizeof(TESTCRYPTOPROVIDERDATA));
    TestData.plaintextLen = OPCUA_SECURITY_TEST_PLAINTEXT_LENGTH;
    td = &TestData;

    /* printf("\nTest CryptoProvider - 17500\n"); */

/* empty comment */
/* Generate needed test objects */
/* empty comment */
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(sUri, &td->myCryptoPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    td->empty.Length = 0;
    td->empty.Data = OpcUa_Null;

    td->seed.Length = 32;
    td->seed.Data = (OpcUa_Byte*)OpcUa_Memory_Alloc(td->seed.Length*sizeof(OpcUa_Byte));
    OpcUa_GotoErrorIfAllocFailed(td->seed.Data);

    td->secret.Length = 32;
    td->secret.Data = (OpcUa_Byte*)OpcUa_Memory_Alloc(td->secret.Length*sizeof(OpcUa_Byte));
    OpcUa_GotoErrorIfAllocFailed(td->secret.Data);

    memcpy((void *)td->seed.Data, (void *)randomData1, td->seed.Length);
    memcpy((void *)td->secret.Data, (void *)randomData2, td->secret.Length);

    uStatus = OpcUa_CryptoProvider_Create(  OpcUa_String_GetRawString(&td->myCryptoPolicy),
                                            &td->myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_Test_CryptoProvider_GenerateKey(td);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_Test_CryptoProvider_DeriveKey(td);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_Test_CryptoProvider_DeriveChannelKeysets(td);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_Test_CryptoProvider_GenerateAsymmetricKeypair(td);
    OpcUa_GotoErrorIfBad(uStatus);

#if 0 /* CHZ: Fix required TODO */
    uStatus = OpcUa_Test_CryptoProvider_CreateCertificate(td);
    OpcUa_GotoErrorIfBad(uStatus);
#endif

    uStatus = OpcUa_TestHelper_GetDEREncodedCertificateFromFile(sCertificateFile, &td->pCertificateData);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_Test_CryptoProvider_GetPublicKeyFromCert(td);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_Test_CryptoProvider_GetSignatureFromCert(td);
    OpcUa_GotoErrorIfBad(uStatus);

/* empty comment */
/* Common data for Encryption/Decryption & Sign/Verify tests */
/* empty comment */
    OpcUa_Key_Initialize(&td->privateKey);

    uStatus = OpcUa_TestHelper_GetDEREncodedPrivateKeyFromFile(sPrivateKeyFile, &td->privateKey);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_TestHelper_GetDEREncodedCertificateFromFile(sCertificateFile, &td->pCertificateData);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_Key_Initialize(&td->pPublicKey);
    uStatus = td->myCryptoProvider.GetPublicKeyFromCert(&td->myCryptoProvider, &td->pCertificateData, OpcUa_Null, &td->pPublicKey);
    OpcUa_GotoErrorIfBad(uStatus);

    td->pPublicKey.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pPublicKey.Key.Length*sizeof(OpcUa_Byte));
    uStatus = td->myCryptoProvider.GetPublicKeyFromCert(&td->myCryptoProvider, &td->pCertificateData, OpcUa_Null, &td->pPublicKey);
    ASSERT(OpcUa_IsGood(uStatus));

    td->pPlaintext = (OpcUa_Byte*)OpcUa_Alloc(td->plaintextLen*sizeof(OpcUa_Byte));
    OpcUa_MemCpy(td->pPlaintext, td->plaintextLen, sPlaintext, td->plaintextLen);

    OpcUa_Key_Initialize(&td->pServerNonce);
    uStatus = td->myCryptoProvider.GenerateKey(&td->myCryptoProvider, -1, &td->pServerNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    td->pServerNonce.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pServerNonce.Key.Length*sizeof(OpcUa_Byte));

    uStatus = td->myCryptoProvider.GenerateKey(&td->myCryptoProvider, -1, &td->pServerNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_Key_Initialize(&td->pClientNonce);
    uStatus = td->myCryptoProvider.GenerateKey(&td->myCryptoProvider, -1, &td->pClientNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    td->pClientNonce.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pClientNonce.Key.Length*sizeof(OpcUa_Byte));

    uStatus = td->myCryptoProvider.GenerateKey(&td->myCryptoProvider, -1, &td->pClientNonce);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = td->myCryptoProvider.DeriveChannelKeysets(&td->myCryptoProvider, td->pClientNonce.Key, td->pServerNonce.Key, td->pServerNonce.Key.Length, &td->pClientKeyset, &td->pServerKeyset);
    OpcUa_GotoErrorIfBad(uStatus);

    td->pClientKeyset.EncryptionKey.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pClientKeyset.EncryptionKey.Key.Length*sizeof(OpcUa_Byte));
    td->pClientKeyset.InitializationVector.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pClientKeyset.InitializationVector.Key.Length*sizeof(OpcUa_Byte));
    td->pClientKeyset.SigningKey.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pClientKeyset.SigningKey.Key.Length*sizeof(OpcUa_Byte));

    td->pServerKeyset.EncryptionKey.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pServerKeyset.EncryptionKey.Key.Length*sizeof(OpcUa_Byte));
    td->pServerKeyset.InitializationVector.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pServerKeyset.InitializationVector.Key.Length*sizeof(OpcUa_Byte));
    td->pServerKeyset.SigningKey.Key.Data = (OpcUa_Byte*)OpcUa_Alloc(td->pServerKeyset.SigningKey.Key.Length*sizeof(OpcUa_Byte));

    uStatus = td->myCryptoProvider.DeriveChannelKeysets(&td->myCryptoProvider, td->pClientNonce.Key, td->pServerNonce.Key, td->pServerNonce.Key.Length, &td->pClientKeyset, &td->pServerKeyset);
    OpcUa_GotoErrorIfBad(uStatus);

    td->initialVector.Length = td->pClientKeyset.InitializationVector.Key.Length;
    td->initialVector.Data = (OpcUa_Byte*)OpcUa_Alloc(td->initialVector.Length*sizeof(OpcUa_Byte));
    OpcUa_MemCpy(td->initialVector.Data, td->initialVector.Length, td->pClientKeyset.InitializationVector.Key.Data, td->initialVector.Length);

/* empty comment */
/* Encryption/Decryption & Sign/Verify tests */
/* empty comment */

    uStatus = OpcUa_Test_CryptoProvider_SymmetricEncrypt(td);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_Test_CryptoProvider_AsymmetricEncrypt(td);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_Test_CryptoProvider_SymmetricSign(td);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_Test_CryptoProvider_AsymmetricSign(td);
    OpcUa_GotoErrorIfBad(uStatus);

/* empty comment */
/* Cleanup Common Data */
/* empty comment */
    OpcUa_Test_CryptoKeysetCleanup(&td->pClientKeyset, OpcUa_False);
    OpcUa_Test_CryptoKeysetCleanup(&td->pServerKeyset, OpcUa_False);

    OpcUa_Test_CryptoKeyCleanup(&td->pClientNonce, OpcUa_False);
    OpcUa_Test_CryptoKeyCleanup(&td->pServerNonce, OpcUa_False);

    if (td->initialVector.Data)
    {
        OpcUa_Free(td->initialVector.Data);
        td->initialVector.Data = OpcUa_Null;
    }

    if (td->pPlaintext)
    {
        OpcUa_Free(td->pPlaintext);
        td->pPlaintext = OpcUa_Null;
    }

    OpcUa_Test_CryptoKeyCleanup(&td->pPublicKey, OpcUa_False);
    OpcUa_Test_CryptoKeyCleanup(&td->privateKey, OpcUa_False);

    OpcUa_Test_CryptoByteStringCleanup(&(td->pCertificateData), OpcUa_False);

/* empty comment */
/* Cleanup test objects */
/* empty comment */
    OpcUa_CryptoProvider_Delete(&td->myCryptoProvider);

    OpcUa_Free(td->seed.Data);
    OpcUa_Free(td->secret.Data);

    OpcUa_TestHelper_CleanupCryptoPolicy(&td->myCryptoPolicy);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    printf("Test did not finish!\n");

/* empty comment */
/* Cleanup Common Data */
/* empty comment */

    OpcUa_Test_CryptoKeysetCleanup(&td->pClientKeyset, OpcUa_False);

    OpcUa_Test_CryptoKeysetCleanup(&td->pServerKeyset, OpcUa_False);


    OpcUa_Test_CryptoKeyCleanup(&td->pClientNonce, OpcUa_False);

    OpcUa_Test_CryptoKeyCleanup(&td->pServerNonce, OpcUa_False);

    if (td->initialVector.Data)
    {
      OpcUa_Free(td->initialVector.Data);
      td->initialVector.Data = OpcUa_Null;
    }

    if (td->pPlaintext)
    {
      OpcUa_Free(td->pPlaintext);
      td->pPlaintext = OpcUa_Null;
    }

    OpcUa_Test_CryptoByteStringCleanup(&(td->pCertificateData), OpcUa_False);

    OpcUa_Test_CryptoKeyCleanup(&td->pPublicKey, OpcUa_False);
    OpcUa_Test_CryptoKeyCleanup(&td->privateKey, OpcUa_False);

/* empty comment */
/* Cleanup test objects */
/* empty comment */
    if(td->seed.Data != OpcUa_Null)
    {
        OpcUa_Free(td->seed.Data);
    }

    if(td->secret.Data != OpcUa_Null)
    {
        OpcUa_Free(td->secret.Data);
    }

    OpcUa_TestHelper_CleanupCryptoPolicy(&td->myCryptoPolicy);

OpcUa_FinishErrorHandling;
}
