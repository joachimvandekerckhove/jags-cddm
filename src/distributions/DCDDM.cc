/* ==========================
    Circular diffusion model
   ==========================
*/

#include <config.h>
#include "DCDDM.h"

#include <util/dim.h>
#include <rng/RNG.h>
#include <util/nainf.h>
#include <cmath>
#include <JRmath.h>
#include <stdio.h>

using std::vector;
using std::log;
using std::string;

static inline double DRIFT (vector<double const*> const &par) { return *par[0]; }
static inline double BOUND (vector<double const*> const &par) { return *par[1]; }
static inline double TZERO (vector<double const*> const &par) { return *par[2]; }
static inline double THETA (vector<double const*> const &par) { return *par[3]; }

const double inv2pi = 0.159154943091895;
const double log2pi = 1.837877066409345;

const double j0_over_J1_of_j0[] =
	{
 	    1.041844e+00, 1.404196e-01, 1.165914e-02, 7.941334e-04, 4.865202e-05,
 	    2.790466e-06, 1.530105e-07, 8.120647e-09, 4.204391e-10, 2.134880e-11,
	    1.067190e-12, 5.266376e-14, 2.570958e-15, 1.243643e-16, 5.968584e-18,
	    2.844917e-19, 1.347894e-20, 6.352313e-22, 2.979549e-23, 1.391634e-24,
	    6.474955e-26, 3.002217e-27, 1.387638e-28, 6.395233e-30, 2.939587e-31,
	    1.347897e-32, 6.166633e-34, 2.815350e-35, 1.282843e-36, 5.834842e-38,
	    2.649421e-39, 1.201119e-40, 5.437219e-42, 2.457888e-43, 1.109627e-44,
	    5.003269e-46, 2.253318e-47, 1.013701e-48, 4.555557e-50, 2.045221e-51,
	    9.173347e-53, 4.110778e-54, 1.840549e-55, 8.234077e-57, 3.680807e-58,
	    1.644169e-59, 7.339043e-61, 3.273670e-62, 1.459302e-63, 6.501029e-65
	};
const double j0_squared[] =
	{
	    5.783185962947,	   30.471262343662,	   74.887006790695,	  139.040284426460,	  222.932303617634,
	  326.563352932328,	  449.933528518036,	  593.042869655955,	  755.891394783933,	  938.479113475694,
	 1140.806031099645,	 1362.872150854105,	 1604.677474740231,	 1866.222004061853,	 2147.505739697844,
	 2448.528682258052,	 2769.290832176359,	 3109.792189768249,	 3470.032755267558,	 3850.012528850570,
	 4249.731510652210,	 4669.189700777160,	 5108.387099307649,	 5567.323706308981,	 6045.999521833563,
	 6544.414545923833,	 7062.568778614458,	 7600.462219933975,	 8158.094869906055,	 8735.466728550457,
	 9332.577795883786,	 9949.428071920078,	10586.017556671255,	11242.346250147502,	11918.414152357564,
	12614.221263308977,	13329.767583008270,	14065.053111461117,	14820.077848672467,	15594.841794646662,
	16389.344949387512,	17203.587312898377,	18037.568885182227,	18891.289666241712,	19764.749656079162,
	20657.948854696686,	21570.887262096137,	22503.564878279212,	23455.981703247398,	24428.137737002071
	};

#define smax 50  /* Maximum number of steps in infinite sum */


namespace jags {
	namespace cddm {

		DCDDM::DCDDM()
			: VectorDist("dcddm", 4)
		{}

		unsigned int DCDDM::length(vector<unsigned int> const &len) const
		{
			return 2;
		}

		bool DCDDM::checkParameterLength(vector<unsigned int> const &len) const
		{
			if (len[0] != 1) return 0;
			if (len[1] != 1) return 0;
			if (len[2] != 1) return 0;
			if (len[3] != 1) return 0;
			return 1;
		}

		bool DCDDM::checkParameterValue(vector<double const *> const &par,
				vector<unsigned int> const &len) const
		{
			double drift = DRIFT(par);
			double bound = BOUND(par);
			double tzero = TZERO(par);
			double theta = THETA(par);

			if (tzero < 0) return false;
			if (bound < 0) return false;

			return true;
		}

		double DCDDM::logDensity(double const *x, unsigned int length,
				PDFType type,
				vector<double const *> const &par,
				vector<unsigned int> const &len,
				double const *lower, double const *upper) const
		{
			double c = x[0];
			double t = x[1];

			double drift = DRIFT(par);
			double bound = BOUND(par);
			double tzero = TZERO(par);
			double theta = THETA(par);
			double inva2 = 1.0 / (bound*bound);

			double exponand, sum = 0.0, logPDF;
/*
			double mu1 = drift*cos(theta), mu2 = drift*sin(theta);
			printf("mu1: %f = %f * cos(%f)\n", mu1, drift, theta);
			printf("mu2: %f = %f * sin(%f)\n", mu2, drift, theta);

			for (int i=0; i<smax; i++) {
				exponand = j0_squared[i] * (t-tzero) * inva2 * -0.5;
				sum += exp(exponand) * j0_over_J1_of_j0[i];
				printf ("exponand = %f\n", exponand);
				printf ("sum = %f\n", sum);
			}

			logPDF = log(sum) + log(inva2);
			printf("logPDF = %f\n", logPDF);
			logPDF += bound*(mu1*cos(c)+mu2*sin(c));
			printf("logPDF = %f\n", logPDF);
			logPDF -= (drift*drift*(t-tzero))*0.5;
			printf("logPDF = %f\n", logPDF);
			return logPDF;
*/
			for (int i=0; i<smax; i++) {
				exponand = -0.5 * (t - tzero) * inva2 * j0_squared[i];
				sum += j0_over_J1_of_j0[i] * exp(exponand);
				printf("exponand = %f\n", exponand);
				printf("sum = %f\n", sum);
			}

			logPDF = -log2pi - 2*log(bound) + drift * bound * cos(c - theta) - 0.5 * drift * drift * (t - tzero) + log(sum);
			printf("logPDF = %f\n", logPDF);
			return logPDF;
		}

		void DCDDM::randomSample(double *x, unsigned int length,
				vector<double const *> const &par,
				vector<unsigned int> const &len,
				double const *lower, double const *upper,
				RNG *rng) const
		{
                        x[0] = 0.0;
                        x[1] = 0.5;
			return;
		}

		void DCDDM::support(double *lower, double *upper, unsigned int length,
			   vector<double const *> const &par,
			   vector<unsigned int> const &len) const
		{
			lower[0] = JAGS_NEGINF;
			upper[0] = JAGS_POSINF;
			lower[1] = 0.0;
			upper[1] = JAGS_POSINF;
		}

		void DCDDM::typicalValue(double *x, unsigned int length,
				vector<double const *> const &par,
				vector<unsigned int> const &len,
				double const *lower, double const *upper) const
		{
			x[0] = 0.0;
			x[1] = 0.5;
		}

		bool DCDDM::isSupportFixed(vector<bool> const &fixmask) const
		{
			return true;
		}

		unsigned int DCDDM::df(vector<unsigned int> const &len) const
		{
			return 1;
		}

	} //namespace cddm

} //namespace jags
