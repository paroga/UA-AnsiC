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

#include "stringtest.h"
#include <unittest.h>
/* base */
#include <opcua_proxystub.h>

/* core */
#include <opcua_core.h>

/* self */
#include <opcua_string.h>
#include <opcua_p_string.h>

#ifndef _WIN32
# include <unistd.h>
#endif

#define sleepShutdownDelay 1000
#define IDBASE 10000

OpcUa_CharA g_pShortestStringCaps[] = {"ABC"};
OpcUa_CharA g_pShortestString[] = {"abc"};
OpcUa_CharA g_pShorterString[] = {"a1b2c3d4e5"};
OpcUa_CharA g_pShortString[] = {"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789"};
OpcUa_CharA g_pLongestString[] = {"OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa" \
                                  "OpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUaOpcUa!" };

/* forward declarations of test cases */
OpcUa_StatusCode OpcUa_StringTest_Initialize();
OpcUa_StatusCode OpcUa_StringTest_CreateNewString();
OpcUa_StatusCode OpcUa_StringTest_Delete();
OpcUa_StatusCode OpcUa_StringTest_CreateDelete();
OpcUa_StatusCode OpcUa_StringTest_Clear();
OpcUa_StatusCode OpcUa_StringTest_AttachToString();
OpcUa_StatusCode OpcUa_StringTest_IsEmptyNull();
OpcUa_StatusCode OpcUa_StringTest_GetRawString();
OpcUa_StatusCode OpcUa_StringTest_StrSizeLen();
OpcUa_StatusCode OpcUa_StringTest_StrnCat();
OpcUa_StatusCode OpcUa_StringTest_StrnCpy();
OpcUa_StatusCode OpcUa_StringTest_StrnCmp();
OpcUa_StatusCode OpcUa_StringTest_Additional();
/* TODO: add test cases here */

/** Test Entry Point */
void OpcUa_StringTest_Run()
{
    OpcUa_StringTest_Initialize();
    OpcUa_StringTest_CreateNewString();
    OpcUa_StringTest_Delete();
    OpcUa_StringTest_CreateDelete();
    OpcUa_StringTest_Clear();
    OpcUa_StringTest_AttachToString();
    OpcUa_StringTest_IsEmptyNull();
    OpcUa_StringTest_GetRawString();
    OpcUa_StringTest_StrSizeLen();
    OpcUa_StringTest_StrnCat();
    OpcUa_StringTest_StrnCpy();
    OpcUa_StringTest_StrnCmp();
    OpcUa_StringTest_Additional();
}

/* Returns */
/* 0 == strings match */
/* Non-zero == strings don't match */
int
CompareStringToSource (
    OpcUa_String* pUaString,
    OpcUa_StringA Source
)
{
    OpcUa_UInt32 uCharacter;
    OpcUa_CharA* pSource;
    OpcUa_CharA* pString;

    for (uCharacter = 0, pSource = &Source [0], pString = OpcUa_String_GetRawString(pUaString); uCharacter < OpcUa_String_StrSize(pUaString); ++uCharacter) {
        if (*pSource != *pString) {
            return 1;
        }
    }

    return 0;
}


OpcUa_StatusCode OpcUa_StringTest_Initialize()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_String String;

    CASE_ID (IDBASE);
    /* Create a string structure and call OpcUa_String_Intialize on it. */
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_Initialize (&String));
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_StringTest_CreateNewString()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_String* pUaString;


    CASE_ID (IDBASE + 1);
    /* Create a string with a null source and specify a zero terminated string. */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_String_CreateNewString (OpcUa_Null, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString), OpcUa_BadInvalidArgument);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 2);
    /* Create a string with a null source and specify a nonzero length. */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_String_CreateNewString (OpcUa_Null, 16, 0, OpcUa_True, OpcUa_True, &pUaString), OpcUa_BadInvalidArgument);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 3);
    /* Create a string with a valid source and a null a_ppNewString. */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_String_CreateNewString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, OpcUa_Null), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}

