#!/bin/bash

# Command for compile c mat with MPI
# bash execute.sh compile run matMPI
compile='compile' # compile file 
run='run' # run file 
#if you use both can compile and run
args=("$@")

secuencial_exec(){
	for i in {1..10}
	do
		for k in 100, 400, 800, 1000, 2000
			do
			mpirun -np 1 -hosts head ./"$1" $k
			done
	done
}

parallel_exec(){
	for i in {1..10}
	do
		for k in 100, 400, 800, 1000, 2000
			do
			mpirun -np 4 -hosts head,wn1,wn2,wn3 ./"$1" $k
			done
	done
}

echo " ${args[0]} ${args[1]} ${args[2]}"

if [ "${args[0]}" == "$compile" ] &&  [ "${args[1]}" == "$run" ];
then
	mpicc matMPI.c -o executable
	secuencial_exec ./executable
	parallel_exec ./executable
else
	echo "revisa los comandos de entrada"

fi
