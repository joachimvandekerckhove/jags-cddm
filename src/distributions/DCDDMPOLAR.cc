/* ==========================
    Circular diffusion model
   ==========================
*/

#include <config.h>
#include "DCDDMPOLAR.h"
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

//TODO: Make these inlines into class methods
static inline double DRIFTLENGTH (vector<double const*> const &par) { return *par[0]; }
static inline double DRIFTANGLE  (vector<double const*> const &par) { return *par[1]; }
static inline double BOUND       (vector<double const*> const &par) { return *par[2]; }
static inline double NDT         (vector<double const*> const &par) { return *par[3]; }

#define smax 50  /* Maximum number of steps in infinite sum */

namespace jags {
	namespace cddm {

		DCDDMPOLAR::DCDDMPOLAR()
			: DCDDM("dcddmpolar")
		{}

		bool DCDDMPOLAR::checkParameterValue(vector<double const *> const &par,
				vector<unsigned int> const &len) const
		{
			double driftLength = DCDDMPOLAR::driftLength(par);
			double bound       = DCDDMPOLAR::bound(par);
			double ndt         = DCDDMPOLAR::ndt(par);

			if (driftLength < 0)  return false;
			if (ndt         < 0)  return false;
			if (bound       < 0)  return false;

			return true;
		}
		
		double DCDDMPOLAR::driftLength(vector<double const*> const &par) 
		{
			return *par[0];
		}
		
		double DCDDMPOLAR::driftAngle(vector<double const*> const &par) 
		{
			return *par[1];
		}
		
		double DCDDMPOLAR::bound(vector<double const*> const &par) 
		{
			return *par[2];
		}
		
		double DCDDMPOLAR::ndt(vector<double const*> const &par) 
		{
			return *par[3];
		}

		double DCDDMPOLAR::logDensity(double const *x, unsigned int length,
				PDFType type,
				vector<double const *> const &par,
				vector<unsigned int> const &len,
				double const *lower, double const *upper) const
		{
			double c = x[0];
			double t = x[1];

			double driftlength = DCDDMPOLAR::driftLength(par);
			double driftangle  = DCDDMPOLAR::driftAngle(par);
			double bound       = DCDDMPOLAR::bound(par);
			double ndt         = DCDDMPOLAR::ndt(par);

			double logPDF = cddmLogDensity(c, t, driftlength, driftangle, bound, ndt);

			return isnan(logPDF) ? JAGS_NEGINF : logPDF;
		}

		void DCDDMPOLAR::randomSample(double *x, unsigned int length,
				vector<double const *> const &par,
				vector<unsigned int> const &len,
				double const *lower, double const *upper,
				RNG *rng) const
		{
			double driftlength = DCDDMPOLAR::driftLength(par);
			double driftangle  = DCDDMPOLAR::driftAngle(par);
			double bound       = DCDDMPOLAR::bound(par);
			double ndt         = DCDDMPOLAR::ndt(par);

                        cddmRandomSample(x, driftlength, driftangle, bound, ndt, rng);

			return;
		}

	} //namespace cddm

} //namespace jags
