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
#include "guidtest.h"
#include <opcua_guid.h>
#include <opcua_memory.h>
#include <opcua_string.h>

/** Test entry point */
void OpcUa_GuidTest_Run()
{
    OpcUa_GuidTest_Create();
    OpcUa_GuidTest_ToString();
    OpcUa_GuidTest_FromString();
    OpcUa_GuidTest_IsEqual();
    OpcUa_GuidTest_IsNull();
    OpcUa_GuidTest_GuidCopy();
}

/* Tests for OpcUa_Guid_Create */
void OpcUa_GuidTest_Create()
{
    OpcUa_GuidTest_4000();
}

/* Tests for OpcUa_Guid_ToString */
void OpcUa_GuidTest_ToString()
{
    OpcUa_GuidTest_4001();
    OpcUa_GuidTest_4002();
    OpcUa_GuidTest_4003();
    OpcUa_GuidTest_4021();
}

/* Tests for OpcUa_Guid_FromString */
void OpcUa_GuidTest_FromString()
{
    OpcUa_GuidTest_4004();
    OpcUa_GuidTest_4005();
    OpcUa_GuidTest_4006();
    OpcUa_GuidTest_4007();
    OpcUa_GuidTest_4022();
}

/* Tests for OpcUa_Guid_IsEqual */
void OpcUa_GuidTest_IsEqual()
{
    OpcUa_GuidTest_4008();
    OpcUa_GuidTest_4009();
    OpcUa_GuidTest_4010();
    OpcUa_GuidTest_4011();
    OpcUa_GuidTest_4012();
}

/* Tests for OpcUa_Guid_IsNull */
void OpcUa_GuidTest_IsNull()
{
    OpcUa_GuidTest_4013();
    OpcUa_GuidTest_4014();
    OpcUa_GuidTest_4015();
    OpcUa_GuidTest_4016();
}

/* Tests for OpcUa_Guid_Copy */
void OpcUa_GuidTest_GuidCopy()
{
    OpcUa_GuidTest_4017();
    OpcUa_GuidTest_4018();
    OpcUa_GuidTest_4019();
    OpcUa_GuidTest_4020();
}

