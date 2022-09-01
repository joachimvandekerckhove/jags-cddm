# -*- mode: ruby -*-
# vi: set ft=ruby :


user = "vagrant"
if ENV['USERNAME']
  user = ENV['USERNAME']
end

email = ""
if ENV['USEREMAIL']
  email = ENV['USEREMAIL']
end

Vagrant.configure("2") do |config|

  config.vm.box = "ubuntu/focal64"
  config.vm.hostname = "cddm-haver"
  config.vm.define "cddm-haver"
  config.vm.synced_folder ".", "/srv/host/"
  
  config.vm.provider "virtualbox" do |vb|
    vb.memory = "2048"
    vb.cpus = 2
  end
  
  config.vm.provision "shell",
    args: ["#{user}", "#{email}"],
    privileged: false,
    inline: <<-'SHELL'
  
      DEBIAN_FRONTEND=noninteractive


      echo === Install packages ===

      sudo apt-get --assume-yes update
      sudo apt-get --assume-yes upgrade

      sudo apt-get install \
          r-cran-rjags \
          git \
          make build-essential autoconf \
          g++ f2c gfortran \
          libblas-dev liblapack-dev libpcre3-dev libreadline-dev libgsl-dev \
          libtool* --assume-yes
  
  
      echo === Set up github ===
      
      # Obtain the user's key for github
      HOME=/home/vagrant
      mkdir -p $HOME/.ssh/
      cp /srv/host/id_rsa $HOME/.ssh/

      # Configure git
      cat <<- EOF > $HOME/.ssh/config
	Host github.com
	  User git
	  Hostname github.com
	  IdentityFile ~/.ssh/id_rsa
      EOF

      chmod 600 ~/.ssh/id_rsa
      git config --global user.name "$1"
      git config --global user.email "$2"

      # Trust github
      ssh-keyscan github.com >> $HOME/.ssh/known_hosts

      
      echo === Install CDDM module ===
      
      GDIR=$HOME/jags-module
      git clone http://github.com/joachimvandekerckhove/jags-cddm $GDIR
      cd $GDIR && \
          ./makedcddm.sh
          
          
      echo === Download examples ===
      
      GDIR=$HOME/cddm-sampleFiles
      git clone https://github.com/Adrifelcha/cddm-sampleFiles $GDIR
      
      
      echo === Testing CDDM module ===
      
      echo 'load cddm' | jags
      git config -l
      
      
      echo === Convenience aliases ===
      
      echo "alias sl='ls -ltr'" >> ~/.bashrc
      echo "alias scx='screen -x'" >> ~/.bashrc
      
  SHELL
end
