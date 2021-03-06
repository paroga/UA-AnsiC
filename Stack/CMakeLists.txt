# Copyright (c) 1996-2016, OPC Foundation. All rights reserved.
#
# The source code in this file is covered under a dual-license scenario:
#   - RCL: for OPC Foundation members in good-standing
#   - GPL V2: everybody else
#
# RCL license terms accompanied with this source code. See http://opcfoundation.org/License/RCL/1.00/
#
# GNU General Public License as published by the Free Software Foundation;
# version 2 of the License are accompanied with this source code. See http://opcfoundation.org/License/GPLv2
#
# This source code is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

set(UASTACK_INCLUDE_DIRECTORIES
    core
    proxystub/clientproxy
    proxystub/serverstub
    securechannel
    stackcore
    transport/https
    transport/tcp
)

set(UASTACK_PUBLIC_HEADERS
    core/opcua_buffer.h
    core/opcua_config.h
    core/opcua_core.h
    core/opcua_cryptofactory.h
    core/opcua_datetime.h
    core/opcua_errorhandling.h
    core/opcua_exclusions.h
    core/opcua_guid.h
    core/opcua_list.h
    core/opcua_memory.h
    core/opcua_memorystream.h
    core/opcua_mutex.h
    core/opcua_pkifactory.h
    core/opcua_proxystub.h
    core/opcua_semaphore.h
    core/opcua_socket.h
    core/opcua_stackstatuscodes.h
    core/opcua_statuscodes.h
    core/opcua_string.h
    core/opcua_thread.h
    core/opcua_threadpool.h
    core/opcua_timer.h
    core/opcua_trace.h
    core/opcua_utilities.h

    proxystub/clientproxy/opcua_asynccallstate.h
    proxystub/clientproxy/opcua_channel.h
    proxystub/clientproxy/opcua_channel_internal.h
    proxystub/clientproxy/opcua_clientapi.h
    proxystub/clientproxy/opcua_clientproxy.h

    proxystub/serverstub/opcua_endpoint.h
    proxystub/serverstub/opcua_endpoint_internal.h
    proxystub/serverstub/opcua_serverapi.h
    proxystub/serverstub/opcua_serverstub.h
    proxystub/serverstub/opcua_servicetable.h

    securechannel/opcua_securechannel_types.h
    securechannel/opcua_secureconnection.h
    securechannel/opcua_securelistener.h
    securechannel/opcua_securelistener_channelmanager.h
    securechannel/opcua_securelistener_policymanager.h
    securechannel/opcua_securestream.h
    securechannel/opcua_soapsecurechannel.h
    securechannel/opcua_tcpsecurechannel.h

    stackcore/opcua_attributes.h
    stackcore/opcua_binaryencoder.h
    stackcore/opcua_binaryencoderinternal.h
    stackcore/opcua_browsenames.h
    stackcore/opcua_builtintypes.h
    stackcore/opcua_connection.h
    stackcore/opcua_credentials.h
    stackcore/opcua_crypto.h
    stackcore/opcua_decoder.h
    stackcore/opcua_encodeableobject.h
    stackcore/opcua_encoder.h
    stackcore/opcua_enumeratedtype.h
    stackcore/opcua_extensionobject.h
    stackcore/opcua_identifiers.h
    stackcore/opcua_listener.h
    stackcore/opcua_messagecontext.h
    stackcore/opcua_pki.h
    stackcore/opcua_securechannel.h
    stackcore/opcua_stream.h
    stackcore/opcua_stringtable.h
    stackcore/opcua_types.h

    transport/https/opcua_https_internal.h
    transport/https/opcua_httpsconnection.h
    transport/https/opcua_httpslistener.h
    transport/https/opcua_httpslistener_connectionmanager.h
    transport/https/opcua_httpsstream.h

    transport/tcp/opcua_tcpconnection.h
    transport/tcp/opcua_tcplistener.h
    transport/tcp/opcua_tcplistener_connectionmanager.h
    transport/tcp/opcua_tcpstream.h
)
set(UASTACK_PRIVATE_HEADERS
    core/opcua.h
)
set(UASTACK_SOURCES
    core/opcua_buffer.c
    core/opcua_core.c
    core/opcua_datetime.c
    core/opcua_guid.c
    core/opcua_list.c
    core/opcua_memory.c
    core/opcua_memorystream.c
    core/opcua_proxystub.c
    core/opcua_string.c
    core/opcua_thread.c
    core/opcua_threadpool.c
    core/opcua_timer.c
    core/opcua_trace.c
    core/opcua_utilities.c

    proxystub/clientproxy/opcua_asynccallstate.c
    proxystub/clientproxy/opcua_channel.c
    proxystub/clientproxy/opcua_clientapi.c

    proxystub/serverstub/opcua_endpoint.c
    proxystub/serverstub/opcua_serverapi.c
    proxystub/serverstub/opcua_servicetable.c

    securechannel/opcua_secureconnection.c
    securechannel/opcua_securelistener.c
    securechannel/opcua_securelistener_channelmanager.c
    securechannel/opcua_securelistener_policymanager.c
    securechannel/opcua_securestream.c
    securechannel/opcua_soapsecurechannel.c
    securechannel/opcua_tcpsecurechannel.c

    stackcore/opcua_binarydecoder.c
    stackcore/opcua_binaryencoder.c
    stackcore/opcua_builtintypes.c
    stackcore/opcua_connection.c
    stackcore/opcua_crypto.c
    stackcore/opcua_decoder.c
    stackcore/opcua_encodeableobject.c
    stackcore/opcua_encoder.c
    stackcore/opcua_enumeratedtype.c
    stackcore/opcua_extensionobject.c
    stackcore/opcua_listener.c
    stackcore/opcua_messagecontext.c
    stackcore/opcua_pki.c
    stackcore/opcua_securechannel.c
    stackcore/opcua_stream.c
    stackcore/opcua_stringtable.c
    stackcore/opcua_types.c

    transport/https/opcua_https_internal.c
    transport/https/opcua_httpsconnection.c
    transport/https/opcua_httpslistener.c
    transport/https/opcua_httpslistener_connectionmanager.c
    transport/https/opcua_httpsstream.c

    transport/tcp/opcua_tcpconnection.c
    transport/tcp/opcua_tcplistener.c
    transport/tcp/opcua_tcplistener_connectionmanager.c
    transport/tcp/opcua_tcpstream.c
)


