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

#include "buffertest.h"
#include <unittest.h>
/* base */
#include <opcua_proxystub.h>

/* core */
#include <opcua_core.h>

/* self */
#include <opcua_buffer.h>

#ifndef _WIN32
# include <unistd.h>
#endif

#define IDBASE 14000


OpcUa_Byte WriteData[] = {"AaBbCcDdEe"};
OpcUa_UInt32 uWriteDataLen = 10;
OpcUa_Byte ShortWriteData [] = {"A"};
OpcUa_UInt32 uShortWriteDataLen = 1;

/* forward declarations of test cases */
OpcUa_StatusCode OpcUa_BufferTest_InitializeCreateDelete();
OpcUa_StatusCode OpcUa_BufferTest_CreateDelete();
OpcUa_StatusCode OpcUa_BufferTest_Clear();
OpcUa_StatusCode OpcUa_BufferTest_GetSetPosition();
OpcUa_StatusCode OpcUa_BufferTest_GetData();
OpcUa_StatusCode OpcUa_BufferTest_WriteRead();

/* TODO: add test cases here */

/** Test Entry Point */
void OpcUa_BufferTest_Run()
{
    OpcUa_BufferTest_InitializeCreateDelete();
    OpcUa_BufferTest_CreateDelete();
    OpcUa_BufferTest_Clear();
    OpcUa_BufferTest_GetSetPosition();
    OpcUa_BufferTest_GetData();
    OpcUa_BufferTest_WriteRead();
}


OpcUa_StatusCode OpcUa_BufferTest_InitializeCreateDelete()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_Buffer UaBuf;
    OpcUa_Buffer* UaBufHandle;


    CASE_ID (IDBASE);
    /* Call with a_pBuffer = OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_Buffer_Initialize (OpcUa_Null, OpcUa_Null, 0, 16, 0, OpcUa_False), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

#if 0 /*  The behavior of the buffer class got changed. This is allowed by now. (test removed) */
    CASE_ID (IDBASE + 1);
    /* Call with an uninitialized buffer structure with a_uBlockSize = 0. */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_Buffer_Initialize (&UaBuf, OpcUa_Null, 0, 0, 0, OpcUa_False), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();
