if (EXECUTE_POST_BUILD)
	if (CMAKE_STRIP)
		execute_process(COMMAND ${CMAKE_STRIP} -s ${TARGET_FILE})
	endif ()
endif ()
