#!/bin/bash

cp -r /home/vagrant/jags-cddm /tmp/

cd /tmp/jags-cddm

autoreconf -fvi && ./configure --prefix=/usr

make && sudo make install

echo 'load cddm' | jags
