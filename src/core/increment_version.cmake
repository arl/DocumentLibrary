#####################################################
#                                                   #
#    CMake Script to increment version number       #
#                                                   #
#                                                   #
#    mandatory parameters :                         #
#       IN      : INPUT file                        #
#       LOOKFOR : the word after which the number   #
#                 found has to be incremented       #
#                 the word and the number must be   #
#                 on the same line and the number   #
#                 must be preceded by a space due   #
#                 to regex greediness in CMake      #
#                                                   #
#                                                   #
#    additionnal parameters :                       #
#        OUT    : OUTPUT file, if not defined, then #
#                  INput file will be overwritten   #
#####################################################

if(NOT DEFINED OUT)
	set(OUT "${IN}")
endif(NOT DEFINED OUT)

message("increment_version.cmake : looking for : '${LOOKFOR}' in '${IN}'") 
message("Output file : ${OUT}") 

# read the whole file in $CONTENT
file(READ ${IN} CONTENT)

# replace number in the line containing $LOOKFOR by @NUMBER@
string(REGEX REPLACE
	"^(.*${LOOKFOR}.* )([0-9]+)(.*)$" "\\1@NUMBER@\\3"
	CONTENT "${CONTENT}")

# increment regex referenced number
math(EXPR NUMBER "${CMAKE_MATCH_2} + 1")

# log new version number
message("new ${LOOKFOR} version number : ${NUMBER}") 

# replace @NUMBER@ by new version number
string(CONFIGURE "${CONTENT}" CONTENT @ONLY)

# now write the file to $OUT
file(WRITE "${OUT}" "${CONTENT}")
