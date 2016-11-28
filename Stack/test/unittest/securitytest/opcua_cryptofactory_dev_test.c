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
#include <opcua_crypto.h>
#include <opcua_cryptofactory.h>
#include <opcua_cryptofactory_dev_test.h>
#include <opcua_securitytest_helperfunctions.h>

/* for test output */
#include <stdio.h>

#include "unittest.h"

/*============================================================================
 * OpcUa_Test_CreateLowSecurityCryptoProvider
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CreateLowSecurityCryptoProvider()
{
    printf("\nCreateLowSecurityCryptoProvider");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_CreateStandardSecurityCryptoProvider
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CreateStandardSecurityCryptoProvider()
{
    OpcUa_String            myCryptoPolicy;
    OpcUa_CryptoProvider    myCryptoProvider;
    OpcUa_StatusCode        uStatus                     = OpcUa_Good;

    printf("\nCreateStandardSecurityCryptoProvider");

    /* Generate needed test objects */
#if 0
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_P_OpenSSL_BasicMessageSecurity_Name, &myCryptoPolicy);
#else
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &myCryptoPolicy);
#endif
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&myCryptoPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_CryptoProvider_Delete(&myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

        printf("...failed!");
        return OpcUa_Bad;

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Test_CreateHighSecurityCryptoProvider
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CreateHighSecurityCryptoProvider()
{
    printf("\nCreateHighSecurityCryptoProvider");
    printf("...not implemented!");

    return OpcUa_BadNotImplemented;
}

/*============================================================================
 * OpcUa_Test_CreateNoSecurityCryptoProvider
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CreateNoSecurityCryptoProvider()
{
    OpcUa_String            myCryptoPolicy;
    OpcUa_CryptoProvider    myCryptoProvider;
    OpcUa_StatusCode        uStatus                     = OpcUa_Good;

    printf("\nCreateNoSecurityCryptoProvider");

    /* Generate needed test objects */
    uStatus = OpcUa_TestHelper_GenerateNoSecurityCryptoPolicy(OpcUa_SecurityPolicy_Basic128Rsa15, &myCryptoPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_CryptoProvider_Create(   OpcUa_String_GetRawString(&myCryptoPolicy),
                                                            &myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_CryptoProvider_Delete(&myCryptoProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    printf("...passed!");
    return OpcUa_Good;

OpcUa_BeginErrorHandling;

        printf("...failed!");
        return OpcUa_Bad;

OpcUa_FinishErrorHandling;
}
