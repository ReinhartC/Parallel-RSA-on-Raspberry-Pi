# Raspberry Pi Nodes SSH Configuration
***(Do the following on each respecting nodes)***

### SSH key creation
Create the SSH key for the node on **every node**
```
ssh-keygen
```
After key is successfully created, copy the key to a file with the node's hostname:
```
cd ~/.ssh
cp id_rsa.pub NODENAME
```
**Note**
+ `NODENAME` is the hostname for the node. *E.g. `master_node`*

### Nodes key authorizing
On each of the **slave nodes**, do the following to authorize the master node's SSH key to grant master node's connection access.
```
cd ~/.ssh
scp master_node:/home/pi/.ssh/master_node .
cat master_node>>authorized_keys
```

Then, on the **master node**, authorize the key of every slave nodes to grant connection access for every slave nodes.

```
cd ~/.ssh
scp slave_node_X:/home/pi/.ssh/slave_node_X .
cat slave_node_X>>authorized_keys
```
**Note**
+ Do the process above for every slave nodes and replace the `X` on the `slave_node_X` to the hostname of each of the slave node

<p align="center">
	<a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Configurations/Connections.md">
		<b>← Return to connection configuration</b>
	</a>  
	<a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/blob/master/Configurations/NFS.md">
		<b>Continue to NFS configuration →</b>
	</a>   
</p>
<p align="center">
	<a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Configurations">
		Return to main config page
	</a>  
</p>