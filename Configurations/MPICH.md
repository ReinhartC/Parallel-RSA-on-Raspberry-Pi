# Raspberry Pi Nodes MPICH Configuration
**Do the following on every nodes**

==INSTALLING MPICH2==
(/home/pi)
mkdir mpich2
cd mpich2

(/home/pi/mpich2)
wget http://www.mpich.org/static/downloads/3.1/mpich-3.1.tar.gz
tar xfz mpich-3.1.tar.gz

sudo mkdir /home/rpimpi
sudo mkdir /home/rpimpi/mpi-install

mkdir /home/pi/mpi-build
cd /home/pi/mpi-build

==INSTALLING FORTRAN==
(/home/pi/mpi-build)
sudo apt-get -y install gfortran
sudo /home/pi/mpich2/mpich-3.1/configure -prefix=/home/rpimpi/mpi-install
sudo make
sudo make install
cd ..

(/home/pi)
sudo nano .bashrc
#Set at end of file PATH=$PATH:/home/rpimpi/mpi-install/bin#
sudo reboot

==TESTING INSTALLS==
which mpicc
which mpicxx
which mpirun
which mpiexec

mpiexec -n 1 hostname
#should output node hostname#

[<- Return to main config page](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Configurations "Configurations")