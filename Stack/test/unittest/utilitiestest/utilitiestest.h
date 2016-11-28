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

#ifndef __UTILITIES_TEST_H__
#define __UTILITIES_TEST_H__

/** Test Entry Point */
void OpcUa_UtilitiesTest_Run();

/* Tests for OpcUa_Utilities_QSort */
void OpcUa_UtilitiesTest_QSort();
OpcUa_StatusCode OpcUa_UtilitiesTest_13000();
OpcUa_StatusCode OpcUa_UtilitiesTest_13001();
OpcUa_StatusCode OpcUa_UtilitiesTest_13002();
OpcUa_StatusCode OpcUa_UtilitiesTest_13003();
OpcUa_StatusCode OpcUa_UtilitiesTest_13004();
OpcUa_StatusCode OpcUa_UtilitiesTest_13005();
OpcUa_StatusCode OpcUa_UtilitiesTest_13006();

/* Tests for OpcUa_Utilities_BSearch */
void OpcUa_UtilitiesTest_BSearch();
OpcUa_StatusCode OpcUa_UtilitiesTest_13007();
OpcUa_StatusCode OpcUa_UtilitiesTest_13008();
OpcUa_StatusCode OpcUa_UtilitiesTest_13009();
OpcUa_StatusCode OpcUa_UtilitiesTest_13010();
OpcUa_StatusCode OpcUa_UtilitiesTest_13011();
OpcUa_StatusCode OpcUa_UtilitiesTest_13012();
OpcUa_StatusCode OpcUa_UtilitiesTest_13013();
OpcUa_StatusCode OpcUa_UtilitiesTest_13014();
OpcUa_StatusCode OpcUa_UtilitiesTest_13015();
OpcUa_StatusCode OpcUa_UtilitiesTest_13016();

/* Tests for XxxType_CopyTo/Compare */
void OpcUa_UtilitiesTest_CopyCompare();
OpcUa_StatusCode OpcUa_UtilitiesTest_13020();
OpcUa_StatusCode OpcUa_UtilitiesTest_13021();

/* Tests for Binary Encoder and Decoder */
void OpcUa_UtilitiesTest_EncodeDecode();
OpcUa_StatusCode OpcUa_UtilitiesTest_13030();
OpcUa_StatusCode OpcUa_UtilitiesTest_13031();

#endif
