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
  config.vm.network "forwarded_port", guest: 8787, host: 8788

  config.vm.provider "virtualbox" do |vb|
    vb.memory = "2048"
    vb.cpus = 5
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
      
      cd $HOME
      git clone git@github.com:joachimvandekerckhove/jags-cddm.git
      cd jags-cddm && ./makedcddm.sh


      echo === Install von Mises module ===

      cd /tmp
      git clone git@github.com:yeagle/jags-vonmises.git
      cd jags-vonmises && \
          autoreconf -fvi && \
          ./configure --prefix /usr && \
          make && \
          sudo make install
          sudo cp /usr/lib/JAGS/modules-4/vonmises.* /usr/lib/x86_64-linux-gnu/JAGS/modules-4/ #todo
          
          
      echo === Download examples ===
      
      cd $HOME
      git clone git@github.com:Adrifelcha/cddm-sampleFiles.git
      
      
      echo === Convenience aliases ===
      
      echo "alias sl='ls -ltr'" >> ~/.bashrc
      echo "alias scx='screen -x'" >> ~/.bashrc


      echo === Install RStudio Server ===

      cd /tmp
      wget https://download2.rstudio.org/server/bionic/amd64/rstudio-server-1.3.1093-amd64.deb
      sudo apt-get install ./rstudio-server-1.3.1093-amd64.deb --assume-yes
      sudo R -e "install.packages('R2jags')"
      sudo R -e "install.packages('magrittr')"
      sudo R -e "install.packages('readr')"
      sudo R -e "install.packages('posterior')"


      echo === Testing ===

      echo 'load cddm' | jags
      echo 'load vonmises' | jags
      git config -l

  SHELL
end
