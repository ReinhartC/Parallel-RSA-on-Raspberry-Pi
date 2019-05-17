# RSA Encryption and Decryption on Raspberry Pi Builds
*The builds for RSA Encryption and Decryption*

## Description
Every builds execute the same main processes, but only differs in the job distribution to processor(s) or node(s) of some of the processes that consume a lot of time and processing power.

#### For every build, the main or key processes are:
1. Loading the keys from the key file (*Public key* on encryption and *Private key* on decryption)
2. Loading the input file that will be processed (*Plaintext* on encryption and *Ciphertext* on decryption)
3. Encrypt/Decrypt the given lines from the input file
4. Print the result to an output file

## Builds
+ [Serial](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/Serial "Serial build") build is the RSA encryption and decryption implementation that is executed in a *single processor* on a *single node*
+ [OMP](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/OMP "OMP build") build is the RSA encryption and decryption implementation that is executed with *multiple processors* on a *single node* with the **OpenMP**
+ [MPI](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/MPI "MPI build") build is the RSA encryption and decryption implementation that is executed with a *single processor* on *multiple nodes* in a parallel cluster with the **MPI**
+ [OMP_MPI](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/OMP_MPI "OMP_MPI build") build is the RSA encryption and decryption implementation that is executed with *multiple processors* on *multiple nodes* in a parallel cluster with the **OpenMP and MPI Hybrid**
+ [LB_OMP_MPI](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/LB_OMP_MPI "LB_OMP_MPI build") build is the RSA encryption and decryption implementation that is executed with *multiple processors* on *multiple nodes* in a parallel cluster with the **OpenMP and MPI Hybrid**, that used a load balancing in the **MPI** work distribution.

### Job Distribution
Build | Job distribution
----- | -----
**Serial** | `(line)*(char)*p`
**OMP** | `(line)*(char/c)*p`
**MPI** | `(line/n)*(char)*p`
**OMP_MPI** | `(line/n)*(char/c)*p`

**Note**
- `line` is the amount of lines on input
- `char` is the amount of characters on each line
- `p` is the process (Encryption/Decryption)
- `n` is the node amount
- `c` is the core amount on each node
- The OpenMP Operations such as `Pragma omp parallel` is negligible to the performance
- The MPI Operations such as `Brodcast`, `Send`, `Receive` are negligible to the performance

### Runtime Comparison
+ Time comparison between the builds above could be seen [here](http://tiny.cc/RSABuildsTimeComparison "RSA Builds Time Comparison")
+ Time comparison for Load Balancing on **OpenMP and MPI Hybrid** Implementation could be seen [here](http://tiny.cc/RSALoadBalanceTimeCompare "RSA OpenMP and MPI Hybrid Load Balance Time Comparison")

## Other
+ [~Inputs](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/~Inputs "~Inputs") are some of the sample inputs for testing on some different sizes
+ [~Keys](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/~Keys "~Keys") are the keys that are used in the encryption and decryption process, as well as a key generator to generate a new pair of keys

<p align="center">
    <a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master">
        Return Home
    </a>  
</p>
