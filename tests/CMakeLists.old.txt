set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/tests)
set(CTEST_BINARY_DIRECTORY ${PROJECT_BINARY_DIR}/tests)

file(GLOB TEST_SOURCES "*.cpp")

foreach(file ${TEST_SOURCES})
	string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" file_without_ext ${file})
	add_executable(${file_without_ext} ${file})
	target_link_libraries(${file_without_ext} ${PROJECT_LIBS})
	add_test(${file_without_ext} ${file_without_ext})
	set_tests_properties(${file_without_ext}
		PROPERTIES
		PASS_REGULAR_EXPRESSION "Test passed")
	set_tests_properties(${file_without_ext}
		PROPERTIES
		FAIL_REGULAR_EXPRESSION "(Exception|Test failed)")
	set_tests_properties(${file_without_ext}
		PROPERTIES
		TIMEOUT 120)
endforeach()
