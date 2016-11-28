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

#include "opcua_list.h"
#include "listtest.h"

void OpcUa_ListTest_Run()
{
    OpcUa_ListTest_ElementCreate();
    OpcUa_ListTest_ElementInitialize();
    OpcUa_ListTest_ElementDelete();
    OpcUa_ListTest_Initialize();
    OpcUa_ListTest_Clear();
    OpcUa_ListTest_Delete();
    OpcUa_ListTest_AddElement();
    OpcUa_ListTest_ResetCurrent();
    OpcUa_ListTest_GetNextElement();
    OpcUa_ListTest_GetCurrentElement();
    OpcUa_ListTest_GetPrevElement();
    OpcUa_ListTest_GetElement();
    OpcUa_ListTest_DeleteElement();
    OpcUa_ListTest_RemoveFirstElement();
    OpcUa_ListTest_AddElementToEnd();
    OpcUa_ListTest_GetNumberOfElements();
}

/* Tests for OpcUa_ListElement_Ititialize */
void OpcUa_ListTest_ElementCreate()
{
    OpcUa_ListTest_5000();
    OpcUa_ListTest_5001();
}

/* Tests for OpcUa_ListElement_Initialize */
void OpcUa_ListTest_ElementInitialize()
{
    OpcUa_ListTest_5002();
    OpcUa_ListTest_5003();
}

/* Tests for OpcUa_ListElement_Delete */
void OpcUa_ListTest_ElementDelete()
{
    OpcUa_ListTest_5004();
    OpcUa_ListTest_5005();
    OpcUa_ListTest_5006();
}

/* Tests for OpcUa_List_Create */
void OpcUa_ListTest_Create()
{
    OpcUa_ListTest_5007();
    OpcUa_ListTest_5008();
}

/* Tests for OpcUa_List_Initialize */
void OpcUa_ListTest_Initialize()
{
    OpcUa_ListTest_5009();
}

/* Tests for OpcUa_List_Clear */
void OpcUa_ListTest_Clear()
{
    OpcUa_ListTest_5010();
    OpcUa_ListTest_5011();
    OpcUa_ListTest_5012();
    OpcUa_ListTest_5013();
}

/* Tests for OpcUa_List_Delete */
void OpcUa_ListTest_Delete()
{
    OpcUa_ListTest_5014();
    OpcUa_ListTest_5015();
    OpcUa_ListTest_5016();
    OpcUa_ListTest_5017();
}

/* Tests for OpcUa_List_AddElement */
void OpcUa_ListTest_AddElement()
{
    OpcUa_ListTest_5018();
    OpcUa_ListTest_5019();
    OpcUa_ListTest_5020();
    OpcUa_ListTest_5021();
}

/* Tests for OpcUa_List_ResetCurrent */
void OpcUa_ListTest_ResetCurrent()
{
    OpcUa_ListTest_5022();
    OpcUa_ListTest_5023();
    OpcUa_ListTest_5024();
}

/* Tests for OpcUa_List_GetNextElement */
void OpcUa_ListTest_GetNextElement()
{
    OpcUa_ListTest_5025();
    OpcUa_ListTest_5026();
    OpcUa_ListTest_5027();
}

/* Tests for OpcUa_List_GetCurrentElement */
void OpcUa_ListTest_GetCurrentElement()
{
    OpcUa_ListTest_5028();
    OpcUa_ListTest_5029();
    OpcUa_ListTest_5030();
}

/* Tests for OpcUa_List_GetPrevElement */
void OpcUa_ListTest_GetPrevElement()
{
    OpcUa_ListTest_5031();
    OpcUa_ListTest_5032();
    OpcUa_ListTest_5033();
}

/* Tests for OpcUa_List_GetElement */
void OpcUa_ListTest_GetElement()
{
    OpcUa_ListTest_5034();
    OpcUa_ListTest_5035();
    OpcUa_ListTest_5036();
    OpcUa_ListTest_5037();
}

/* Tests for OpcUa_List_DeleteCurrentElement */
void OpcUa_ListTest_DeleteCurrentElement()
{
    OpcUa_ListTest_5038();
    OpcUa_ListTest_5039();
    OpcUa_ListTest_5040();
}

