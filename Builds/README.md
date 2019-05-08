# RSA Encryption and Decryption on Raspberry Pi Builds
*These are the RSA Encryption and Decryption builds*

## Description
Every builds execute the same main processes, but only differs in the job distribution of some of the processes that is iterated a lot and uses a lot of processing consumption.

The main processes are:
1. Loading the keys (Public key on encryption and private key on decryption)
2. Open the files that will be processed (Plaintext on encryption and ciphertext on decryption)
3. Get how many lines of texts in the file
4. Encrypt/decrypt all characters in every line that is given
5. Print the result to a file after every line

## Builds
+ [Serial](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/Serial "Serial build") build is the RSA encryption and decryption implementation that is executed in a single processor on a single node
+ [OMP](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/OMP "OMP build") build is the RSA encryption and decryption implementation that is executed with multiple processor on a single node with the OpenMP
+ [MPI](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/MPI "MPI build") build is the RSA encryption and decryption implementation that is executed with a single processor on each node in a parallel cluster with the MPI (CURRENTLY IN PROGRESS)
+ [OMP_MPI](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/OMP_MPI "OMP_MPI build") build is the RSA encryption and decryption implementation that is executed with multiple processor on each node in a parallel cluster with the OpenMP and MPI Hybrid (CURRENTLY IN PROGRESS)

## Others
+ [~Inputs](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/~Inputs "~Inputs") are some of the sample inputs for testing in different sizes
+ [~Key](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/~Key "~Key") are the keys that are used in the encryption and decryption process, as well as a key generator

<p align="center">
    <a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master">
        Return Home
    </a>  
</p>
