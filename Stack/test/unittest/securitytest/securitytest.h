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

#ifndef __SECURITY_TEST_H__
#define __SECURITY_TEST_H__

#define OPCUA_SECURITY_TEST_STORE_DIR           "../PKI/CA/"
#define OPCUA_SECURITY_TEST_CA_DIR              "../PKI/CA/certs/"
#define OPCUA_SECURITY_TEST_CA_FILE             "../PKI/CA/certs/ua_untrusted_root.der"
#define OPCUA_SECURITY_TEST_CERT_FILE           "../PKI/CA/certs/uaserver_untrustedRootCert.der"
#define OPCUA_SECURITY_TEST_CERT_FILE_SAVE      "../PKI/CA/certs/uaserver_untrustedRootCert_Copy_DELETEME.der"
#define OPCUA_SECURITY_TEST_CLR_DIR             "../PKI/CA/crl/"
#define OPCUA_SECURITY_TEST_INITIALVECTOR       "09cf4f3c09cf4f309cf4f3c09cf4f3"
#define OPCUA_SECURITY_TEST_PKCS12CERTPASSWORD  ""
#define OPCUA_SECURITY_TEST_PKCS12CERT          "../PKI/CA/certs/uasampleserver.pfx"
#define OPCUA_SECURITY_TEST_CERTIFICATEFILE     "../PKI/CA/certs/uaserver_untrustedRootCert.der"
#define OPCUA_SECURITY_TEST_PRIVATEKEYFILE      "../PKI/CA/private/uaserver_privateKey.pem"
#define OPCUA_SECURITY_TEST_RANDOMDATA1         { "asdfwertlkjhvgzt??lpnnuu7ssi2999" }
#define OPCUA_SECURITY_TEST_RANDOMDATA2         { "??lpnnuu7ssi2999asdfwertlkjhvgzt" }

/* size has to be multiple of AES_BLOCK_SIZE */
#define OPCUA_SECURITY_TEST_PLAINTEXT_LENGTH    2496
#define OPCUA_SECURITY_TEST_PLAINTEXT           "(c) Copyright 2005-2006 The OPC Foundation\nALL RIGHTS RESERVED.\nDISCLAIMER:\nThis code is provided by the OPC Foundation solely to assist in understanding and use of the appropriate OPC Specification(s) and may be used as set forth in the License Grant section of the OPC Specification. This code is provided as-is and without warranty or support of any sort and is subject to the Warranty and Liability Disclaimers which appear in the printed OPC Specification.\n(c) Copyright 2005-2006 The OPC Foundation\nALL RIGHTS RESERVED.\nDISCLAIMER:\nThis code is provided by the OPC Foundation solely to assist in understanding and use of the appropriate OPC Specification(s) and may be used as set forth in the License Grant section of the OPC Specification. This code is provided as-is and without warranty or support of any sort and is subject to the Warranty and Liability Disclaimers which appear in the printed OPC Specification.\n(c) Copyright 2005-2006 The OPC Foundation\nALL RIGHTS RESERVED.\nDISCLAIMER:\nThis code is provided by the OPC Foundation solely to assist in understanding and use of the appropriate OPC Specification(s) and may be used as set forth in the License Grant section of the OPC Specification. This code is provided as-is and without warranty or support of any sort and is subject to the Warranty and Liability Disclaimers which appear in the printed OPC Specification.\n(c) Copyright 2005-2006 The OPC Foundation\nALL RIGHTS RESERVED.\nDISCLAIMER:\nThis code is provided by the OPC Foundation solely to assist in understanding and use of the appropriate OPC Specification(s) and may be used as set forth in the License Grant section of the OPC Specification. This code is provided as-is and without warranty or support of any sort and is subject to the Warranty and Liability Disclaimers which appear in the printed OPC Specification.\n(c) Copyright 2005-2006 The OPC Foundation\nALL RIGHTS RESERVED.\nDISCLAIMER:\nThis code is provided by the OPC Foundation solely to assist in understanding and use of the appropriate OPC Specification(s) and may be used as set forth in the License Grant section of the OPC Specification. This code is provided as-is and without warranty or support of any sort and is subject to the Warranty and Liability Disclaimers which appear in the printed OPC Specification.\n(c) Copyright 2005-2006 The OPC Foundation\nALL RIGHTS RESERVED.\nDISCLAIMER:\nThis code is provided by the OPC Foundation solely to assist in understanding and use of the appropriate OPC Specification(s) and may be used as set forth in the License Grant section of the OPC Specification. This code is provided as-is and without warranty or support of any sort and is subject to the Warranty and Liability Disclaimers which appear in the printed OPC Specification.\n(c) Copyright 2005-2006 The OPC Foundation\nALL RIGHTS RESERVED.\nDISCLAIMER:\nThis code is provided by the OPC Foundation solely to assist in understanding and use of the appropriate OPC Specification(s) and may be used as set forth in the License Grant section of the OPC Specification. This code is provided as-is and without warranty or support of any sort and is subject to the Warranty and Liability Disclaimers which appear in the printed OPC Specification.\n(c) Copyright 2005-2006 The OPC Foundation\nALL RIGHTS RESERVED.\nDISCLAIMER:\nThis code is provided by the OPC Foundation solely to assist in understanding and use of the appropriate OPC Specification(s) and may be used as set forth in the License Grant section of the OPC Specification. This code is provided as-is and without warranty or support of any sort and is subject to the Warranty and Liability Disclaimers which appear in the printed OPC Specification. 12345678"

/** Test Entry Point */
void OpcUa_SecurityTest_Run();
void OpcUa_SecurityDevTest_Run();

#endif
