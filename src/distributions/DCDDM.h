/*
 *  This file is part of the JAGS-CDDM repository.
 *
 *  Source: https://github.com/joachimvandekerckhove/jags-cddm
 *  Authors: Joachim Vandekerckhove and Adriana F. Chávez De la Peña
 *
 *  See README.md for instructions and licensing.
 *
 */

#ifndef DCDDM_H_
#define DCDDM_H_

#include <distribution/VectorDist.h>
#include <rng/RNG.h>

namespace jags {

	namespace cddm {

		class DCDDM : public VectorDist
		{
			public:

				DCDDM(std::string const &call);

				unsigned int length(std::vector<unsigned int> const &len) const;

				bool checkParameterLength(std::vector<unsigned int> const &len) const;

				void support(double *lower, double *upper, unsigned int length,
						std::vector<double const *> const &par,
						std::vector<unsigned int> const &len) const;

				void typicalValue(double *x, unsigned int length,
						std::vector<double const *> const &par,
						std::vector<unsigned int> const &len,
						double const *lower, double const *upper) const;

				bool isSupportFixed(std::vector<bool> const &fixmask) const;

				unsigned int df(std::vector<unsigned int> const &len) const;

				double cddmLogDensity(double const c,
						double const t,
						double const driftlength,
						double const driftangle,
						double const bound,
						double const ndt) const;

				void cddmRandomSample(double *x,
						double const driftlength,
						double const driftangle,
						double const bound,
						double const ndt,
						RNG *rng) const;

		};

	} //namespace cddm

} //namespace jags

#endif /* DCDDM_H_ */
