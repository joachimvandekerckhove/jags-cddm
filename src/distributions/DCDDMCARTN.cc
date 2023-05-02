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

namespace jags {
	namespace cddm {

		DCDDMCARTN::DCDDMCARTN()
			: DCDDM("dcddmcartn")
		{}

		bool DCDDMCARTN::checkParameterValue(vector<double const *> const &par,
				vector<unsigned int> const &len) const
		{
			double bound = DCDDMCARTN::bound(par);
			double ndt   = DCDDMCARTN::ndt(par);

			if (ndt   < 0)  return false;
			if (bound < 0)  return false;

			return true;
		}
		
		double DCDDMCARTN::driftLength(vector<double const*> const &par) 
		{
			return std::sqrt( (*par[0]) * (*par[0]) + (*par[1]) * (*par[1]) );
		}
		
		double DCDDMCARTN::driftAngle(vector<double const*> const &par) 
		{
			return std::atan2(*par[1] , *par[0]);
		}
		
		double DCDDMCARTN::bound(vector<double const*> const &par) 
		{
			return *par[2];
		}
		
		double DCDDMCARTN::ndt(vector<double const*> const &par) 
		{
			return *par[3];
		}

		double DCDDMCARTN::logDensity(double const *x, unsigned int length,
				PDFType type,
				vector<double const *> const &par,
				vector<unsigned int> const &len,
				double const *lower, double const *upper) const
		{
			double c = x[0];
			double t = x[1];

			double driftlength = DCDDMCARTN::driftLength(par);
			double driftangle  = DCDDMCARTN::driftAngle(par);
			double bound       = DCDDMCARTN::bound(par);
			double ndt         = DCDDMCARTN::ndt(par);

			double logPDF = cddmLogDensity(c, t, driftlength, driftangle, bound, ndt);

			return isnan(logPDF) ? JAGS_NEGINF : logPDF;
		}

		void DCDDMCARTN::randomSample(double *x, unsigned int length,
				vector<double const *> const &par,
				vector<unsigned int> const &len,
				double const *lower, double const *upper,
				RNG *rng) const
		{
			double driftlength = DCDDMCARTN::driftLength(par);
			double driftangle  = DCDDMCARTN::driftAngle(par);
			double bound       = DCDDMCARTN::bound(par);
			double ndt         = DCDDMCARTN::ndt(par);
			
			cddmRandomSample(x, driftlength, driftangle, bound, ndt, rng);

			return;
		}

	} //namespace cddm

} //namespace jags
