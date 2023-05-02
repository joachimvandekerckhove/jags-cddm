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
					
			static double driftLength(std::vector<double const*> const &par);
		
			static double driftAngle(std::vector<double const*> const &par);
		
			static double bound(std::vector<double const*> const &par);
		
			static double ndt(std::vector<double const*> const &par);
		};
	} //namespace cddm
} //namespace jags

#endif /* DCDDMCARTN_H_ */
