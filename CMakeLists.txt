cmake_minimum_required(VERSION 3.5)

project(myEngine)

############################################################
# Set compiler flags
############################################################
# If supported by your compiler, you can add the -Wall, -Wextra, –fstack-protector-all and -g3 flags here.
set(OWN_GXX_FLAGS "-std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OWN_GXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OWN_GXX_FLAGS}")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${OWN_GXX_FLAGS}")

############################################################
# List all sources
############################################################
set(engine_sources
		tools/easy_image.cc
		src/engine.cc
		tools/ini_configuration.cc
		src/LineDrawer.cpp
		src/LineDrawer.h
		tools/l_parser.h
		tools/l_parser.cc
		src/MyLSystem2D.cpp
		src/MyLSystem2D.h
		tools/vector3d.cc
		src/Figure.cpp
		src/Figure.h
		src/Drawing3D.cpp
		src/Drawing3D.h
		src/Solid3D.cpp
		src/Solid3D.h
		src/MyParser.cpp
		src/MyParser.h
		src/MyLSystem3D.cpp
		src/MyLSystem3D.h
)

############################################################
# Create an executable
############################################################
set(exe_name "engine")
add_executable( ${exe_name} ${engine_sources} )
install( TARGETS ${exe_name} DESTINATION ${PROJECT_SOURCE_DIR}/ )
