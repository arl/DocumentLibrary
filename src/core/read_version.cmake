#####################################################
#                                                   #
#    CMake Script to read version number            #
#                                                   #
#                                                   #
#    mandatory parameters :                         #
#       IN      : INPUT file                        #
#       LOOKFOR : Strings to look for when          #
#                 computing version number          #
#                                                   #
#####################################################

# put the lines from file IN that contain string LOOKFOR in a list
file(STRINGS ${IN} LINES REGEX "${LOOKFOR}")

set(VERSION "")
set(NUMLINES "0")

foreach(loop_var ${LINES})

	# extract number part of current line
	# WARNING : number must be after a space because CMake regexes can't be
	# made greedy for now
	string(REGEX MATCH ".* ([0-9]+).*" TMPNUM ${loop_var})

	# add a dot after version number
	if(NOT ${NUMLINES} EQUAL "0")
		set(VERSION "${VERSION}.")
	endif(NOT ${NUMLINES} EQUAL "0")

	# add current line version number to version string
	set(VERSION "${VERSION}${CMAKE_MATCH_1}")

	# increment line counter
	math(EXPR NUMLINES "${NUMLINES} + 1")
endforeach(loop_var)

message (${VERSION})
