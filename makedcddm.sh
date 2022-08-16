#!/bin/bash

autoreconf -fvi && ./configure --prefix=/usr

make && sudo make install
