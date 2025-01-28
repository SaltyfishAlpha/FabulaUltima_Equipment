############# TEST Project ###############

add_executable(FUEquipTEST test1/test1.cpp)
target_link_libraries(FUEquipTEST PUBLIC ${RUNTIME_TARGET_NAME})

add_executable(FUEquipTEST2 test2/test2.cpp)
target_link_libraries(FUEquipTEST2 PUBLIC ${RUNTIME_TARGET_NAME})

add_executable(FUEquipTEST3 test3/test3.cpp)
target_link_libraries(FUEquipTEST3 PUBLIC ${RUNTIME_TARGET_NAME})