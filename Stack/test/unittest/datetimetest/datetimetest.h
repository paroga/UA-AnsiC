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

#ifndef __DATETIME_TEST_H_
#define __DATETIME_TEST_H_

/* #define WITH_SET_UNSET */

/** Test entry point */
void OpcUa_DateTimeTest_Run();

#ifdef WITH_SET_UNSET
/* Tests for OpcUa_DateTime_Set() */
void OpcUa_DateTimeTest_Set();
void OpcUa_DateTimeTest_3000();
void OpcUa_DateTimeTest_3001();

/* Tests for OpcUa_DateTime_UnSet() */
void OpcUa_DateTimeTest_UnSet();
void OpcUa_DateTimeTest_3002();
void OpcUa_DateTimeTest_3003();
#endif

/* Tests for OpcUa_DateTime_GetDateTimeFromString() */
void OpcUa_DateTimeTest_GetDateTimeFromString();
OpcUa_StatusCode OpcUa_DateTimeTest_3004();
OpcUa_StatusCode OpcUa_DateTimeTest_3005();
OpcUa_StatusCode OpcUa_DateTimeTest_3006();
OpcUa_StatusCode OpcUa_DateTimeTest_3007();

/* Tests for OpcUa_DateTime_GetStringFromDateTime() */
void OpcUa_DateTimeTest_GetStringFromDateTime();
OpcUa_StatusCode OpcUa_DateTimeTest_3008();
OpcUa_StatusCode OpcUa_DateTimeTest_3009();
OpcUa_StatusCode OpcUa_DateTimeTest_3010();

#endif
