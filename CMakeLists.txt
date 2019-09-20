cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 14)

project(join_server)

if ($ENV{TRAVIS_BUILD_NUMBER})
 project(${PROJECT_NAME} VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ($ENV{TRAVIS_BUILD_NUMBER})
 project(${PROJECT_NAME}  VERSION 0.0.1)
endif ($ENV{TRAVIS_BUILD_NUMBER})


find_package(Boost COMPONENTS unit_test_framework system REQUIRED)


include_directories(".")
include_directories("src_async")
include_directories("serv")
include_directories("db_struct")
include_directories(${Boost_INCLUDE_DIR})
link_directories(/usr/lib)

add_executable(${PROJECT_NAME} main.cpp server/session.cpp server/server.cpp)



set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    )



target_link_libraries(${PROJECT_NAME}        
        ${Boost_SYSTEM_LIBRARY}
        async
)


install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")

set(CPACK_PACKAGE_CONTACT alexisvabel@gmail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
include(CPack)
