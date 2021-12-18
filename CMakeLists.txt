cmake_minimum_required(VERSION 3.4.1)

project(nacos-sdk-cpp)

#add_definitions(-DNACOS_AUTH)
#add_definitions(-DNACOS_SPAS)

include_directories (
.
include
include/config
include/constant
include/factory
include/listen
include/naming
include/naming/selectors
include/naming/subscribe
include/server
include/thread
include/utils
)

aux_source_directory( src SRCS_DIRS)
aux_source_directory( src/config SRCS_DIRS)
aux_source_directory( src/constant SRCS_DIRS)
aux_source_directory( src/crypto SRCS_DIRS)
aux_source_directory( src/crypto/base64 SRCS_DIRS)
aux_source_directory( src/crypto/hmac_sha1 SRCS_DIRS)
aux_source_directory( src/crypto/hmac_sha1/hmac SRCS_DIRS)
aux_source_directory( src/crypto/hmac_sha1/sha SRCS_DIRS)
aux_source_directory( src/crypto/md5 SRCS_DIRS)
aux_source_directory( src/factory SRCS_DIRS)
aux_source_directory( src/http SRCS_DIRS)
aux_source_directory( src/http/delegate SRCS_DIRS)
aux_source_directory( src/init SRCS_DIRS)
aux_source_directory( src/json SRCS_DIRS)
aux_source_directory( src/json/rapidjson SRCS_DIRS)
aux_source_directory( src/json/rapidjson/error SRCS_DIRS)
aux_source_directory( src/json/rapidjson/internal SRCS_DIRS)
aux_source_directory( src/json/rapidjson/msinttypes SRCS_DIRS)
aux_source_directory( src/listen SRCS_DIRS)
aux_source_directory( src/log SRCS_DIRS)
aux_source_directory( src/naming SRCS_DIRS)
aux_source_directory( src/naming/beat SRCS_DIRS)
aux_source_directory( src/naming/cache SRCS_DIRS)
aux_source_directory( src/naming/selectors SRCS_DIRS)
aux_source_directory( src/naming/subscribe SRCS_DIRS)
aux_source_directory( src/security SRCS_DIRS)
aux_source_directory( src/server SRCS_DIRS)
aux_source_directory( src/thread SRCS_DIRS)
aux_source_directory( src/utils SRCS_DIRS)
aux_source_directory( test SRCS_DIRS)
aux_source_directory( test/testcase SRCS_DIRS)

aux_source_directory(test TEST_SRCS_DIRS)
aux_source_directory(test/testcase TEST_SRCS_DIRS)

add_executable(nacos-cli.out ${TEST_SRCS_DIRS} ${SRCS_DIRS})
add_library(nacos-cli SHARED ${SRCS_DIRS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
target_link_libraries(nacos-cli PRIVATE Threads::Threads curl)
target_link_libraries(nacos-cli.out PRIVATE Threads::Threads curl)
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)

install(TARGETS nacos-cli
		LIBRARY)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
		DESTINATION include/nacos-sdk-cpp
		FILES_MATCHING PATTERN "*.h*")

install(TARGETS nacos-cli)
