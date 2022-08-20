# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|

  config.vm.box = "ubuntu/focal64"
  config.vm.hostname = "cddm-haver"
  config.vm.define "cddm-haver"
  
  config.vm.provider "virtualbox" do |vb|
    vb.memory = "2048"
    vb.cpus = 2
  end
  
  config.vm.provision "shell", args: [], inline: <<-'SHELL'
  
      echo === Install packages ===

      DEBIAN_FRONTEND=noninteractive

      apt-get --assume-yes update
      apt-get --assume-yes upgrade

      apt-get install \
          jags \
          make build-essential autoconf \
          g++ f2c gfortran \
          libblas-dev liblapack-dev libpcre3-dev libreadline-dev libgsl-dev \
          libtool* --assume-yes
          
      echo === Install CDDM module ===

      git clone http://github.com/joachimvandekerckhove/jags-cddm /tmp/jags-module
      cd /tmp/jags-module && \
          ./makedcddm.sh

      echo === Testing CDDM module ===

      echo 'load cddm' | jags
      
  SHELL

end
