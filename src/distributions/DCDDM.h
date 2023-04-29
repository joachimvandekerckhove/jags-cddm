/*  This class contains functions for the cddm (diffusion) model */

#include <rng/RNG.h>

namespace jags {

	namespace cddm {

		double cddmLogDensity(double const c,
				double const t,
				double const driftlength,
				double const driftangle,
				double const bound,
				double const ndt);

		void cddmRandomSample(double *x,
				double const driftlength,
				double const driftangle,
				double const bound,
				double const ndt,
				RNG *rng);

	} //namespace cddm

} //namespace jags