/* Tests for OpcUa_List_DeleteElement */
void OpcUa_ListTest_DeleteElement()
{
    OpcUa_ListTest_5041();
    OpcUa_ListTest_5042();
    OpcUa_ListTest_5043();
    OpcUa_ListTest_5044();
}

/* Tests for OpcUa_List_RemoveFirstElement */
void OpcUa_ListTest_RemoveFirstElement()
{
    OpcUa_ListTest_5045();
    OpcUa_ListTest_5046();
    OpcUa_ListTest_5047();
}



/* Tests for OpcUa_List_AddElementToEnd */
void OpcUa_ListTest_AddElementToEnd()
{
    OpcUa_ListTest_5048();
    OpcUa_ListTest_5049();
    OpcUa_ListTest_5050();
}

/* Test for OpcUa_List_GetNumberOfElements */
void OpcUa_ListTest_GetNumberOfElements()
{
    OpcUa_ListTest_5051();
    OpcUa_ListTest_5052();
    OpcUa_ListTest_5053();
}

/****************\
*   UNIT TESTS     *
\****************/

/* Allocate and delete a list element (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5000()
{
    OpcUa_ListElement* p_elem;

    CASE_ID(5000);

    ASSERT_GOOD(OpcUa_ListElement_Create(&p_elem));
    p_elem->data = OpcUa_Null;
    p_elem->nextElement = (OpcUa_ListElement*)OpcUa_Null;
    p_elem->prevElement = (OpcUa_ListElement*)OpcUa_Null;
    OpcUa_ListElement_Delete(&p_elem);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_ListElement_Create with OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5001()
{
    CASE_ID(5001);
    ASSERT_STATUS(OpcUa_ListElement_Create((OpcUa_ListElement **)OpcUa_Null),OpcUa_BadInvalidArgument);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_ListElement_Initialize with OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5002()
{
    CASE_ID(5002);
    OpcUa_ListElement_Initialize((OpcUa_ListElement *)OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Initialize a valid list element (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5003()
{
    OpcUa_ListElement* p_elem;

    CASE_ID(5003);

    ASSERT_GOOD(OpcUa_ListElement_Create(&p_elem));
    OpcUa_ListElement_Initialize(p_elem);
    ASSERT((p_elem->data == OpcUa_Null) &&
           (p_elem->nextElement == OpcUa_Null) &&
           (p_elem->prevElement == OpcUa_Null));
    OpcUa_ListElement_Delete(&p_elem);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_ListElement_Delete with OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5004()
{
    CASE_ID(5004);

    OpcUa_ListElement_Delete((OpcUa_ListElement **)OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_ListElement_Delete with a pointer to OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5005()
{
    OpcUa_ListElement* p_elem = (OpcUa_ListElement *)OpcUa_Null;

    CASE_ID(5005);

    OpcUa_ListElement_Delete(&p_elem);
    END_CASE();

    return OpcUa_Good;
}

/* Delete a valid OpcUa_ListElement (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5006()
{
    SNAPSHOT_TYPE s1, s2, s3;
    OpcUa_ListElement* p_elem;
    OpcUa_Int32 i;

    CASE_ID(5006);

    MEM_SNAPSHOT(&s1);
    ASSERT_GOOD(OpcUa_ListElement_Create(&p_elem));
    OpcUa_ListElement_Initialize(p_elem);
    p_elem->data = (OpcUa_Void*)&i;
    OpcUa_ListElement_Delete(&p_elem);
    ASSERT(p_elem == OpcUa_Null);
    MEM_SNAPSHOT(&s2);

    CHECK_SNAPSHOTS(&s3,&s1,&s2);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_Create with OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5007()
{
    CASE_ID(5007);

    ASSERT_STATUS(OpcUa_List_Create((OpcUa_List **)OpcUa_Null),OpcUa_BadInvalidArgument);

    return OpcUa_Good;
}

/* Create a list (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5008()
{
    OpcUa_List* p_list;

    CASE_ID(5008);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT(p_list->currtElement == OpcUa_Null);
    ASSERT(p_list->firstElement == OpcUa_Null);
    ASSERT(p_list->lastElement == OpcUa_Null);
    ASSERT(p_list->uintNbElements == 0);
    ASSERT(p_list->pMutex != OpcUa_Null);

    OpcUa_List_Delete(&p_list);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_Initialize with OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5009()
{
    CASE_ID(5009);

    ASSERT_STATUS(OpcUa_List_Initialize((OpcUa_List *)OpcUa_Null),OpcUa_BadInvalidArgument);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_Clear with OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5010()
{
    CASE_ID(5010);
    OpcUa_List_Clear((OpcUa_List *)OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Clear a valid list (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5011()
{
    OpcUa_List* p_list;
    OpcUa_Int32 a, b, c;

    CASE_ID(5011);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&a));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&b));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&c));

    OpcUa_List_Clear(p_list);

    ASSERT(p_list->currtElement == OpcUa_Null);
    ASSERT(p_list->firstElement == OpcUa_Null);
    ASSERT(p_list->lastElement == OpcUa_Null);
    ASSERT(p_list->uintNbElements == 0);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Clear a valid list multiple times (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5012()
{
    OpcUa_List* p_list;

    CASE_ID(5012);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    OpcUa_List_Clear(p_list);
    OpcUa_List_Clear(p_list);
    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Use a list after clearing it (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5013()
{
    OpcUa_List* p_list;
    OpcUa_Int32 a;

    CASE_ID(5013);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    OpcUa_List_Clear(p_list);
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&a));
    OpcUa_List_Clear(p_list);
    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_Delete with OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5014()
{
    CASE_ID(5014);

    OpcUa_List_Delete((OpcUa_List **)OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_Delete with a pointer to OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5015()
{
    OpcUa_List* p_list = (OpcUa_List *)OpcUa_Null;

    CASE_ID(5015);

    OpcUa_List_Delete(&p_list);
    END_CASE();

    return OpcUa_Good;
}

/* Delete an empty list (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5016()
{
    OpcUa_List* p_list;

    CASE_ID(5016);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Delete a non-empty list (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5017()
{
    OpcUa_List* p_list;
    OpcUa_Int32 a, b, c;
    SNAPSHOT_TYPE s1, s2, s3;

    CASE_ID(5017);

    MEM_SNAPSHOT(&s1);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&a));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&b));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&c));

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);

    MEM_SNAPSHOT(&s2);

    CHECK_SNAPSHOTS(&s3,&s1,&s2);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_AddElement with OpcUa_Null for the list (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5018()
{
    OpcUa_Int32 a;

    CASE_ID(5018);

    ASSERT_STATUS(OpcUa_List_AddElement((OpcUa_List *)OpcUa_Null,(OpcUa_Void*)&a),OpcUa_BadInvalidArgument);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_AddElement with OpcUa_Null for the data (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5019()
{
    OpcUa_List* p_list;

    CASE_ID(5019);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_STATUS(OpcUa_List_AddElement(p_list,OpcUa_Null),OpcUa_BadInvalidArgument);
    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Add a single element (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5020()
{
    OpcUa_List* p_list;
    OpcUa_Int32 a;

    CASE_ID(5020);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&a));
    ASSERT(p_list->firstElement == p_list->lastElement);
    ASSERT(p_list->firstElement->data == &a);
    ASSERT(p_list->uintNbElements == 1);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Add multiple elements (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5021()
{
    OpcUa_List* p_list;
    OpcUa_Int32 i_array[10];
    OpcUa_UInt32 i;

    CASE_ID(5021);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));

    for(i=0;i<10;i++)
        ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)(i_array + i)));

    OpcUa_List_ResetCurrent(p_list);

    ASSERT((OpcUa_Int32*)OpcUa_List_GetCurrentElement(p_list) == i_array + 9);
    for(i=1;i<10;i++)
        ASSERT((OpcUa_Int32*)OpcUa_List_GetNextElement(p_list) == i_array + 9 - i);

    ASSERT(p_list->uintNbElements == 10);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_ResetCurrent with OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5022()
{
    CASE_ID(5022);
    ASSERT_STATUS(OpcUa_List_ResetCurrent((OpcUa_List *)OpcUa_Null),OpcUa_BadInvalidArgument);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_ResetCurrent with an empty list (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5023()
{
    OpcUa_List* p_list;

    CASE_ID(5023);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_GOOD(OpcUa_List_ResetCurrent(p_list));
    ASSERT(p_list->currtElement == OpcUa_Null);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_ResetCurrent on a list with several elements (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5024()
{
    OpcUa_List* p_list;
    OpcUa_Int32 a, b, c;

    CASE_ID(5024);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&c));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&b));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&a));
    ASSERT_GOOD(OpcUa_List_ResetCurrent(p_list));
    ASSERT(p_list->currtElement == p_list->firstElement);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_GetNextElement with OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5025()
{
    CASE_ID(5025);
    ASSERT(OpcUa_List_GetNextElement((OpcUa_List *)OpcUa_Null) == (OpcUa_List *)OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_GetNextElement on an empty list (GOOD CASE?) */
