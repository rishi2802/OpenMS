// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2022.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Timo Sachsenberg $
// $Authors: Andreas Bertsch, Jang Jang Jin$
// --------------------------------------------------------------------------

#include <OpenMS/CHEMISTRY/Residue.h>

#include <OpenMS/CHEMISTRY/ResidueModification.h>
#include <OpenMS/CHEMISTRY/ModificationsDB.h>
#include <OpenMS/CONCEPT/Macros.h>
#include <OpenMS/CONCEPT/LogStream.h>

#include <iostream>

using namespace std;

namespace OpenMS
{
  Residue::Residue() :
    name_("unknown"),
    average_weight_(0.0f),
    mono_weight_(0.0f),
    modification_(nullptr),
    loss_average_weight_(0.0f),
    loss_mono_weight_(0.0f),
    pka_(0.0),
    pkb_(0.0),
    pkc_(-1.0)
  {
  }

  Residue::Residue(const String& name,
            const String& three_letter_code,
            const String& one_letter_code,
            const EmpiricalFormula& formula,
            double pka,
            double pkb,
            double pkc,
            double gb_sc,
            double gb_bb_l,
            double gb_bb_r,
            const set<String>& synonyms):
                
    name_(name),
    synonyms_(synonyms),
    three_letter_code_(three_letter_code),
    one_letter_code_(one_letter_code),
    formula_(formula),
    average_weight_(formula.getAverageWeight()),
    mono_weight_(formula.getMonoWeight()),
    modification_(nullptr),
    loss_average_weight_(0.0f),
    loss_mono_weight_(0.0f),
    pka_(pka),
    pkb_(pkb),
    pkc_(pkc),
    gb_sc_(gb_sc),
    gb_bb_l_(gb_bb_l),
    gb_bb_r_(gb_bb_r)
  {
    if (!formula_.isEmpty())
    {
      internal_formula_ = formula_ - getInternalToFull();
    }
  } 

  Residue::~Residue()
  {
  }

  void Residue::setName(const String& name)
  {
    name_ = name;
  }

  const String& Residue::getName() const
  {
    return name_;
  }

  String Residue::getResidueTypeName(const Residue::ResidueType res_type)
  {
    switch (res_type)
    {
    case Residue::Full:
      return "full";

    case Residue::Internal:
      return "internal";

    case Residue::NTerminal:
      return "N-terminal";

    case Residue::CTerminal:
      return "C-terminal";

    case Residue::AIon:
      return "a-ion";

    case Residue::BIon:
      return "b-ion";

    case Residue::CIon:
      return "c-ion";

    case Residue::XIon:
      return "x-ion";

    case Residue::YIon:
      return "y-ion";

    case Residue::ZIon:
      return "z-ion";

    case Residue::Precursor:
      return "precursor-ion";

    case Residue::BIonMinusH20:
      return "b-H2O-ion";

    case Residue::YIonMinusH20:
      return "y-H2O-ion";

    case Residue::BIonMinusNH3:
      return "B-NH3-ion";

    case Residue::YIonMinusNH3:
      return "y-NH3-ion";

    case Residue::NonIdentified:
      return "Non-identified ion";

    case Residue::Unannotated:
      return "unannotated";

    default:
      cerr << "Error: Residue::getResidueTypeName - residue type has no name. The developer should add a residue name to Residue.cpp" << endl;
    }
    return "";
  }

  void Residue::setSynonyms(const set<String>& synonyms)
  {
    synonyms_ = synonyms;
  }

  void Residue::addSynonym(const String& synonym)
  {
    synonyms_.insert(synonym);
  }

  const set<String>& Residue::getSynonyms() const
  {
    return synonyms_;
  }

  void Residue::setThreeLetterCode(const String& three_letter_code)
  {
    OPENMS_PRECONDITION(three_letter_code.empty() || three_letter_code.size() == 3, "Three letter code needs to be a String of size 3")
    three_letter_code_ = three_letter_code;
  }

  const String& Residue::getThreeLetterCode() const
  {
    OPENMS_POSTCONDITION(three_letter_code_.empty() || three_letter_code_.size() == 3, "Three letter code needs to be a String of size 3")
    return three_letter_code_;
  }

  void Residue::setOneLetterCode(const String& one_letter_code)
  {
    OPENMS_PRECONDITION(one_letter_code.empty() || one_letter_code.size() == 1, "One letter code needs to be a String of size 1")
    one_letter_code_ = one_letter_code;
  }

  const String& Residue::getOneLetterCode() const
  {
    OPENMS_POSTCONDITION(one_letter_code_.empty() || one_letter_code_.size() == 1, "One letter code needs to be a String of size 1")
    return one_letter_code_;
  }

