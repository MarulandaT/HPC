import numpy as np
n= 4
A = np.random.randint(0,9,size = (n,n))
B = np.random.randint(0,9,size = (n,n))
C = np.matmul(A,B)
namein = "matext"+str(n)+"x"+str(n)+".txt"
nameres = "resultado"+str(n)+"x"+str(n)+".txt"
np.savetxt(namein,np.concatenate((A,B),0),fmt = "%d",delimiter = " ")
np.savetxt(nameres,C,fmt="%d",delimiter= " ")