if (WIN32)
    list(APPEND UASTACK_INCLUDE_DIRECTORIES
        platforms/win32
    )
    list(APPEND UASTACK_PUBLIC_HEADERS
        platforms/win32/opcua_p_binary.h
        platforms/win32/opcua_p_crypto.h
        platforms/win32/opcua_p_cryptofactory.h
        platforms/win32/opcua_p_datetime.h
        platforms/win32/opcua_p_guid.h
        platforms/win32/opcua_p_interface.h
        platforms/win32/opcua_p_internal.h
        platforms/win32/opcua_p_memory.h
        platforms/win32/opcua_p_mutex.h
        platforms/win32/opcua_p_openssl.h
        platforms/win32/opcua_p_openssl_pki.h
        platforms/win32/opcua_p_pki.h
        platforms/win32/opcua_p_pkifactory.h
        platforms/win32/opcua_p_semaphore.h
        platforms/win32/opcua_p_socket.h
        platforms/win32/opcua_p_socket_interface.h
        platforms/win32/opcua_p_socket_internal.h
        platforms/win32/opcua_p_socket_ssl.h
        platforms/win32/opcua_p_string.h
        platforms/win32/opcua_p_thread.h
        platforms/win32/opcua_p_timer.h
        platforms/win32/opcua_p_trace.h
        platforms/win32/opcua_p_types.h
        platforms/win32/opcua_p_utilities.h
        platforms/win32/opcua_p_win32_pki.h
        platforms/win32/opcua_platformdefs.h
    )
    list(APPEND UASTACK_SOURCES
        platforms/win32/opcua_p_binary.c
        platforms/win32/opcua_p_cryptofactory.c
        platforms/win32/opcua_p_datetime.c
        platforms/win32/opcua_p_guid.c
        platforms/win32/opcua_p_interface.c
        platforms/win32/opcua_p_internal.c
        platforms/win32/opcua_p_memory.c
        platforms/win32/opcua_p_mutex.c
        platforms/win32/opcua_p_openssl.c
        platforms/win32/opcua_p_openssl_3des.c
        platforms/win32/opcua_p_openssl_aes.c
        platforms/win32/opcua_p_openssl_hmac_sha.c
        platforms/win32/opcua_p_openssl_nosecurity.c
        platforms/win32/opcua_p_openssl_pki.c
        platforms/win32/opcua_p_openssl_pki_nosecurity.c
        platforms/win32/opcua_p_openssl_random.c
        platforms/win32/opcua_p_openssl_rsa.c
        platforms/win32/opcua_p_openssl_sha.c
        platforms/win32/opcua_p_openssl_x509.c
        platforms/win32/opcua_p_pkifactory.c
        platforms/win32/opcua_p_semaphore.c
        platforms/win32/opcua_p_socket.c
        platforms/win32/opcua_p_socket_interface.c
        platforms/win32/opcua_p_socket_internal.c
        platforms/win32/opcua_p_socket_ssl.c
        platforms/win32/opcua_p_string.c
        platforms/win32/opcua_p_thread.c
        platforms/win32/opcua_p_timer.c
        platforms/win32/opcua_p_trace.c
        platforms/win32/opcua_p_utilities.c
        platforms/win32/opcua_p_win32_pki.c
    )
    list(APPEND UASTACK_LIBRARIES
        crypt32
        rpcrt4
        ws2_32
    )
