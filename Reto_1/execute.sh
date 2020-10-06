#!/bin/bash

# Command for compile c++ transp mat mul and run 
# bash execute.sh compile run auto clean Sec_Transp Thread_Transp Fork_Transp
compile='compile' # compile file 
run='run' # run file 
#if you use both can compile and run
args=("$@")

auto_exec(){
	for i in {1..10}
	do
		for k in 1000 10000 100000 1000000 10000000
			do
			./"$1" $k
			done
	done
}

echo " ${args[0]} ${args[1]} ${args[2]} ${args[3]}"

if [ "${args[0]}" == "$compile" ] &&  [ "${args[1]}" == "$run" ];
then
	echo "Ejecucion secuencial NEEDLE"
    echo "Intentos Aciertos Miliseg Pi"
	g++ -o ${args[2]} ${args[2]}.cpp 
	auto_exec ${args[2]}
    
	echo "Ejecucion con hilos NEEDLE"
    echo "Intentos Aciertos Miliseg Pi"
	g++ -o ${args[2]}_thread ${args[2]}_thread.cpp -pthread
	auto_exec ${args[2]}_thread
    
	echo "Optimizacion O2 NEEDLE" 
    echo "Intentos Aciertos Miliseg Pi"
	g++ -o ${args[2]} -O2 ${args[2]}.cpp 
	auto_exec ${args[2]}

    echo "Ejecucion secuencial DARTBOARD"
    echo "Intentos Aciertos Miliseg Pi"
	g++ -o ${args[3]} ${args[3]}.cpp 
	auto_exec ${args[3]}
    
	echo "Ejecucion con hilos DARTBOARD"
    echo "Intentos Aciertos Miliseg Pi"
	g++ -o ${args[3]}_thread ${args[3]}_thread.cpp -pthread
	auto_exec ${args[3]}_thread
    
	echo "Optimizacion O2 DARTBOARD" 
    echo "Intentos Aciertos Miliseg Pi"
	g++ -o ${args[3]} -O2 ${args[3]}.cpp 
	auto_exec ${args[3]}

else 
	echo "revisa los comandos de entrada"

fi
