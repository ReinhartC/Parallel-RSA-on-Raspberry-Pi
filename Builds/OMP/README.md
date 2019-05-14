# RSA OpenMP Build
*Multithreaded RSA Encryption and Decryption with OpenMP Implementation*

## Description
This RSA Encryption and Decryption build works on multithread using the OpenMP implementation. The idea of this build is to process the input file which in every line, the encryption or decryption process for the characters in the line is done with multithreading, creating a faster processing on every line.

#### The processes are:
1. Loading the keys from the key file (*Public key* on encryption and *Private key* on decryption)
2. Loading the input file that will be processed (*Plaintext* on encryption and *Ciphertext* on decryption)
3. Encrypt/Decrypt the given lines from the input file with multithread on every line.
4. Print the result to an output file


<p align="center">
	<a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/Serial">
		<b>← Serial Build</b>
	</a>  
	<a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds/MPI">
		<b>MPI Build →</b>
	</a>  
</p>
<p align="center">
    <a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds">
        Return to Builds
    </a>  
</p>