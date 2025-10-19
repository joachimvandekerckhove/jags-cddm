#!/usr/bin/env Rscript

# Test Von Mises distribution using Rcpp

# Load Rcpp
library(Rcpp)

# Source the C++ code
sourceCpp("rvonmises.cpp")

# Function to plot histogram with theoretical PDF
plot_vonmises_test <- function(sample, mu, kappa, title) {
  hist(sample, breaks = 50, freq = FALSE, 
       main = title,
       xlab = "Angle (radians)", ylab = "Density",
       col = "lightblue", border = "black")
  
  x <- seq(-pi, pi, length.out = 1000)
  pdf <- exp(kappa * cos(x - mu)) / (2 * pi * besselI(kappa, 0))
  lines(x, pdf, col = "red", lwd = 2)
  
  legend("topright", legend = c("Sample", "Theoretical PDF"), 
         col = c("lightblue", "red"), lty = c(1, 1), lwd = c(1, 2))
}

# Main execution
main <- function() {
  mu <- runif(1, -pi, pi)
  kappa <- runif(1, 0, 10)
  n <- 100000
  
  cat("Generating VonMises(mu =", round(mu, 3), ", kappa =", round(kappa, 3), ")\n")
  
  sample <- rvonmises_rcpp(n, mu, kappa)
  
  outfile <- "vonmises_test.pdf"
  pdf(outfile, width = 8, height = 6)
  title <- paste("VonMises(mu =", round(mu, 3), ", kappa =", round(kappa, 3), ")")
  plot_vonmises_test(sample, mu, kappa, title)
  dev.off()
  
  z <- exp(1i * sample)
  R <- mean(z)
  mu_est <- Arg(R)
  rho <- Mod(R)
  kappa_est <- rho * (2 - rho^2) / (1 - rho^2)
  
  cat("Plot saved to", outfile, "\n")
  cat("Sample size:", length(sample), "\n")
  cat("True: mu =", round(mu, 3), ", kappa =", round(kappa, 3), "\n")
  cat("Est:  mu =", round(mu_est, 3), ", kappa =", round(kappa_est, 3), "\n")
}

if (!interactive()) {
  main()
}
