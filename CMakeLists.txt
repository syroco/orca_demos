cmake_minimum_required(VERSION 3.1)
project(orca_demos)

find_package(gazebo REQUIRED)
find_package(catkin REQUIRED COMPONENTS orca)

catkin_package()

include_directories(${GAZEBO_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

add_definitions(-DGAZEBO_VERSION_MAJOR=${gazebo_VERSION_MAJOR})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_definitions(-fext-numeric-literals)
endif()

add_executable(orca_gazebo_example orca_gazebo_example.cc)
target_link_libraries(orca_gazebo_example ${GAZEBO_LIBRARIES} ${catkin_LIBRARIES})
install(TARGETS orca_gazebo_example DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

add_executable(human_model_test_01 human_model_test_01.cc)
target_link_libraries(human_model_test_01 ${GAZEBO_LIBRARIES} ${catkin_LIBRARIES})
install(TARGETS human_model_test_01 DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

add_executable(human_model_test_02 human_model_test_02.cc)
target_link_libraries(human_model_test_02 ${GAZEBO_LIBRARIES} ${catkin_LIBRARIES})
install(TARGETS human_model_test_02 DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
