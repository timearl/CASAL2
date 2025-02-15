/**
 * @file Normal.h
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 24/09/2014
 * @section LICENSE
 *
 * Copyright Casal2 Project 2024 - https://github.com/Casal2/
 *
 * @section DESCRIPTION
 *
 * << Add Description >>
 */
#ifndef ESTIMATES_CREATORS_NORMAL_H_
#define ESTIMATES_CREATORS_NORMAL_H_

// headers
#include "../../../Estimates/Creators/Creator.h"

// namespaces
namespace niwa {
namespace estimates {
namespace creators {

/**
 *
 */
class Normal : public estimates::Creator {
public:
  Normal() = delete;
  explicit Normal(shared_ptr<Model> model);
  virtual ~Normal() = default;
  void DoCopyParameters(niwa::Estimate* estimate, unsigned index) override final;

private:
  // members
  vector<Double> mu_;
  vector<Double> cv_;
};

} /* namespace creators */
} /* namespace estimates */
} /* namespace niwa */

#endif /* NORMAL_H_ */