  double Residue::getPka() const
  {
    return pka_;
  }

  double Residue::getPkb() const
  {
    return pkb_;
  }

  double Residue::getPkc() const
  {
    return pkc_;
  }

  double Residue::getPiValue() const
  {
    double pi = 0;
    double temp1 = getPka();
    double temp2 = getPkb();
    double temp3 = getPkc();

    if (temp3 >= 0 && temp3 < temp1)
    {
      pi = (temp3 + temp2) / 2;
    }
    else if (temp3 >= temp2)
    {
      pi = (temp1 + temp3) / 2;
    }
    else
    {
      pi = (temp1 + temp2) / 2;
    }

    return pi;
  }

  void Residue::setPka(double value)
  {
    pka_ = value;
  }

  void Residue::setPkb(double value)
  {
    pkb_ = value;
  }

  void Residue::setPkc(double value)
  {
    pkc_ = value;
  }

  void Residue::setLossFormulas(const vector<EmpiricalFormula>& loss_formulas)
  {
    loss_formulas_ = loss_formulas;
  }

  void Residue::addLossFormula(const EmpiricalFormula& loss_formula)
  {
    loss_formulas_.push_back(loss_formula);
  }

  const vector<EmpiricalFormula>& Residue::getLossFormulas() const
  {
    return loss_formulas_;
  }

  void Residue::addLossName(const String& name)
  {
    loss_names_.push_back(name);
  }

  void Residue::setLossNames(const vector<String>& names)
  {
    loss_names_ = names;
  }

  const vector<String>& Residue::getLossNames() const
  {
    return loss_names_;
  }

  void Residue::setNTermLossFormulas(const vector<EmpiricalFormula>& NTerm_loss_formulas)
  {
    NTerm_loss_formulas_ = NTerm_loss_formulas;
  }

  void Residue::addNTermLossFormula(const EmpiricalFormula& NTerm_loss_formula)
  {
    NTerm_loss_formulas_.push_back(NTerm_loss_formula);
  }

  const vector<EmpiricalFormula> & Residue::getNTermLossFormulas() const
  {
    return NTerm_loss_formulas_;
  }

  void Residue::addNTermLossName(const String& name)
  {
    NTerm_loss_names_.push_back(name);
  }

  void Residue::setNTermLossNames(const vector<String>& names)
  {
    NTerm_loss_names_ = names;
  }

  const vector<String>& Residue::getNTermLossNames() const
  {
    return NTerm_loss_names_;
  }

  void Residue::setFormula(const EmpiricalFormula& formula)
  {
    formula_ = formula;
    internal_formula_ = formula_ - getInternalToFull();
    average_weight_ = formula_.getAverageWeight();
    mono_weight_ = formula_.getMonoWeight();
  }

  EmpiricalFormula Residue::getFormula(ResidueType res_type) const
  {
    switch (res_type)
    {
    case Full:
      return formula_;

    case Internal:
      return internal_formula_;

    case NTerminal:
      return internal_formula_ + getInternalToNTerm();

    case CTerminal:
      return internal_formula_ + getInternalToCTerm();

    case BIon:
      return internal_formula_ + getInternalToBIon();

    case AIon:
      return internal_formula_ + getInternalToAIon();

    case CIon:
      return internal_formula_ + getInternalToCIon();

    case XIon:
      return internal_formula_ + getInternalToXIon();

    case YIon:
      return internal_formula_ + getInternalToYIon();

    case ZIon:
      return internal_formula_ + getInternalToZIon();

    default:
      cerr << "Residue::getFormula: unknown ResidueType" << endl;
      return formula_;
    }
  }

  void Residue::setAverageWeight(double weight)
  {
    average_weight_ = weight;
    return;
  }

  double Residue::getAverageWeight(ResidueType res_type) const
  {

    switch (res_type)
    {
    case Full:
      return average_weight_;

    case Internal:
      return average_weight_ - getInternalToFull().getAverageWeight();

    case NTerminal:
      return average_weight_ + (getInternalToNTerm() - getInternalToFull()).getAverageWeight();

    case CTerminal:
      return average_weight_ + (getInternalToCTerm() - getInternalToFull()).getAverageWeight();

    case BIon:
      return average_weight_ + (getInternalToBIon() - getInternalToFull()).getAverageWeight();

    case AIon:
      return average_weight_ + (getInternalToAIon() - getInternalToFull()).getAverageWeight();

    case CIon:
      return average_weight_ + (getInternalToCIon() - getInternalToFull()).getAverageWeight();

    case XIon:
      return average_weight_ + (getInternalToXIon() - getInternalToFull()).getAverageWeight();

    case YIon:
      return average_weight_ + (getInternalToYIon() - getInternalToFull()).getAverageWeight();

    case ZIon:
      return average_weight_ + (getInternalToZIon() - getInternalToFull()).getAverageWeight();

    default:
      cerr << "Residue::getAverageWeight: unknown ResidueType" << endl;
      return average_weight_;
    }
  }

