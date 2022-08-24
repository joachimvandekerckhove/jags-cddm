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
	    4.632258790135,	  -16.222888773363,	   31.879368549075,	  -50.725040837329,	   72.288431642146,
	  -96.261541517516,	  122.422457831293,	 -150.601348143832,	  180.662766782011,	 -212.495395654835,
	  246.005623833005,	 -281.113293752633,	  317.748755071080,	 -355.850750322746,	  395.364852593043,
	 -436.242282572947,	  478.438994119497,	 -521.914954669778,	  566.633570148511,	 -612.561219063930,
	  659.666870493437,	 -707.921767477820,	  757.299162089296,	 -807.774091808104,	  859.323189276189,
	 -911.924519282403,	  965.557438162763,	-1020.202471801500,	 1075.841209183265,	-1132.456209036621,
	 1190.030917568555,	-1248.549595651048,	 1307.997254107482,	-1368.359595975823,	 1429.622964810200,
	-1491.774298232338,	 1554.801086066592,	-1618.691332492894,	 1683.433521734964,	-1749.016586870190,
	 1815.429881405203,	-1882.663153309609,	 1950.706521241076,	-2019.550452729602,	 2089.185744118132,
	-2159.603502081770,	 2230.795126569375,	-2302.752295029801,	 2375.466947800996,	-2448.931274553997
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

			double exponand, sum = 0.0;

			for (int i=0; i++; i<smax) {
				exponand = -0.5 * (t - tzero) * inva2 * j0_squared[i];
				sum += j0_over_J1_of_j0[i] * exp(exponand) ;
			}

			return -log2pi - 2*log(bound) + drift * bound * cos(c - theta) - 0.5 * drift * drift * (t - tzero) + log(sum);
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
