# Raspberry Pi Nodes Connection Configuration
***(Make changes to these files on every nodes)***

### Changing node hostname and known hosts

#### `/etc/hostname`
Change to desired hostname

*E.g.*
```
master_node
```

#### `/etc/hosts`
Change the hostname to the new hostname and add all the other nodes' IP along with the configured hostname

*E.g.*
```
127.0.0.1       localhost
::1             localhost ip6-localhost ip6-loopback
ff02::1         ip6-allnodes
ff02::2         ip6-allrouters

127.0.1.1       master_node
192.168.0.51    slave_node_1
192.168.0.52    slave_node_2
```

### Setting up static IP and allowing the node for multiple connection

#### `/etc/network/interfaces`
Backup the original file and create a new one with the following.
```
auto lo
iface lo inet loopback

auto eth0
allow-hotplug eth0
iface eth0 inet static
address X1.Y1.Z1.NE
netmask 255.255.255.0

auto wlan0
allow-hotplug wlan0
iface wlan0 inet static
wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf
address X2.Y2.Z2.NW
netmask 255.255.255.0
broadcast X2.Y2.Z2.255
gateway X2.Y2.Z2.1

iface default inet dhcp
```
**Note**
+ `X1.Y1.Z1` is the subnet to your ethernet connection and `NE` is the preferred static address for the node on your ethernet network. *E.g. `192.168.0.50`*.

+`X2.Y2.Z2` is the subnet to your wireless connection and `NW` is the preferred static address for the node on your wireless network. *E.g. `192.168.1.101`*.

#### `/etc/wpa_supplicant/wpa_supplicant.conf`
Change this file with the following to set the node to automatically connect to the desired wireless network on every boot.
```
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
country=US

network={
        ssid="NETWORKSSID"
        psk="PASSKEY"
        scan_ssid=1
        proto=RSN
        key_mgmt=WPA-PSK
        pairwise=CCMP TKIP
        group=CCMP TKIP
        id_str="home"
        priority=5
}
```

`NETWORKSSID` is your wireless network name and `PASSKEY` is the password for the network

<p align="center"> 
	<a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/blob/master/Configurations/SSH.md">
		<b>Continue to SSH configuration →</b>
	</a>   
</p>
<p align="center">
	<a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Configurations">
		Return to main config page
	</a>  
</p>