/*
 *  Copyright (C) 2022 Joachim Vandekerckhove <joachim@uci.edu>
 *
 *  When using this module, please cite:
 *      Wabersich, D., & Vandekerckhove, J. (2014). Extending JAGS: A
 *      tutorial on adding custom distributions to JAGS (with a
 *      diffusion model example). Behavior Research Methods, 46, 15-28.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
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
            << "     Wabersich, D., & Vandekerckhove, J. (2014). Extending JAGS: A\n"
            << "        tutorial on adding custom distributions to JAGS (with a\n"
            << "        diffusion model example). Behavior Research Methods, 46, 15-28.\n";

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
