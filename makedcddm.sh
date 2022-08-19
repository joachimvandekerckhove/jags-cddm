#!/bin/bash

autoreconf -fvi && ./configure

make && sudo make install
