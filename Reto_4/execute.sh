#!/bin/bash

# Command for compile c mat with openMP + MPI
# bash execute.sh compile run
compile='compile' # compile file 
run='run' # run file 
#if you use both can compile and run
args=("$@")

exec(){
	for trys in {1..2}
	do
		for msize in 32 64 128 
        do
            for wnodes in 1 4
            do
                for numthreads in 1 4
                do
                    export OMP_NUM_THREADS=$numthreads
                    mpirun -np $wnodes -machinefile mfile ./run $msize
                done
            done
            export OMP_NUM_THREADS=8
            mpirun -np 4 -machinefile mfile ./run $msize
		done
	done
}

echo " ${args[0]} ${args[1]} "

if [ "${args[0]}" == "$compile" ] &&  [ "${args[1]}" == "$run" ];
then
	mpicc -fopenmp MMOpenMPI.c -o run
	exec
    echo "success"
else
	echo "revisa los comandos de entrada"

fi
