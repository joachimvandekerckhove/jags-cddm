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

#define smax 50  /* Maximum number of steps in infinite sum */

namespace jags {

	namespace cddm {

		const double inv2pi = 0.159154943091895;
		const double log2pi = 1.837877066409345;

		const double j0_over_J1_of_j0[] = {
			 4.632259e+00, -1.622289e+01,  3.187937e+01, -5.072504e+01,  7.228843e+01,
			-9.626154e+01,  1.224225e+02, -1.506013e+02,  1.806628e+02, -2.124954e+02,
			 2.460056e+02, -2.811133e+02,  3.177488e+02, -3.558508e+02,  3.953649e+02,
			-4.362423e+02,  4.784390e+02, -5.219150e+02,  5.666336e+02, -6.125612e+02,
			 6.596669e+02, -7.079218e+02,  7.572992e+02, -8.077741e+02,  8.593232e+02,
			-9.119245e+02,  9.655574e+02, -1.020202e+03,  1.075841e+03, -1.132456e+03,
			 1.190031e+03, -1.248550e+03,  1.307997e+03, -1.368360e+03,  1.429623e+03,
			-1.491774e+03,  1.554801e+03, -1.618691e+03,  1.683434e+03, -1.749017e+03,
			 1.815430e+03, -1.882663e+03,  1.950707e+03, -2.019550e+03,  2.089186e+03,
			-2.159604e+03,  2.230795e+03, -2.302752e+03,  2.375467e+03, -2.448931e+03,
		};

		const double j0_squared[] = {
			    5.783185962947,	   30.471262343662,	   74.887006790695,	  139.040284426460,	  222.932303617634,
			  326.563352932328,	  449.933528518036,	  593.042869655955,	  755.891394783933,	  938.479113475694,
			 1140.806031099645,	 1362.872150854105,	 1604.677474740231,	 1866.222004061853,	 2147.505739697844,
			 2448.528682258052,	 2769.290832176359,	 3109.792189768249,	 3470.032755267558,	 3850.012528850570,
			 4249.731510652210,	 4669.189700777160,	 5108.387099307649,	 5567.323706308981,	 6045.999521833563,
			 6544.414545923833,	 7062.568778614458,	 7600.462219933975,	 8158.094869906055,	 8735.466728550457,
			 9332.577795883786,	 9949.428071920078,	10586.017556671255,	11242.346250147502,	11918.414152357564,
			12614.221263308977,	13329.767583008270,	14065.053111461117,	14820.077848672467,	15594.841794646662,
			16389.344949387512,	17203.587312898377,	18037.568885182227,	18891.289666241712,	19764.749656079162,
			20657.948854696686,	21570.887262096137,	22503.564878279212,	23455.981703247398,	24428.137737002071
		};

		DCDDM::DCDDM(std::string const &call)
			: VectorDist(call, 4)
		{}

		unsigned int DCDDM::length(vector<unsigned int> const &len) const
		{
			return 2;
		}

		bool DCDDM::checkParameterLength(vector<unsigned int> const &len) const
		{
			if (len[0] != 1)  return false;
			if (len[1] != 1)  return false;
			if (len[2] != 1)  return false;
			if (len[3] != 1)  return false;

			return true;
		}

		void DCDDM::support(double *lower, double *upper, unsigned int length,
			   vector<double const *> const &par,
			   vector<unsigned int> const &len) const
		{
			lower[0] = JAGS_NEGINF;
			upper[0] = JAGS_POSINF;
			lower[1] = 0.0;
			upper[1] = JAGS_POSINF;
		}

		void DCDDM::typicalValue(double *x, unsigned int length,
				vector<double const *> const &par,
				vector<unsigned int> const &len,
				double const *lower, double const *upper) const
		{
			x[0] = 0.0;
			x[1] = 0.5;
		}

		bool DCDDM::isSupportFixed(vector<bool> const &fixmask) const
		{
			return fixmask[3];
		}

		unsigned int DCDDM::df(vector<unsigned int> const &len) const
		{
			return 1;
		}

		double DCDDM::cddmLogDensity(double const c,
				double const t,
				double const mux,
				double const muy,
				double const eta,
				double const tau) const
		{
			double inveta2 = 1.0 / (eta * eta),
				nhdt = -0.5 * (t - tau),
				sum  =  0.0;

			for (int i=0; i<smax; i++) {
				sum += exp(j0_squared[i] * inveta2 * nhdt) * j0_over_J1_of_j0[i];
			}

			double logPDF = log(sum) + log(inveta2) - log2pi;
			logPDF += (mux * cos(c) + muy * sin(c)) * eta;
			logPDF += (mux * mux    + muy * muy   ) * nhdt;

			return isnan(logPDF) ? JAGS_NEGINF : logPDF;
		}

		void DCDDM::cddmRandomSample(double *x,
				double const mux,
				double const muy,
				double const eta,
				double const tau,
				RNG *rng) const
		{
                        x[0] = 0.0;
                        x[1] = 0.5;
			return;
		}

	} //namespace cddm

} //namespace jags
