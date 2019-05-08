# RSA Encryption and Decryption OMP Build
*These are the RSA Encryption and Decryption with OMP Multithreading*

### Description
> This build utilizes multi core processing from one node. After the input file is loaded, processing of each line of the load is distributed onto multiple cores so the process runs on multithread.

The main processes are:
1. Loading the keys (Public key on encryption and private key on decryption)
2. Open the files that will be processed (Plaintext on encryption and ciphertext on decryption)
3. Get how many lines of texts in the file
4. Encrypt/decrypt all characters in every line that is given and do it on multith
5. Print the result to a file after every line

<p align="center">
    <a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds">
        Return to Builds
    </a>  
</p>