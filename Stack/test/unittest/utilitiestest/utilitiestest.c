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

#include "utilitiestest.h"
#include "opcua_utilities.h"
#include "opcua_string.h"
#include "opcua_memory.h"
#include "opcua_memorystream.h"
#include "opcua_encoder.h"
#include "opcua_binaryencoder.h"
#include "opcua_messagecontext.h"

int int_compare(const void* a, const void* b)
{
    const OpcUa_Int32* p_a = (OpcUa_Int32*)a;
    const OpcUa_Int32* p_b = (OpcUa_Int32*)b;

    if(*p_a < *p_b)
    {
        return -1;
    }

    if(*p_a == *p_b)
    {
        return 0;
    }

    return 1;
}

void OpcUa_UtilitiesTest_Run()
{
    OpcUa_UtilitiesTest_QSort();
    OpcUa_UtilitiesTest_BSearch();
#if 0
    OpcUa_UtilitiesTest_CopyCompare();
    OpcUa_UtilitiesTest_EncodeDecode();
#endif
}

/* Tests for OpcUa_QSort */
void OpcUa_UtilitiesTest_QSort()
{
    OpcUa_UtilitiesTest_13000();
    OpcUa_UtilitiesTest_13001();
    OpcUa_UtilitiesTest_13002();
    OpcUa_UtilitiesTest_13003();
    OpcUa_UtilitiesTest_13004();
    OpcUa_UtilitiesTest_13005();
    OpcUa_UtilitiesTest_13006();
}

/* Tests for OpcUa_BSearch */
void OpcUa_UtilitiesTest_BSearch()
{
    OpcUa_UtilitiesTest_13007();
    OpcUa_UtilitiesTest_13008();
    OpcUa_UtilitiesTest_13009();
    OpcUa_UtilitiesTest_13010();
    OpcUa_UtilitiesTest_13011();
    OpcUa_UtilitiesTest_13012();
    OpcUa_UtilitiesTest_13013();
    OpcUa_UtilitiesTest_13014();
    OpcUa_UtilitiesTest_13015();
    OpcUa_UtilitiesTest_13016();
}

#if 0
/* Tests for XxxType_CopyTo/Compare */
void OpcUa_UtilitiesTest_CopyCompare()
{
    OpcUa_UtilitiesTest_13020();
    OpcUa_UtilitiesTest_13021();
}

/* Tests for Binary Encoder and Decoder */
void OpcUa_UtilitiesTest_EncodeDecode()
{
    OpcUa_UtilitiesTest_13030();
    OpcUa_UtilitiesTest_13031();
}
#endif

