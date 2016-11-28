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

#include <unittest.h>

#include <opcua_core.h>

#include <opcua_cryptofactory_test.h>
#include <opcua_cryptofactory_dev_test.h>
#include <opcua_cryptoprovider_test.h>
#include <opcua_cryptoprovider_dev_test.h>

#include <opcua_pkifactory_test.h>
#include <opcua_pkifactory_dev_test.h>
#include <opcua_pkiprovider_test.h>
#include <opcua_pkiprovider_dev_test.h>
/* #include <opcua_pkiapi_test.h> deactivated to wrong design */

#include <securitytest.h>

/* some actions lead to singleton allocations in OpenSSL which are  */
/* falsely recognized as memory leaks by the tests. To prevent this */
/* some dry runs are made in this function.                         */
/* TODO: Extend for use with other providers or make optional.      */
 OpcUa_StatusCode OpcUa_SecurityTest_WarmUp()
{
    OpcUa_CertificateStoreConfiguration     CertificateStoreCfg;
    OpcUa_PKIProvider                       myPKIProvider;
    OpcUa_CryptoProvider                    myCryptoProvider;
    /* SNAPSHOT_TYPE                           s1, s2, s3; */
    OpcUa_Void*                             pCertificateStore   = OpcUa_Null;
    OpcUa_StatusCode                        uStatus             = OpcUa_Good;
    /* OpcUa_Int                               bLeak               = OpcUa_False; */
    OpcUa_Key                               RsaPublicKey;
    OpcUa_Key                               RsaPrivateKey;

    printf("Warming up ... ");

    /********/

    OpcUa_CertificateStoreConfiguration_Initialize(&CertificateStoreCfg);
    OpcUa_MemSet(&myPKIProvider, 0, sizeof(OpcUa_PKIProvider));
    OpcUa_MemSet(&RsaPrivateKey, 0, sizeof(OpcUa_Key));
    OpcUa_MemSet(&RsaPublicKey, 0, sizeof(OpcUa_Key));

    CertificateStoreCfg.strPkiType                              = OPCUA_P_PKI_TYPE_OPENSSL;
    CertificateStoreCfg.strTrustedCertificateListLocation       = OPCUA_SECURITY_TEST_CA_DIR;
    CertificateStoreCfg.strRevokedCertificateListLocation       = OPCUA_SECURITY_TEST_CLR_DIR;
    CertificateStoreCfg.strIssuerCertificateStoreLocation       = OPCUA_SECURITY_TEST_CA_DIR;
    CertificateStoreCfg.strRevokedIssuerCertificateListLocation = OPCUA_SECURITY_TEST_CA_DIR;


    /* MEM_SNAPSHOT(&s1); */
    uStatus = OpcUa_PKIProvider_Create(&CertificateStoreCfg, &myPKIProvider);
    if(OpcUa_IsGood(uStatus))
    {
        uStatus = myPKIProvider.OpenCertificateStore(&myPKIProvider, &pCertificateStore);
        if(OpcUa_IsGood(uStatus))
        {
            uStatus = myPKIProvider.CloseCertificateStore(&myPKIProvider, &pCertificateStore);
            if(OpcUa_IsGood(uStatus))
            {
                uStatus = OpcUa_PKIProvider_Delete(&myPKIProvider);
                if(OpcUa_IsGood(uStatus))
                {
                    /* MEM_SNAPSHOT(&s2); */
                    /* bLeak = _CrtMemDifference(&s3, &s1, &s2); */

                    /********/

                    /* MEM_SNAPSHOT(&s1); */
                    uStatus = OpcUa_CryptoProvider_Create(OpcUa_SecurityPolicy_Basic128Rsa15, &myCryptoProvider);
                    if(OpcUa_IsGood(uStatus))
                    {
                        uStatus = myCryptoProvider.GenerateAsymmetricKeypair(&myCryptoProvider, OpcUa_Crypto_Rsa_Id, 1024, &RsaPublicKey, &RsaPrivateKey);
                        if(OpcUa_IsGood(uStatus))
                        {
                            RsaPublicKey.Key.Data = OpcUa_Alloc(RsaPublicKey.Key.Length);
                            RsaPrivateKey.Key.Data = OpcUa_Alloc(RsaPrivateKey.Key.Length);
                            myCryptoProvider.GenerateAsymmetricKeypair(&myCryptoProvider, OpcUa_Crypto_Rsa_Id, 1024, &RsaPublicKey, &RsaPrivateKey);
                            OpcUa_Key_Clear(&RsaPublicKey);
                            OpcUa_Key_Clear(&RsaPrivateKey);
                            OpcUa_CryptoProvider_Delete(&myCryptoProvider);
                            /* MEM_SNAPSHOT(&s2); */
                            /* bLeak = _CrtMemDifference(&s3, &s1, &s2); */
                        }
                    }
                }
            }
        }
    }

    /********/

    printf("done\n");

    return uStatus;
}

