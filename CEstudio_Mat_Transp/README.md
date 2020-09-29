# MULTIPLICANDO MATRICES DE FOMA OPTIMIZADA EN C. 

Para ejecutar la optimización con matriz transpuesta y seguidamente con la optimización por compilación con -O1 -O2 y -O3 es necesario ejecutar el siguiente script: 

"bash execute.sh compile run auto clean Sec_Transp Thread_Transp Fork_Transp"

Este script funciona en sistemas operativos linux que cuenten con el compilador gcc/g++. 
Como salida de esta ejecución se obtiene la ejecución tras correr 10 veces el algoritmo de multiplicación de matrices con matrices cuadradas de dimensión 10, 100, 250, 500, 1000, 2000, 3000 teniendo en cuenta que una de las matrices estará transpuesta. 
Finalmente la salida obtenida por consola hace referencia a la relación entre dimensión de la matriz y tiempo de ejecución en milisegundos del respectivo caso de ejecución/optimización con matriz transpuesta.

Colaboradores: 
Angel David Santa Giraldo 
Samir Mateo Soto Buitrago 
Luis Miguel Marulanda Torres 