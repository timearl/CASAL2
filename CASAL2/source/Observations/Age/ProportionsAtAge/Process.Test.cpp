/**
 * @file ProcessProportionsAtAge.Test.cpp
 * @author Scott Rasmussen (scott.rasmussen@zaita.com)
 * @github https://github.com/Zaita
 * @date 17/10/2014
 * @section LICENSE
 *
 * Copyright Casal2 Project 2024 - https://github.com/Casal2/
 *
 */
#ifdef TESTMODE

// Headers
#include <iostream>

#include "ObjectiveFunction/ObjectiveFunction.h"
#include "Observations/Manager.h"
#include "TestResources/TestFixtures/InternalEmptyModel.h"
#include "TimeStep.h"

// Namespaces
namespace niwa {
namespace age {

using niwa::testfixtures::InternalEmptyModel;
using std::cout;
using std::endl;

const std::string test_cases_observation_proportions_at_age_single =
    R"(
@model
min_age 2
max_age 25
age_plus t
start_year 1975
final_year 2002
base_weight_units kgs
initialisation_phases phase1
time_steps one two three

@categories
format sex
names male female
age_lengths age_size_male age_size_female

@initialisation_phase phase1
years 200
exclude_processes fishing

@time_step one
processes halfm fishing

@time_step two
processes halfm

@time_step three
processes recruitment ageing
@derived_quantity ssb
type biomass
categories male female
time_step_proportion 1.0
selectivities male_maturity female_maturity
time_step one

@ageing ageing
categories male female
@recruitment recruitment
type beverton_holt
categories male female
proportions 0.5 0.5
r0 5e6
steepness 0.9
recruitment_multipliers 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00
ssb ssb
standardise_years 1975:2001
age 2

@mortality halfm
type constant_rate
categories male female
relative_m_by_age [type=constant; c=1] halfm.one
m 0.20 0.20
time_step_proportions 0.5 0.5

@mortality fishing
type event_biomass
categories male female
years 1975:2002
catches 1191 1488 1288 2004 609 750 997 596 302 344 544 362 509 574 804 977 991 2454 2775 2898 4094 3760 3761 3673 3524 3700 3700 3700
U_max 0.9
selectivities observation.male observation.female
penalty event_mortality_penalty

@selectivity male_maturity
type logistic
a50 5
ato95 2

@selectivity female_maturity
type logistic
a50 5
ato95 2

@selectivity one
type constant
c 1
@age_length age_size_male
type von_bertalanffy
length_weight [type=none]
k 0.277
t0 0.11
linf 90.3

@age_length age_size_female
type von_bertalanffy
length_weight [type=none]
k 0.202
t0 -0.20
linf 113.4

@penalty event_mortality_penalty
type process
log_scale True
multiplier 10

@observation observation
type process_proportions_at_age
likelihood lognormal
time_step two
process halfm
categories male+female
min_age 2
max_age 14
selectivities male=[type=logistic; a50=9; ato95=4] female=[type=logistic; a50=9; ato95=4; alpha=0.7]
years 1992
table obs
1992 0.0241 0.0473 0.0448 0.071 0.078 0.104 0.0672 0.1213 0.0869 0.111 0.0788 0.0436 0.122
end_table
table error_values
1992 1.399 0.795 0.764 0.663 0.724 0.735 0.709 0.684 0.673 0.59 0.669 0.878 0.53
end_table

@report DQ
type derived_quantity
)";

/**
 *
 */
TEST_F(InternalEmptyModel, Observation_Process_Proportions_At_Age_Single) {
  AddConfigurationLine(test_cases_observation_proportions_at_age_single, __FILE__, 31);
  LoadConfiguration();

  model_->Start(RunMode::kBasic);

  ObjectiveFunction& obj_function = model_->objective_function();
  EXPECT_DOUBLE_EQ(250.08682113999456, obj_function.score());

  Observation* observation = model_->managers()->observation()->GetObservation("observation");

  map<unsigned, vector<obs::Comparison> >& comparisons = observation->comparisons();
  ASSERT_EQ(1u, comparisons.size());

  unsigned year = 1992;
  ASSERT_FALSE(comparisons.find(year) == comparisons.end());
  ASSERT_EQ(13u, comparisons[year].size());
  EXPECT_EQ("male+female", comparisons[year][0].category_);
  EXPECT_DOUBLE_EQ(1.399, comparisons[year][0].error_value_);
  EXPECT_NEAR(0, comparisons[year][0].expected_, 1e-6);
  EXPECT_DOUBLE_EQ(0.0241, comparisons[year][0].observed_);
  EXPECT_DOUBLE_EQ(240.56840045905705, comparisons[year][0].score_);

  EXPECT_EQ("male+female", comparisons[year][1].category_);
  EXPECT_DOUBLE_EQ(0.79500000000000004, comparisons[year][1].error_value_);
  EXPECT_NEAR(0.0070693859439931051, comparisons[year][1].expected_, 1e-6);
  EXPECT_DOUBLE_EQ(0.047300000000000002, comparisons[year][1].observed_);
  EXPECT_DOUBLE_EQ(4.3426123755188861, comparisons[year][1].score_);

  EXPECT_EQ("male+female", comparisons[year][2].category_);
  EXPECT_DOUBLE_EQ(0.76400000000000001, comparisons[year][2].error_value_);
  EXPECT_NEAR(0.0119291330, comparisons[year][2].expected_, 1e-6);
  EXPECT_DOUBLE_EQ(0.0448, comparisons[year][2].observed_);
  EXPECT_DOUBLE_EQ(2.2347239537272152, comparisons[year][2].score_);

  EXPECT_EQ("male+female", comparisons[year][3].category_);
  EXPECT_DOUBLE_EQ(0.66300000000000003, comparisons[year][3].error_value_);
  EXPECT_NEAR(0.019859549506180619, comparisons[year][3].expected_, 1e-6);
  EXPECT_DOUBLE_EQ(0.070999999999999994, comparisons[year][3].observed_);
  EXPECT_DOUBLE_EQ(2.4050903064568567, comparisons[year][3].score_);

  EXPECT_EQ("male+female", comparisons[year][4].category_);
  EXPECT_DOUBLE_EQ(0.72399999999999998, comparisons[year][4].error_value_);
  EXPECT_NEAR(0.032194953597, comparisons[year][4].expected_, 1e-6);
  EXPECT_DOUBLE_EQ(0.078, comparisons[year][4].observed_);
  EXPECT_DOUBLE_EQ(0.99209112268452371, comparisons[year][4].score_);
}

const std::string test_cases_observation_proportions_at_age_double =
    R"(
@model
min_age 2
max_age 25
age_plus t
start_year 1975
final_year 2002
base_weight_units kgs
initialisation_phases phase1
time_steps one two three
@categories
format sex
names male female
age_lengths age_size_male age_size_female

@initialisation_phase phase1
years 200
exclude_processes fishing

@time_step one
processes halfm fishing

@time_step two
processes halfm

@time_step three
processes recruitment ageing

@derived_quantity ssb
type biomass
categories male female
time_step_proportion 1.0
selectivities male_maturity female_maturity
time_step one

@ageing ageing
categories male female

@recruitment recruitment
type beverton_holt
categories male female
proportions 0.5 0.5
r0 5e6
age 2
steepness 0.9
recruitment_multipliers 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00 1.00
ssb ssb
standardise_years 1975:2001

@mortality halfm
type constant_rate
categories male female
relative_m_by_age [type=constant; c=1] halfm.one
m 0.20 0.20
time_step_proportions 0.5 0.5

@mortality fishing
type event_biomass
categories male female
years 1975:2002
catches 1191 1488 1288 2004 609 750 997 596 302 344 544 362 509 574 804 977 991 2454 2775 2898 4094 3760 3761 3673 3524 3700 3700 3700
U_max 0.9
selectivities observation.male observation.female
penalty event_mortality_penalty

@selectivity male_maturity
type logistic
a50 5
ato95 2

@selectivity female_maturity
type logistic
a50 5
ato95 2

@selectivity one
type constant
c 1

@age_length age_size_male
type von_bertalanffy
length_weight [type=none]
k 0.277
t0 0.11
linf 90.3

@age_length age_size_female
type von_bertalanffy
length_weight [type=none]
k 0.202
t0 -0.20
linf 113.4

@penalty event_mortality_penalty
type process
log_scale True
multiplier 10

@observation observation
type process_proportions_at_age
likelihood lognormal
time_step one
process halfm
categories male female
min_age 3
max_age 15
selectivities male=[type=logistic; a50=9; ato95=4] female=[type=logistic; a50=9; ato95=4; alpha=0.7]
years 1992
table obs
1992 0.0173 0.0193 0.0241 0.0346 0.0365 0.0657 0.0427 0.0667 0.0326 0.0307 0.0272 0.0141 0.0319 0.0353 0.0249 0.0146 0.0133 0.0547 0.0488 0.0745 0.0660 0.0750 0.0646 0.0304 0.0147 0.0399
end_table
table error_values
1992 1.091 0.770 0.539 0.421 0.412 0.297 0.367 0.322 0.391 0.510 0.523 0.734 0.481 0.612 0.643 0.756 0.772 0.399 0.369 0.331 0.306 0.304 0.309 0.461 0.752 0.423
end_table

@report DQ
type derived_quantity
)";

