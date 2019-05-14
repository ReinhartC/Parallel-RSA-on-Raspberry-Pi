# Parallel RSA on Raspberry Pi
***Informatics Final Project assignment***

## Project Description
The main purpose of this project is to research the best method to decrease processing time of a process through parallel computing approach. The case study that is used in this project is the RSA Encryption and Decryption with Raspberry Pi Cluster as the device.

The implementation that is used in this project is using OpenMP for the multithreading and MPI for the parallelization of the processes with the goal of decreasing the processing time compared to the one without said implementation on the same device.

The project can be seen in [Builds](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Builds "Builds") folder which contains the implementations and details for the RSA Encryption and Decryption builds.

## Project Implementation
### Tools
+ Raspberry Pi 3 *(I used 3 pieces of Model B 2015 in this project)*
+ SD Cards
+ Router or Switch
+ Ethernet Cables

### Setting Up
Make sure every Raspberry Pi is:
+ Running Linux based OS *(I used Raspbian in this project)*
+ SSH is enabled
+ Filesystem is Expanded
+ Bloatwares are uninstalled *(Optional to free up space. **Highly reccomended** for 8GB SD Cards)*
+ Automatically boots to CLI *(Optional for faster boot)*

To set up the cluster, simply connect each Raspberry Pi with each other through a router or a switch. Then head on to the [Configurations](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Configurations) folder which contains a tutorial to configure the Raspberry Pis to be headless and also a detailed tutorial to create a Parallel Raspberry Pi Cluster with MPICH.

<br><br>
<p align="center">
    <a>
    	Author
    </a>  
</p>
<p align="center">
    <a>
        <b>Reinhart Caesar - 0511540000132<b>
    </a>  
</p>
