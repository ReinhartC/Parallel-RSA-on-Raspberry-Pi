# Raspberry Pi Nodes SSH Configuration
**Do the following on each respecting nodes**

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
`NODENAME` is the hostname for the node. *E.g. `master_node`*

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
do this for every slave nodes

**Note**

Replace the `X` on the `slave_node_x` to the hostname of the slave node

[**← Return to main config page**](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Configurations "Configurations")
[**Continue to NFS configuration →**](https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/blob/master/Configurations/NFS.md "NFS config")