# JAGS-CDDM

Implements the circular drift diffusion model in JAGS.

`git clone`, then `./makedcddm.sh` should install the module.

In JAGS, `load cddm` as a module.  This exposes two new distributions:

Use `X[1:2,] ~ dcddmcartn(driftx, drifty, bound, nondecision)` for the
Cartesian parameter set.

Use `X[1:2,] ~ dcddmpolar(driftLength, driftAngle, bound, nondecision)` for the
polar parameter set.

In both cases, `X[1,]` is choice in radians and `X[2,]` is RT in seconds.


## Pre-release

This is an untested pre-release.  Do not use unless you know what you are doing
and how to evaluate the accuracy of results.


## Citation

When using this module, please cite:

    Wabersich, D., & Vandekerckhove, J. (2014). Extending JAGS: A
      tutorial on adding custom distributions to JAGS (with a
      diffusion model example). Behavior Research Methods, 46, 15-28.


## Vagrantfile

If you use Vagrant, the Vagrantfile spawns a VM that installs and tests this
module.  Currently requires a GitHub account with associated keyfile.


## Copyleft

This module is Copyright (C) 2023 Joachim Vandekerckhove <joachim@uci.edu>
and Adriana Chávez De la Peña <achavezd@uci.edu>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