OpcUa_StatusCode OpcUa_StringTest_Delete()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_String* pUaString;

    CASE_ID (IDBASE + 4);
    /* Pass an OpcUa_Null pointer (a_ppString = OpcUa_Null) and check for memory leaks */
    MEM_SNAPSHOT (&s1);
    OpcUa_String_Delete ((OpcUa_String**)OpcUa_Null);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    CASE_ID (IDBASE + 5);
    /* Pass an OpcUa_Null pointer (a_ppString = OpcUa_Null) and check for memory leaks */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    OpcUa_String_Delete (&pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_StringTest_CreateDelete()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_String* pUaString;
    OpcUa_CharA* pAllocatedString;
    OpcUa_UInt32 uAllocatedStringSize;


    CASE_ID (IDBASE + 6);
    /* Create a copy of a null-terminated string and then delete it. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (0 == CompareStringToSource (pUaString, g_pShortString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 7);
    /* Create a copy of a string by specifying the length and then delete it. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, 26, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (0 == CompareStringToSource (pUaString, g_pShortString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 8);
    /* Create a copy of a string by specifying a zero length and then delete it. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, 0, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (0 == CompareStringToSource (pUaString, g_pShortString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 9);
    /* Create a new ua string by pointing to a static string.  Then delete it, making sure the static string isn't deleted. */
    /* a_bDoCopy = OpcUa_False, a_bFreeOnClear = OpcUa_False */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_False, OpcUa_False, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 10);
    /* Create a new ua string by pointing to a a dynamically allocated string. Then delete it, making sure the allocated string is deleted. */
    /* a_bDoCopy = OpcUa_False, a_bFreeOnClear = OpcUa_True */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    uAllocatedStringSize = OpcUa_StrLenA (g_pShortString) + 1;
    pAllocatedString = (OpcUa_CharA*)OpcUa_Alloc (uAllocatedStringSize);
    ASSERT (OpcUa_Null != pAllocatedString);
    OpcUa_Memory_MemCpy (pAllocatedString, uAllocatedStringSize, g_pShortString, uAllocatedStringSize);
    ASSERT_GOOD (OpcUa_String_CreateNewString (pAllocatedString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_False, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_StringTest_Clear()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_String* pUaString;
    OpcUa_CharA* pAllocatedString;
    OpcUa_UInt32 uAllocatedStringSize;


    CASE_ID (IDBASE + 11);
    /* Create a copy of a string, clear it, delete it, and check for memory leaks. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    OpcUa_String_Clear (pUaString);
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 12);
    /* Create a ua string from a dynamically allocated string, clear it, delete it, and check for memory leaks. */
    /* (a_bDoCopy = OpcUa_False, a_bFreeOnClear = OpcUa_True) */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    uAllocatedStringSize = OpcUa_StrLenA (g_pShortString) + 1;
    pAllocatedString = (OpcUa_CharA*)OpcUa_Alloc (uAllocatedStringSize);
    ASSERT (OpcUa_Null != pAllocatedString);
    OpcUa_Memory_MemCpy (pAllocatedString, uAllocatedStringSize, g_pShortString, uAllocatedStringSize);
    ASSERT_GOOD (OpcUa_String_CreateNewString (pAllocatedString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_False, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    OpcUa_String_Clear (pUaString);
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_StringTest_AttachToString()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_String* pUaString;
    OpcUa_CharA* pAllocatedString;
    OpcUa_UInt32 uAllocatedStringSize;


    CASE_ID (IDBASE + 13);
    /* Call attach with a_pNewString set to OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    ASSERT_STATUS (OpcUa_String_AttachToString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, OpcUa_Null), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 14);
    /* Call attach with a_strSource set to OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_STATUS (OpcUa_String_AttachToString (OpcUa_Null, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, pUaString), OpcUa_BadInvalidArgument);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 15);
    /* Create a copy of a string, then attach a longer null-terminated string */
    /*(a_bDoCopy = OpcUa_True, a_bFreeOnClear = OpcUa_True), and then delete it. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT_GOOD (OpcUa_String_AttachToString (g_pLongestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, pUaString));
    ASSERT (0 == CompareStringToSource (pUaString, g_pLongestString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 16);
    /* Create a copy of a string, then attach a longer null-terminated string */
    /*(a_bDoCopy = OpcUa_True, a_bFreeOnClear = OpcUa_True), and then delete it. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    uAllocatedStringSize = OpcUa_StrLenA (g_pLongestString) + 1;
    pAllocatedString = (OpcUa_CharA*)OpcUa_Alloc (uAllocatedStringSize);
    ASSERT (OpcUa_Null != pAllocatedString);
    OpcUa_Memory_MemCpy (pAllocatedString, uAllocatedStringSize, g_pLongestString, uAllocatedStringSize);
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT_GOOD (OpcUa_String_AttachToString (pAllocatedString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_False, OpcUa_True, pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_StringTest_IsEmptyNull()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_String* pUaString;


    CASE_ID (IDBASE + 17);
    /* Call with a_pString = OpcUa_Null.  OpcUa_String_IsEmpty should return OpcUa_False. */
    MEM_SNAPSHOT (&s1);
    ASSERT (OpcUa_False == OpcUa_String_IsEmpty (OpcUa_Null));
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 18);
    /* Create an empty string. OpcUa_String_IsEmpty should return OpcUa_True. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString ("", OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (OpcUa_True == OpcUa_String_IsEmpty (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 19);
    /* Create a copy of a string. OpcUa_String_IsEmpty should return OpcUa_False. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (OpcUa_False == OpcUa_String_IsEmpty (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 20);
    /* Create a string with a null source and specify a zero length.  OpcUa_String_IsEmpty should return OpcUa_False. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (OpcUa_Null, 0, 0, OpcUa_False, OpcUa_False, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (OpcUa_False == OpcUa_String_IsEmpty (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 21);
    /* Create a string with a non-null source and specify a zero length.  OpcUa_String_IsEmpty should return OpcUa_True. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, 0, 0, OpcUa_False, OpcUa_False, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (OpcUa_True == OpcUa_String_IsEmpty (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 22);
    /* Call with a_pString = OpcUa_Null.  OpcUa_String_IsNull should return OpcUa_True. */
    MEM_SNAPSHOT (&s1);
    ASSERT (OpcUa_True == OpcUa_String_IsNull (OpcUa_Null));
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 23);
    /* Create an empty string. OpcUa_String_IsNull should return OpcUa_False. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString ("", OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (OpcUa_False == OpcUa_String_IsNull (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 24);
    /* Create a copy of a string. OpcUa_String_IsNull should return OpcUa_False. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (OpcUa_False == OpcUa_String_IsNull (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 25);
    /* Create a string with a null source and specify a zero length.  OpcUa_String_IsEmpty should return OpcUa_True. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (OpcUa_Null, 0, 0, OpcUa_False, OpcUa_False, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (OpcUa_True == OpcUa_String_IsNull (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_StringTest_GetRawString()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_String* pUaString;

    CASE_ID (IDBASE + 26);
    /* Create a string with a null source and specify a zero length.  OpcUa_String_GetRawString should return OpcUa_Null. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (OpcUa_Null, 0, 0, OpcUa_False, OpcUa_False, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (OpcUa_Null == OpcUa_String_GetRawString (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 27);
    /* Create a copy of a string.  The pointer returned from OpcUa_String_GetRawString should not match the source string. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (g_pShortString != OpcUa_String_GetRawString (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 28);
    /* Create a string (a_bDoCopy = OpcUa_False, a_bFreeOnClear = OpcUa_False).  The pointer returned from OpcUa_String_GetRawString should match the source string. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_False, OpcUa_False, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (g_pShortString == OpcUa_String_GetRawString (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_StringTest_StrSizeLen()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_String* pUaString;


    CASE_ID (IDBASE + 29);
    /* Call with a_pString = OpcUa_Null.  OpcUa_String_StrSize should return 0. */
    MEM_SNAPSHOT (&s1);
    ASSERT (0 == OpcUa_String_StrSize (OpcUa_Null));
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 30);
    /* Pass a string directly to StrSize. */
    MEM_SNAPSHOT (&s1);
    ASSERT ((OpcUa_UInt32)OpcUa_StrLenA (&g_pShortString [0]) == OpcUa_String_StrSize ((OpcUa_String*)&g_pShortString [0]));
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 31);
    /* Create a copy of a null-terminated string.  Verify that OpcUa_String_StrSize returns the correct length. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT ((OpcUa_UInt32)OpcUa_StrLenA (&g_pShortString [0]) == OpcUa_String_StrSize (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 32);
    /* Create a copy of a string and specify the length.  Verify that OpcUa_String_StrSize returns the correct length. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, 10, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (10 == OpcUa_String_StrSize (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 33);
    /* Create a ua string that points to a null-terminated, static string.  Verify that OpcUa_StrSize returns the correct length. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_False, OpcUa_False, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT ((OpcUa_UInt32)OpcUa_StrLenA (&g_pShortString [0]) == OpcUa_String_StrSize (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 34);
    /* Create a ua string that points to a static string and specify a length.  Verify that OpcUa_StrSize returns the correct length. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, 10, 0, OpcUa_False, OpcUa_False, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (10 == OpcUa_String_StrSize (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 35);
    /* Call with a_pString = OpcUa_Null.  OpcUa_String_StrLen should return 0. */
    MEM_SNAPSHOT (&s1);
    ASSERT (0 == OpcUa_String_StrLen (OpcUa_Null));
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 36);
    /* Pass a string directly to StrLen. */
    MEM_SNAPSHOT (&s1);
    ASSERT ((OpcUa_UInt32)OpcUa_StrLenA (&g_pShortString [0]) == OpcUa_String_StrLen ((OpcUa_String*)&g_pShortString [0]));
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 37);
    /* Create a copy of a null-terminated string.  Verify that OpcUa_String_StrLen returns the correct length. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT ((OpcUa_UInt32)OpcUa_StrLenA (&g_pShortString [0]) == OpcUa_String_StrLen (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 38);
    /* Create a copy of a string and specify the length.  Verify that OpcUa_String_StrLen returns the correct length. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, 10, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (10 == OpcUa_String_StrLen (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 39);
    /* Create a ua string that points to a null-terminated, static string.  Verify that OpcUa_StrLen returns the correct length. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_False, OpcUa_False, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT ((OpcUa_UInt32)OpcUa_StrLenA (&g_pShortString [0]) == OpcUa_String_StrLen (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 40);
    /* Create a ua string that points to a static string and specify a length.  Verify that OpcUa_StrLen returns the correct length. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, 10, 0, OpcUa_False, OpcUa_False, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT (10 == OpcUa_String_StrLen (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_StringTest_StrnCat()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_String* pUaStringSrc;
    OpcUa_String* pUaStringDest;
    OpcUa_UInt32 uUaStringDestLen;
    OpcUa_CharA* pStringSrc;
    OpcUa_UInt32 uStringSrcAllocated;
    OpcUa_CharA* pStringDest;
    OpcUa_UInt32 uStringDestAllocated;

    CASE_ID (IDBASE + 41);
    /* Call with pDestString = OpcUa_Null. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_STATUS (OpcUa_String_StrnCat (OpcUa_Null, pUaStringSrc, OPCUA_STRING_LENDONTCARE), OpcUa_BadInvalidArgument);
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 42);
    /* Create an null string, and concatenate a longer string onto it.  Verify the length of the new string. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(OpcUa_Null, 0, 0, OpcUa_False, OpcUa_False, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    uUaStringDestLen = OpcUa_String_StrLen (pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCat (pUaStringDest, pUaStringSrc, OPCUA_STRING_LENDONTCARE));
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == (OpcUa_String_StrLen (pUaStringSrc) + uUaStringDestLen));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 43);
    /* Create an empty string, and concatenate a longer string onto it.  Verify the length of the new string. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString("", OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    uUaStringDestLen = OpcUa_String_StrLen (pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCat (pUaStringDest, pUaStringSrc, OPCUA_STRING_LENDONTCARE));
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == (OpcUa_String_StrLen (pUaStringSrc) + uUaStringDestLen));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 44);
    /* Create a copy of a string, and concatenate a null string onto it.  Verify the length of the new string. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    uUaStringDestLen = OpcUa_String_StrLen (pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(OpcUa_Null, 0, 0, OpcUa_False, OpcUa_False, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCat (pUaStringDest, pUaStringSrc, OPCUA_STRING_LENDONTCARE));
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == (OpcUa_String_StrLen (pUaStringSrc) + uUaStringDestLen));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 45);
    /* Create a copy of a string, and concatenate another string onto it.  Verify the length of the new string. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    uUaStringDestLen = OpcUa_String_StrLen (pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pLongestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCat (pUaStringDest, pUaStringSrc, OPCUA_STRING_LENDONTCARE));
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == (OpcUa_String_StrLen (pUaStringSrc) + uUaStringDestLen));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 46);
    /* Create two strings that point to static, null-terminated strings.  Concatenate them together. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_False, OpcUa_False, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    uUaStringDestLen = OpcUa_String_StrLen (pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pLongestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_False, OpcUa_False, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCat (pUaStringDest, pUaStringSrc, OPCUA_STRING_LENDONTCARE));
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == (OpcUa_String_StrLen (pUaStringSrc) + uUaStringDestLen));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 47);
    /* Create two strings that point to dynamic, null-terminated strings.  Concatenate them together. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    /* Create the dynamically-allocated strings */
    uStringDestAllocated = OpcUa_StrLenA (g_pShortString) + 1;
    pStringDest = (OpcUa_CharA*)OpcUa_Alloc (uStringDestAllocated);
    ASSERT (OpcUa_Null != pStringDest);
    OpcUa_Memory_MemCpy (pStringDest, uStringDestAllocated, g_pShortString, uStringDestAllocated);
    uStringSrcAllocated = OpcUa_StrLenA (g_pLongestString) + 1;
    pStringSrc = (OpcUa_CharA*)OpcUa_Alloc (uStringSrcAllocated);
    ASSERT (OpcUa_Null != pStringSrc);
    OpcUa_Memory_MemCpy (pStringSrc, uStringSrcAllocated, g_pLongestString, uStringSrcAllocated);
    /* Now process the UA strings */
    ASSERT_GOOD (OpcUa_String_CreateNewString(pStringDest, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_False, OpcUa_True, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    uUaStringDestLen = OpcUa_String_StrLen (pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(pStringSrc, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_False, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCat (pUaStringDest, pUaStringSrc, OPCUA_STRING_LENDONTCARE));
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == (OpcUa_String_StrLen (pUaStringSrc) + uUaStringDestLen));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 48);
    /* Create two strings copies and concatenate them together specifying a length less than the source string. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    uUaStringDestLen = OpcUa_String_StrLen (pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShorterString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCat (pUaStringDest, pUaStringSrc, 7));
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == 7 + uUaStringDestLen);
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 49);
    /* Create two strings copies and concatenate them together specifying a length greater than the sum of the lengths of the source and destination strings. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    uUaStringDestLen = OpcUa_String_StrLen (pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShorterString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCat (pUaStringDest, pUaStringSrc, 10000));
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == OpcUa_String_StrLen (pUaStringSrc) + uUaStringDestLen);
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 50);
    /* Create two strings copies and concatenate them together specifying a length of 0. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    uUaStringDestLen = OpcUa_String_StrLen (pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShorterString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCat (pUaStringDest, pUaStringSrc, 0));
#if 0
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == OpcUa_String_StrLen (pUaStringSrc) + uUaStringDestLen);
#else
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == uUaStringDestLen);
#endif
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_StringTest_StrnCpy()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_String* pUaStringSrc;
    OpcUa_String* pUaStringDest;
    OpcUa_CharA* pStringSrc;
    OpcUa_UInt32 uStringSrcAllocated;
    OpcUa_CharA* pStringDest;
    OpcUa_UInt32 uStringDestAllocated;


    CASE_ID (IDBASE + 51);
    /* Call with pDestString = OpcUa_Null. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_STATUS (OpcUa_String_StrnCpy (OpcUa_Null, pUaStringSrc, OPCUA_STRING_LENDONTCARE), OpcUa_BadInvalidArgument);
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    CASE_ID (IDBASE + 52);
    /* Call with pSrcString = OpcUa_Null. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    ASSERT_GOOD (OpcUa_String_StrnCpy (pUaStringDest, OpcUa_Null, OPCUA_STRING_LENDONTCARE));
    ASSERT (0 == OpcUa_String_StrLen (pUaStringDest));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    CASE_ID (IDBASE + 53);
    /* Create an null string, and copy a longer string onto it.  Verify the length of the new string. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(OpcUa_Null, 0, 0, OpcUa_False, OpcUa_False, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCpy (pUaStringDest, pUaStringSrc, OPCUA_STRING_LENDONTCARE));
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == OpcUa_String_StrLen (pUaStringSrc));
    ASSERT (0 == CompareStringToSource (pUaStringDest, OpcUa_String_GetRawString (pUaStringSrc)));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 54);
    /* Create an empty string, and copy a longer string onto it.  Verify the length of the new string. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString("", OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCpy (pUaStringDest, pUaStringSrc, OPCUA_STRING_LENDONTCARE));
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == OpcUa_String_StrLen (pUaStringSrc));
    ASSERT (0 == CompareStringToSource (pUaStringDest, OpcUa_String_GetRawString (pUaStringSrc)));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    CASE_ID (IDBASE + 55);
    /* Create a copy of a string, and copy a null string onto it.  Verify the length of the new string. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(OpcUa_Null, 0, 0, OpcUa_False, OpcUa_False, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCpy (pUaStringDest, pUaStringSrc, OPCUA_STRING_LENDONTCARE));
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == OpcUa_String_StrLen (pUaStringSrc));
    ASSERT (0 == CompareStringToSource (pUaStringDest, OpcUa_String_GetRawString (pUaStringSrc)));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    CASE_ID (IDBASE + 56);
    /* Create a copy of a string, and copy another string onto it.  Verify the length of the new string. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pLongestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCpy (pUaStringDest, pUaStringSrc, OPCUA_STRING_LENDONTCARE));
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == OpcUa_String_StrLen (pUaStringSrc));
    ASSERT (0 == CompareStringToSource (pUaStringDest, OpcUa_String_GetRawString (pUaStringSrc)));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 57);
    /* Create two strings that point to static, null-terminated strings.  Copy the second onto the first. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_False, OpcUa_False, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pLongestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_False, OpcUa_False, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCpy (pUaStringDest, pUaStringSrc, OPCUA_STRING_LENDONTCARE));
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == OpcUa_String_StrLen (pUaStringSrc));
    ASSERT (0 == CompareStringToSource (pUaStringDest, OpcUa_String_GetRawString (pUaStringSrc)));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 58);
    /* Create two strings that point to dynamic, null-terminated strings.  Copy the second onto the first. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    /* Create the dynamically-allocated strings */
    uStringDestAllocated = OpcUa_StrLenA (g_pShortString) + 1;
    pStringDest = (OpcUa_CharA*)OpcUa_Alloc (uStringDestAllocated);
    ASSERT (OpcUa_Null != pStringDest);
    OpcUa_Memory_MemCpy (pStringDest, uStringDestAllocated, g_pShortString, uStringDestAllocated);
    uStringSrcAllocated = OpcUa_StrLenA (g_pLongestString) + 1;
    pStringSrc = (OpcUa_CharA*)OpcUa_Alloc (uStringSrcAllocated);
    ASSERT (OpcUa_Null != pStringSrc);
    OpcUa_Memory_MemCpy (pStringSrc, uStringSrcAllocated, g_pLongestString, uStringSrcAllocated);
    /* Now process the UA strings */
    ASSERT_GOOD (OpcUa_String_CreateNewString(pStringDest, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_False, OpcUa_True, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(pStringSrc, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_False, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCpy (pUaStringDest, pUaStringSrc, OPCUA_STRING_LENDONTCARE));
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == OpcUa_String_StrLen (pUaStringSrc));
    ASSERT (0 == CompareStringToSource (pUaStringDest, OpcUa_String_GetRawString (pUaStringSrc)));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 59);
    /* Create two strings copies and copy the second onto the first specifying a length less than the source string. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShorterString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCpy (pUaStringDest, pUaStringSrc, 7));
    ASSERT (7 == OpcUa_String_StrLen (pUaStringDest));
    ASSERT (0 == CompareStringToSource (pUaStringDest, OpcUa_String_GetRawString (pUaStringSrc)));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 60);
    /* Create two strings copies and copy the second onto the first specifying a length greater than the sum of the lengths of the source and destination strings. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShorterString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCpy (pUaStringDest, pUaStringSrc, 10000));
    ASSERT (OpcUa_String_StrLen (pUaStringDest) == OpcUa_String_StrLen (pUaStringSrc));
    ASSERT (0 == CompareStringToSource (pUaStringDest, OpcUa_String_GetRawString (pUaStringSrc)));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 61);
    /* Create two strings copies and copy the second onto the first specifying a length of 0. */
    pUaStringSrc = OpcUa_Null;
    pUaStringDest = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringDest));
    ASSERT (OpcUa_Null != pUaStringDest);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShorterString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaStringSrc));
    ASSERT (OpcUa_Null != pUaStringSrc);
    ASSERT_GOOD (OpcUa_String_StrnCpy (pUaStringDest, pUaStringSrc, 0));
    ASSERT (0 == OpcUa_String_StrLen (pUaStringDest));
    OpcUa_String_Delete (&pUaStringSrc);
    OpcUa_String_Delete (&pUaStringDest);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_StringTest_StrnCmp()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_String* pUaString1;
    OpcUa_String* pUaString2;


    CASE_ID (IDBASE + 62);
    /* Call with pString1 = OpcUa_Null and pString2 = OpcUa_Null. */
    pUaString1 = OpcUa_Null;
    pUaString2 = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT (0 == OpcUa_String_StrnCmp (pUaString1, pUaString2, 10, OpcUa_False));
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 63);
    /* Call with pString1 = OpcUa_Null. */
    pUaString1 = OpcUa_Null;
    pUaString2 = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString2));
    ASSERT (OpcUa_Null != pUaString2);
    ASSERT (OpcUa_String_StrnCmp (pUaString1, pUaString2, 10, OpcUa_False) < 0);
    OpcUa_String_Delete (&pUaString1);
    OpcUa_String_Delete (&pUaString2);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 64);
    /* Call with pString2 = OpcUa_Null. */
    pUaString1 = OpcUa_Null;
    pUaString2 = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString1));
    ASSERT (OpcUa_Null != pUaString1);
    ASSERT (OpcUa_String_StrnCmp (pUaString1, pUaString2, 10, OpcUa_False) > 0);
    OpcUa_String_Delete (&pUaString1);
    OpcUa_String_Delete (&pUaString2);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 65);
    /* Call with two strings that differ only by case (bIgnoreCase = OpcUa_False).  Then reverse the strings and compare them again . */
    pUaString1 = OpcUa_Null;
    pUaString2 = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString1));
    ASSERT (OpcUa_Null != pUaString1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortestStringCaps, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString2));
    ASSERT (OpcUa_Null != pUaString2);
    ASSERT (OpcUa_String_StrnCmp (pUaString1, pUaString2, OPCUA_STRING_LENDONTCARE, OpcUa_False) > 0);
    ASSERT (OpcUa_String_StrnCmp (pUaString2, pUaString1, OPCUA_STRING_LENDONTCARE, OpcUa_False) < 0);
    OpcUa_String_Delete (&pUaString1);
    OpcUa_String_Delete (&pUaString2);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 66);
    /* Call with two strings that differ only by case (bIgnoreCase = OpcUa_True).  Then reverse the strings and compare them again. */
    pUaString1 = OpcUa_Null;
    pUaString2 = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString1));
    ASSERT (OpcUa_Null != pUaString1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortestStringCaps, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString2));
    ASSERT (OpcUa_Null != pUaString2);
    ASSERT (0 == OpcUa_String_StrnCmp (pUaString1, pUaString2, OPCUA_STRING_LENDONTCARE, OpcUa_True));
    ASSERT (0 == OpcUa_String_StrnCmp (pUaString2, pUaString1, OPCUA_STRING_LENDONTCARE, OpcUa_True));
    OpcUa_String_Delete (&pUaString1);
    OpcUa_String_Delete (&pUaString2);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 67);
    /* Compare a string to itself. */
    pUaString1 = OpcUa_Null;
    pUaString2 = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pLongestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString1));
    ASSERT (OpcUa_Null != pUaString1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pLongestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString2));
    ASSERT (OpcUa_Null != pUaString2);
    ASSERT (0 == OpcUa_String_StrnCmp (pUaString1, pUaString2, OPCUA_STRING_LENDONTCARE, OpcUa_False));
    ASSERT (0 == OpcUa_String_StrnCmp (pUaString1, pUaString1, OPCUA_STRING_LENDONTCARE, OpcUa_False));
    OpcUa_String_Delete (&pUaString1);
    OpcUa_String_Delete (&pUaString2);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 68);
    /* Compare a very short string and a very long string, specifying a length longer than the short string. */
    pUaString1 = OpcUa_Null;
    pUaString2 = OpcUa_Null;
    MEM_SNAPSHOT (&s1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pShortestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString1));
    ASSERT (OpcUa_Null != pUaString1);
    ASSERT_GOOD (OpcUa_String_CreateNewString(g_pLongestString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString2));
    ASSERT (OpcUa_Null != pUaString2);
    ASSERT (OpcUa_String_StrnCmp (pUaString1, pUaString2, 10, OpcUa_False) < 0);
    ASSERT (OpcUa_String_StrnCmp (pUaString2, pUaString1, 10, OpcUa_False) > 0);
    OpcUa_String_Delete (&pUaString1);
    OpcUa_String_Delete (&pUaString2);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}


