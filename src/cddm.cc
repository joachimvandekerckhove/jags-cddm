/*
 *  This file is part of the JAGS-CDDM repository.
 *
 *  Source: https://github.com/joachimvandekerckhove/jags-cddm
 *  Authors: Joachim Vandekerckhove and Adriana F. Chávez De la Peña
 *
 *  See README.md for instructions and licensing.
 *
 */

#include <module/Module.h>
#include <distributions/DCDDMCARTN.h>
#include <distributions/DCDDMPOLAR.h>
#include <iostream>

using std::vector;

namespace jags {

	namespace cddm {

		class CDDMModule : public Module {
			public:
				CDDMModule();
				~CDDMModule();
		};

		CDDMModule::CDDMModule() : Module("cddm")
		{
			std::cout << "\n"
				<< "  The JAGS-CDDM module is released under the LGPL v2.1 or later.\n"
				<< "  When using this module, please cite:\n"
				<< "     Villarreal, M., Chávez De la Peña, A. F., Vandekerckhove, J. & Lee, M. D. (2023).\n"
				<< "        Bayesian hierarchical circular drift diffusion models.\n"
				<< "  Source: https://github.com/joachimvandekerckhove/jags-cddm\n";

			//load distributions
			insert(new DCDDMCARTN());
			insert(new DCDDMPOLAR());
		}

		CDDMModule::~CDDMModule()
		{
			vector<Function*> const &fvec = functions();
			for (unsigned int i = 0; i < fvec.size(); ++i) {
				delete fvec[i];
			}
			vector<Distribution*> const &dvec = distributions();
			for (unsigned int i = 0; i < dvec.size(); ++i) {
				delete dvec[i];
			}
		}

	} // namespace cddm

} // namespace jags

jags::cddm::CDDMModule _cddm_module;
