
################################################################################
### Selecting CMake minimum version

CMAKE_MINIMUM_REQUIRED (VERSION 3.4.0)

################################################################################
### Setting Global Parameters

SET(PROJECT_NAME "os lab5 lib1")
SET(BINARY_NAME "lab5lib1")

PROJECT("${PROJECT_NAME}")

SET(LIBRARY_VERSION_MAJOR 0)
SET(LIBRARY_VERSION_STRING 0.1)

INCLUDE(GNUInstallDirs)

SET(LIBRARY_BASE_PATH "${PROJECT_SOURCE_DIR}/src")

################################################################################
### Project definition

INCLUDE_DIRECTORIES (
        "${LIBRARY_BASE_PATH}"
)

SET(PUBLIC_SOURCES_FILES
        "${LIBRARY_BASE_PATH}/public/src/api.c"
        )

SET(PRIVATE_SOURCES_FILES
        "${LIBRARY_BASE_PATH}/private/src/operations.c"
        )

SET(PUBLIC_HEADERS_FILES
        "${LIBRARY_BASE_PATH}/public/headers/api.h"
        )

SET(PRIVATE_HEADERS_FILES
        "${LIBRARY_BASE_PATH}/private/headers/operations.h"
        )

SET(CMAKE_C_VISIBILITY_PRESET hidden)

ADD_LIBRARY (
        ${BINARY_NAME} SHARED ${PUBLIC_SOURCES_FILES} ${PRIVATE_SOURCES_FILES}
)

SET_TARGET_PROPERTIES (
        ${BINARY_NAME} PROPERTIES
        VERSION		${LIBRARY_VERSION_STRING}
        SOVERSION	${LIBRARY_VERSION_MAJOR}
        PUBLIC_HEADER  ${PUBLIC_HEADERS_FILES}
)

################################################################################
### Adding compilator definitions, like debug symbols

ADD_DEFINITIONS("-g")

################################################################################
### Library installation directives

INSTALL (
        TARGETS ${BINARY_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
