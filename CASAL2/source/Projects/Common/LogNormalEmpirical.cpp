/**
 * @file LogNormalEmpirical.cpp
 * @author  Craig Marsh
 * @date 07/04/2017
 * @section LICENSE
 *
 * Copyright Casal2 Project 2024 - https://github.com/Casal2/
 *
 */

// headers
#include "LogNormalEmpirical.h"

#include "../../Model/Objects.h"
#include "../../Utilities/RandomNumberGenerator.h"

// namespaces
namespace niwa {
namespace projects {

/**
 * Default constructor
 */

LogNormalEmpirical::LogNormalEmpirical(shared_ptr<Model> model) : Project(model) {
  parameters_.Bind<Double>(PARAM_MEAN, &mean_, "The mean of the Gaussian process", "", 0.0);
  parameters_.Bind<unsigned>(PARAM_START_YEAR, &start_year_, "The start year of sampling", "", false);
  parameters_.Bind<unsigned>(PARAM_FINAL_YEAR, &final_year_, "The final year of sampling", "", false);
}

/**
 * Validate
 */
void LogNormalEmpirical::DoValidate() {
  // if no values specified then set default as the model lifespan
  if (!parameters_.Get(PARAM_START_YEAR)->has_been_defined())
    start_year_ = model_->start_year();
  if (!parameters_.Get(PARAM_FINAL_YEAR)->has_been_defined())
    final_year_ = model_->final_year();

  if (start_year_ < model_->start_year())
    LOG_ERROR_P(PARAM_START_YEAR) << start_year_ << " must be greater than or equal to the model start year " << model_->start_year();
  if (final_year_ > model_->final_year())
    LOG_ERROR_P(PARAM_FINAL_YEAR) << final_year_ << " must be less than or equal to the model final year " << model_->final_year();

  if (final_year_ <= start_year_)
    LOG_ERROR_P(PARAM_FINAL_YEAR) << final_year_ << " must be greater than start year " << start_year_;
}

/**
 * Reset
 */
void LogNormalEmpirical::DoReset() {
  utilities::RandomNumberGenerator& rng = utilities::RandomNumberGenerator::Instance();
  // Empirically calculate the years to sample from
  Double   Random_draw = 0.0;
  unsigned year        = 0;
  for (unsigned project_year : years_) {
    Random_draw = floor(rng.uniform((double)start_year_, ((double)final_year_ + 0.99999)));
    year        = 0;
    if (!utilities::To<Double>(Random_draw, year))
      LOG_ERROR() << "Random draw " << Random_draw << " could not be converted to Double";
    resampled_years_[project_year] = year;
    LOG_FINEST() << "Value from year: " << year << " used in projection year: " << project_year;
  }
  // Empirically calculate sigma R from the resampled years
  vector<Double> values;
  Double         Total    = 0;
  Double         Total_sq = 0;
  // m=sqrt((norm2(d)-sum(d)*sum(d)/n)/(n-1)); // this is CASAL's std-dev function
  for (auto resampled_years : resampled_years_) {
    values.push_back(log(stored_values_[resampled_years.second]));

    Total += log(stored_values_[resampled_years.second]);
    Total_sq += log(stored_values_[resampled_years.second]) * log(stored_values_[resampled_years.second]);
    LOG_FINEST() << "using value " << stored_values_[resampled_years.second] << " sampled from year " << resampled_years.second << " to be applied in year "
                 << resampled_years.first;
  }
  LOG_FINEST() << "Total = " << Total;

  // we must be in the first reset call ()
  Double Var = Total_sq - Total * Total / values.size();
  Var /= (values.size() - 1);
  sigma_ = sqrt(Var);
  LOG_FINEST() << "sigma = " << sigma_ << " mean = " << mean_ << " variance = " << Var;  //<< " rho = " << rho_;

  for (unsigned project_year : years_) {
    // if (parameters_.Get(PARAM_RHO)->has_been_defined()) {
    //  normal_draw_by_year_[project_year] = rng.normal(0.0, 1.0);
    //} else {
    normal_draw_by_year_[project_year] = rng.normal(AS_DOUBLE(mean_), AS_DOUBLE(sigma_));
    //}
    LOG_FINEST() << "generated value = " << normal_draw_by_year_[project_year] << " to be applied in year " << project_year;
  }
}

/**
 *  Update the parameter with a random draw from a lognormal distribution with specified inputs
 */
void LogNormalEmpirical::DoUpdate() {
  /*
  if (parameters_.Get(PARAM_RHO)->has_been_defined()) {
    LOG_WARNING() << "Calculating an AR(1) process for projections.";
    Double last_value_ = projected_parameters_[model_->current_year() - 1];
    Double Z = lognormal_draw_by_year_[model_->current_year()];
    value_ = exp((alpha_ + rho_ * last_value_ + Z) - 0.5 * sigma_ * sigma_);
  } else {
  */
  // Just a standard normal deviation
  value_ = exp(normal_draw_by_year_[model_->current_year()] - 0.5 * sigma_ * sigma_);

  // Store this value to be pulled out next projection year
  value_ = value_ * multiplier_;

  LOG_FINE() << "Setting value to: " << value_;
  (this->*DoUpdateFunc_)(value_, true, model_->current_year());
}

} /* namespace projects */
} /* namespace niwa */