/* Create a new guid (GOOD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4000()
{
    OpcUa_Guid guid;
    OpcUa_Guid* pGuid = OpcUa_Null;

    CASE_ID(400);

    pGuid = OpcUa_Guid_Create(&guid);

    ASSERT(pGuid != OpcUa_Null);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Guid_ToString with OpcUa_Null for the guid (BAD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4001()
{
    OpcUa_CharA  str_guid[OPCUA_GUID_LEXICAL_LENGTH + 1];
    OpcUa_CharA* ret = OpcUa_Null;

    CASE_ID(4001);

    ret = OpcUa_Guid_ToStringA((OpcUa_Guid*) OpcUa_Null, str_guid);

    ASSERT(ret == (OpcUa_CharA*)OpcUa_Null);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Guid_ToString with OpcUa_Null for the string (BAD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4002()
{
    OpcUa_Guid      guid;
    OpcUa_CharA*    ret = OpcUa_Null;

    CASE_ID(4002);

    ASSERT(OpcUa_Guid_Create(&guid) == &guid);

    ret = OpcUa_Guid_ToStringA(&guid, (OpcUa_CharA*)OpcUa_Null);
    ASSERT(ret == (OpcUa_CharA*)OpcUa_Null);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_Guid_ToString to convert a guid to a string (GOOD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4003()
{
    OpcUa_Guid guid1;
    OpcUa_Guid guid2;
    OpcUa_CharA str_guid[OPCUA_GUID_LEXICAL_LENGTH + 1];

    CASE_ID(4003);

    OpcUa_MemSet(&guid1, 0, sizeof(OpcUa_Guid));
    OpcUa_MemSet(&guid2, 0, sizeof(OpcUa_Guid));
    OpcUa_MemSet(&str_guid, 0, (sizeof(OpcUa_CharA)*OPCUA_GUID_LEXICAL_LENGTH + 1));

    ASSERT(OpcUa_Guid_Create(&guid1) == &guid1);

    ASSERT(OpcUa_Guid_ToStringA(&guid1, str_guid) == str_guid);

    ASSERT_GOOD(OpcUa_Guid_FromString(str_guid, &guid2));

    ASSERT(OpcUa_Guid_IsEqual(&guid1, &guid2) == OpcUa_True);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Guid_FromString with OpcUa_Null for the buffer (BAD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4004()
{
    OpcUa_Guid guid;

    CASE_ID(4004);

    ASSERT_STATUS(OpcUa_Guid_FromString((OpcUa_CharA*) OpcUa_Null, &guid), OpcUa_BadInvalidArgument);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Guid_FromString with OpcUa_Null for the guid (BAD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4005()
{
    OpcUa_CharA str_guid[OPCUA_GUID_LEXICAL_LENGTH + 1];

    CASE_ID(4005);

    ASSERT_STATUS(OpcUa_Guid_FromString(str_guid,(OpcUa_Guid*) OpcUa_Null),OpcUa_BadInvalidArgument);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Guid_FromString with an improperly formatted string (BAD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4006()
{
    OpcUa_CharA str_guid[OPCUA_GUID_LEXICAL_LENGTH + 1];
    OpcUa_Guid guid;
    OpcUa_Int32 i;

    CASE_ID(4006);

    for(i=0;i<OPCUA_GUID_LEXICAL_LENGTH;i++)
    {
        str_guid[i] = '*';
    }

    ASSERT_STATUS(OpcUa_Guid_FromString(str_guid,&guid),OpcUa_BadInvalidArgument);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_Guid_FromString to convert a string to a guid (GOOD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4007()
{
    OpcUa_CharA str_guid[OPCUA_GUID_LEXICAL_LENGTH + 1];
    OpcUa_Guid guid1, guid2;

    CASE_ID(4007);

    ASSERT(OpcUa_Guid_Create(&guid1) == &guid1);
    ASSERT(OpcUa_Guid_ToStringA(&guid1,str_guid) == str_guid);
    ASSERT_GOOD(OpcUa_Guid_FromString(str_guid,&guid2));
    ASSERT(OpcUa_Guid_IsEqual(&guid1,&guid2) == OpcUa_True);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Guid_IsEqual with OpcUa_Null for the first argument (BAD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4008()
{
    OpcUa_Guid guid;

    CASE_ID(4008);

    ASSERT(OpcUa_Guid_Create(&guid) == &guid);
    ASSERT(OpcUa_Guid_IsEqual((OpcUa_Guid*) OpcUa_Null,&guid) == OpcUa_False);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Guid_IsEqual with OpcUa_Null for the second argument (BAD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4009()
{
    OpcUa_Guid guid;

    CASE_ID(4009);

    ASSERT(OpcUa_Guid_Create(&guid) == &guid);
    ASSERT(OpcUa_Guid_IsEqual(&guid,(OpcUa_Guid*) OpcUa_Null) == OpcUa_False);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_Guid_IsEqual to compare a guid to itself (GOOD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4010()
{
    OpcUa_Guid guid;

    CASE_ID(4010);

    ASSERT(OpcUa_Guid_Create(&guid) == &guid);
    ASSERT(OpcUa_Guid_IsEqual(&guid,&guid) == OpcUa_True);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_Guid_IsEqual to compare two different guids (GOOD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4011()
{
    OpcUa_Guid guid1, guid2;

    CASE_ID(4011);

    ASSERT(OpcUa_Guid_Create(&guid1) == &guid1);
    ASSERT(OpcUa_Guid_Create(&guid2) == &guid2);
    ASSERT(OpcUa_Guid_IsEqual(&guid1,&guid2) == OpcUa_False);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_Guid_IsEqual to compare two copies of the same guid (GOOD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4012()
{
    OpcUa_Guid guid1, guid2;

    CASE_ID(4012);

    ASSERT(OpcUa_Guid_Create(&guid1) == &guid1);
    OpcUa_Guid_Copy(&guid2, &guid1);
    ASSERT(OpcUa_Guid_IsEqual(&guid1,&guid2) == OpcUa_True);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Guid_IsNull with OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4013()
{
    CASE_ID(4013);

    ASSERT(OpcUa_Guid_IsNull((OpcUa_Guid*) OpcUa_Null) == OpcUa_True);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Guid_IsNull with OpcUa_Guid_Null as the argument (GOOD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4014()
{
    CASE_ID(4014);

    ASSERT(OpcUa_Guid_IsNull(&OpcUa_Guid_Null) == OpcUa_True);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Guid_IsNull with a copy of OpcUa_Guid_Null (GOOD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4015()
{
    OpcUa_Guid null_guid;

    CASE_ID(4015);

    OpcUa_Guid_Copy(&null_guid, &OpcUa_Guid_Null);

    ASSERT(OpcUa_Guid_IsNull(&null_guid) == OpcUa_True);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Guid_IsNull with a non-null guid (GOOD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4016()
{
    OpcUa_Guid guid;

    CASE_ID(4016);

    ASSERT(OpcUa_Guid_Create(&guid) == &guid);
    ASSERT(OpcUa_Guid_IsNull(&guid) == OpcUa_False);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Guid_Copy with OpcUa_Null as the source (BAD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4017()
{
    OpcUa_Guid guid;

    CASE_ID(4017);

    OpcUa_Guid_Copy(&guid, (OpcUa_Guid*)OpcUa_Null);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Guid_Copy with OpcUa_Null as the destination (BAD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4018()
{
    OpcUa_Guid guid;

    CASE_ID(4018);

    ASSERT(OpcUa_Guid_Create(&guid) == &guid);
    OpcUa_Guid_Copy((OpcUa_Guid*)OpcUa_Null, &guid);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_Guid_Copy with the same guid for the source and destination (GOOD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4019()
{
    OpcUa_Guid guid;

    CASE_ID(4019);

    ASSERT(OpcUa_Guid_Create(&guid) == &guid);
    OpcUa_Guid_Copy(&guid, &guid);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_Guid_Copy to make a copy of a guid (GOOD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4020()
{
    OpcUa_Guid guid1, guid2;

    CASE_ID(4020);

    ASSERT(OpcUa_Guid_Create(&guid1) == &guid1);
    OpcUa_Guid_Copy(&guid2, &guid1);
    ASSERT(OpcUa_Guid_IsEqual(&guid1,&guid2) == OpcUa_True);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_Guid_ToString to convert a guid to a string (GOOD CASE) */