OpcUa_StatusCode OpcUa_StringTest_Additional()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_String* pUaString = OpcUa_Null;
    OpcUa_String aUaStringSrc = OPCUA_STRING_STATICINITIALIZER;
    OpcUa_UInt32 uLen = 0, uLen2 = 0;


    CASE_ID (IDBASE + 69);
    /* Create a copy of a string, call initialize on the copy, then delete it. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD (OpcUa_String_CreateNewString (g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT (OpcUa_Null != pUaString);
    ASSERT_GOOD (OpcUa_String_Initialize (pUaString));
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();


    CASE_ID (IDBASE + 70);
    /* create an empty string, copy-attach some content and cat some other content afterwards. compare size. */
    MEM_SNAPSHOT (&s1);
    pUaString = OpcUa_Null;
    ASSERT_GOOD(OpcUa_String_CreateNewString(OpcUa_Null, 0, 0, OpcUa_True, OpcUa_True, &pUaString));
    ASSERT_GOOD(OpcUa_String_AttachToString(g_pShortString, OPCUA_STRINGLENZEROTERMINATED, 0, OpcUa_True, OpcUa_True, pUaString));
    uLen = OpcUa_String_StrLen(pUaString);
    ASSERT_GOOD(OpcUa_String_StrnCat(pUaString, OpcUa_String_FromCString("OPCUA"), 5));
    uLen2 = OpcUa_String_StrLen(pUaString);
    ASSERT ((uLen + 5) == uLen2);
    OpcUa_String_Delete (&pUaString);
    ASSERT (OpcUa_Null == pUaString);
    MEM_SNAPSHOT(&s2);
    CHECK_SNAPSHOTS (&s3, &s1, &s2);
    END_CASE();

    return OpcUa_Good;
}
