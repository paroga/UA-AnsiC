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

#include "unittest.h"

#include "memorytest.h"
#include "opcua_memory.h"

/* REMARK: For performance reasons memory functions do not check arguments. */
/*         Tests which simulate a bad case are removed for this reason. */

/** Test Entry Point */
void OpcUa_MemoryTest_Run()
{
    OpcUa_MemoryTest_TestAlloc();
    OpcUa_MemoryTest_TestReAlloc();
    OpcUa_MemoryTest_TestFree();
    OpcUa_MemoryTest_TestMemCpy();
}

/** Test OpcUa_Memory_Alloc */
void OpcUa_MemoryTest_TestAlloc()
{
    /*OpcUa_MemoryTest_6000(); Does not get checked intentionally! */
    OpcUa_MemoryTest_6001();
    OpcUa_MemoryTest_6002();
    /*OpcUa_MemoryTest_6003(); Does not get checked intentionally! */
}

/** Test OpcUa_Memory_ReAlloc() */
void OpcUa_MemoryTest_TestReAlloc()
{
    OpcUa_MemoryTest_6005();
    OpcUa_MemoryTest_6006();
    OpcUa_MemoryTest_6007();
    OpcUa_MemoryTest_6008();
}

/* Tests for OpcUa_Memory_Free() */
void OpcUa_MemoryTest_TestFree()
{
    OpcUa_MemoryTest_6009();
    OpcUa_MemoryTest_6010();
    OpcUa_MemoryTest_6011();
}

/* Tests for OpcUa_Memory_MemCpy() */
void OpcUa_MemoryTest_TestMemCpy()
{
    /*OpcUa_MemoryTest_6012(); Does not get checked intentionally! */
    /*OpcUa_MemoryTest_6013(); Does not get checked intentionally! */
    OpcUa_MemoryTest_6014();
    /* OpcUa_MemoryTest_6015(); Does not get checked intentionally! */
}

