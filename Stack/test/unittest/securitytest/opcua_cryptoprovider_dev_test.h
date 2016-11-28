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

OpcUa_StatusCode OpcUa_Test_Generate0BitsKey();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_Generate128BitsKey();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_Generate256BitsKey();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_Generate512BitsKey();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_Derive0BitsKey();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_Derive128BitsKey();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_Derive256BitsKey();
/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_Derive512BitsKey();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_Derive0BitsChannelKeyset();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_Derive128BitsChannelKeyset();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_Derive256BitsChannelKeyset();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_Derive512BitsChannelKeyset();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_Generate1024BitsRSAKeypair();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_Generate2048BitsRSAKeypair();


/* ToDo: consider different keylength in testcases */
/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_GenerateECKeypair();


/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_GenerateSelfsignedRootCertificateWith1024BitsRSAKey();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_GenerateSelfsignedRootCertificateWith2048BitsRSAKey();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_GenerateSelfsignedRootCertificateWithECKey();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_GetPrivateKeyFromDEREncodedX509Certificate();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_GetPublicKeyFromDEREncodedX509Certificate();

/**
  @brief
*/
OpcUa_StatusCode OpcUa_Test_GetSignatureFromDEREncodedX509Certificate();


/**
  @brief "RSA-PKCS-#1-V1.5"
*/
OpcUa_StatusCode OpcUa_Test_AsymmetricEncryptionWith1024BitsRSAKey();

/**
  @brief "RSA-PKCS-#1-V1.5"
*/
OpcUa_StatusCode OpcUa_Test_AsymmetricEncryptionWith2048BitsRSAKey();


/**
  @brief "RSA-PKCS-#1-V1.5-SHA1"
*/
OpcUa_StatusCode OpcUa_Test_AsymmetricSignatureWith1024BitsRSAKeyAndSHA1();

/**
  @brief "RSA-PKCS-#1-V1.5-SHA256"
*/
OpcUa_StatusCode OpcUa_Test_AsymmetricSignatureWith1024BitsRSAKeyAndSHA256();

/**
  @brief "RSA-PKCS-#1-V1.5-SHA1"
*/
OpcUa_StatusCode OpcUa_Test_AsymmetricSignatureWith2048BitsRSAKeyAndSHA1();

/**
  @brief "RSA-PKCS-#1-V1.5-SHA256"
*/
OpcUa_StatusCode OpcUa_Test_AsymmetricSignatureWith2048BitsRSAKeyAndSHA256();


/**
  @brief "AES-128-CBC"
*/
OpcUa_StatusCode OpcUa_Test_SymmetricEncryptionWith128BitsAESKeyUsingCBC();

/**
  @brief "AES-256-CBC"
*/
OpcUa_StatusCode OpcUa_Test_SymmetricEncryptionWith256BitsAESKeyUsingCBC();

/**
  @brief "AES-512-CBC"
*/
OpcUa_StatusCode OpcUa_Test_SymmetricEncryptionWith512BitsAESKeyUsingCBC();


/**
  @brief "HMAC-SHA128"
*/
OpcUa_StatusCode OpcUa_Test_SymmetricSignatureWithHMACAndSHA128();

/**
  @brief "HMAC-SHA256"
*/
OpcUa_StatusCode OpcUa_Test_SymmetricSignatureWithHMACAndSHA256();

/**
  @brief "HMAC-SHA512"
*/
OpcUa_StatusCode OpcUa_Test_SymmetricSignatureWithHMACAndSHA512();
