/*
 *  This file is part of the JAGS-CDDM repository.
 *
 *  Source: https://github.com/joachimvandekerckhove/jags-cddm
 *  Authors: Joachim Vandekerckhove and Adriana F. Chávez De la Peña
 *
 *  See README.md for instructions and licensing.
 *
 */

#ifndef DCDDMCARTN_H_
#define DCDDMCARTN_H_

#include "DCDDM.h"

namespace jags {

	namespace cddm {

		class DCDDMCARTN : public DCDDM
		{
			public:
				DCDDMCARTN();

				bool checkParameterValue(std::vector<double const *> const &par,
						std::vector<unsigned int> const &len) const;

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

				static double mux(std::vector<double const*> const &par);

				static double muy(std::vector<double const*> const &par);

				static double eta(std::vector<double const*> const &par);

				static double tau(std::vector<double const*> const &par);
		};
	} //namespace cddm

} //namespace jags

#endif /* DCDDMCARTN_H_ */
