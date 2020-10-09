#!/bin/bash

# Command for compile c++ transp mat mul and run 
# bash execute.sh compile run auto clean Sec_Transp Thread_Transp Fork_Transp
compile='compile' # compile file 
run='run' # run file 
#if you use both can compile and run
args=("$@")

auto_exec(){
	for i in {1..5}
	do
		for k in 100 200 300 400 500 1000
			do
			./"$1" $k
			done
	done
}

echo " ${args[0]} ${args[1]} ${args[2]} ${args[3]}"

if [ "${args[0]}" == "$compile" ] &&  [ "${args[1]}" == "$run" ];
then
	echo "Ejecucion secuencial"
	g++ -o ${args[2]} ${args[2]}.cpp 
	auto_exec ${args[2]}
    
	echo "Ejecucion con OpenMP"
	g++ -o ${args[3]} -fopenmp ${args[3]}.cpp 
	auto_exec ${args[3]}
    
	
else 
	echo "revisa los comandos de entrada"

fi
