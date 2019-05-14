# RSA MPI Build
*Parallel RSA Encryption and Decryption with MPI Implementation*

## Description
This RSA Encryption and Decryption build works on parallel using the MPI implementation. The idea of this build divide the load that is given from the input file onto each nodes, then process the divided load on parallel on every node. Then, the result of the process from every node is merged back.

#### The processes are:
1. Loading the keys from the key file (*Public key* on encryption and *Private key* on decryption)
2. Loading the input file that will be processed (*Plaintext* on encryption and *Ciphertext* on decryption)
3. Divide the load from the input file to every node in the cluster
4. Encrypt/Decrypt the given lines from the input file on parallel on every node.
5. Merge the result and print it to an output file

<p align="center">
	<a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/OMP">
		<b>← OpenMP Build</b>
	</a> 
	<a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/OMP_MPI">
		<b>OpenMP and MPI Hybrid Build →</b>
	</a>
</p>
<p align="center">
    <a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds">
        Return to Builds
    </a>  
</p>