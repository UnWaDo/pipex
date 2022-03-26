#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'
GREEN='\033[0;32m'

test () {
	rm -f out test_out
	./pipex in "$2" "$3" out
	< in $2 | $3 > test_out
	if [[ $(diff out test_out) == "" ]]; then
	    echo -e "$1: ${GREEN}OK${NC}"
	else
	    echo -e "$1: ${RED}KO${NC}"
		echo "EXPECTED:"
		cat test_out
		echo "GOT:"
		cat out
	fi
}

make
<< BG > in
Some lines
Of text
And one more
BG

test "cat and wc" "cat" "wc -l"
test "invalid argument" "sed -q" "wc -l"
test "invalid command" "lll" "wc -l"
test "sed with quotes" "cat" "sed 's/i/of/'"