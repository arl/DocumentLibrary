#! /bin/bash
# description	:	increment a number in a file
#					to find this number you must provide a regexp that will uniquely identify
#					one and only one line. This line must contain one and only one integer
#					that will be incremented
# usage			:	increment_number.sh  IN REGEXP [OUT]
#
#		IN
#			input file
#
#		REGEXP
#			regular expression to look for in IN
#
#		[OUT]
#			output file, defaults to IN if not specified
#

if [ $# -lt 2 ]; then

	printf "bad number of arguments\n\n"
	printf "\tusage\t:increment_number.sh  IN REGEXP [OUT]\n"
	printf "\n"
    printf "\t\tIN\n"
  	printf "\t\t\tinput file\n"
    printf "\t\tREGEXP\n"
  	printf "\t\t\tregular expression to look for in IN\n"
    printf "\t\t[OUT]\n"
  	printf "\t\t\toutput file, defaults to standard output if not specified\n"
    exit 1 
fi

IN=$1
REGEXP=$2
OUT=${3:-""}

CMD="$REGEXP.*[0-9]+|[0-9]+.*$REGEXP"
MATCHES=$(egrep $CMD $IN | wc -l)

if [ $MATCHES -eq 0 ]; then
	echo "no matches for $REGEXP in \'$IN\'"
	exit 1
elif [ $MATCHES -gt 1 ]; then
	echo "more than one match for $REGEXP in \'$IN\'"
	exit 2
fi

LINE_NUM=$(egrep -n $CMD $IN | awk -F : ' { print $1 } ')
MATCHING_LINE=$(cat $IN | sed -n "$LINE_NUM"p)
OLD_BUILD=$(echo $MATCHING_LINE | sed -e 's/[^0-9]*\([0-9]*\)[^0-9]*/\1/g')
BUILD=$(($OLD_BUILD+1))

# now we are sur we've got something to do, create a tmp file to put the result in
PROGNAME=${0##*/}
TMPFILE=$(mktemp $PROGNAME.XXXXXX) || exit 5

# rewrite the eventual lines before the match
if [ $LINE_NUM -gt 1 ]; then
	MAX_LINE=$(($LINE_NUM-1))
	cat $IN | sed -n 1,"$MAX_LINE"p >> $TMPFILE
fi

# now rewrite the same file with the incremented number
cat $IN | sed -n "$LINE_NUM"p | sed -e s/"$OLD_BUILD"/"$BUILD"/g >> $TMPFILE

# rewrite the eventual lines after the match
MAX_LINE=$(cat $IN | wc -l)

if [ $LINE_NUM -lt $MAX_LINE ]; then
	START_LINE=$(($LINE_NUM+1))
	cat $IN | sed -n "$START_LINE","$MAX_LINE"p >> $TMPFILE
fi

# now writes the result to the desired file
if [ -z $OUT ]; then
	# to standard output
	cat $TMPFILE
	rm -f $TMPFILE
else
	# to output file
	mv $TMPFILE $OUT
fi

exit 0