OpcUa_StatusCode OpcUa_GuidTest_4021()
{
    OpcUa_Guid guid1;
    OpcUa_Guid guid2;
    OpcUa_String* pString = OpcUa_Null;
    OpcUa_StringA str_guid = OpcUa_Null;

    CASE_ID(4021);

    OpcUa_MemSet(&guid1, 0, sizeof(OpcUa_Guid));
    OpcUa_MemSet(&guid2, 0, sizeof(OpcUa_Guid));

    ASSERT(OpcUa_Guid_Create(&guid1) == &guid1);

    ASSERT_GOOD(OpcUa_Guid_ToString(&guid1, &pString));

    ASSERT(OpcUa_String_StrLen(pString) == OPCUA_GUID_LEXICAL_LENGTH);

    str_guid = OpcUa_String_GetRawString(pString);

    ASSERT_GOOD(OpcUa_Guid_FromString(str_guid, &guid2));

    ASSERT(OpcUa_Guid_IsEqual(&guid1, &guid2) == OpcUa_True);

    OpcUa_String_Delete(&pString);

    END_CASE();

    return OpcUa_Good;
}

/* Test if OpcUa_Guid_FromString accepts guids with braces. */
OpcUa_StatusCode OpcUa_GuidTest_4022()
{
    OpcUa_Guid guid1;

    OpcUa_StringA str_guid1 = "{bf77c28a-1e24-4e90-b5e8-1988c168f2f1}";

    CASE_ID(4022);

    OpcUa_MemSet(&guid1, 0, sizeof(OpcUa_Guid));

    ASSERT_GOOD(OpcUa_Guid_FromString(str_guid1, &guid1));

    END_CASE();

    return OpcUa_Good;
}
