/*  This class contains functions for the cddm (diffusion) model */

namespace jags {

	namespace cddm {

		double logDensity(double const c,
				double const t,
				double const driftlength,
				double const driftangle,
				double const bound,
				double const ndt) const;

		void randomSample(double *x,
				double const driftlength,
				double const driftangle,
				double const bound,
				double const ndt,
				RNG *rng) const;
		};

	} //namespace cddm

} //namespace jags