/* Call OpcUa_QSort with OpcUa_Null for the array of elements (BAD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13000()
{
    CASE_ID(13000);

    OpcUa_QSort(OpcUa_Null,5,sizeof(OpcUa_Int32),int_compare,OpcUa_Null);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_QSort with a single OpcUa_Int32 (GOOD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13001()
{
    OpcUa_Int32 a = 1;

    CASE_ID(13001);

    OpcUa_QSort(&a,1,sizeof(OpcUa_Int32),int_compare,OpcUa_Null);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_QSort with an unsorted list (GOOD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13002()
{
    const OpcUa_Int32 unsorted[10] = {5,14,2,4,8,1,12,11,3,10};
    const OpcUa_Int32 sorted[10]   = {1,2,3,4,5,8,10,11,12,14};
    OpcUa_Int32 i_array[10];
    OpcUa_Int32 i;

    CASE_ID(13002);

    for(i=0;i<10;i++)
    {
        i_array[i] = unsorted[i];
    }

    OpcUa_QSort((OpcUa_Void*)i_array,
                10,
                sizeof(OpcUa_Int32),
                int_compare,
                OpcUa_Null);

    for(i=0;i<10;i++)
    {
        ASSERT(i_array[i] == sorted[i]);
    }
    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_QSort with a sorted list (GOOD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13003()
{
    OpcUa_Int32 i_array[10];
    OpcUa_Int32 i;

    CASE_ID(13003);

    for(i=0;i<10;i++)
        i_array[i] = i;

    OpcUa_QSort((OpcUa_Void*) i_array,10,sizeof(OpcUa_Int32),int_compare,OpcUa_Null);

    for(i=0;i<10;i++)
        ASSERT(i_array[i] == i);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_QSort with a greatest-first sorted array (GOOD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13004()
{
    OpcUa_Int32 i_array[10];
    OpcUa_Int32 i;

    CASE_ID(13004);

    for(i=1; i<=10; i++)
    {
        i_array[i-1] = 11-i;
    }

    OpcUa_QSort((OpcUa_Void*)i_array,
                10,
                sizeof(OpcUa_Int32),
                int_compare,
                OpcUa_Null);

    for(i=1; i<=10; i++)
    {
        ASSERT(i_array[i-1] == i);
    }

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_QSort with an array of all the same element (GOOD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13005()
{
    OpcUa_Int32 i_array[10];
    OpcUa_Int32 i;

    CASE_ID(13005);

    for(i=0;i<10;i++)
    {
        i_array[i] = 2;
    }

    OpcUa_QSort((OpcUa_Void*)i_array,
                10,
                sizeof(OpcUa_Int32),
                int_compare,
                OpcUa_Null);

    for(i=0;i<10;i++)
    {
        ASSERT(i_array[i] == 2);
    }

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_QSort with many of the same element (GOOD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13006()
{
    const OpcUa_Int32 unsorted[10] = {5,2,2,4,8,2,12,2,2,10};
    const OpcUa_Int32 sorted[10] = {2,2,2,2,2,4,5,8,10,12};
    OpcUa_Int32 i_array[10];
    OpcUa_Int32 i;

    CASE_ID(13006);

    for(i=0;i<10;i++)
    {
        i_array[i] = unsorted[i];
    }

    OpcUa_QSort((OpcUa_Void*)i_array,
                10,
                sizeof(OpcUa_Int32),
                int_compare,
                OpcUa_Null);

    for(i=0;i<10;i++)
    {
        ASSERT(i_array[i] == sorted[i]);
    }

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_BSearch with OpcUa_Null for the array (BAD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13007()
{
    OpcUa_Int32 a;

    CASE_ID(13007);

    ASSERT(OpcUa_BSearch((OpcUa_Void*) &a,OpcUa_Null,5,sizeof(OpcUa_Int32),int_compare,OpcUa_Null) == OpcUa_Null);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_BSearch with OpcUa_Null for the key (BAD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13008()
{
    OpcUa_Int32 sorted[10] = {5,8,9,11,12,15,16,17,20,21};

    CASE_ID(13008);

    ASSERT(OpcUa_BSearch(OpcUa_Null,(OpcUa_Void*) sorted,10,sizeof(OpcUa_Int32),int_compare,OpcUa_Null) == OpcUa_Null);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_BSearch with an array of one element (GOOD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13009()
{
    OpcUa_Int32 sorted[1] = {1};
    OpcUa_Int32 key = 1;

    CASE_ID(13009);

    ASSERT(OpcUa_BSearch((OpcUa_Void*) &key,(OpcUa_Void*)sorted,1,sizeof(OpcUa_Int32),int_compare,OpcUa_Null) == (OpcUa_Void*) sorted);

    END_CASE();

    return OpcUa_Good;
}

/* Call OpcUa_BSearch with an array of one element (GOOD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13010()
{
    OpcUa_Int32 sorted[1] = {1};
    OpcUa_Int32 key = 2;

    CASE_ID(13010);

    ASSERT(OpcUa_BSearch((OpcUa_Void*)&key,(OpcUa_Void*)sorted,1,sizeof(OpcUa_Int32),int_compare,OpcUa_Null) == OpcUa_Null);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_BSearch to search for the first element in an array of 12 OpcUa_Int32 (GOOD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13011()
{
    OpcUa_Int32 sorted[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
    OpcUa_Int32 key = 1;

    CASE_ID(13011);

    ASSERT(OpcUa_BSearch((OpcUa_Void*)&key,(OpcUa_Void*)&sorted,12,sizeof(OpcUa_Int32),int_compare,OpcUa_Null) == (OpcUa_Void*) &sorted[0]);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_BSearch to search for the last element in an array of 12 OpcUa_Int32 (GOOD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13012()
{
    OpcUa_Int32 sorted[12]  = {1,2,3,4,5,6,7,8,9,10,11,12};
    OpcUa_Int32 key         = 12;
    OpcUa_Void* retval      = OpcUa_Null;

    CASE_ID(13012);

    retval = OpcUa_BSearch( (OpcUa_Void*)&key,
                            (OpcUa_Void*)&sorted,
                            12,
                            sizeof(OpcUa_Int32),
                            int_compare,
                            OpcUa_Null);

    ASSERT(retval == (OpcUa_Void*)(&sorted[11]));

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_BSearch to search for an element in the first quarter of an array (GOOD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13013()
{
    OpcUa_Int32 sorted[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
    OpcUa_Int32 key = 2;

    CASE_ID(13013);

    ASSERT(OpcUa_BSearch((OpcUa_Void*)&key,(OpcUa_Void*)&sorted,12,sizeof(OpcUa_Int32),int_compare,OpcUa_Null) == (OpcUa_Void*) &sorted[1]);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_BSearch to search for an element in the second quarter of an array (GOOD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13014()
{
    OpcUa_Int32 sorted[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
    OpcUa_Int32 key = 5;

    CASE_ID(13014);

    ASSERT(OpcUa_BSearch((OpcUa_Void*)&key,(OpcUa_Void*)&sorted,12,sizeof(OpcUa_Int32),int_compare,OpcUa_Null) == (OpcUa_Void*) &sorted[4]);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_BSearch to search for an element in the third quarter of an array (GOOD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13015()
{
    OpcUa_Int32 sorted[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
    OpcUa_Int32 key = 8;

    CASE_ID(13015);

    ASSERT(OpcUa_BSearch((OpcUa_Void*)&key,(OpcUa_Void*)&sorted,12,sizeof(OpcUa_Int32),int_compare,OpcUa_Null) == (OpcUa_Void*) &sorted[7]);

    END_CASE();

    return OpcUa_Good;
}

/* Use OpcUa_BSearch to search for an element in the fourth quarter of an array (GOOD CASE_ID) */
OpcUa_StatusCode OpcUa_UtilitiesTest_13016()
{
    OpcUa_Int32 sorted[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
    OpcUa_Int32 key = 11;

    CASE_ID(13016);

    ASSERT(OpcUa_BSearch((OpcUa_Void*)&key,(OpcUa_Void*)&sorted,12,sizeof(OpcUa_Int32),int_compare,OpcUa_Null) == (OpcUa_Void*) &sorted[10]);

    END_CASE();

    return OpcUa_Good;
}

#if 0
/* Copy Encodeable Objects, and verify using OpcUa_ExtensionObject_Compare */
OpcUa_StatusCode OpcUa_UtilitiesTest_13020()
{
    OpcUa_ExtensionObject a,b,c;
    OpcUa_Node *aobj;

    CASE_ID(13020);

    OpcUa_ExtensionObject_Initialize(&a);
    OpcUa_ExtensionObject_Initialize(&b);
    OpcUa_ExtensionObject_Initialize(&c);
    a.Encoding=OpcUa_ExtensionObjectEncoding_EncodeableObject;
    a.TypeId.NodeId.IdentifierType=OpcUa_IdentifierType_Numeric;
    a.TypeId.NodeId.Identifier.Numeric=OpcUaId_Node_Encoding_DefaultBinary;
    a.Body.EncodeableObject.Type=&OpcUa_Node_EncodeableType;
    a.Body.EncodeableObject.Object=OpcUa_Alloc(sizeof(OpcUa_Node));
    OpcUa_Node_Initialize(a.Body.EncodeableObject.Object);
    aobj = (OpcUa_Node*)a.Body.EncodeableObject.Object;
    aobj->NodeId.IdentifierType=OpcUa_IdentifierType_String;
    ASSERT_GOOD(OpcUa_String_Copy(&aobj->NodeId.Identifier.String, OpcUa_String_FromCString("test_node0")));
    ASSERT_GOOD(OpcUa_ExtensionObject_CopyTo(&a, &b));
    ASSERT(a.Body.EncodeableObject.Object != b.Body.EncodeableObject.Object);
    ASSERT(OpcUa_NodeId_Compare(&aobj->NodeId, (OpcUa_NodeId*)b.Body.EncodeableObject.Object) == OPCUA_EQUAL);
    ASSERT(OpcUa_ExtensionObject_Compare(&a, &b) == OPCUA_EQUAL);
    ASSERT(OpcUa_ExtensionObject_Compare(&a, &c) != OPCUA_EQUAL);
    aobj->NoOfReferences=1;
    aobj->References = (OpcUa_ReferenceNode*)OpcUa_Alloc(sizeof(OpcUa_ReferenceNode));
    OpcUa_ReferenceNode_Initialize(aobj->References);
    ASSERT_GOOD(OpcUa_ExtensionObject_CopyTo(&a, &c));
    ASSERT(OpcUa_ExtensionObject_Compare(&a, &b) != OPCUA_EQUAL);
    ASSERT(OpcUa_ExtensionObject_Compare(&a, &c) == OPCUA_EQUAL);
    ASSERT_GOOD(OpcUa_ExtensionObject_CopyTo(&c, &b));
    ASSERT(OpcUa_ExtensionObject_Compare(&a, &b) == OPCUA_EQUAL);
    ASSERT(!strcmp(OpcUa_String_GetRawString(&((OpcUa_Node*)b.Body.EncodeableObject.Object)->NodeId.Identifier.String), "test_node0"));
    ASSERT_GOOD(OpcUa_String_Copy(&aobj->NodeId.Identifier.String, OpcUa_String_FromCString("test_node1")));
    ASSERT(OpcUa_ExtensionObject_Compare(&a, &b) != OPCUA_EQUAL);
    ASSERT_GOOD(OpcUa_ExtensionObject_CopyTo(&a, &b));
    ASSERT(!strcmp(OpcUa_String_GetRawString(&((OpcUa_Node*)b.Body.EncodeableObject.Object)->NodeId.Identifier.String), "test_node1"));
    ASSERT(OpcUa_ExtensionObject_Compare(&a, &b) == OPCUA_EQUAL);
    ASSERT(OpcUa_ExtensionObject_Compare(&b, &c) != OPCUA_EQUAL);
    ASSERT_GOOD(OpcUa_ExtensionObject_CopyTo(&b, &c));
    ASSERT(!strcmp(OpcUa_String_GetRawString(&((OpcUa_Node*)c.Body.EncodeableObject.Object)->NodeId.Identifier.String), "test_node1"));
    ASSERT(OpcUa_ExtensionObject_Compare(&c, &a) == OPCUA_EQUAL);
    OpcUa_ExtensionObject_Clear(&a);
    OpcUa_ExtensionObject_Clear(&b);
    OpcUa_ExtensionObject_Clear(&c);

    END_CASE();

    return OpcUa_Good;
}

/* Copy Variant Objects with embedded Extension Objects */
OpcUa_StatusCode OpcUa_UtilitiesTest_13021()
{
    OpcUa_Variant a,b,c;
    OpcUa_MonitoringParameters *aobj1;
    OpcUa_DataChangeFilter* aobj2;

    CASE_ID(13021);

    OpcUa_Variant_Initialize(&a);
    OpcUa_Variant_Initialize(&b);
    OpcUa_Variant_Initialize(&c);
    a.Datatype = OpcUaType_ExtensionObject;
    a.ArrayType = OpcUa_VariantArrayType_Scalar;
    OpcUa_ExtensionObject_Create(&a.Value.ExtensionObject);
    OpcUa_EncodeableObject_CreateExtension(&OpcUa_MonitoringParameters_EncodeableType, a.Value.ExtensionObject, (void**)&aobj1);
    aobj1->ClientHandle = 42;
    aobj1->SamplingInterval = 42.42;
    aobj1->QueueSize = 99;
    aobj1->DiscardOldest = OpcUa_True;
    ASSERT_GOOD(OpcUa_Variant_CopyTo(&a, &b));
    ASSERT(a.Value.ExtensionObject != b.Value.ExtensionObject);
    ASSERT(a.Value.ExtensionObject->Body.EncodeableObject.Object != b.Value.ExtensionObject->Body.EncodeableObject.Object);
    ASSERT(a.Value.ExtensionObject->Body.EncodeableObject.Type == b.Value.ExtensionObject->Body.EncodeableObject.Type);
    ASSERT(OpcUa_Variant_Compare(&a, &b) == OPCUA_EQUAL);
    ASSERT(OpcUa_Variant_Compare(&a, &c) != OPCUA_EQUAL);
    OpcUa_EncodeableObject_CreateExtension(&OpcUa_DataChangeFilter_EncodeableType, &aobj1->Filter, (void**)&aobj2);
    aobj2->Trigger = OpcUa_DataChangeTrigger_StatusValueTimestamp;
    aobj2->DeadbandType = OpcUa_DeadbandType_Percent;
    aobj2->DeadbandValue = 1.1;
    a.Value.ExtensionObject->TypeId.NodeId.IdentifierType = OpcUa_IdentifierType_String;
    ASSERT_GOOD(OpcUa_String_Copy(&a.Value.ExtensionObject->TypeId.NodeId.Identifier.String, OpcUa_String_FromCString("Not Used")));
    a.Value.ExtensionObject->BodySize=12345;
    /*
    aobj1->Filter.TypeId.NodeId.IdentifierType = OpcUa_IdentifierType_String;
    ASSERT_GOOD(OpcUa_String_Copy(&aobj1->Filter.TypeId.NodeId.Identifier.String, OpcUa_String_FromCString("Test")));
    aobj1->Filter.BodySize=12345;
    */
    ASSERT_GOOD(OpcUa_Variant_CopyTo(&a, &c));

    ASSERT(OpcUa_Variant_Compare(&a, &b) != OPCUA_EQUAL);
    ASSERT(OpcUa_Variant_Compare(&a, &c) == OPCUA_EQUAL);
    ASSERT_GOOD(OpcUa_Variant_CopyTo(&c, &b));
    ASSERT(OpcUa_Variant_Compare(&a, &b) == OPCUA_EQUAL);
    ASSERT(!strcmp(OpcUa_String_GetRawString(&b.Value.ExtensionObject->TypeId.NodeId.Identifier.String), "Not Used"));
    ASSERT(b.Value.ExtensionObject->BodySize==12345);
    aobj1->DiscardOldest = OpcUa_False;
    ASSERT(OpcUa_Variant_Compare(&a, &b) != OPCUA_EQUAL);
    ASSERT_GOOD(OpcUa_Variant_CopyTo(&a, &b));
    ASSERT(OpcUa_Variant_Compare(&a, &b) == OPCUA_EQUAL);
    ASSERT(OpcUa_Variant_Compare(&b, &c) != OPCUA_EQUAL);
    ASSERT_GOOD(OpcUa_Variant_CopyTo(&b, &c));
    ASSERT(OpcUa_Variant_Compare(&c, &a) == OPCUA_EQUAL);
    OpcUa_Variant_Clear(&a);
    OpcUa_Variant_Clear(&b);
    OpcUa_Variant_Clear(&c);

    END_CASE();

    return OpcUa_Good;
}

/* Test recursion limit of variant objects */
OpcUa_StatusCode OpcUa_UtilitiesTest_13030()
{
    OpcUa_Variant u,v,*p;
    int i;
    OpcUa_StatusCode      uStatus = OpcUa_Good;
    OpcUa_OutputStream*   pOutputStream;
    OpcUa_InputStream*    pInputStream;
    OpcUa_Byte*           pBuffer;
    OpcUa_UInt32          uBufferSize;
    OpcUa_UInt32          uInputSize;
    OpcUa_MessageContext  cContext;
    OpcUa_Encoder*        pEncoder;
    OpcUa_Decoder*        pDecoder;
    OpcUa_Handle          hEncoderContext;
    CASE_ID(13030);
    OpcUa_Variant_Initialize(&u);
    OpcUa_Variant_Initialize(&v);
    p = &u;
    for (i = 0; i<110; i++)
    {
        p->Datatype = OpcUaType_DataValue;
        p->ArrayType = OpcUa_VariantArrayType_Scalar;
        p->Value.DataValue = (OpcUa_DataValue*)OpcUa_Alloc(sizeof(OpcUa_DataValue));
        OpcUa_DataValue_Initialize(p->Value.DataValue);
        p->Value.DataValue->StatusCode = OpcUa_Good;
        p = &p->Value.DataValue->Value;
        ASSERT_GOOD(OpcUa_MemoryStream_CreateWriteable(65536, 0, &pOutputStream));
        OpcUa_MessageContext_Initialize(&cContext);
        ASSERT_GOOD(OpcUa_BinaryEncoder_Create(&pEncoder));
        ASSERT_GOOD(pEncoder->Open(pEncoder, pOutputStream, &cContext, &hEncoderContext));
        ASSERT_GOOD(pEncoder->WriteVariant(hEncoderContext, "test", &u, OpcUa_Null));
        OpcUa_Encoder_Close(pEncoder, &hEncoderContext);
        OpcUa_Encoder_Delete(&pEncoder);
        ASSERT_GOOD(pOutputStream->Close((OpcUa_Stream*)pOutputStream));
        ASSERT_GOOD(pOutputStream->GetPosition((OpcUa_Stream*)pOutputStream, &uInputSize));
        ASSERT_GOOD(OpcUa_MemoryStream_DetachBuffer(pOutputStream, &pBuffer, &uBufferSize));
        pOutputStream->Delete((OpcUa_Stream**)&pOutputStream);
        ASSERT_GOOD(OpcUa_MemoryStream_CreateReadable(pBuffer, uInputSize, &pInputStream));
        ASSERT_GOOD(OpcUa_BinaryDecoder_Create(&pDecoder));
        ASSERT_GOOD(pDecoder->Open(pDecoder, pInputStream, &cContext, &hEncoderContext));
        OpcUa_Variant_Clear(&v);
        uStatus = pDecoder->ReadVariant(hEncoderContext, "test", &v);
        OpcUa_Decoder_Close(pDecoder, &hEncoderContext);
        OpcUa_Decoder_Delete(&pDecoder);
        OpcUa_MessageContext_Clear(&cContext);
        pInputStream->Close((OpcUa_Stream*)pInputStream);
        pInputStream->Delete((OpcUa_Stream**)&pInputStream);
        ASSERT((i<=100?OpcUa_Good:OpcUa_BadEncodingLimitsExceeded)==uStatus);
    }
    OpcUa_Variant_Clear(&u);
    OpcUa_Variant_Clear(&v);
    END_CASE();
    return OpcUa_Good;
}

/* Test recursion limit of encodeable objects */
OpcUa_StatusCode OpcUa_UtilitiesTest_13031()
{
    OpcUa_RequestHeader   u,v,*p,*q;
    int i;
    OpcUa_StatusCode      uStatus = OpcUa_Good;
    OpcUa_OutputStream*   pOutputStream;
    OpcUa_InputStream*    pInputStream;
    OpcUa_Byte*           pBuffer;
    OpcUa_UInt32          uBufferSize;
    OpcUa_UInt32          uInputSize;
    CASE_ID(13031);
    OpcUa_RequestHeader_Initialize(&u);
    OpcUa_RequestHeader_Initialize(&v);
    p = &u;
    for (i = 0; i<110; i++)
    {
        OpcUa_EncodeableObject_CreateExtension(&OpcUa_RequestHeader_EncodeableType, &p->AdditionalHeader, (void**)&q);
        p->AdditionalHeader.TypeId.NodeId.IdentifierType = OpcUa_IdentifierType_Numeric;
        p->AdditionalHeader.TypeId.NodeId.NamespaceIndex = 0;
        p->AdditionalHeader.TypeId.NodeId.Identifier.Numeric = OpcUaId_RequestHeader_Encoding_DefaultBinary;
        p = q;
        ASSERT_GOOD(OpcUa_MemoryStream_CreateWriteable(65536, 0, &pOutputStream));
        ASSERT_GOOD(OpcUa_EncodeableObject_Encode(&OpcUa_RequestHeader_EncodeableType, &u, OpcUa_Null, pOutputStream));
        ASSERT_GOOD(pOutputStream->Close((OpcUa_Stream*)pOutputStream));
        ASSERT_GOOD(pOutputStream->GetPosition((OpcUa_Stream*)pOutputStream, &uInputSize));
        ASSERT_GOOD(OpcUa_MemoryStream_DetachBuffer(pOutputStream, &pBuffer, &uBufferSize));
        pOutputStream->Delete((OpcUa_Stream**)&pOutputStream);
        ASSERT_GOOD(OpcUa_MemoryStream_CreateReadable(pBuffer, uInputSize, &pInputStream));
        OpcUa_RequestHeader_Clear(&v);
        uStatus = OpcUa_EncodeableObject_Decode(&OpcUa_RequestHeader_EncodeableType, pInputStream, OpcUa_Null, &v);
        pInputStream->Close((OpcUa_Stream*)pInputStream);
        pInputStream->Delete((OpcUa_Stream**)&pInputStream);
        ASSERT((i<=100?OpcUa_Good:OpcUa_BadEncodingLimitsExceeded)==uStatus);
    }
    OpcUa_RequestHeader_Clear(&u);
    OpcUa_RequestHeader_Clear(&v);
    END_CASE();
    return OpcUa_Good;
}
#endif
