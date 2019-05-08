# RSA Encryption and Decryption MPI Build
*This is the RSA Encryption and Decryption with MPI Parallel*

### Description
> This build utilizes single core processing from each of the nodes in a parallel cluster. It divides the load from the input file onto multiple nodes and process each of the load that is given onto each of the nodes with a single core processing.

The main processes are:
1. Loading the keys (Public key on encryption and private key on decryption)
2. Open the files that will be processed (Plaintext on encryption and ciphertext on decryption)
3. Get how many lines of texts in the file *and distribute the load to each nodes*
4. Encrypt/decrypt all characters in every line that is given
5. Print the result to a file after every line

<p align="center">
    <a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds">
        Return to Builds
    </a>  
</p>