void OpcUa_SecurityTest_Run()
{
    OpcUa_SecurityTest_WarmUp();

    /***************************/
    /*** CryptoFactory tests ***/
    /***************************/
    OpcUa_Test_CryptoFactory(OpcUa_SecurityPolicy_Basic128Rsa15);

    /*printf("\n");*/

    /****************************/
    /*** CryptoProvider tests ***/
    /****************************/
    OpcUa_Test_CryptoProvider(OpcUa_SecurityPolicy_Basic128Rsa15);

    /* printf("\n"); */

    /************************/
    /*** PKIFactory tests ***/
    /************************/

    OpcUa_Test_PKIFactory("TestCfg");

    /* printf("\n"); */

    /*************************/
    /*** PKIProvider tests ***/
    /*************************/

    OpcUa_Test_PKIProvider("TestCfg");

    /* printf("\n"); */
}

#if 0 /* CHZ: seem unfinished and not part of the unit test */
void OpcUa_SecurityDevTest_Run()
{
    /***************************/
    /*** CryptoFactory tests ***/
    /***************************/

    OpcUa_Test_CreateLowSecurityCryptoProvider();

    OpcUa_Test_CreateStandardSecurityCryptoProvider();

    OpcUa_Test_CreateHighSecurityCryptoProvider();

    OpcUa_Test_CreateNoSecurityCryptoProvider();

    /****************************/
    /*** CryptoProvider tests ***/
    /****************************/

    OpcUa_Test_Generate0BitsKey();

    OpcUa_Test_Generate128BitsKey();

    OpcUa_Test_Generate256BitsKey();

    OpcUa_Test_Generate512BitsKey();

    OpcUa_Test_Derive0BitsKey();

    OpcUa_Test_Derive128BitsKey();

    OpcUa_Test_Derive256BitsKey();

    OpcUa_Test_Derive512BitsKey();

    OpcUa_Test_Derive0BitsChannelKeyset();

    OpcUa_Test_Derive128BitsChannelKeyset();

    OpcUa_Test_Derive256BitsChannelKeyset();

    OpcUa_Test_Derive512BitsChannelKeyset();

    OpcUa_Test_Generate1024BitsRSAKeypair();

    OpcUa_Test_Generate2048BitsRSAKeypair();

    OpcUa_Test_GenerateECKeypair();

    OpcUa_Test_GenerateSelfsignedRootCertificateWith1024BitsRSAKey();

    OpcUa_Test_GenerateSelfsignedRootCertificateWith2048BitsRSAKey();

    OpcUa_Test_GenerateSelfsignedRootCertificateWithECKey();

    OpcUa_Test_GetPrivateKeyFromDEREncodedX509Certificate();

    OpcUa_Test_GetPublicKeyFromDEREncodedX509Certificate();

    OpcUa_Test_GetSignatureFromDEREncodedX509Certificate();

    OpcUa_Test_AsymmetricEncryptionWith1024BitsRSAKey();

    OpcUa_Test_AsymmetricEncryptionWith2048BitsRSAKey();

    OpcUa_Test_AsymmetricSignatureWith1024BitsRSAKeyAndSHA1();

    OpcUa_Test_AsymmetricSignatureWith1024BitsRSAKeyAndSHA256();

    OpcUa_Test_AsymmetricSignatureWith2048BitsRSAKeyAndSHA1();

    OpcUa_Test_AsymmetricSignatureWith2048BitsRSAKeyAndSHA256();

    OpcUa_Test_SymmetricEncryptionWith128BitsAESKeyUsingCBC();

    OpcUa_Test_SymmetricEncryptionWith256BitsAESKeyUsingCBC();

    OpcUa_Test_SymmetricEncryptionWith512BitsAESKeyUsingCBC();

    OpcUa_Test_SymmetricSignatureWithHMACAndSHA128();

    OpcUa_Test_SymmetricSignatureWithHMACAndSHA256();

    OpcUa_Test_SymmetricSignatureWithHMACAndSHA512();

    /************************/
    /*** PKIFactory tests ***/
    /************************/

    OpcUa_Test_CreatePKIProvider();

    /*************************/
    /*** PKIProvider tests ***/
    /*************************/

    OpcUa_Test_OpenCertificateStore();

    OpcUa_Test_CloseCertificateStore();

    OpcUa_Test_LoadCertificateFromStore();

    OpcUa_Test_SaveCertificateIntoStore();

    OpcUa_Test_ValidateCertificate();

#if 0 /* CHZ: inactive, because those tests dont test the PKIProvider API */
    /*********************/
    /*** PKI API tests ***/
    /*********************/

    printf("\n\n5.PKI API tests:\n");

    OpcUa_Test_LoadCertificateFromFileDER();

    OpcUa_Test_LoadCertificateFromFilePEM();

    OpcUa_Test_LoadPrivateKeyFromFilePEM();
#endif

  printf("\n");
}
#endif