  void Residue::setMonoWeight(double weight)
  {
    mono_weight_ = weight;
    return;
  }

  double Residue::getMonoWeight(ResidueType res_type) const
  {
    switch (res_type)
    {
    case Full:
      return mono_weight_;

    case Internal:
      return mono_weight_ - internal_to_full_monoweight_;

    case NTerminal:
      return mono_weight_ + internal_to_nterm_monoweight_;

    case CTerminal:
      return mono_weight_ + internal_to_cterm_monoweight_;

    case BIon:
      return mono_weight_ + internal_to_b_monoweight_;

    case AIon:
      return mono_weight_ + internal_to_a_monoweight_;

    case CIon:
      return mono_weight_ + internal_to_c_monoweight_;

    case XIon:
      return mono_weight_ + internal_to_x_monoweight_;

    case YIon:
      return mono_weight_ + internal_to_y_monoweight_;

    case ZIon:
      return mono_weight_ + internal_to_z_monoweight_;

    default:
      cerr << "Residue::getMonoWeight: unknown ResidueType" << endl;
      return mono_weight_;
    }
  }

  void Residue::setModification(const ResidueModification* mod)
  {
    modification_ = mod;

    // update all the members
    if (mod->getAverageMass() != 0)
    {
      average_weight_ = mod->getAverageMass();
    }
    if (mod->getMonoMass() != 0)
    {
      mono_weight_ = mod->getMonoMass();
    }
    // update mono_weight_ by DiffMonoMass, if MonoMass is not known, but DiffMonoMass is
    // as in the case of XLMOD.obo modifications
    if ( (mod->getMonoMass() == 0) && (mod->getDiffMonoMass() != 0) )
    {
      mono_weight_ += mod->getDiffMonoMass();
    }

    if (!mod->getDiffFormula().isEmpty())
    {
      setFormula(getFormula() + mod->getDiffFormula());
    }
    else if (!mod->getFormula().empty())
    {
      String formula = mod->getFormula();
      formula.removeWhitespaces();
      setFormula(EmpiricalFormula(formula));
    }

    // neutral losses
    loss_formulas_.clear();
    loss_names_.clear();
    if (mod->hasNeutralLoss())
    {
      loss_formulas_.insert(loss_formulas_.end(), mod->getNeutralLossDiffFormulas().begin(), mod->getNeutralLossDiffFormulas().end());
      loss_names_.insert(loss_names_.end(), loss_names_.begin(), loss_names_.end());
    }
  }

  const ResidueModification* Residue::getModification() const
  {
    return modification_;
  }

  void Residue::setModification(const String& name)
  {
    ModificationsDB* mod_db = ModificationsDB::getInstance();
    const ResidueModification* mod = mod_db->getModification(name, one_letter_code_, ResidueModification::ANYWHERE);
    setModification(mod);
  }

  void Residue::setModification(const ResidueModification& mod)
  {
    ModificationsDB* mod_db = ModificationsDB::getInstance();
    //TODO think again. Most functions here or in ModificationsDB only check for fullID
    const ResidueModification* modindb = mod_db->searchModification(mod);
    if (modindb == nullptr)
    {
      modindb = mod_db->addNewModification_(mod);
    }
    setModification(modindb);
  }

  void Residue::setModificationByDiffMonoMass(double diffMonoMass)
  {
    ModificationsDB* mod_db = ModificationsDB::getInstance();
    bool multimatch = false;
    // quickly check for user-defined modification added by createUnknownFromMassString (e.g. M[+12321])
    String diffMonoMassStr = ResidueModification::getDiffMonoMassWithBracket(diffMonoMass);
    const ResidueModification* mod = mod_db->searchModificationsFast(one_letter_code_ + diffMonoMassStr, multimatch);
    const double tol = 0.002;
    if (mod == nullptr)
    {
      mod = mod_db->getBestModificationByDiffMonoMass(diffMonoMass, tol, one_letter_code_, ResidueModification::ANYWHERE);
    }
    if (mod == nullptr)
    {
      OPENMS_LOG_WARN << "Modification with monoisotopic mass diff. of " << diffMonoMassStr << " not found in databases with tolerance " << tol << ". Adding unknown modification." << std::endl;
      mod = ResidueModification::createUnknownFromMassString(String(diffMonoMass),
                                                                        diffMonoMass,
                                                                        true,
                                                                        ResidueModification::ANYWHERE,
                                                                        this);
    }
    setModification(mod);
  }

