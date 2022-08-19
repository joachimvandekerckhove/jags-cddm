#!/bin/bash

autoreconf -fvi && ./configure --prefix=/usr

make && sudo make install

sudo cp /usr/lib/JAGS/modules-4/cddm.* /usr/lib/x86_64-linux-gnu/JAGS/modules-4/ #todo
