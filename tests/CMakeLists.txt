#Add test source files
set(TEST_FILES
	pm_counter_test.c
	pm_gauge_test.c
	pm_histogram_test.c
	pm_summary_test.c
	pm_collector_test.c
	pm_formatter_test.c
	pm_exposer_test.c
#Add more test files as needed
    )

# Create test executable
foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} prometheus)
endforeach(TEST_FILE)
