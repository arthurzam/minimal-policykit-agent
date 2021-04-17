cmake_minimum_required(VERSION 3.5)

project(minimal-policykit-agent LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS DBus Widgets REQUIRED)
find_package(PolkitQt5-1)

add_executable(${PROJECT_NAME}
    main.cpp
    dialogagent.cpp
    dialogagent.h
    authenticationagent.cpp
    authenticationagent.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Widgets
    Qt5::DBus
    PolkitQt5-1::Core
    PolkitQt5-1::Agent
)

include(GNUInstallDirs)
set(POLKIT_AGENT_BINARY_DIR "${CMAKE_INSTALL_BINDIR}" CACHE FILEPATH "Directory for install polkit agent")
install(
    TARGETS ${PROJECT_NAME}
    DESTINATION "${POLKIT_AGENT_BINARY_DIR}"
)
