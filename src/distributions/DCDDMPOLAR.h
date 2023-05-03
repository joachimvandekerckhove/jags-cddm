#ifndef DCDDMPOLAR_H_
#define DCDDMPOLAR_H_

#include "DCDDM.h"

namespace jags {

	namespace cddm {

		class DCDDMPOLAR : public DCDDM
		{
			public:
				DCDDMPOLAR();

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

				static double driftLength(std::vector<double const*> const &par);

				static double driftAngle(std::vector<double const*> const &par);

				static double bound(std::vector<double const*> const &par);

				static double ndt(std::vector<double const*> const &par);
		};
	} //namespace cddm

} //namespace jags

#endif /* DCDDMPOLAR_H_ */
