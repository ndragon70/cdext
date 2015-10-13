cmake_minimum_required (VERSION 3.0)

project(cdext)

set(VERSION "1.2.4")
set(COMMON_SRC cd-ext.c parse.c)

add_executable(cdext-ccd    ${COMMON_SRC} ccd.c)
add_executable(cdext-list   ${COMMON_SRC} list.c)
add_executable(cdext-delete ${COMMON_SRC} delete.c)
add_executable(cdext-save   ${COMMON_SRC} save.c)
add_executable(cdext-setcd  ${COMMON_SRC} setcd.c)

include(GNUInstallDirs)
install(TARGETS cdext-ccd    DESTINATION bin)
install(TARGETS cdext-list   DESTINATION bin)
install(TARGETS cdext-delete DESTINATION bin)
install(TARGETS cdext-save   DESTINATION bin)
install(TARGETS cdext-setcd  DESTINATION bin)
install(FILES "cdext.1" DESTINATION "${CMAKE_INSTALL_MANDIR}/man1" COMPONENT doc)
install(FILES "cdext-setup.bash" DESTINATION "/etc/profile.d" RENAME cdext.sh)

set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_GENERATOR "RPM")
set(CPACK_PACKAGE_NAME "cdext")
set(CPACK_PACKAGE_RELEASE 0)
set(CPACK_PACKAGE_CONTACT "James Newell")
set(CPACK_PACKAGE_VENDOR "Newell Inc")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
# hack to produce a more sane rpm
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION /usr/local /usr/local/bin /etc/profile.d /usr/local/share /usr/local/share/man /usr/local/share/man/man1)
include(CPack)
