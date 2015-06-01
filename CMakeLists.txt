cmake_minimum_required (VERSION 2.8)

project(cdext)

set(VERSION "1.2.0")
set(COMMON_SRC cd-ext.c parse.c)

add_executable(cdext-ccd    ${COMMON_SRC} ccd.c)
add_executable(cdext-list   ${COMMON_SRC} list.c)
add_executable(cdext-delete ${COMMON_SRC} delete.c)
add_executable(cdext-save   ${COMMON_SRC} save.c)
add_executable(cdext-setcd  ${COMMON_SRC} setcd.c)

install(TARGETS cdext-ccd    DESTINATION bin)
install(TARGETS cdext-list   DESTINATION bin)
install(TARGETS cdext-delete DESTINATION bin)
install(TARGETS cdext-save   DESTINATION bin)
install(TARGETS cdext-setcd  DESTINATION bin)

set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_GENERATOR "RPM")
set(CPACK_PACKAGE_NAME "cdext")
set(CPACK_PACKAGE_RELEASE 0)
set(CPACK_PACKAGE_CONTACT "James Newell")
set(CPACK_PACKAGE_VENDOR "Newell Inc")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
include(CPack)
