# Raspberry Pi Nodes NFS Configuration
**Do the following on each respecting nodes**

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

[<- Return to main config page](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Configurations "Configurations")