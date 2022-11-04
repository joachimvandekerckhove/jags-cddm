#!/bin/bash

cp -r ../jags-cddm /tmp/

cd /tmp/jags-cddm

autoreconf -fvi && ./configure --prefix=/usr

make && sudo make install

sudo cp /usr/lib/JAGS/modules-4/cddm.* /usr/lib/x86_64-linux-gnu/JAGS/modules-4/ #todo