#endif


    CASE_ID (IDBASE + 2);
    /* Call with an uninitialized buffer structure with a non-zero a_uBlockSize. */
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_Buffer_Initialize (&UaBuf, OpcUa_Null, 0, 16, 0, OpcUa_False));
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 3);
    /* Call with a null handle pointer (handle = OpcUa_Null). */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_Buffer_Create (OpcUa_Null, 0, 16, 0, OpcUa_False, OpcUa_Null), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 4);
    /* Call with blocksize = 0. */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_Buffer_Create (OpcUa_Null, 0, 0, 0, OpcUa_False, OpcUa_Null), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 5);
    /* Call with a null handle pointer (handle = OpcUa_Null). */
    MEM_SNAPSHOT (&s1);
    OpcUa_Buffer_Delete (OpcUa_Null);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 6);
    /* Call with a null handle (*handle = OpcUa_Null). */
    UaBufHandle = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    OpcUa_Buffer_Delete (&UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_BufferTest_CreateDelete()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_Buffer* UaBufHandle;
    OpcUa_Byte* pData;


    CASE_ID (IDBASE + 7);
    /* Create a UA buffer with a null data pointer, then delete it and check for memory leaks. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    ASSERT_GOOD (OpcUa_Buffer_Create (OpcUa_Null, 0, 16, 0, OpcUa_False, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 8);
    /* Allocate a buffer, create a UA buffer (a_bFreeBuffer = OpcUa_True), then delete it and check for memory leaks. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    ASSERT_GOOD (OpcUa_Buffer_Create (pData, 256, 256, 0, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 9);
    /* Allocate a buffer, create a UA buffer (a_bFreeBuffer = OpcUa_False), then delete it and verify that the buffer wasn't deleted. */
    UaBufHandle = OpcUa_Null;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_Buffer_Create (pData, 256, 256, 0, OpcUa_False, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    OpcUa_Memory_Free (pData);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_BufferTest_Clear()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_Buffer* UaBufHandle;
    OpcUa_Byte* pData;


    CASE_ID (IDBASE + 10);
    /* Call OpcUa_Buffer_Clear with a_pBuffer = OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    OpcUa_Buffer_Clear (OpcUa_Null);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 11);
    /* Allocate a buffer, create a UA buffer (a_bFreeBuffer = OpcUa_True), call OpcUa_Buffer_Clear on the UA buffer, then delete it and check for memory leaks. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    ASSERT_GOOD (OpcUa_Buffer_Create (pData, 256, 256, 0, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    OpcUa_Buffer_Clear (UaBufHandle);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 12);
    /* Allocate a buffer, create a UA buffer (a_bFreeBuffer = OpcUa_False), call OpcUa_Buffer_Clear, then delete it and verify that the buffer wasn't deleted. */
    UaBufHandle = OpcUa_Null;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    ASSERT_GOOD (OpcUa_Buffer_Create (pData, 256, 256, 0, OpcUa_False, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    MEM_SNAPSHOT (&s1);
    OpcUa_Buffer_Clear (UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    OpcUa_Memory_Free (pData);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_BufferTest_GetSetPosition()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_Buffer* UaBufHandle;
    OpcUa_Byte* pData;
    OpcUa_UInt32 uSet;
    OpcUa_UInt32 uGet;


    CASE_ID (IDBASE + 13);
    /* Call OpcUa_Buffer_GetPosition with handle = OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_Buffer_GetPosition (OpcUa_Null, &uGet), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 14);
    /* Call OpcUa_Buffer_GetPosition with position = OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    ASSERT_GOOD (OpcUa_Buffer_Create (OpcUa_Null, 0, 16, 0, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_STATUS (OpcUa_Buffer_GetPosition (UaBufHandle, OpcUa_Null), OpcUa_BadInvalidArgument);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 15);
    /* Call OpcUa_Buffer_SetPosition with handle = OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_Buffer_SetPosition (OpcUa_Null, OpcUa_BufferPosition_End), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 16);
    /* Create a Ua buffer.  Call OpcUa_Buffer_SetPosition with the position past the end of the buffer. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    ASSERT_GOOD (OpcUa_Buffer_Create (OpcUa_Null, 0, 16, 0, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_STATUS (OpcUa_Buffer_SetPosition (UaBufHandle, 4096), OpcUa_BadEndOfStream);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 17);
    /* Create a Ua buffer.  Call OpcUa_Buffer_SetPosition with the position somewhere in the buffer.  Call OpcUa_Buffer_GetPosition and verify that the position is set correctly. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    uSet = 128;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    ASSERT_GOOD (OpcUa_Buffer_Create (pData, 256, 256, 0, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_GOOD (OpcUa_Buffer_SetPosition (UaBufHandle, uSet));
    ASSERT_GOOD (OpcUa_Buffer_GetPosition (UaBufHandle, &uGet));
    ASSERT (uGet == uSet);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 18);
    /* Create a Ua buffer.  Call OpcUa_Buffer_SetPosition with the position at the end of the buffer.  Call OpcUa_Buffer_GetPosition and verify that the position is set correctly. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    ASSERT_GOOD (OpcUa_Buffer_Create (pData, 256, 256, 0, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_GOOD (OpcUa_Buffer_SetPosition (UaBufHandle, OpcUa_BufferPosition_End));
    ASSERT_GOOD (OpcUa_Buffer_GetPosition (UaBufHandle, &uGet));
    ASSERT (uGet == 256);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_BufferTest_GetData()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_Buffer* UaBufHandle;
    OpcUa_Byte* pData;
    OpcUa_Byte* pGet;
    OpcUa_UInt32 uLength;


    CASE_ID (IDBASE + 19);
    /* Call with handle = OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_Buffer_GetData (OpcUa_Null, &pData, &uLength), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

#if 0 /* case 20 and 21 are now allowed by the buffer api */
    CASE_ID (IDBASE + 20);
    /* Call with data = OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    ASSERT_GOOD (OpcUa_Buffer_Create (pData, 256, 256, 0, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_STATUS (OpcUa_Buffer_GetData (UaBufHandle, OpcUa_Null, &uLength), OpcUa_BadInvalidArgument);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 21);
    /* Call with length = OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    ASSERT_GOOD (OpcUa_Buffer_Create (pData, 256, 256, 0, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_STATUS (OpcUa_Buffer_GetData (UaBufHandle, &pData, OpcUa_Null), OpcUa_BadInvalidArgument);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();
#endif


    CASE_ID (IDBASE + 22);
    /* Create a UA buffer with a null data pointer, then call OpcUa_Buffer_GetData and verify the settings. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    ASSERT_GOOD (OpcUa_Buffer_Create (OpcUa_Null, 0, 16, 0, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_GOOD (OpcUa_Buffer_GetData (UaBufHandle, &pData, &uLength));
    ASSERT (OpcUa_Null == pData);
    ASSERT (0 == uLength);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 23);
    /* Allocate a buffer, create a UA buffer (a_bFreeBuffer = OpcUa_True), then call OpcUa_Buffer_GetData and verify the settings. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    ASSERT_GOOD (OpcUa_Buffer_Create (pData, 256, 256, 0, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_GOOD (OpcUa_Buffer_GetData (UaBufHandle, &pGet, &uLength));
    ASSERT (pGet == pData);
    ASSERT (256 == uLength);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 24);
    /* Allocate a buffer, create a UA buffer (a_bFreeBuffer = OpcUa_False), then call OpcUa_Buffer_GetData and verify the settings. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    ASSERT_GOOD (OpcUa_Buffer_Create (pData, 256, 256, 0, OpcUa_False, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_GOOD (OpcUa_Buffer_GetData (UaBufHandle, &pGet, &uLength));
    ASSERT (pGet == pData);
    ASSERT (256 == uLength);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    OpcUa_Memory_Free (pData);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_BufferTest_WriteRead()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_Buffer* UaBufHandle;
    OpcUa_Byte* pData;
    OpcUa_UInt32 uRead;


    CASE_ID (IDBASE + 25);
    /* Call with handle = OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_Buffer_Write (OpcUa_Null, WriteData, uWriteDataLen), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 26);
    /* Call with data = OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    ASSERT_GOOD (OpcUa_Buffer_Create (pData, 256, 256, 0, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_STATUS (OpcUa_Buffer_Write (UaBufHandle, OpcUa_Null, uWriteDataLen), OpcUa_BadInvalidArgument);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 27);
    /* Allocate a buffer, create a UA buffer (a_bFreeBuffer = OpcUa_True, a_uMaxSize is a fixed value), then Write past the end of it. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    ASSERT_GOOD (OpcUa_Buffer_Create (pData, 256, 256, 256, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_GOOD (OpcUa_Buffer_SetPosition (UaBufHandle, 250));
    ASSERT_STATUS (OpcUa_Buffer_Write (UaBufHandle, WriteData, uWriteDataLen), OpcUa_BadEndOfStream);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 28);
    /* Call with handle = OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    uRead = 256;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    ASSERT_GOOD (OpcUa_Buffer_Create (OpcUa_Null, 0, 16, 0, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_STATUS (OpcUa_Buffer_Read (OpcUa_Null, pData, &uRead), OpcUa_BadInvalidArgument);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    OpcUa_Memory_Free (pData);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 29);
    /* Call with data = OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    uRead = 256;
    ASSERT_GOOD (OpcUa_Buffer_Create (OpcUa_Null, 0, 16, 0, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_STATUS (OpcUa_Buffer_Read (UaBufHandle, OpcUa_Null, &uRead), OpcUa_BadInvalidArgument);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 30);
    /* Call with length = OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    ASSERT_GOOD (OpcUa_Buffer_Create (OpcUa_Null, 0, 16, 0, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_STATUS (OpcUa_Buffer_Read (UaBufHandle, pData, OpcUa_Null), OpcUa_BadInvalidArgument);
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    OpcUa_Memory_Free (pData);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 31);
    /* Create a UA buffer with a null data pointer (a_uMaxSize = 0), then Write to it.  Read the values back and verify that the Write was successful. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    uRead = 256;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    ASSERT_GOOD (OpcUa_Buffer_Create (OpcUa_Null, 0, 16, 0, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_GOOD (OpcUa_Buffer_Write (UaBufHandle, WriteData, uWriteDataLen));
    ASSERT_GOOD (OpcUa_Buffer_SetPosition (UaBufHandle, OpcUa_BufferPosition_Start));
    ASSERT_GOOD (OpcUa_Buffer_Read (UaBufHandle, pData, &uRead));
    ASSERT (uRead == uWriteDataLen);
    ASSERT (0 == memcmp (WriteData, pData, uWriteDataLen));
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    OpcUa_Memory_Free (pData);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

#if 0
    CASE_ID (IDBASE + 32);
    /* Create a UA buffer with a null data pointer (a_uMaxSize = 1, a_uBlockSize = 256), then Write to it.  */
    /* Read the values back and verify that the Write was successful.  Write to it again and verify that the write failed. */
    MEM_SNAPSHOT (&s1);
    UaBufHandle = OpcUa_Null;
    uRead = 256;
    pData = (OpcUa_Byte*)OpcUa_Memory_Alloc (256);
    ASSERT (OpcUa_Null != pData);
    ASSERT_GOOD (OpcUa_Buffer_Create (OpcUa_Null, 0, 256, 1, OpcUa_True, &UaBufHandle));
    ASSERT (OpcUa_Null != UaBufHandle);
    ASSERT_GOOD (OpcUa_Buffer_Write (UaBufHandle, ShortWriteData, uShortWriteDataLen));
    ASSERT_GOOD (OpcUa_Buffer_SetPosition (UaBufHandle, OpcUa_BufferPosition_Start));
    ASSERT_GOOD (OpcUa_Buffer_Read (UaBufHandle, pData, &uRead));
    ASSERT (uRead == uShortWriteDataLen);
    ASSERT (0 == memcmp (ShortWriteData, pData, uShortWriteDataLen));
    ASSERT_GOOD (OpcUa_Buffer_SetPosition (UaBufHandle, OpcUa_BufferPosition_End));
#if 0
    ASSERT_STATUS (OpcUa_Buffer_Write(UaBufHandle, ShortWriteData, uShortWriteDataLen), OpcUa_BadEndOfStream);
#else
    ASSERT_STATUS (OpcUa_Buffer_Read (UaBufHandle, pData, &uRead), OpcUa_BadEndOfStream);
#endif
    OpcUa_Buffer_Delete (&UaBufHandle);
    ASSERT (OpcUa_Null == UaBufHandle);
    OpcUa_Memory_Free (pData);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();
#endif

    return OpcUa_Good;
}
