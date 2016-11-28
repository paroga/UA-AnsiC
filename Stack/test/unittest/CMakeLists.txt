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

set(UAUNITTEST_INCLUDE_DIRECTORIES
    buffertest
    datetimetest
    guidtest
    listtest
    memorytest
    mutextest
    securitytest
    semaphoretest
    sockettest
    stringtest
    threadpooltest
    threadtest
    timertest
    utilitiestest
    .
)

set(UAUNITTEST_HEADERS
    buffertest/buffertest.h
    datetimetest/datetimetest.h
    guidtest/guidtest.h
    listtest/listtest.h
    memorytest/memorytest.h
    mutextest/mutextest.h
    securitytest/opcua_cryptofactory_dev_test.h
    securitytest/opcua_cryptofactory_test.h
    securitytest/opcua_cryptoprovider_dev_test.h
    securitytest/opcua_cryptoprovider_test.h
    securitytest/opcua_pkiapi_test.h
    securitytest/opcua_pkifactory_dev_test.h
    securitytest/opcua_pkifactory_test.h
    securitytest/opcua_pkiprovider_dev_test.h
    securitytest/opcua_pkiprovider_test.h
    securitytest/opcua_securitytest_helperfunctions.h
    securitytest/securitytest.h
    semaphoretest/semaphoretest.h
    sockettest/sockettest.h
    stringtest/stringtest.h
    threadpooltest/threadpooltest.h
    threadtest/threadtest.h
    timertest/timertest.h
    utilitiestest/utilitiestest.h
    unittest.h
)

set(UAUNITTEST_SOURCES
    buffertest/buffertest.c
    datetimetest/datetimetest.c
    guidtest/guidtest.c
    listtest/listtest.c
    memorytest/memorytest.c
    mutextest/mutextest.c
    #securitytest/opcua_cryptofactory_dev_test.c
    #securitytest/opcua_cryptofactory_test.c
    #securitytest/opcua_cryptoprovider_dev_test.c
    #securitytest/opcua_cryptoprovider_test.c
    #securitytest/opcua_pkiapi_test.c
    #securitytest/opcua_pkifactory_dev_test.c
    #securitytest/opcua_pkifactory_test.c
    #securitytest/opcua_pkiprovider_dev_test.c
    #securitytest/opcua_pkiprovider_test.c
    #securitytest/opcua_securitytest_helperfunctions.c
    #securitytest/securitytest.c
    semaphoretest/semaphoretest.c
    sockettest/sockettest.c
    stringtest/stringtest.c
    threadpooltest/threadpooltest.c
    threadtest/threadtest.c
    timertest/timertest.c
    utilitiestest/utilitiestest.c
    unittest.c
)


add_executable(uaunittest ${UAUNITTEST_HEADERS} ${UAUNITTEST_SOURCES})
target_include_directories(uaunittest PRIVATE ${UAUNITTEST_INCLUDE_DIRECTORIES})
target_link_libraries(uaunittest uastack)
add_test(NAME unittest COMMAND uaunittest --batch)
