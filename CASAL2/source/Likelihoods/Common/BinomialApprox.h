/**
 * @file BinomialApprox.h
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @version 1.0
 * @date 22/03/2013
 * @section LICENSE
 *
 * Copyright Casal2 Project 2024 - https://github.com/Casal2/
 *
 * @section DESCRIPTION
 *
 * The time class represents a moment of time.
 *
 * $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
 */
#ifndef LIKELIHOODS_BINOMIALAPPROX_H_
#define LIKELIHOODS_BINOMIALAPPROX_H_

// Headers
#include "../../Likelihoods/Likelihood.h"

// Namespaces
namespace niwa {
namespace likelihoods {

/**
 * Class definition
 */
class BinomialApprox : public niwa::Likelihood {
public:
  // Methods
  BinomialApprox(shared_ptr<Model> model) : Likelihood(model) {};
  virtual ~BinomialApprox() = default;
  void   DoValidate() override final {};
  Double AdjustErrorValue(const Double process_error, const Double error_value) override final;
  void   GetScores(map<unsigned, vector<observations::Comparison> >& comparisons) override final;
  void   SimulateObserved(map<unsigned, vector<observations::Comparison> >& comparisons) override final;
};

} /* namespace likelihoods */
} /* namespace niwa */
#endif /* LIKELIHOODS_BINOMIALAPPROX_H_ */
