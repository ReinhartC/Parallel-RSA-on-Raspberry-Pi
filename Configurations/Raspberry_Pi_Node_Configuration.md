# Raspberry Pi Nodes Configuration
## Node connection config

**Make changes to these files**

#### `/etc/hostname`
#change to desired hostname#

/etc/hosts
#change add all nodes' ip and hostname#

/etc/network/interfaces
#fill with this#
auto lo
iface lo inet loopback

auto eth0
allow-hotplug eth0
iface eth0 inet static
address 192.168.0.51 #adjust address#
netmask 255.255.255.0

auto wlan0
allow-hotplug wlan0
iface wlan0 inet static
wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf
address 192.168.1.101 #adjust address#
netmask 255.255.255.0
broadcast 192.168.1.255
gateway 192.168.1.1

iface default inet dhcp
#eof#

sudo raspi-config
#set to boot to cli logged in for faster boot time#


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

==CREATE SSH TO EACH NODE FOR AUTOLOGIN==
#on master_node#
ssh-keygen
cd ~/.ssh
cp id_rsa.pub master_node

#on slave_node_1#
ssh-keygen
cd ~/.ssh
cp id_rsa.pub slave_node_1
scp master_node:/home/pi/.ssh/master_node .
cat master_node>>authorized_keys

#on slave_node_2#
ssh-keygen
cd ~/.ssh
cp id_rsa.pub slave_node_2
scp master_node:/home/pi/.ssh/master_node .
cat master_node>>authorized_keys

#back on master_node#
cd ~/.ssh
scp slave_node_1:/home/pi/.ssh/slave_node_1 .
cat slave_node_1>>authorized_keys
scp slave_node_2:/home/pi/.ssh/slave_node_2 .
cat slave_node_2>>authorized_keys


==INSTALL NFS KERNEL SERVER (FOR CLUSTER SHARE FOLDER)==
sudo apt-get -y install nfs-kernel-server
cd ~
mkdir ParallelRSA
cat /etc/exports
sudo nano /etc/exports
#add at the end of the file /home/pi/ParallelRSA *(rw,sync,no_root_squash,no_subtree_check)#
sudo chmod 777 /home/pi/ParallelRSA

#config and service restart#
sudo update-rc.d rpcbind enable
sudo service rpcbind restart
sudo exportfs -a
sudo service nfs-kernel-server restart

==CONFIGURE SHARED FOLDER ON SLAVE NODES==
#on slave_node_1 and slave_node_2#
cd ~
mkdir ParallelRSA
sudo mount -t nfs master_node:/home/pi/ParallelRSA ./ParallelRSA
#check with#
df -h
sudo nano /etc/fstab
#at end of file add master_node:/home/pi/ParallelRSA /home/pi/ParallelRSA nfs rw 0 0#

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