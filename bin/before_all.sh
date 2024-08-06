#!/bin/bash

yum install -y wget
wget $TUM_HEAD
yum localinstall -y tum_head-2.1.0-2.x86_64.rpm

yum install -y kernel-devel
export ACLOCAL_PATH=/usr/share/aclocal
git clone https://gitlab.com/etherlab.org/ethercat.git
cd ethercat || return
git checkout stable-1.5
chmod +x bootstrap
./bootstrap
./configure --disable-8139too --with-linux-dir=/usr/src/kernels/3.10.0-1160.119.1.el7.x86_64
make modules
make modules_install install
