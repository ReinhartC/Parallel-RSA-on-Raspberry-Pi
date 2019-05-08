/etc/hostname
#change to desired hostname#

/etc/hosts
#change hostname and add all nodes' ip and hostname#

/etc/network/interfaces
#backup original file and add a new one with this#

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