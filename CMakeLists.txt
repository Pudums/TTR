cmake_minimum_required(VERSION 3.16)

project(Project)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lgrpc++")

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include_directories(/home/megaserg01/grpc/include/)
file(GLOB CPP
		"src/*"
		"src/Controller/*"
		"src/View/*"
		"src/Server/*"
		"src/Server/protobuf/*"
		)

file(GLOB H
		"include/*"
		"include/Controller/*"
		"include/View/*"
		"include/Server/*"
		"include/Server/protobuf/*"
		)

find_package(
		Qt5
		COMPONENTS Quick
		QuickCompiler
		Widgets
		Gui
		REQUIRED)

add_library(QT_VIEW ${CPP} ${H})
target_include_directories(QT_VIEW PUBLIC include include/View include/Controller)
target_link_libraries(QT_VIEW
		PUBLIC
		Qt5::Quick
		Qt5::Gui
		Qt5::Widgets
		Qt5::Core
		)
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS foo.proto)
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS EXPORT_MACRO DLL_EXPORT TTR.proto)
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS DESCRIPTORS PROTO_DESCS TTR.proto)
#protobuf_generate_python(PROTO_PY foo.proto)
add_executable(bar bar.cc ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(bar ${Protobuf_LIBRARIES})
find_package(Threads REQUIRED)

if(GRPC_AS_SUBMODULE)
	# One way to build a projects that uses gRPC is to just include the
	# entire gRPC project tree via "add_subdirectory".
	# This approach is very simple to use, but the are some potential
	# disadvantages:
	# * it includes gRPC's CMakeLists.txt directly into your build script
	#   without and that can make gRPC's internal setting interfere with your
	#   own build.
	# * depending on what's installed on your system, the contents of submodules
	#   in gRPC's third_party/* might need to be available (and there might be
	#   additional prerequisites required to build them). Consider using
	#   the gRPC_*_PROVIDER options to fine-tune the expected behavior.
	#
	# A more robust approach to add dependency on gRPC is using
	# cmake's ExternalProject_Add (see cmake_externalproject/CMakeLists.txt).

	# Include the gRPC's cmake build (normally grpc source code would live
	# in a git submodule called "third_party/grpc", but this example lives in
	# the same repository as gRPC sources, so we just look a few directories up)
	add_subdirectory(../../.. ${CMAKE_CURRENT_BINARY_DIR}/grpc EXCLUDE_FROM_ALL)
	message(STATUS "Using gRPC via add_subdirectory.")

	# After using add_subdirectory, we can now use the grpc targets directly from
	# this build.
	set(_PROTOBUF_LIBPROTOBUF libprotobuf)
	set(_REFLECTION grpc++_reflection)
	if(CMAKE_CROSSCOMPILING)
		find_program(_PROTOBUF_PROTOC protoc)
	else()
		set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
	endif()
	set(_GRPC_GRPCPP grpc++)
	if(CMAKE_CROSSCOMPILING)
		find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
	else()
		set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
	endif()
elseif(GRPC_FETCHCONTENT)
	# Another way is to use CMake's FetchContent module to clone gRPC at
	# configure time. This makes gRPC's source code available to your project,
	# similar to a git submodule.
	message(STATUS "Using gRPC via add_subdirectory (FetchContent).")
	include(FetchContent)
	FetchContent_Declare(
			grpc
			GIT_REPOSITORY https://github.com/grpc/grpc.git
			# when using gRPC, you will actually set this to an existing tag, such as
			# v1.25.0, v1.26.0 etc..
			# For the purpose of testing, we override the tag used to the commit
			# that's currently under test.
			GIT_TAG        vGRPC_TAG_VERSION_OF_YOUR_CHOICE)
	FetchContent_MakeAvailable(grpc)

	# Since FetchContent uses add_subdirectory under the hood, we can use
	# the grpc targets directly from this build.
	set(_PROTOBUF_LIBPROTOBUF libprotobuf)
	set(_REFLECTION grpc++_reflection)
	set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
	set(_GRPC_GRPCPP grpc++)
	if(CMAKE_CROSSCOMPILING)
		find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
	else()
		set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
	endif()
else()
	# This branch assumes that gRPC and all its dependencies are already installed
	# on this system, so they can be located by find_package().

	# Find Protobuf installation
	# Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
	set(protobuf_MODULE_COMPATIBLE TRUE)
#	find_package(Protobuf CONFIG REQUIRED)
	message(STATUS "Using protobuf ${Protobuf_VERSION}")

	set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
	set(_REFLECTION gRPC::grpc++_reflection)
	if(CMAKE_CROSSCOMPILING)
		find_program(_PROTOBUF_PROTOC protoc)
	else()
		set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
	endif()

	# Find gRPC installation
	# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
	find_package(gRPC CONFIG REQUIRED)
	message(STATUS "Using gRPC ${gRPC_VERSION}")

	set(_GRPC_GRPCPP gRPC::grpc++)
	if(CMAKE_CROSSCOMPILING)
		find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
	else()
		set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
	endif()
endif()

# Proto file
get_filename_component(hw_proto "protobuf/TTR.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/src/Server/protobuf/TTR.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/include/Server/protobuf/TTR.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/src/Server/protobuf/TTR.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/include/Server/protobuf/TTR.grpc.pb.h")
add_custom_command(
		OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
		COMMAND ${_PROTOBUF_PROTOC}
		ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
		--cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
		-I "${hw_proto_path}"
		--plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
		"${hw_proto}"
		DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Targets greeter_[async_](client|server)
foreach(_target
		greeter_client greeter_server
		greeter_async_client greeter_async_client2 greeter_async_server)
	add_executable(${_target} "${_target}.cc"
			${hw_proto_srcs}
			${hw_grpc_srcs})
	target_link_libraries(${_target}
			${_REFLECTION}
			${_GRPC_GRPCPP}
			${_PROTOBUF_LIBPROTOBUF})
endforeach()

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} QT_VIEW)
