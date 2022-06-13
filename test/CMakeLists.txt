
add_executable(${CMAKE_PROJECT_NAME}_test ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

ament_target_dependencies(${CMAKE_PROJECT_NAME}_test
    rclcpp
    sensor_msgs
)

target_include_directories(${CMAKE_PROJECT_NAME}_test 
							PRIVATE 
							${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME}_test iwr6843aop)

#install(TARGETS ${PROJECT_NAME}_test DESTINATION "bin" CONFIGURATIONS Release Debug
#                        COMPONENT "${PROJECT_NAME}-bin")
                        

install(TARGETS ${CMAKE_PROJECT_NAME}_test 
	DESTINATION lib/${PROJECT_NAME})
	
	
