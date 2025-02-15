/**
 * @file ObjectiveFunction.h
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @version 1.0
 * @date 21/02/2013
 * @section LICENSE
 *
 * Copyright Casal2 Project 2024 - https://github.com/Casal2/
 *
 * @section DESCRIPTION
 *
 * The objective function is the score for the model. Each model
 * run is capable of generating a score, the lower the score
 * the better the run.
 *
 * The objective function is used when minimising the model to
 * try and find a set of parameters that fit the best.
 *
 * $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
 */
#ifndef OBJECTIVEFUNCTION_H_
#define OBJECTIVEFUNCTION_H_

// Headers
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../Utilities/Types.h"

// Namespaces
namespace niwa {
class Model;
using niwa::utilities::Double;
using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;

/**
 * Struct definition
 */
namespace objective {
struct Score {
  string label_;
  Double score_;
};
}  // namespace objective

/**
 * Class definition
 */
class ObjectiveFunction {
  friend class Model;

public:
  // Methods
  virtual ~ObjectiveFunction() = default;
  void CalculateScore();
  void Clear();

  // Accessors
  const vector<objective::Score>& score_list() const { return score_list_; }
  Double                          score() const { return score_; }
  Double                          penalties() const { return penalties_; }
  Double                          priors() const { return priors_; }
  Double                          likelihoods() const { return likelihoods_; }
  Double                          additional_priors() const { return additional_priors_; }
  Double                          jacobians() const { return jacobians_; }

private:
  // methods
  ObjectiveFunction(shared_ptr<Model> model);

  // Members
  shared_ptr<Model>        model_             = nullptr;
  Double                   score_             = 0.0;
  Double                   penalties_         = 0.0;
  Double                   priors_            = 0.0;
  Double                   likelihoods_       = 0.0;
  Double                   additional_priors_ = 0.0;
  Double                   jacobians_         = 0.0;
  vector<objective::Score> score_list_;
};

} /* namespace niwa */
#endif /* OBJECTIVEFUNCTION_H_ */
