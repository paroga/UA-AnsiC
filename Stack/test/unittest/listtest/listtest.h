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

#ifndef __LIST_TEST_H__
#define __LIST_TEST_H__

/** Test Entry Point */
void OpcUa_ListTest_Run();

/* Tests for OpcUa_ListElement_Create */
void OpcUa_ListTest_ElementCreate();
OpcUa_StatusCode OpcUa_ListTest_5000();
OpcUa_StatusCode OpcUa_ListTest_5001();

/* Tests for OpcUa_ListElement_Initialize */
void OpcUa_ListTest_ElementInitialize();
OpcUa_StatusCode OpcUa_ListTest_5002();
OpcUa_StatusCode OpcUa_ListTest_5003();

/* Tests for OpcUa_ListElement_Delete */
void OpcUa_ListTest_ElementDelete();
OpcUa_StatusCode OpcUa_ListTest_5004();
OpcUa_StatusCode OpcUa_ListTest_5005();
OpcUa_StatusCode OpcUa_ListTest_5006();

/* Tests for OpcUa_List_Create */
void OpcUa_ListTest_Create();
OpcUa_StatusCode OpcUa_ListTest_5007();
OpcUa_StatusCode OpcUa_ListTest_5008();

/* Tests for OpcUa_List_Initialize */
void OpcUa_ListTest_Initialize();
OpcUa_StatusCode OpcUa_ListTest_5009();

/* Tests for OpcUa_List_Clear */
void OpcUa_ListTest_Clear();
OpcUa_StatusCode OpcUa_ListTest_5010();
OpcUa_StatusCode OpcUa_ListTest_5011();
OpcUa_StatusCode OpcUa_ListTest_5012();
OpcUa_StatusCode OpcUa_ListTest_5013();

/* Tests for OpcUa_List_Delete */
void OpcUa_ListTest_Delete();
OpcUa_StatusCode OpcUa_ListTest_5014();
OpcUa_StatusCode OpcUa_ListTest_5015();
OpcUa_StatusCode OpcUa_ListTest_5016();
OpcUa_StatusCode OpcUa_ListTest_5017();

/* Tests for OpcUa_List_AddElement */
void OpcUa_ListTest_AddElement();
OpcUa_StatusCode OpcUa_ListTest_5018();
OpcUa_StatusCode OpcUa_ListTest_5019();
OpcUa_StatusCode OpcUa_ListTest_5020();
OpcUa_StatusCode OpcUa_ListTest_5021();

/* Tests for OpcUa_List_ResetCurrent */
void OpcUa_ListTest_ResetCurrent();
OpcUa_StatusCode OpcUa_ListTest_5022();
OpcUa_StatusCode OpcUa_ListTest_5023();
OpcUa_StatusCode OpcUa_ListTest_5024();

/* Tests for OpcUa_List_GetNextElement */
void OpcUa_ListTest_GetNextElement();
OpcUa_StatusCode OpcUa_ListTest_5025();
OpcUa_StatusCode OpcUa_ListTest_5026();
OpcUa_StatusCode OpcUa_ListTest_5027();

/* Tests for OpcUa_List_GetCurrentElement */
void OpcUa_ListTest_GetCurrentElement();
OpcUa_StatusCode OpcUa_ListTest_5028();
OpcUa_StatusCode OpcUa_ListTest_5029();
OpcUa_StatusCode OpcUa_ListTest_5030();

/* Tests for OpcUa_List_GetPrevElement */
void OpcUa_ListTest_GetPrevElement();
OpcUa_StatusCode OpcUa_ListTest_5031();
OpcUa_StatusCode OpcUa_ListTest_5032();
OpcUa_StatusCode OpcUa_ListTest_5033();

/* Tests for OpcUa_List_GetElement */
void OpcUa_ListTest_GetElement();
OpcUa_StatusCode OpcUa_ListTest_5034();
OpcUa_StatusCode OpcUa_ListTest_5035();
OpcUa_StatusCode OpcUa_ListTest_5036();
OpcUa_StatusCode OpcUa_ListTest_5037();

/* Tests for OpcUa_List_DeleteCurrentElement */
void OpcUa_ListTest_DeleteCurrentElement();
OpcUa_StatusCode OpcUa_ListTest_5038();
OpcUa_StatusCode OpcUa_ListTest_5039();
OpcUa_StatusCode OpcUa_ListTest_5040();

/* Tests for OpcUa_List_DeleteElement */
void OpcUa_ListTest_DeleteElement();
OpcUa_StatusCode OpcUa_ListTest_5041();
OpcUa_StatusCode OpcUa_ListTest_5042();
OpcUa_StatusCode OpcUa_ListTest_5043();
OpcUa_StatusCode OpcUa_ListTest_5044();

/* Tests for OpcUa_List_RemoveFirstElement */
void OpcUa_ListTest_RemoveFirstElement();
OpcUa_StatusCode OpcUa_ListTest_5045();
OpcUa_StatusCode OpcUa_ListTest_5046();
OpcUa_StatusCode OpcUa_ListTest_5047();

/* Tests for OpcUa_List_AddElementToEnd */
void OpcUa_ListTest_AddElementToEnd();
OpcUa_StatusCode OpcUa_ListTest_5048();
OpcUa_StatusCode OpcUa_ListTest_5049();
OpcUa_StatusCode OpcUa_ListTest_5050();

/* Test for OpcUa_List_GetNumberOfElements */
void OpcUa_ListTest_GetNumberOfElements();
OpcUa_StatusCode OpcUa_ListTest_5051();
OpcUa_StatusCode OpcUa_ListTest_5052();
OpcUa_StatusCode OpcUa_ListTest_5053();

/* Test for OpcUa_List_InsertElement*/
void OpcUa_ListTest_InsertElement();
OpcUa_StatusCode OpcUa_ListTest_5054();
OpcUa_StatusCode OpcUa_ListTest_5055();
OpcUa_StatusCode OpcUa_ListTest_5056();
OpcUa_StatusCode OpcUa_ListTest_5057();
OpcUa_StatusCode OpcUa_ListTest_5058();

#endif
