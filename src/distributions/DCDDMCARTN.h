/*  This class contains functions for the cddm (diffusion) model */
#ifndef DCDDMCARTN_H_
#define DCDDMCARTN_H_

#include <distribution/VectorDist.h>

namespace jags {

	namespace cddm {

	/**
	 * Implements the circular drift diffusion model
	 *
	 * <pre>
         * X[i,1] <- choice (in radians)
         * X[i,2] <- response time (in seconds)
         * X[i,1:2] ~ dcddm(drift_x, drift_y, bound, ndt)
	 * </pre>
         *
	 * @short circular drift diffusion model
	 */

		class DCDDMCARTN : public VectorDist
		{
		public:
			DCDDMCARTN();

			unsigned int length(std::vector<unsigned int> const &len) const;

			double logDensity(double const *x, unsigned int length,
					PDFType type,
					std::vector<double const *> const &par,
					std::vector<unsigned int> const &len,
					double const *lower, double const *upper) const;

			void randomSample(double *x, unsigned int length,
					std::vector<double const *> const &par,
					std::vector<unsigned int> const &len,
					double const *lower, double const *upper,
					RNG *rng) const;

			void typicalValue(double *x, unsigned int length,
					std::vector<double const *> const &par,
					std::vector<unsigned int> const &len,
					double const *lower, double const *upper) const;

			bool checkParameterValue(std::vector<double const *> const &par,
					std::vector<unsigned int> const &len) const;

			bool checkParameterLength(std::vector<unsigned int> const &len) const;

			void support(double *lower, double *upper, unsigned int length,
					std::vector<double const *> const &par,
					std::vector<unsigned int> const &len) const;

			bool isSupportFixed(std::vector<bool> const &fixmask) const;

			unsigned int df(std::vector<unsigned int> const &len) const;

		};
	} //namespace cddm
} //namespace jags

#endif /* DCDDMCARTN_H_ */
