# Raspberry Pi Nodes NFS Configuration
**Do the following on each respecting nodes**

## Setting up NFS Kernel Server
Set up NFS kernel server on the **master node** with the following.
```
sudo apt-get -y install nfs-kernel-server
```

### Shared folder setup
Create a shared folder on the **master node** which will be accessible to the slave nodes with the following.
```
cd ~
mkdir FOLDERNAME
```
Then, do `cat /etc/exports` and then `sudo nano /etc/exports` and add at the following line to the end of the file 
```
/home/pi/FOLDERNAME *(rw,sync,no_root_squash,no_subtree_check)
```
After that, change the ownership of the folder with the following.
```
sudo chmod 777 /home/pi/FOLDERNAME
```

**Note**

Change `FOLDERNAME` with a desired name for the shared folder

### NFS service startup
Do the following on **master node** to start up the NFS kernel server service.
```
sudo update-rc.d rpcbind enable
sudo service rpcbind restart
sudo exportfs -a
sudo service nfs-kernel-server restart
```

## Accessing the shared folder
### Shared folder target setup
On the **slave nodes**, do the following to create a folder to be the shared folder mount.
```
cd ~
mkdir ParallelRSA
sudo mount -t nfs master_node:/home/pi/FOLDERNAMESOURCE ./FOLDERNAMETARGET
```
Check with `df -h` to see whether or not the shared folder has been mounted

To create an automatic mount after boot (only if the shared folder source is active), edit `sudo nano /etc/fstab` and add the following at the end of the file
`SOURCEHOSTNAME:/home/pi/FOLDERNAMESOURCE /home/pi/FOLDERNAMETARGET nfs rw 0 0`

**Note**

`FOLDERNAMESOURCE` is the shared folder name on the source node, `FOLDERNAMETARGET` is the folder name of the shared folder mount folder on the node (To avoid confusion, the `FOLDERNAMESOURCE` and `FOLDERNAMETARGET` is better to be the same).

`SOURCEHOSTNAME` is the IP address or hostname of the node that provides the shared folder

<p align="center">
	<a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Configurations/SSH.md">
		<b>← Return to SSH configuration</b>
	</a>  
	<a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/blob/master/Configurations/MPICH.md">
		<b>Continue to MPICH configuration →</b>
	</a>
</p>
<p align="center">
	<a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Configurations">
		Return to main config page
	</a>  
</p>