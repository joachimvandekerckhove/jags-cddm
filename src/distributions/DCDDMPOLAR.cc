/*
 *  This file is part of the JAGS-CDDM repository.
 *
 *  Source: https://github.com/joachimvandekerckhove/jags-cddm
 *  Authors: Joachim Vandekerckhove and Adriana F. Chávez De la Peña
 *
 *  See README.md for instructions and licensing.
 *
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

namespace jags {

	namespace cddm {

		DCDDMPOLAR::DCDDMPOLAR()
			: DCDDM("dcddmpolar")
		{}

		bool DCDDMPOLAR::checkParameterValue(vector<double const *> const &par,
				vector<unsigned int> const &len) const
		{
			if (DCDDMPOLAR::driftLength(par) < 0)  return false;
			if (DCDDMPOLAR::eta(par)         < 0)  return false;
			if (DCDDMPOLAR::tau(par)         < 0)  return false;

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

		double DCDDMPOLAR::mux(vector<double const*> const &par)
		{
			return (*par[0]) * cos(*par[1]) ;
		}

		double DCDDMPOLAR::muy(vector<double const*> const &par)
		{
			return (*par[0]) * sin(*par[1]);
		}

		double DCDDMPOLAR::eta(vector<double const*> const &par)
		{
			return *par[2];
		}

		double DCDDMPOLAR::tau(vector<double const*> const &par)
		{
			return *par[3];
		}

		double DCDDMPOLAR::logDensity(double const *x, unsigned int length,
				PDFType type,
				vector<double const *> const &par,
				vector<unsigned int> const &len,
				double const *lower, double const *upper) const
		{
			double logPDF = cddmLogDensity(
				x[0], 
				x[1],
				DCDDMPOLAR::mux(par),
				DCDDMPOLAR::muy(par),
				DCDDMPOLAR::eta(par),
				DCDDMPOLAR::tau(par)
			);

			return isnan(logPDF) ? JAGS_NEGINF : logPDF;
		}

		void DCDDMPOLAR::randomSample(double *x, unsigned int length,
				vector<double const *> const &par,
				vector<unsigned int> const &len,
				double const *lower, double const *upper,
				RNG *rng) const
		{
			cddmRandomSample(
				x,
				DCDDMPOLAR::mux(par),
				DCDDMPOLAR::muy(par),
				DCDDMPOLAR::eta(par),
				DCDDMPOLAR::tau(par),
				rng
			);

			return;
		}

	} //namespace cddm

} //namespace jags
