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
#include <opcua_pkifactory.h>
#include <opcua_pkifactory_test.h>
#include <opcua_securitytest_helperfunctions.h>

/* for test output */
#include <stdio.h>

#include <securitytest.h>
#include "unittest.h"

/*============================================================================
 * OpcUa_Test_Bad_PKIFactory
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Test_PKIFactory(OpcUa_StringA sConfig)
{
    OpcUa_CertificateStoreConfiguration     CertificateStoreCfg;
    OpcUa_PKIProvider                       myPKIProvider;
    SNAPSHOT_TYPE                           s1, s2, s3;

OpcUa_InitializeStatus(OpcUa_Module_TestModule, "Test_PKIFactory");

    OpcUa_ReferenceParameter(sConfig);

    /* printf("\nTest PKIFactory - 39000\n"); */

    //
    // Generate needed test objects
    //
    OpcUa_CertificateStoreConfiguration_Initialize(&CertificateStoreCfg);
    CertificateStoreCfg.strPkiType                         = OPCUA_P_PKI_TYPE_OPENSSL;
    CertificateStoreCfg.strRevokedCertificateListLocation  = OPCUA_SECURITY_TEST_CLR_DIR;
    CertificateStoreCfg.strTrustedCertificateListLocation  = OPCUA_SECURITY_TEST_CA_DIR;

//
// OpcUa_PKIProvider_Create
//
    //
    // Basic test - all parameters are good
    //
    CASE_ID(39000);
    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(OpcUa_PKIProvider_Create(&CertificateStoreCfg, &myPKIProvider));
    END_CASE();

    //
    // Basic test - all parameters are good
    //
    CASE_ID(39001);
    ASSERT_GOOD(OpcUa_PKIProvider_Delete(&myPKIProvider));
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS(&s3, &s1, &s2);
    END_CASE();

    //
    // Basic test - Null Certificate Store Info
    //
    CASE_ID(39002);
    ASSERT_STATUS(OpcUa_PKIProvider_Create(OpcUa_Null, &myPKIProvider), OpcUa_BadInvalidArgument);
    OpcUa_PKIProvider_Delete(&myPKIProvider);

    END_CASE();

    //
    // Basic test - Null Provider Pointer
    //
    CASE_ID(39003);
    ASSERT_STATUS(OpcUa_PKIProvider_Create(&CertificateStoreCfg, OpcUa_Null), OpcUa_BadInvalidArgument);
    END_CASE();

//
// OpcUa_PKIProvider_Delete
//
    //
    // Basic test - Null Provider Pointer
    //
    CASE_ID(39004);
    ASSERT_STATUS(OpcUa_PKIProvider_Delete(OpcUa_Null), OpcUa_BadInvalidArgument);
    END_CASE();

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}
