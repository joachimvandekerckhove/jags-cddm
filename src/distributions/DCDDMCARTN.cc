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
			if (DCDDMCARTN::eta(par) < 0)  return false;
			if (DCDDMCARTN::tau(par) < 0)  return false;

			return true;
		}

		double DCDDMCARTN::mux(vector<double const*> const &par)
		{
			return *par[0];
		}

		double DCDDMCARTN::muy(vector<double const*> const &par)
		{
			return *par[1];
		}

		double DCDDMCARTN::eta(vector<double const*> const &par)
		{
			return *par[2];
		}

		double DCDDMCARTN::tau(vector<double const*> const &par)
		{
			return *par[3];
		}

		double DCDDMCARTN::logDensity(double const *x, unsigned int length,
				PDFType type,
				vector<double const *> const &par,
				vector<unsigned int> const &len,
				double const *lower, double const *upper) const
		{
			double logPDF = cddmLogDensity(
				x[0],
				x[1],
				DCDDMCARTN::mux(par),
				DCDDMCARTN::muy(par),
				DCDDMCARTN::eta(par),
				DCDDMCARTN::tau(par));

			return isnan(logPDF) ? JAGS_NEGINF : logPDF;
		}

		void DCDDMCARTN::randomSample(double *x, unsigned int length,
				vector<double const *> const &par,
				vector<unsigned int> const &len,
				double const *lower, double const *upper,
				RNG *rng) const
		{
			cddmRandomSample(
				x,
				DCDDMCARTN::mux(par),
				DCDDMCARTN::muy(par),
				DCDDMCARTN::eta(par),
				DCDDMCARTN::tau(par),
				rng);

			return;
		}

	} //namespace cddm

} //namespace jags
