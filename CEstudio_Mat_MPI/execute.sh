#!/bin/bash

# Command for compile c++ transp mat mul and run 
# bash execute.sh compile run matMPI
compile='compile' # compile file 
run='run' # run file 
#if you use both can compile and run
args=("$@")

auto_exec(){
	for i in {1..5}
	do
		for k in 4, 8, 12, 16
			do
			./"$1" $k
			done
	done
}

echo " ${args[0]} ${args[1]} ${args[2]}"

if [ "${args[0]}" == "$compile" ] &&  [ "${args[1]}" == "$run" ];
then
	echo "Ejecucion secuencial"
	mpicc ${args[2]}.c –o ${args[2]}
	mpirun -np 1 -machinefile mfile ./executable
	#auto_exec ${args[2]}
    
	#echo "Ejecucion con OpenMP"
	#g++ -o ${args[3]} -fopenmp ${args[3]}.cpp 
	#auto_exec ${args[3]}
    
	
else 
	echo "revisa los comandos de entrada"

fi
