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
#include <opcua_cryptofactory_test.h>
#include <opcua_securitytest_helperfunctions.h>

/* for test output */
#include <stdio.h>

#include <securitytest.h>

#include "unittest.h"

/*============================================================================
 * OpcUa_Test_CryptoFactory
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_CryptoFactory(OpcUa_StringA sUri)
{
    OpcUa_String            myCryptoPolicy;
    OpcUa_CryptoProvider    myCryptoProvider;
    OpcUa_StatusCode        uStatus             = OpcUa_Good;
    SNAPSHOT_TYPE           s1, s2, s3;

    OpcUa_MemSet(&myCryptoProvider, 0, sizeof(OpcUa_CryptoProvider));

    /* printf("\nTest CryptoFactory - 17000\n"); */

    /**/
    /* Generate needed test objects*/
    /**/
    uStatus = OpcUa_TestHelper_GenerateStandardSecurityCryptoPolicy(sUri, &myCryptoPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

/**/
/* OpcUa_CryptoProvider_Create*/
/**/
    /**/
    /* Basic test - all parameters are good*/
    /**/
    CASE_ID(17000);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(OpcUa_CryptoProvider_Create(OpcUa_String_GetRawString(&myCryptoPolicy), &myCryptoProvider));
    END_CASE();

    /**/
    /* Basic test - all parameters are good*/
    /**/
    CASE_ID(17001);
    ASSERT_GOOD(OpcUa_CryptoProvider_Delete(&myCryptoProvider));
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /**/
    /* Basic test - Null URI*/
    /**/
    CASE_ID(17002);
    MEM_SNAPSHOT(&s1);
    ASSERT_STATUS(OpcUa_CryptoProvider_Create(OpcUa_Null, &myCryptoProvider), OpcUa_BadInvalidArgument);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /**/
    /* Basic test - Invalid Uri String*/
    /**/
    CASE_ID(17010);
    MEM_SNAPSHOT(&s1);
    ASSERT_STATUS(OpcUa_CryptoProvider_Create("\0",&myCryptoProvider), OpcUa_BadSecurityPolicyRejected);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    /* Basic test - Null Provider Structure*/
    CASE_ID(17023);
    MEM_SNAPSHOT(&s1);
    ASSERT_STATUS(OpcUa_CryptoProvider_Create(OpcUa_String_GetRawString(&myCryptoPolicy), OpcUa_Null), OpcUa_BadInvalidArgument);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

/* OpcUa_CryptoProvider_Delete*/
    /* Basic test - Delete with NULL Pointer*/
    CASE_ID(17025);
    MEM_SNAPSHOT(&s1);
    ASSERT_STATUS(OpcUa_CryptoProvider_Delete(OpcUa_Null), OpcUa_BadInvalidArgument);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    OpcUa_TestHelper_CleanupCryptoPolicy(&myCryptoPolicy);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    END_CASE();

    OpcUa_TestHelper_CleanupCryptoPolicy(&myCryptoPolicy);

OpcUa_FinishErrorHandling;
}
