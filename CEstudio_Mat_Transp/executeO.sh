#!/bin/bash

# Command for compile c++ transp mat mul and run 
compile='compile' # compile file 
run='run' # run file 
auto='auto'
clean='clean'
#if you use both can compile and run
args=("$@")

auto_exec(){
	for i in {1..10}
	do
		for k in 10, 100, 250 
			do
			./"$1" $k
			done
	done
}

echo " ${args[0]} ${args[1]} ${args[2]} ${args[3]}"

if [ "${args[0]}" == "$compile" ] &&  [ "${args[1]}" == "$run" ] && [ "${args[2]}" == "$auto" ] && [ "${args[3]}" == "$clean" ];
then
	#rm time.csv
	
	echo "ejecucion secuencial O1"
	g++ -O1 -o ${args[4]} ${args[4]}.cpp 
	auto_exec ${args[4]}
	echo "ejecucion con hilos O1"
	gcc -pthread -O1 -o ${args[5]} ${args[5]}.c
	auto_exec ${args[5]}
	echo "ejecucion con procesos O1" 
	g++ -O1 -o ${args[6]} ${args[6]}.cpp 
	auto_exec ${args[6]}

	echo "ejecucion secuencial O2"
	g++ -O2 -o ${args[4]} ${args[4]}.cpp 
	auto_exec ${args[4]}
	echo "ejecucion con hilos O2"
	gcc -pthread -O2 -o ${args[5]} ${args[5]}.c
	auto_exec ${args[5]}
	echo "ejecucion con procesos O2"
	g++ -O2 -o ${args[6]} ${args[6]}.cpp 
	auto_exec ${args[6]}

	echo "ejecucion secuencial O3"
	g++ -O3 -o ${args[4]} ${args[4]}.cpp 
	auto_exec ${args[4]}
	echo "ejecucion con hilos O3"
	gcc -pthread -O3 -o ${args[5]} ${args[5]}.c
	auto_exec ${args[5]}
	echo "ejecucion con procesos O3"
	g++ -O3 -o ${args[6]} ${args[6]}.cpp 
	auto_exec ${args[6]}
else 
	echo "revisa los comandos de entrada"

fi
