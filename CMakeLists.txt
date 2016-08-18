cmake_minimum_required (VERSION 3.0)

project(cdext)

set(VERSION "1.3.0")
set(SRC 
 cd-ext.c
 parse.c
 ccd.c
 list.c 
 delete.c
 save.c
 setcd.c
)

set(BUILD_SHARED_LIBRARIES OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_library(cdext-common STATIC ${SRC})
add_executable(cdext cdext-multi.c)
target_link_libraries(cdext cdext-common)
 
include(GNUInstallDirs)
install(TARGETS cdext DESTINATION bin)

install(FILES "cdext.1" DESTINATION "${CMAKE_INSTALL_MANDIR}/man1" COMPONENT doc)
install(FILES "cdext-setup.bash" DESTINATION "/etc/profile.d" RENAME cdext.sh)

set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_GENERATOR "RPM")
set(CPACK_PACKAGE_NAME "cdext")

if (NOT CPACK_PACKAGE_RELEASE)
set(CPACK_PACKAGE_RELEASE 0)
set(CPACK_RPM_PACKAGE_RELEASE ${CPACK_PACKAGE_RELEASE})
else()
set(CPACK_RPM_PACKAGE_RELEASE ${CPACK_PACKAGE_RELEASE})
endif()

if (CPACK_RPM_PACKAGE_RELEASE EQUAL 0)
# need the space or the if not variable check fails
set(CPACK_RPM_PACKAGE_RELEASE "0 ")
endif()

set(CPACK_RPM_PACKAGE_LICENSE "BSD")
set(CPACK_PACKAGE_CONTACT "James Newell")
set(CPACK_PACKAGE_VENDOR "Newell Inc")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_RPM_PACKAGE_DESCRIPTION "Provides change directory extensions for bash.")
set(CPACK_RPM_PACKAGE_SUMMARY "Provides change directory extensions for bash.")
set(CPACK_RPM_RELOCATION_PATHS "/etc/profile.d")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
# hack to produce a more sane rpm
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION /usr/local /usr/local/bin /etc/profile.d /usr/local/share /usr/local/share/man /usr/local/share/man/man1)
include(CPack)
