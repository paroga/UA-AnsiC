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
#include <opcua_datetime.h>
#include <opcua_core.h>
#include "datetimetest.h"


/** Test entry point */
void OpcUa_DateTimeTest_Run()
{
#ifdef WITH_SET_UNSET
    OpcUa_DateTimeTest_Set();
    OpcUa_DateTimeTest_UnSet();
#endif
    OpcUa_DateTimeTest_GetDateTimeFromString();
    OpcUa_DateTimeTest_GetStringFromDateTime();
}

#ifdef WITH_SET_UNSET
/* Tests for OpcUa_DateTime_Set() */
void OpcUa_DateTimeTest_Set()
{
    OpcUa_DateTimeTest_3000();
    OpcUa_DateTimeTest_3001();
}

/* Tests for OpcUa_DateTime_UnSet() */
void OpcUa_DateTimeTest_UnSet()
{
    OpcUa_DateTimeTest_3002();
    OpcUa_DateTimeTest_3003();
}
#endif

/* Tests for OpcUa_DateTime_GetDateTimeFromString() */
void OpcUa_DateTimeTest_GetDateTimeFromString()
{
    OpcUa_DateTimeTest_3004();
    OpcUa_DateTimeTest_3005();
#if 0
    OpcUa_DateTimeTest_3006();
    OpcUa_DateTimeTest_3007();
#endif
}

/* Tests for OpcUa_DateTime_GetStringFromDateTime() */
void OpcUa_DateTimeTest_GetStringFromDateTime()
{
#if 0
    OpcUa_DateTimeTest_3008();
    OpcUa_DateTimeTest_3009();
    OpcUa_DateTimeTest_3010();
#endif
}

#ifdef WITH_SET_UNSET
/* Call OpcUa_DateTime_Set on an unset datetime (GOOD CASE) */
void OpcUa_DateTimeTest_3000()
{
    OpcUa_DateTime dt;

    OpcUa_DateTime_UnSet(&dt);
    ASSERT(OpcUa_DateTime_Set(dt) == OpcUa_False);
}

/* Call OpcUa_DateTime Set on a set datetime (GOOD CASE) */
void OpcUa_DateTimeTest_3001()
{
    OpcUa_DateTime dt;

    dt = OpcUa_DateTime_UtcNow();
    ASSERT(OpcUa_DateTime_Set(dt) == OpcUa_True);
}

/* Use OpcUa_DateTime_UnSet to unset a datetime that was previously set (GOOD CASE) */
void OpcUa_DateTimeTest_3002()
{
    OpcUa_DateTime dt;

    dt = OpcUa_DateTime_UtcNow();
    OpcUa_DateTime_UnSet(&dt);
    ASSERT(OpcUa_DateTime_Set(dt) == OpcUa_False);
}

/* Use OpcUa_DateTime_UnSet to unset a datetime that was already unset (GOOD CASE) */
void OpcUa_DateTimeTest_3003()
{
    OpcUa_DateTime dt;

    OpcUa_DateTime_UnSet(&dt);
    OpcUa_DateTime_UnSet(&dt);
    ASSERT(OpcUa_DateTime_Set(dt) == OpcUa_False);
}
#endif

/* Use OpcUa_DateTime_GetDateTimeFromString to convert a string that
   corresponds to a valid date and time into a datetime (GOOD CASE) */
OpcUa_StatusCode OpcUa_DateTimeTest_3004()
{
    OpcUa_CharA date_and_timeA[25] = "2006-07-25T13:34:29.000Z";
    OpcUa_CharA date_and_timeB[25];
    OpcUa_DateTime dt;

    CASE_ID(3004);
    ASSERT_GOOD(OpcUa_DateTime_GetDateTimeFromString(date_and_timeA,&dt));
    /*logentry("dt=%i,%i\n", dt.dwHighDateTime, dt.dwLowDateTime);*/
    ASSERT_GOOD(OpcUa_DateTime_GetStringFromDateTime(dt,date_and_timeB,25));
    /*logentry("date_and_timeB=%s\n", date_and_timeB);*/
    ASSERT(memcmp(date_and_timeA,date_and_timeB,25*sizeof(OpcUa_CharA)) == 0);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_DateTime_GetDateTimeFromString with a short string (BAD CASE) */
OpcUa_StatusCode OpcUa_DateTimeTest_3005()
{
    OpcUa_CharA date_and_time[13] = "2006-07-25T1";
    OpcUa_DateTime dt;

    CASE_ID(3005);
    ASSERT_STATUS(OpcUa_DateTime_GetDateTimeFromString(date_and_time,&dt),OpcUa_BadSyntaxError);
    END_CASE();

    return OpcUa_Good;
}

#if 0
OpcUa_StatusCode OpcUa_DateTimeTest_3006()
{
    const OpcUa_UInt64 year_11599 = 3155378975999999999; /* 11599-12-31T23:59:59.999Z */
    OpcUa_DateTime dt;
    OpcUa_CharA date_and_time[25];

    CASE_ID(3006);

    dt = OpcUa_DateTime_UtcNow();
    ASSERT_GOOD(OpcUa_DateTime_GetStringFromDateTime(dt,date_and_time,25));

    dt.dwLowDateTime = (OpcUa_UInt32)(year_11599);
    dt.dwHighDateTime = (OpcUa_UInt32)(year_11599 >> 32);
    ASSERT(OpcUa_DateTime_GetStringFromDateTime(dt,date_and_time,25) == OpcUa_BadOutOfRange);

    END_CASE();
    /* implement this */

    return OpcUa_Good;
}

void OpcUa_DateTimeTest_3007()
{
    CASE_ID(3007);
    logentry("implement me!\n");
    END_CASE();
}

void OpcUa_DateTimeTest_3008()
{
    CASE_ID(3008);
    logentry("implement me!\n");
    END_CASE();
}

void OpcUa_DateTimeTest_3009()
{
    CASE_ID(3009);
    logentry("implement me!\n");
    END_CASE();
}

void OpcUa_DateTimeTest_3010()
{
    CASE_ID(3010);
    logentry("implement me!\n");
    END_CASE();
}
#endif
