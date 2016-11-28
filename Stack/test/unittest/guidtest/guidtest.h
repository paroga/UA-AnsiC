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

#ifndef __GUID_TEST_H__
#define __GUID_TEST_H__

/** Test entry point */
void OpcUa_GuidTest_Run();

/* Tests for OpcUa_Guid_Create */
void OpcUa_GuidTest_Create();
OpcUa_StatusCode OpcUa_GuidTest_4000();

/* Tests for OpcUa_Guid_ToString */
void OpcUa_GuidTest_ToString();
OpcUa_StatusCode OpcUa_GuidTest_4001();
OpcUa_StatusCode OpcUa_GuidTest_4002();
OpcUa_StatusCode OpcUa_GuidTest_4003();
OpcUa_StatusCode OpcUa_GuidTest_4021();

/* Tests for OpcUa_Guid_FromString */
void OpcUa_GuidTest_FromString();
OpcUa_StatusCode OpcUa_GuidTest_4004();
OpcUa_StatusCode OpcUa_GuidTest_4005();
OpcUa_StatusCode OpcUa_GuidTest_4006();
OpcUa_StatusCode OpcUa_GuidTest_4007();
OpcUa_StatusCode OpcUa_GuidTest_4022();

/* Tests for OpcUa_Guid_IsEqual */
void OpcUa_GuidTest_IsEqual();
OpcUa_StatusCode OpcUa_GuidTest_4008();
OpcUa_StatusCode OpcUa_GuidTest_4009();
OpcUa_StatusCode OpcUa_GuidTest_4010();
OpcUa_StatusCode OpcUa_GuidTest_4011();
OpcUa_StatusCode OpcUa_GuidTest_4012();

/* Tests for OpcUa_Guid_IsNull */
void OpcUa_GuidTest_IsNull();
OpcUa_StatusCode OpcUa_GuidTest_4013();
OpcUa_StatusCode OpcUa_GuidTest_4014();
OpcUa_StatusCode OpcUa_GuidTest_4015();
OpcUa_StatusCode OpcUa_GuidTest_4016();

/* Tests for OpcUa_Guid_Copy */
void OpcUa_GuidTest_GuidCopy();
OpcUa_StatusCode OpcUa_GuidTest_4017();
OpcUa_StatusCode OpcUa_GuidTest_4018();
OpcUa_StatusCode OpcUa_GuidTest_4019();
OpcUa_StatusCode OpcUa_GuidTest_4020();

#endif
