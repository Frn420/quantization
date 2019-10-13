#!/bin/bash

prog="$1"
path="$2"
var="my_"

for f in "$path"*.in
do
	diff_out="empty"
	diff_err="empty"
	memory_error="true"
	pom="${f##"$path"}"
	pom2="${pom%.in}"

	valgrind -q --error-exitcode=1 --log-file="log_file" ./"$prog" <"$f" >$var"${pom%in}"out 2>$var"${pom%in}"err

	if [ $? -eq 0 ]; then memory_error="false"; fi #valgrind
	diff $var"${pom%in}"out "${f%in}"out >/dev/null 2>&1
	if [ $? -eq 1  ]; then diff_out="exist"; fi #.out files
	diff $var"${pom%in}"err "${f%in}"err
	if [ $? -eq 1 ]; then err_out="exist"; fi #.err files
	
	if [ "$diff_out" == "empty"  ] && [ "$diff_err" == "empty" ] && [ "$memory_error" == "false" ]
	then
		echo "$pom2" "SUCCES"
	else
		echo "$pom2" "FAILURE"
	fi

	rm $var"${pom%in}"err
	rm $var"${pom%in}"out
	if [ -s log_file ]; then rm log_file; fi
done

