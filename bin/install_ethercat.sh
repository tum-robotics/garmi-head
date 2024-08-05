#!/bin/bash

# Check if the correct number of arguments are passed
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <master0_device> <device_modules>"
    exit 1
fi

# Assign command-line arguments to variables
master0_device="$1"
device_modules="$2"

sudo apt-get install -y git cmake build-essential autoconf libtool kmod

git clone https://gitlab.com/etherlab.org/ethercat.git
cd ethercat || return
git checkout stable-1.5

chmod +x bootstrap
./bootstrap
./configure --disable-8139too --with-linux-dir=/usr/src/linux-headers-"$(uname -r)"
make modules

sudo make modules_install install
sudo depmod

echo "Adding ethercat group..."
sudo groupadd ethercat
sudo usermod -aG ethercat "$(id -un)"

echo "Updating ethercat.conf..."
sudo sed -i "s/\(MASTER0_DEVICE=\).*/\1\"$master0_device\"/" "/etc/ethercat.conf"
sudo sed -i "s/\(DEVICE_MODULES=\).*/\1\"$device_modules\"/" "/etc/ethercat.conf"
sudo cp /etc/ethercat.conf /usr/local/etc/ethercat.conf

limits_path="/etc/security/limits.conf"
limits="
@ethercat              soft    rtprio          99
@ethercat              soft    priority        99
@ethercat              soft    memlock         102400
@ethercat              hard    rtprio          99
@ethercat              hard    priority        99
@ethercat              hard    memlock         102400"

echo "Adding ethercat group to limits.conf..."
if grep -qF "@ethercat" "$limits_path"; then
    echo "limits.conf already contains ethercat group."
else
    echo "$limits" | sudo tee -a "$limits_path" >/dev/null
fi

echo "Setting device access permissions..."
sudo sh -c 'echo KERNEL==\"EtherCAT[0-9]*\", MODE=\"0660\", GROUP=\"ethercat\" > /lib/udev/rules.d/99-EtherCAT.rules'
echo "Registering libraries with dynamic linker..."
sudo sh -c 'echo /usr/local/lib > /etc/ld.so.conf.d/etherlab.conf'
sudo ldconfig

echo "Done."