else ()
    list(APPEND UASTACK_INCLUDE_DIRECTORIES
        platforms/linux
    )
    list(APPEND UASTACK_PUBLIC_HEADERS
        platforms/linux/opcua_p_binary.h
        platforms/linux/opcua_p_crypto.h
        platforms/linux/opcua_p_cryptofactory.h
        platforms/linux/opcua_p_datetime.h
        platforms/linux/opcua_p_guid.h
        platforms/linux/opcua_p_interface.h
        platforms/linux/opcua_p_internal.h
        platforms/linux/opcua_p_memory.h
        platforms/linux/opcua_p_mutex.h
        platforms/linux/opcua_p_openssl.h
        platforms/linux/opcua_p_openssl_pki.h
        platforms/linux/opcua_p_pki.h
        platforms/linux/opcua_p_pkifactory.h
        platforms/linux/opcua_p_semaphore.h
        platforms/linux/opcua_p_socket.h
        platforms/linux/opcua_p_socket_interface.h
        platforms/linux/opcua_p_socket_internal.h
        platforms/linux/opcua_p_socket_ssl.h
        platforms/linux/opcua_p_string.h
        platforms/linux/opcua_p_thread.h
        platforms/linux/opcua_p_timer.h
        platforms/linux/opcua_p_trace.h
        platforms/linux/opcua_p_types.h
        platforms/linux/opcua_p_utilities.h
        platforms/linux/opcua_platformdefs.h
    )
    list(APPEND UASTACK_SOURCES
        platforms/linux/opcua_p_binary.c
        platforms/linux/opcua_p_cryptofactory.c
        platforms/linux/opcua_p_datetime.c
        platforms/linux/opcua_p_guid.c
        platforms/linux/opcua_p_interface.c
        platforms/linux/opcua_p_internal.c
        platforms/linux/opcua_p_memory.c
        platforms/linux/opcua_p_mutex.c
        platforms/linux/opcua_p_openssl.c
        platforms/linux/opcua_p_openssl_3des.c
        platforms/linux/opcua_p_openssl_aes.c
        platforms/linux/opcua_p_openssl_hmac_sha.c
        platforms/linux/opcua_p_openssl_nosecurity.c
        platforms/linux/opcua_p_openssl_pki.c
        platforms/linux/opcua_p_openssl_pki_nosecurity.c
        platforms/linux/opcua_p_openssl_random.c
        platforms/linux/opcua_p_openssl_rsa.c
        platforms/linux/opcua_p_openssl_sha.c
        platforms/linux/opcua_p_openssl_x509.c
        platforms/linux/opcua_p_pkifactory.c
        platforms/linux/opcua_p_semaphore.c
        platforms/linux/opcua_p_socket.c
        platforms/linux/opcua_p_socket_interface.c
        platforms/linux/opcua_p_socket_internal.c
        platforms/linux/opcua_p_socket_ssl.c
        platforms/linux/opcua_p_string.c
        platforms/linux/opcua_p_thread.c
        platforms/linux/opcua_p_timer.c
        platforms/linux/opcua_p_trace.c
        platforms/linux/opcua_p_utilities.c
    )
    list(APPEND UASTACK_LIBRARIES
        pthread
    )
endif ()


add_library(uastack STATIC ${UASTACK_PUBLIC_HEADERS} ${UASTACK_PRIVATE_HEADERS} ${UASTACK_SOURCES})
set_property(TARGET uastack PROPERTY COMPILE_DEFINITIONS _UA_STACK_BUILD_DLL)
set_property(TARGET uastack PROPERTY PUBLIC_HEADER ${UASTACK_PUBLIC_HEADERS})
target_include_directories(uastack SYSTEM PRIVATE ${OPENSSL_INCLUDE_DIR})
target_include_directories(uastack PUBLIC ${UASTACK_INCLUDE_DIRECTORIES})
target_link_libraries(uastack ${UASTACK_LIBRARIES} ${OPENSSL_LIBRARIES})
