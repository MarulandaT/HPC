# MULTIPLICANDO MATRICES DE FOMA SECUENCIAL Y PARALELA EN C. 

Para ejecutar correctamente el código se necesita seguir los siguientes pasos: 

1. Editar el script de python "gen_archivo.py" en la línea 2 para ajustar el parametro n a la dimensión deseada de matrices cuadradas para multiplicar, es decir, si n = 2000 entonces se generarán dos matrices de tamaño 2000 x 2000. 

2. Ejecutar el script de python "gen_archivo.py" con el comando python3 gen_archivo.py. 

3. Verificar que en el directorio del proyecto se haya creado el archivo "matrices.txt", el cual contiene las dos matrices que serán multiplicadas. 

4. Editar el código MatMulSecuencial.c en la línea 10 con el fin de específicar al lenguaje C las dimensiones de las matrices a multiplicar. 

5. Compilar el código MatMulSecuencial.c utilizando sistemas linux a través del comando: 
$ gcc -osecuencial MatMulSecuencial.c

6. Ejecutar el compilado que se ha generado en un sistema linux utilizando el comando: 
$ ./secuencial
Posterior a esta ejecución se visualiza en pantalla el tiempo empleado por el algoritmo secuencial en milisegundos. 
Para el caso en el que se quiera comprobar si la multiplicación se hizo correctamente, es necesario ubicar el ficheo "resultadosecuencial.txt" en la carpeta donde se almacenó el proyecto. 

7. Editar el código MatMulThread.c en la línea 13 con el fin de específicar al lenguaje C las dimensiones de las matrices a multiplicar y modificar también la línea 12 con el fin de especificar la cantidad de hilos para ejecutar el código. 

8. Compilar el código MatMulThreads.c utilizando sistemas linux a través del comando: 
$ gcc -othread -pthread MatMulThreads.c

9. Ejecutar el compilado que se ha generado en un sistema linux utilizando el comando: 
$ ./thread
Posterior a esta ejecución se visualiza en pantalla el tiempo empleado por el algoritmo secuencial en milisegundos. 
Para el caso en el que se quiera comprobar si la multiplicación se hizo correctamente, es necesario ubicar el ficheo "resultadothreads.txt" en la carpeta donde se almacenó el proyecto. 

10. Finalmente, con la información obtenida es posible hacer análisis de speedup. 

