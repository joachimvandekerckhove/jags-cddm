/* ==========================
    Circular diffusion model
   ==========================
*/

#include <config.h>
#include "DCDDMCARTN.h"
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

static inline double DRIFTLENGTH (vector<double const*> const &par) { return std::sqrt( (*par[0]) * (*par[0]) + (*par[1]) * (*par[1]) ); }
static inline double DRIFTANGLE  (vector<double const*> const &par) { return std::atan2( (*par[1]) , (*par[0])                         ); }
static inline double BOUND       (vector<double const*> const &par) { return *par[2]; }
static inline double NDT         (vector<double const*> const &par) { return *par[3]; }

#define smax 50  /* Maximum number of steps in infinite sum */

#define DEBUG FALSE

namespace jags {
	namespace cddm {

		DCDDMCARTN::DCDDMCARTN()
			: VectorDist("dcddmcartn", 4)
		{}

		unsigned int DCDDMCARTN::length(vector<unsigned int> const &len) const
		{
			return 2;
		}

		bool DCDDMCARTN::checkParameterLength(vector<unsigned int> const &len) const
		{
			if (DEBUG) printf("checkParameterLength() has been called\n");

			if (len[0] != 1)  return false;
			if (len[1] != 1)  return false;
			if (len[2] != 1)  return false;
			if (len[3] != 1)  return false;

			return true;
		}

		bool DCDDMCARTN::checkParameterValue(vector<double const *> const &par,
				vector<unsigned int> const &len) const
		{
			if (DEBUG) printf("checkParameterValue() has been called\n");

			double bound = BOUND(par);
			double ndt   = NDT(par);

			if (ndt   < 0)  return false;
			if (bound < 0)  return false;

			return true;
		}

		double DCDDMCARTN::logDensity(double const *x, unsigned int length,
				PDFType type,
				vector<double const *> const &par,
				vector<unsigned int> const &len,
				double const *lower, double const *upper) const
		{
			double c = x[0];
			double t = x[1];

			double driftlength = DRIFTLENGTH(par);
			double driftangle  = DRIFTANGLE(par);
			double bound       = BOUND(par);
			double ndt         = NDT(par);

			double logPDF = cddmLogDensity(c, t, driftlength, driftangle, bound, ndt);

			return isnan(logPDF) ? JAGS_NEGINF : logPDF;
		}

		void DCDDMCARTN::randomSample(double *x, unsigned int length,
				vector<double const *> const &par,
				vector<unsigned int> const &len,
				double const *lower, double const *upper,
				RNG *rng) const
		{
			double driftlength = DRIFTLENGTH(par);
			double driftangle  = DRIFTANGLE(par);
			double bound       = BOUND(par);
			double ndt         = NDT(par);

                        cddmRandomSample(x, driftlength, driftangle, bound, ndt, rng);

			return;
		}

		void DCDDMCARTN::support(double *lower, double *upper, unsigned int length,
			   vector<double const *> const &par,
			   vector<unsigned int> const &len) const
		{
			lower[0] = JAGS_NEGINF;
			upper[0] = JAGS_POSINF;
			lower[1] = 0.0;
			upper[1] = JAGS_POSINF;
		}

		void DCDDMCARTN::typicalValue(double *x, unsigned int length,
				vector<double const *> const &par,
				vector<unsigned int> const &len,
				double const *lower, double const *upper) const
		{
			x[0] = 0.0;
			x[1] = 0.5;
		}

		bool DCDDMCARTN::isSupportFixed(vector<bool> const &fixmask) const
		{
			return fixmask[3];
		}

		unsigned int DCDDMCARTN::df(vector<unsigned int> const &len) const
		{
			return 1;
		}

	} //namespace cddm

} //namespace jags
