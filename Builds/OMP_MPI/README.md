# RSA OpenMP and MPI Hybrid Build
*Parallel RSA Encryption and Decryption with OpenMP and MPI Hybrid Implementation*
***(CURRENTLY IN PROGRESS)***

## Description
This RSA Encryption and Decryption build works on parallel using the MPI implementation, but also do the processes with multithreading. Pretty much the combination of OpenMP and MPI build. The idea of this build divide the load that is given from the input file onto each nodes, then process the divided load on parallel on every node. On the process of encrypting or decrypting every line, the processing for the characters are done with multithreading. Then, the result of the process from every node is merged back.

#### The processes are:
1. Loading the keys from the key file (*Public key* on encryption and *Private key* on decryption)
2. Loading the input file that will be processed (*Plaintext* on encryption and *Ciphertext* on decryption)
3. Divide the load from the input file to every node in the cluster
4. Encrypt/Decrypt the given lines from the input file on parallel on every node, which the processing of the characters in every line is done in multithread.
5. Merge the result and print it to an output file

<p align="center">
	<a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/MPI">
		<b>← MPI build</b>
	</a>  
</p>
<p align="center">
    <a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds">
        Return to Builds
    </a>  
</p>