/**
 *
 */
TEST_F(InternalEmptyModel, Observation_Process_Proportions_At_Age_Double) {
  AddConfigurationLine(test_cases_observation_proportions_at_age_double, __FILE__, 194);
  LoadConfiguration();

  model_->Start(RunMode::kBasic);

  ObjectiveFunction& obj_function = model_->objective_function();
  EXPECT_NEAR(30.135766436, obj_function.score(), 1e-6);

  Observation* observation = model_->managers()->observation()->GetObservation("observation");

  map<unsigned, vector<obs::Comparison> >& comparisons = observation->comparisons();
  ASSERT_EQ(1u, comparisons.size());

  unsigned year = 1992;
  ASSERT_FALSE(comparisons.find(year) == comparisons.end());
  ASSERT_EQ(26u, comparisons[year].size());
  EXPECT_EQ("male", comparisons[year][0].category_);
  EXPECT_DOUBLE_EQ(1.091, comparisons[year][0].error_value_);
  EXPECT_NEAR(0.00415746, comparisons[year][0].expected_, 1e-6);
  EXPECT_DOUBLE_EQ(0.0173, comparisons[year][0].observed_);
  EXPECT_NEAR(1.9856983223045572, comparisons[year][0].score_, 1e-6);

  EXPECT_EQ("male", comparisons[year][1].category_);
  EXPECT_DOUBLE_EQ(0.770, comparisons[year][1].error_value_);
  EXPECT_NEAR(0.0070154, comparisons[year][1].expected_, 1e-6);
  EXPECT_DOUBLE_EQ(0.0193, comparisons[year][1].observed_);
  EXPECT_NEAR(1.2818091773704532, comparisons[year][1].score_, 1e-6);

  EXPECT_EQ("male", comparisons[year][2].category_);
  EXPECT_DOUBLE_EQ(0.539, comparisons[year][2].error_value_);
  EXPECT_NEAR(0.01167942, comparisons[year][2].expected_, 1e-6);
  EXPECT_DOUBLE_EQ(0.0241, comparisons[year][2].observed_);
  EXPECT_NEAR(0.7396146371416138, comparisons[year][2].score_, 1e-6);

  EXPECT_EQ("male", comparisons[year][3].category_);
  EXPECT_DOUBLE_EQ(0.421, comparisons[year][3].error_value_);
  EXPECT_NEAR(0.0189341, comparisons[year][3].expected_, 1e-6);
  EXPECT_DOUBLE_EQ(0.0346, comparisons[year][3].observed_);
  EXPECT_NEAR(0.52912129877249725, comparisons[year][3].score_, 1e-6);

  EXPECT_EQ("male", comparisons[year][4].category_);
  EXPECT_DOUBLE_EQ(0.412, comparisons[year][4].error_value_);
  EXPECT_NEAR(0.0292178, comparisons[year][4].expected_, 1e-6);
  EXPECT_DOUBLE_EQ(0.0365, comparisons[year][4].observed_);
  EXPECT_NEAR(-0.63765233048750125, comparisons[year][4].score_, 1e-6);
}

}  // namespace age
} /* namespace niwa */

#endif /* TESTMODE */
