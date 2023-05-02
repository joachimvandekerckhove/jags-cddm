#!/bin/bash

cp -r ../jags-cddm /tmp/

cd /tmp/jags-cddm

autoreconf -fvi && ./configure --prefix=/usr

make && sudo make install

echo 'load cddm' | jags