/* Call OpcUa_Memory_Alloc with 0 (BAD CASE) */
OpcUa_StatusCode OpcUa_MemoryTest_6000()
{
    CASE_ID(6000);

    ASSERT(OpcUa_Memory_Alloc(0) == OpcUa_Null);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_Memory_Alloc to allocate a single byte (GOOD CASE) */
OpcUa_StatusCode OpcUa_MemoryTest_6001()
{
    OpcUa_Byte* pArray = (OpcUa_Byte*)OpcUa_Memory_Alloc(1);

    CASE_ID(6001);

    ASSERT(pArray);
    pArray[0] = 0;
    OpcUa_Memory_Free((OpcUa_Void*) pArray);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_Memory_Alloc to allocate a valid array of bytes (GOOD CASE) */
OpcUa_StatusCode OpcUa_MemoryTest_6002()
{
    OpcUa_Byte*     pArray  = OpcUa_Null;
    OpcUa_UInt32    i       = 0;

    CASE_ID(6002);

    pArray = (OpcUa_Byte*)OpcUa_Memory_Alloc(LARGE_SIZE);

    ASSERT(pArray);

    for(i=0;i<LARGE_SIZE;i++)
    {
        pArray[i] = 0;
    }

    OpcUa_Memory_Free((OpcUa_Void*) pArray);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_Memory_Alloc to allocate a very large array of bytes (BAD CASE) */
OpcUa_StatusCode OpcUa_MemoryTest_6003()
{
    OpcUa_Byte* pArray;

    CASE_ID(6003);

    pArray = (OpcUa_Byte*)OpcUa_Memory_Alloc(OpcUa_UInt32_Max);
    /* TODO: What is the expected behavior here? */
    OpcUa_Memory_Free((OpcUa_Void*) pArray);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_Memory_ReAlloc to resize an empty array to a non-empty array (GOOD CASE) */
OpcUa_StatusCode OpcUa_MemoryTest_6005()
{
    OpcUa_Byte* pArray;
    OpcUa_UInt32 i;

    CASE_ID(6005);

    pArray = (OpcUa_Byte*)OpcUa_Memory_ReAlloc(OpcUa_Null,LARGE_SIZE);
    ASSERT(pArray);
    for(i=0;i<LARGE_SIZE;i++)
    {
        pArray[i] = 0;
    }

    OpcUa_Memory_Free((OpcUa_Void*) pArray);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_Memory_ReAlloc to resize a non-empty array to a non-empty array (BAD CASE) */
OpcUa_StatusCode OpcUa_MemoryTest_6006()
{
    OpcUa_Byte* pArray;

    CASE_ID(6006);

    pArray = (OpcUa_Byte*)OpcUa_Memory_Alloc(LARGE_SIZE);
    ASSERT(pArray);
    pArray = (OpcUa_Byte*)OpcUa_Memory_ReAlloc((OpcUa_Void*) pArray,0);
    ASSERT(pArray == OpcUa_Null);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_Memory_ReAlloc to shorten an array (GOOD CASE) */
OpcUa_StatusCode OpcUa_MemoryTest_6007()
{
    OpcUa_Byte* pArray;
    OpcUa_UInt32 i;

    CASE_ID(6007);

    pArray = (OpcUa_Byte*)OpcUa_Memory_Alloc(LARGE_SIZE);
    ASSERT(pArray);
    for(i=0;i<LARGE_SIZE;i++)
        pArray[i] = (OpcUa_Byte)i;
    pArray = (OpcUa_Byte*)OpcUa_Memory_ReAlloc((OpcUa_Void*) pArray,SMALL_SIZE);
    for(i=0;i<SMALL_SIZE;i++)
        ASSERT(pArray[i] == (OpcUa_Byte)i);
    OpcUa_Memory_Free((OpcUa_Void*) pArray);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_Memory_ReAlloc to lengthen an array (GOOD CASE) */
OpcUa_StatusCode OpcUa_MemoryTest_6008()
{
    OpcUa_Byte* pArray;
    OpcUa_UInt32 i;

    CASE_ID(6008);

    pArray = (OpcUa_Byte*)OpcUa_Memory_Alloc(SMALL_SIZE);
    ASSERT(pArray);
    for(i=0;i<SMALL_SIZE;i++)
        pArray[i] = (OpcUa_Byte)i;
    pArray = (OpcUa_Byte*)OpcUa_Memory_ReAlloc((OpcUa_Void*) pArray,LARGE_SIZE);
    for(i=0;i<SMALL_SIZE;i++)
        ASSERT(pArray[i] == (OpcUa_Byte)i);
    OpcUa_Memory_Free((OpcUa_Void*) pArray);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Memory_Free with OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_MemoryTest_6009()
{
    CASE_ID(6009);

    OpcUa_Memory_Free(OpcUa_Null);

    END_CASE();

    return OpcUa_Good;
}

/* Allocate and deallocate memory using OpcUa_Memory_Alloc and OpcUa_Memory_Free (GOOD CASE) */
OpcUa_StatusCode OpcUa_MemoryTest_6010()
{
    OpcUa_Byte* pArray;

    CASE_ID(6010);

    pArray = (OpcUa_Byte*)OpcUa_Memory_Alloc(LARGE_SIZE);
    ASSERT(pArray);
    OpcUa_Memory_Free((OpcUa_Void*) pArray);

    END_CASE();

    return OpcUa_Good;
}

/*    Allocate and deallocate multiple byte arrays
    using OpcUa_Memory_Alloc and OpcUa_Memory_Free (GOOD CASE) */
OpcUa_StatusCode OpcUa_MemoryTest_6011()
{
    OpcUa_Byte* pArrays[3];

    CASE_ID(6011);

    pArrays[0] = (OpcUa_Byte*)OpcUa_Memory_Alloc(LARGE_SIZE);
    pArrays[1] = (OpcUa_Byte*)OpcUa_Memory_Alloc(LARGE_SIZE);
    pArrays[2] = (OpcUa_Byte*)OpcUa_Memory_Alloc(LARGE_SIZE);
    ASSERT(pArrays[0]);
    ASSERT(pArrays[1]);
    ASSERT(pArrays[2]);
    OpcUa_Memory_Free((OpcUa_Void*) pArrays[0]);
    OpcUa_Memory_Free((OpcUa_Void*) pArrays[1]);
    OpcUa_Memory_Free((OpcUa_Void*) pArrays[2]);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Memory_MemCpy with OpcUa_Null for the source (BAD CASE) */
OpcUa_StatusCode OpcUa_MemoryTest_6012()
{
    OpcUa_Int32 iArray[10];

    CASE_ID(6012);

    ASSERT_STATUS(OpcUa_Memory_MemCpy((OpcUa_Void*) iArray,10*sizeof(OpcUa_Int32),OpcUa_Null,10*sizeof(OpcUa_Int32)),OpcUa_BadInvalidArgument);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Memory_MemCpy with OpcUa_Null for the destination (BAD CASE) */
OpcUa_StatusCode OpcUa_MemoryTest_6013()
{
    OpcUa_Int32 iArray[10];

    CASE_ID(6013);

    ASSERT_STATUS(OpcUa_Memory_MemCpy(OpcUa_Null,10*sizeof(OpcUa_Int32),(OpcUa_Void*) iArray,10*sizeof(OpcUa_Int32)),OpcUa_BadInvalidArgument);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_MemoryTest_MemCpy to make a copy of an array of ten OpcUa_Int32 (GOOD CASE) */
OpcUa_StatusCode OpcUa_MemoryTest_6014()
{
    OpcUa_Int32 iArrayA[10];
    OpcUa_Int32 iArrayB[10];
    OpcUa_Int32 i;

    CASE_ID(6014);

    for(i=0;i<10;i++)
    {
        iArrayA[i] = i;
    }

    ASSERT_GOOD(OpcUa_Memory_MemCpy((OpcUa_Void*) iArrayB,10*sizeof(OpcUa_Int32),(OpcUa_Void*) iArrayA,10*sizeof(OpcUa_Int32)));

    for(i=0;i<10;i++)
    {
        ASSERT(iArrayB[i] == i);
    }

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_MemoryTest_MemCpy to copy an array into a smaller destination buffer (BAD CASE) */
OpcUa_StatusCode OpcUa_MemoryTest_6015()
{
    OpcUa_Int32 iArrayA[20];
    OpcUa_Int32 iArrayB[10];

    CASE_ID(6015);

    ASSERT_STATUS(OpcUa_Memory_MemCpy((OpcUa_Void*) iArrayB,10*sizeof(OpcUa_Int32),(OpcUa_Void*) iArrayA, 20*sizeof(OpcUa_Int32)),OpcUa_BadOutOfRange);

    END_CASE();

    return OpcUa_Good;
}
