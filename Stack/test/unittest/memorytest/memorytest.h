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

#ifndef __MEMORY_TEST_H__
#define __MEMORY_TEST_H__

#define SMALL_SIZE 6
#define LARGE_SIZE 12
#define MAX_NUM_ARRAYS 5

/** Test Entry Point */
void OpcUa_MemoryTest_Run();

/* Tests for OpcUa_Memory_Alloc() */
void OpcUa_MemoryTest_TestAlloc();
OpcUa_StatusCode OpcUa_MemoryTest_6000();
OpcUa_StatusCode OpcUa_MemoryTest_6001();
OpcUa_StatusCode OpcUa_MemoryTest_6002();
OpcUa_StatusCode OpcUa_MemoryTest_6003();

/* Tests for OpcUa_Memory_ReAlloc() */
void OpcUa_MemoryTest_TestReAlloc();
OpcUa_StatusCode OpcUa_MemoryTest_6005();
OpcUa_StatusCode OpcUa_MemoryTest_6006();
OpcUa_StatusCode OpcUa_MemoryTest_6007();
OpcUa_StatusCode OpcUa_MemoryTest_6008();

/* Tests for OpcUa_Memory_Free() */
void OpcUa_MemoryTest_TestFree();
OpcUa_StatusCode OpcUa_MemoryTest_6009();
OpcUa_StatusCode OpcUa_MemoryTest_6010();
OpcUa_StatusCode OpcUa_MemoryTest_6011();

/* Tests for OpcUa_Memory_MemCpy() */
void OpcUa_MemoryTest_TestMemCpy();
OpcUa_StatusCode OpcUa_MemoryTest_6012();
OpcUa_StatusCode OpcUa_MemoryTest_6013();
OpcUa_StatusCode OpcUa_MemoryTest_6014();
OpcUa_StatusCode OpcUa_MemoryTest_6015();

#endif
