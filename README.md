# JAGS-CDDM: Circular Drift Diffusion Model in JAGS

Implements the circular drift diffusion model in JAGS.

## Installation

1. Clone the repository:

```{bash}
git clone https://github.com/joachimvandekerckhove/jags-cddm.git
```

2. Navigate to the cloned directory and execute the installation script:

```{bash}
./makedcddm.sh
```

## Usage

In JAGS, `load cddm` as a module.  This exposes two new distributions:

* For the Cartesian parameter set:

```{r}
X[1:2,] ~ dcddmcartn(driftx, drifty, bound, nondecision)
```

* For the polar parameter set:

```{r}
X[1:2,] ~ dcddmpolar(driftLength, driftAngle, bound, nondecision)
```

In both cases, `X[1,]` is choice in radians and `X[2,]` is RT in seconds.


## Pre-release

This is an untested pre-release.  Do not use unless you know what you are doing
and know how to evaluate the accuracy of results.


## Ongoing work

1. Implement and test an efficient and robust sampler [in progress]
2. Implement and test the more robust likelihood calculation in Smith, Garrett, and Zhou
   (2023; _Computational Brain & Behavior_, 1-13)
3. Implement and test integrated trial-to-trial variability as discussed in Zhou, Osth,
   Lilburn, and Smith (2021; _Psychonomic Bulletin & Review, 28,_ 1112-1130).



## Citation

If you use this module, please cite:

    Villarreal, M., Chávez, A., Mistry, P. K., Menon, V., Vandekerckhove, J., &
      Lee, M. D. (2023, August 8). Bayesian graphical modeling with the circular
      drift diffusion model. https://doi.org/10.31234/osf.io/gjnwk

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