  const String& Residue::getModificationName() const
  {
    if (!isModified()) return String::EMPTY;
    return modification_->getId();
  }

  void Residue::setLowMassIons(const vector<EmpiricalFormula>& low_mass_ions)
  {
    low_mass_ions_ = low_mass_ions;
  }

  const vector<EmpiricalFormula>& Residue::getLowMassIons() const
  {
    return low_mass_ions_;
  }

  double Residue::getBackboneBasicityRight() const
  {
    return gb_bb_r_;
  }

  void Residue::setBackboneBasicityRight(double gb_bb_r)
  {
    gb_bb_r_ = gb_bb_r;
  }

  double Residue::getBackboneBasicityLeft() const
  {
    return gb_bb_l_;
  }

  void Residue::setBackboneBasicityLeft(double gb_bb_l)
  {
    gb_bb_l_ = gb_bb_l;
  }

  double Residue::getSideChainBasicity() const
  {
    return gb_sc_;
  }

  void Residue::setSideChainBasicity(double gb_sc)
  {
    gb_sc_ = gb_sc;
  }

  void Residue::setResidueSets(const set<String>& residue_sets)
  {
    residue_sets_ = residue_sets;
  }

  const set<String> & Residue::getResidueSets() const
  {
    return residue_sets_;
  }

  void Residue::addResidueSet(const String& residue_set)
  {
    residue_sets_.insert(residue_set);
  }

  bool Residue::isModified() const
  {
    return modification_ != nullptr;
  }

  bool Residue::hasNeutralLoss() const
  {
    return !loss_formulas_.empty();
  }

  bool Residue::hasNTermNeutralLosses() const
  {
    return !NTerm_loss_formulas_.empty();
  }

  bool Residue::operator==(const Residue& residue) const
  {
    return name_ == residue.name_ &&
           synonyms_ == residue.synonyms_ &&
           three_letter_code_ == residue.three_letter_code_ &&
           one_letter_code_ == residue.one_letter_code_ &&
           formula_ == residue.formula_ &&
           average_weight_ == residue.average_weight_ &&
           mono_weight_ == residue.mono_weight_ &&
           modification_ == residue.modification_ &&
           loss_names_ == residue.loss_names_ &&
           loss_formulas_ == residue.loss_formulas_ &&
           NTerm_loss_names_ == residue.NTerm_loss_names_ &&
           NTerm_loss_formulas_ == residue.NTerm_loss_formulas_ &&
           loss_average_weight_ == residue.loss_average_weight_ &&
           loss_mono_weight_ == residue.loss_mono_weight_ &&
           low_mass_ions_ == residue.low_mass_ions_ &&
           pka_ == residue.pka_ &&
           pkb_ == residue.pkb_ &&
           pkc_ == residue.pkc_ &&
           gb_sc_ == residue.gb_sc_ &&
           gb_bb_l_ == residue.gb_bb_l_ &&
           gb_bb_r_ == residue.gb_bb_r_ &&
           residue_sets_ == residue.residue_sets_;
  }

  bool Residue::operator==(char one_letter_code) const
  {
    return one_letter_code_[0] == one_letter_code;
  }

  bool Residue::operator!=(char one_letter_code) const
  {
    return one_letter_code_[0] != one_letter_code;
  }

  bool Residue::operator!=(const Residue& residue) const
  {
    return !(*this == residue);
  }

  bool Residue::isInResidueSet(const String& residue_set)
  {
    return residue_sets_.find(residue_set) != residue_sets_.end();
  }

  char Residue::residueTypeToIonLetter(const Residue::ResidueType& res_type)
  {
    switch (res_type)
    {
      case Residue::AIon: return 'a';
      case Residue::BIon: return 'b';
      case Residue::CIon: return 'c';
      case Residue::XIon: return 'x';
      case Residue::YIon: return 'y';
      case Residue::ZIon: return 'z';
      default:
       cerr << "Unknown residue type encountered. Can't map to ion letter." << endl;
    }
    return ' ';
  }

  String Residue::toString() const
  {
    if (getOneLetterCode().empty())
    {
      throw Exception::InvalidValue(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Residue does not have a OneLetterCode. This is a bug. Please report it!", "");
    }
    if (!isModified())
    {
      return one_letter_code_;
    }
    else
    { // this already contains the origin!
      return modification_->toString();
    }
  }

  ostream& operator<<(ostream& os, const Residue& residue)
  {
    os << residue.name_ << " "
    << residue.three_letter_code_ << " "
    << residue.one_letter_code_ << " "
    << residue.formula_;
    return os;
  }

}
