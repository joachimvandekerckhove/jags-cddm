# JAGS-CDDM

Implements the circular drift diffusion model in JAGS.

`git clone`, and 1./makedcddm.sh1 should install the module.

In JAGS, `load cddm` as a module.

`X[1:2,] ~ dcddm(drift, bound, ter0, theta0)` where `X[1,]` is choice in radians and `X[2,]` is RT in seconds.

## Vagrantfile

If you use Vagrant, the Vagrantfile spawns a VM that installs and tests this module.

## Pre-release

This is an untested pre-release.  Do not use unless you know what you are doing and how to evaluate the accuracy of results.
