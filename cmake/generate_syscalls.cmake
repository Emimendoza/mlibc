if(NOT HEADER_DIR)
	message(FATAL_ERROR "Usage: cmake -DHEADER_DIR=<HEADER_DIR> generate_syscalls.cmake")
endif()

file(GLOB SYSCALL_HEADER_FILES "${HEADER_DIR}/unistd_*.h")

set(SYSCALL_NAMES "")

foreach(hfile IN LISTS SYSCALL_HEADER_FILES)
	file(READ "${hfile}" FILE_CONTENTS)
	
	string(REGEX MATCHALL "__NR_[A-Za-z0-9_]+" MATCHES "${FILE_CONTENTS}")
	
	foreach(m IN LISTS MATCHES)
		string(REPLACE "__NR_" "" NAME "${m}")
		list(APPEND SYSCALL_NAMES "${NAME}")
	endforeach()
endforeach()

list(REMOVE_DUPLICATES SYSCALL_NAMES)

set(SYSCALL_DEFINITIONS "")
foreach(name IN LISTS SYSCALL_NAMES)
string(APPEND SYSCALL_DEFINITIONS
"#ifdef __NR_${name}
	#define SYS_${name} __NR_${name}
#endif

")
endforeach()

execute_process(
	COMMAND ${CMAKE_COMMAND} -E echo "${SYSCALL_DEFINITIONS}"
)