OpcUa_StatusCode OpcUa_ListTest_5026()
{
    OpcUa_List* p_list;

    CASE_ID(5026);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT(OpcUa_List_GetNextElement(p_list) == OpcUa_Null);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* OpcUa_List_GetNextElement on a list with two elements (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5027()
{
    OpcUa_List* p_list;
    OpcUa_Int32 a=1, b=2;

    CASE_ID(5027);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));

    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&b));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&a));

    ASSERT_GOOD(OpcUa_List_ResetCurrent(p_list));

    ASSERT(OpcUa_List_GetNextElement(p_list) == (OpcUa_Void*)&b);
    ASSERT(p_list->firstElement->nextElement == p_list->currtElement);

    ASSERT(OpcUa_List_GetNextElement(p_list) == OpcUa_Null);
    ASSERT(p_list->currtElement == OpcUa_Null);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_GetCurrentElement with OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5028()
{
    CASE_ID(5028);

    ASSERT(OpcUa_List_GetCurrentElement((OpcUa_List *)OpcUa_Null) == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_GetCurrentElement on an empty list (GOOD CASE?) */
OpcUa_StatusCode OpcUa_ListTest_5029()
{
    OpcUa_List* p_list;

    CASE_ID(5029);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_GOOD(OpcUa_List_ResetCurrent(p_list));
    ASSERT(OpcUa_List_GetCurrentElement(p_list) == OpcUa_Null);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* OpcUa_List_GetCurrentElement on an array with two elements (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5030()
{
    OpcUa_List* p_list;
    OpcUa_Int32 a, b;

    CASE_ID(5030);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&b));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&a));
    ASSERT_GOOD(OpcUa_List_ResetCurrent(p_list));
    ASSERT(OpcUa_List_GetCurrentElement(p_list) == (OpcUa_Void*)&a);
    ASSERT(OpcUa_List_GetNextElement(p_list) == (OpcUa_Void*)&b);
    ASSERT(OpcUa_List_GetCurrentElement(p_list) == (OpcUa_Void*)&b);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_GetPrevElement with OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5031()
{
    CASE_ID(5031);
    ASSERT(OpcUa_List_GetPrevElement((OpcUa_List *)OpcUa_Null) == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_GetPrevElement with an empty list (GOOD CASE?) */
OpcUa_StatusCode OpcUa_ListTest_5032()
{
    OpcUa_List* p_list;

    CASE_ID(5032);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT(OpcUa_List_GetPrevElement(p_list) == OpcUa_Null);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_GetPrevElement on a list with two elements (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5033()
{
    OpcUa_List* p_list;
    OpcUa_Int32 a, b;

    CASE_ID(5033);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&b));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&a));
    ASSERT_GOOD(OpcUa_List_ResetCurrent(p_list));
    ASSERT(OpcUa_List_GetPrevElement(p_list) == OpcUa_Null);
    ASSERT(OpcUa_List_GetNextElement(p_list) == (OpcUa_Void*)&b);
    ASSERT(OpcUa_List_GetPrevElement(p_list) == (OpcUa_Void*)&a);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_GetElement with OpcUa_Null for the list (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5034()
{
    OpcUa_Int32 i;

    CASE_ID(5034);

    ASSERT(OpcUa_List_GetElement((OpcUa_List *)OpcUa_Null,(OpcUa_Void*)&i) == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_GetElement with an empty list (GOOD CASE) */

OpcUa_StatusCode OpcUa_ListTest_5035()
{
    OpcUa_List* p_list;
    OpcUa_Int32 i;

    CASE_ID(5035);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT(OpcUa_List_GetElement(p_list,(OpcUa_Void*)&i) == OpcUa_Null);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_GetElement with OpcUa_Null for the data (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5036()
{
    OpcUa_List* p_list;

    CASE_ID(5036);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT(OpcUa_List_GetElement(p_list,OpcUa_Null) == OpcUa_Null);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_GetElement with a list of two elements (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5037()
{
    OpcUa_List* p_list;
    OpcUa_Int32 a, b, c;

    CASE_ID(5037);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&b));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&a));
    ASSERT(OpcUa_List_GetElement(p_list,(OpcUa_Void*)&a) == (OpcUa_Void*)&a);
    ASSERT(OpcUa_List_GetElement(p_list,(OpcUa_Void*)&b) == (OpcUa_Void*)&b);
    ASSERT(OpcUa_List_GetElement(p_list,(OpcUa_Void*)&c) == OpcUa_Null);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_DeleteCurrentElement with OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5038()
{
    CASE_ID(5038);

    OpcUa_List_DeleteCurrentElement((OpcUa_List *)OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_DeleteCurrentElement with an empty list (GOOD CASE?) */
OpcUa_StatusCode OpcUa_ListTest_5039()
{
    OpcUa_List* p_list;

    CASE_ID(5039);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    OpcUa_List_DeleteCurrentElement(p_list);
    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_DeleteCurrentElement on a list of three elements (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5040()
{
    OpcUa_List* p_list;
    OpcUa_Int32 a, b, c;

    CASE_ID(5040);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&c));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&b));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&a));
    ASSERT_GOOD(OpcUa_List_ResetCurrent(p_list));
    ASSERT(OpcUa_List_GetNextElement(p_list) == (OpcUa_Void*) &b);

    OpcUa_List_DeleteCurrentElement(p_list);
    ASSERT(OpcUa_List_GetCurrentElement(p_list) == (OpcUa_Void*) &c);
    ASSERT(OpcUa_List_GetPrevElement(p_list) == (OpcUa_Void*) &a);
    ASSERT(p_list->uintNbElements == 2);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_DeleteElement with OpcUa_Null for the list (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5041()
{
    OpcUa_Int32 i;

    CASE_ID(5041);

    ASSERT_STATUS(OpcUa_List_DeleteElement((OpcUa_List*)OpcUa_Null,(OpcUa_Void*)&i),OpcUa_BadInvalidArgument);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_DeleteElement with OpcUa_Null for the data (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5042()
{
    OpcUa_List* p_list;

    CASE_ID(5042);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_STATUS(OpcUa_List_DeleteElement(p_list,OpcUa_Null),OpcUa_BadInvalidArgument);
    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_DeleteElement on an empty list (GOOD CASE?) */
OpcUa_StatusCode OpcUa_ListTest_5043()
{
    OpcUa_List* p_list;
    OpcUa_Int32 i;

    CASE_ID(5043);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_STATUS(OpcUa_List_DeleteElement(p_list,(OpcUa_Void*) &i),OpcUa_BadNotFound);
    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_DeleteElement on a list with three elements (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5044()
{
    OpcUa_List* p_list;
    OpcUa_Int32 a, b, c;

    CASE_ID(5044);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&c));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&b));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&a));
    ASSERT_GOOD(OpcUa_List_DeleteElement(p_list,(OpcUa_Void*)&b));
    ASSERT_GOOD(OpcUa_List_ResetCurrent(p_list));
    ASSERT(OpcUa_List_GetCurrentElement(p_list) == (OpcUa_Void*) &a);
    ASSERT(OpcUa_List_GetNextElement(p_list) == (OpcUa_Void*) &c);
    ASSERT(p_list->uintNbElements == 2);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_RemoveFirstElement with OpcUa_Null (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5045()
{
    CASE_ID(5045);

    ASSERT(OpcUa_List_RemoveFirstElement((OpcUa_List *)OpcUa_Null) == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_RemoveFirstElement on an empty list (GOOD CASE?) */
OpcUa_StatusCode OpcUa_ListTest_5046()
{
    OpcUa_List* p_list;

    CASE_ID(5046);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT(OpcUa_List_RemoveFirstElement(p_list) == OpcUa_Null);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_RemoveFirstElement on a list with two elements (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5047()
{
    OpcUa_List* p_list;
    OpcUa_Int32 a, b;

    CASE_ID(5047);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&b));
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&a));
    ASSERT_GOOD(OpcUa_List_ResetCurrent(p_list));
    ASSERT(OpcUa_List_GetCurrentElement(p_list) == (OpcUa_Void*)&a);
    ASSERT(OpcUa_List_GetNextElement(p_list) == (OpcUa_Void*)&b);
    ASSERT(OpcUa_List_RemoveFirstElement(p_list) == (OpcUa_Void*)&a);
    ASSERT(OpcUa_List_GetPrevElement(p_list) == OpcUa_Null);
    ASSERT(p_list->uintNbElements == 1);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_AddElementToEnd with OpcUa_Null for the list (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5048()
{
    OpcUa_Int32 i;

    CASE_ID(5048);

    ASSERT_STATUS(OpcUa_List_AddElementToEnd((OpcUa_List *)OpcUa_Null,(OpcUa_Void*)&i),OpcUa_BadInvalidArgument);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_AddElementToEnd with null data (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5049()
{
    OpcUa_List* p_list;

    CASE_ID(5049);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_STATUS(OpcUa_List_AddElementToEnd(p_list,OpcUa_Null),OpcUa_BadInvalidArgument);
    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Add two elements to the end of an empty list (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5050()
{
    OpcUa_List* p_list;
    OpcUa_Int32 a, b;

    CASE_ID(5050);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_GOOD(OpcUa_List_AddElementToEnd(p_list,(OpcUa_Void*)&a));
    ASSERT_GOOD(OpcUa_List_ResetCurrent(p_list));
    ASSERT(OpcUa_List_GetCurrentElement(p_list) == (OpcUa_Void*)&a);
    ASSERT_GOOD(OpcUa_List_AddElementToEnd(p_list,(OpcUa_Void*)&b));
    ASSERT_GOOD(OpcUa_List_ResetCurrent(p_list));
    ASSERT(OpcUa_List_GetCurrentElement(p_list) == (OpcUa_Void*)&a);
    ASSERT(OpcUa_List_GetNextElement(p_list) == (OpcUa_Void*)&b);
    ASSERT(OpcUa_List_GetNextElement(p_list) == OpcUa_Null);
    ASSERT(p_list->uintNbElements == 2);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_GetNumberOfElements with OpcUa_Null for the list (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5051()
{
    OpcUa_UInt32 a;

    CASE_ID(5051);

    ASSERT_STATUS(OpcUa_List_GetNumberOfElements((OpcUa_List *)OpcUa_Null,&a),OpcUa_BadInvalidArgument);
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_List_GetNumberOfElements with OpcUa_Null for the OpcUa_UInt32 (BAD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5052()
{
    OpcUa_List* p_list;

    CASE_ID(5052);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_STATUS(OpcUa_List_GetNumberOfElements(p_list,(OpcUa_UInt32 *)OpcUa_Null),OpcUa_BadInvalidArgument);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}

/* Call Opcua_List_GetNumberOfElements on a valid list (GOOD CASE) */
OpcUa_StatusCode OpcUa_ListTest_5053()
{
    OpcUa_List* p_list;
    OpcUa_UInt32 size;
    OpcUa_Int32 a, b;

    CASE_ID(5053);

    ASSERT_GOOD(OpcUa_List_Create(&p_list));
    ASSERT_GOOD(OpcUa_List_GetNumberOfElements(p_list,&size));
    ASSERT(size == 0);
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&b));
    ASSERT_GOOD(OpcUa_List_GetNumberOfElements(p_list,&size));
    ASSERT(size == 1);
    ASSERT_GOOD(OpcUa_List_AddElement(p_list,(OpcUa_Void*)&a));
    ASSERT_GOOD(OpcUa_List_GetNumberOfElements(p_list,&size));
    ASSERT(size == 2);

    OpcUa_List_Delete(&p_list);
    ASSERT(p_list == OpcUa_Null);
    END_CASE();

    return OpcUa_Good;
}
