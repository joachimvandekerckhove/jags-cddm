# Functions to fit the EZ-CDM to data
#
#   Implementation of method described in
#      Qarehdaghi, H., & Rad, J. A. (2022). EZ-CDM for modeling continuous
#      decisions by everyone! Fast, simple, robust, and accurate estimation of
#      circular diffusion model parameters. https://doi.org/10.31234/osf.io/rzqhg
#   Original code found at:
#      https://github.com/HasanQD/EZ-CDM/blob/main/Simple%20EZ-CDM.ipynb

ezcdm.fit <- function(CA, RT) {
  ezcdm.parameters(ezcdm.stats(list(CA = CA, RT = RT)))
}

ezcdm.stats <- function(data) {
  
  CA <- data$CA
  RT <- data$RT
  
  N <- length(CA)
  
  list(
    MCA = atan2(sum(sin(CA))/N, sum(cos(CA))/N),
    VCA = 1 - 1/N*sqrt(sum(cos(CA))**2+sum(sin(CA))**2),
    MRT = mean(RT),
    VRT = var(RT)
  )
  
}

ezcdm.parameters <- function(par) {
  theta_v <- par$MCA
  R       <- 1 - par$VCA
  k0      <- R*(2-R**2)/(1-R**2)
  
  ivrat   <- besselI(k0, 1) / besselI(k0, 0)
  
  k1      <- k0 - (ivrat-R) / (1-ivrat**2-ivrat/k0)
  v       <- ((k1**2*R**2+2*k1*R-k1**2)/par$VRT)**(1/4)
  a       <- k1/v
  t0      <- par$MRT-a/v*R
  
  list(
    v       = v,
    theta_v = theta_v,
    a       = a,
    t0      = t0
  )
  
  c(theta_v, v, a, t0)
}

