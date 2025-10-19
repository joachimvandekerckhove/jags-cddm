#include <Rcpp.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace Rcpp;

// Define M_PI if it's not available
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief Generates a random double in the range [0.0, 1.0).
 */
double next_double() {
    return (double)rand() / (RAND_MAX + 1.0);
}

/**
 * @brief Generates a random number from a standard normal (Gaussian) distribution.
 * Uses the Box-Muller transform.
 */
double random_standard_normal() {
    static double z1;
    static int generate = 0;
    generate = !generate;

    if (!generate) {
       return z1;
    }

    double u1, u2;
    do {
       u1 = next_double();
       u2 = next_double();
    } while (u1 <= 1e-7); // Avoid u1 being zero for log()

    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);
    return z0;
}

/**
 * @brief Generates a random sample from a von Mises distribution.
 * Based on the 1979 paper by D. J. Best and N. I. Fisher.
 */
double random_vonmises(double mu, double kappa) {
    double s;
    double U, V, W, Y, Z;
    double result, mod;

    if (std::isnan(kappa)) {
        return NAN;
    }

    // Handle small kappa approximation
    if (kappa < 1e-8) {
        return M_PI * (2 * next_double() - 1);
    }

    // Main algorithm by Best and Fisher (1979)
    if (kappa <= 1e6) {
        double r = 1 + sqrt(1 + 4 * kappa * kappa);
        double rho = (r - sqrt(2 * r)) / (2 * kappa);
        s = (1 + rho * rho) / (2 * rho);

        while (1) {
            U = next_double();
            Z = cos(M_PI * U);
            W = (1 + s * Z) / (s + Z);
            Y = kappa * (s - W);
            V = next_double();

            if ((Y * (2 - Y) - V >= 0) || (log(Y / V) + 1 - Y >= 0)) {
                break;
            }
        }

        U = next_double();
        result = acos(W);
        if (U < 0.5) {
            result = -result;
        }
    } else { // Fallback to wrapped normal for very large kappa
        result = sqrt(1.0 / kappa) * random_standard_normal();
    }

    // Add mean and wrap to the [-PI, PI] interval
    result += mu;
    result = fmod(result + M_PI, 2 * M_PI);
    if (result < 0) {
        result += 2 * M_PI;
    }
    return result - M_PI;
}

/**
 * @brief Rcpp function to generate Von Mises random variates
 * @param n Number of samples to generate
 * @param mu Mean direction parameter
 * @param kappa Concentration parameter
 * @return NumericVector of Von Mises samples
 */
// [[Rcpp::export]]
NumericVector rvonmises_rcpp(int n, double mu, double kappa) {
    // Seed the random number generator
    srand(time(NULL));
    
    NumericVector result(n);
    
    for (int i = 0; i < n; i++) {
        result[i] = random_vonmises(mu, kappa);
    }
    
    return result;
}

/**
 * @brief Rcpp function to generate a single Von Mises random variate
 * @param mu Mean direction parameter
 * @param kappa Concentration parameter
 * @return Single Von Mises sample
 */
// [[Rcpp::export]]
double rvonmises_single(double mu, double kappa) {
    // Seed the random number generator
    srand(time(NULL));
    
    return random_vonmises(mu, kappa);
}
