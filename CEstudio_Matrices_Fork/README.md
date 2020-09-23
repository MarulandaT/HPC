# MULTIPLICANDO MATRICES UTILIZANDO MULTIPLES PROCESOS CREADOS CON EL LENGUAJE C. 

Con el fin de ejecutar de forma automática las diferentes implementaciones se han agregado dos scripts: 

-execute_fork.sh : Script para ejecutar la multiplicación de matrices utilizando procesos. 
-execute_sec.sh : Script para ejecutar la multiplicación de matrices de forma secuencial. 

Ambos scripts están configurados para ejecutar diez veces el código de procesos o el secuencial multiplicando matrices cuadradas de dimensión 10, 100, 250, 500, 1000 2000 y 3000 respectivamente. 

Para ejecutar el script secuencial en sistemas linux: 
bash compile run auto clean MatMulSecuencial 
y se obtendrá como salida en pantalla la cantidad de milisegundos consumidos para la multiplicación en forma secuencial variando el tamaño de la dimensión. 

Para ejecutar el script por procesos en sistemas linux: 
bash compile run auto clean MatMulFork 
y se obtendrá como salida un archivo llamado "time.csv" donde estará guardada la relación entre el número de ejecución, la dimensión de las matrices cuadradas multiplicadas y el tiempo consumido en milisegundos. 

Colaboradores: 
Angel David Santa Giraldo 
Samir Mateo Soto Buitrago 
Luis Miguel Marulanda Torres 