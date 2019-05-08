# Raspberry Pi Nodes MPICH Configuration
**Do the following on every nodes**

## Installing MPICH
First, create a folder for the installation files.
```
mkdir mpich
cd mpich
```
Then, download the MPICH package.
```
wget http://www.mpich.org/static/downloads/3.1/mpich-3.1.tar.gz
```
Extract the package.
```
tar xfz mpich-3.1.tar.gz
```

After that, create a folder to install the MPICH.
```
sudo mkdir /home/rpimpi
sudo mkdir /home/rpimpi/mpi-install
```
Also create a folder to build the MPICH.
```
mkdir /home/pi/mpi-build
cd /home/pi/mpi-build
```
On the MPICH build folder, install fortran.
```
sudo apt-get -y install gfortran
```
Then, configure the MPICH to the target folder. (This'll took sometime)
```
sudo /home/pi/mpich/mpich-3.1/configure -prefix=/home/rpimpi/mpi-install
```
After the above is done, install the MPICH. (This'll took a lot of time. Make some coffee :coffee:)
```
sudo make
sudo make install
```

After the installation is done, go to the home directory of the pi (`/home/pi`) and do `sudo nano .bashrc` and add the following at the end of the file
```
PATH=$PATH:/home/rpimpi/mpi-install/bin
```

And then `sudo reboot` the pi.

## Testing the MPICH
Run these commands to check whether or not the MPICH is successfully installed.
```
which mpicc
which mpicxx
which mpirun
which mpiexec
```
If it returns the executables on `/home/rpimpi/mpi-install/...`, then the installation is successful.

Also run the following to make sure that the MPICH could work.
```
mpiexec -n 1 hostname
```
If it outputs the node's hostname, it means MPICH is working properly.

**Notes**
+ `mpicc` works for compiling C codes with MPI, while `mpicxx` works for compiling C++ codes with MPI
+ `mpirun` and `mpiexec` have the same function to execute MPI builds

More on the testing of the MPICH on multiple nodes could be seen in the [Node Testing](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/blob/master/Configurations/Testing.md "Testing") page.

<p align="center">
	<a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Configurations/NFS.md">
		<b>← Return to NFS configuration</b>
	</a>  
</p>
<p align="center">
	<a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Configurations">
		Return to main config page
	</a>  
</p>