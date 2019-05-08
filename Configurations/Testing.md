# Raspberry Pi Nodes Testing
**Do the following on master node**

==TESTING NODES==
#make sure the directory where the test file contains is accessible through all nodes#

nano machinefile
#insert ip address and desired processor to use in parallel for each nodes#

#1 cpi#
mpirun -f machinefile -n 3 ./cpi
#should output for each nodes#

#2 mpihello#
nano mpihello.cpp
#make mpi hello world and change stdio.h to iostream#
mpicxx -o mpihello mpihello.cpp
mpirun -f machinefile -n 3 ./mpihello

[<- Return to main config page](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Configurations "Configurations")