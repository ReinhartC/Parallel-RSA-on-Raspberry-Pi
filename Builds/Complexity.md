# RSA Encryption and Decryption Builds Complexity
*These are the RSA Encryption and Decryption Builds Run Complexity*

+ **Serial** `(line)*(char)*p`
+ **OMP** `omp+((line)*(char/c)*p)`
+ **MPI** `mpi+((line/n)*(char)*p)`
+ **OMP_MPI** `mpi+omp+((line/n)*(char/c)*p)`

**Note**
+ `line` is the amount of lines on input
+ `char` is the amount of characters on each line
+ `p` is the process (Encryption/Decryption)
+ `n` is the node amount
+ `c` is the core amount on each node
+ `omp` is the OpenMP Operations (Pragma omp parallel)
+ `mpi` is the MPI Operations (Brodcast, Send, Receive)

<p align="center">
    <a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds">
        Return to Builds
    </a>  
</p>