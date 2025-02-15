/**
 * @file ProcessBiomass.Test.cpp
 * @author Scott Rasmussen (scott.rasmussen@zaita.com)
 * @github https://github.com/Zaita
 * @date 20/10/2014
 * @section LICENSE
 *
 * Copyright Casal2 Project 2024 - https://github.com/Casal2/
 *
 */
#ifdef TESTMODE

// Headers
#include "Process.h"

#include <iostream>

#include "ObjectiveFunction/ObjectiveFunction.h"
#include "Observations/Manager.h"
#include "TestResources/TestFixtures/InternalEmptyModel.h"

// Namespaces
namespace niwa {
namespace age {

using niwa::testfixtures::InternalEmptyModel;
using std::cout;
using std::endl;

const std::string test_cases_observation_process_biomass =
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
type process_biomass
catchability [type=free; q=6.52606e-005]
time_step one
process halfm
categories male+female
selectivities male=[type=logistic; a50=9; ato95=4] female=[type=logistic; a50=9; ato95=4; alpha=0.7]
likelihood lognormal
years 1992:2001
delta 1e-10
table obs
1992    1.50   0.35
1993    1.10   0.35
1994    0.93   0.35
1995    1.33   0.35
1996    1.53   0.35
1997    0.90   0.35
1998    0.68   0.35
1999    0.75   0.35
2000    0.57   0.35
2001    1.23   0.35
end_table

@report DQ
type derived_quantity
)";

/**
 *
 */
TEST_F(InternalEmptyModel, Observation_Process_Biomass) {
  AddConfigurationLine(test_cases_observation_process_biomass, __FILE__, 32);
  LoadConfiguration();

  model_->Start(RunMode::kBasic);

  ObjectiveFunction& obj_function = model_->objective_function();
  EXPECT_DOUBLE_EQ(104784.87854324422, obj_function.score());

  Observation* observation = model_->managers()->observation()->GetObservation("observation");

  map<unsigned, vector<obs::Comparison> >& comparisons = observation->comparisons();
  ASSERT_EQ(10u, comparisons.size());

  unsigned year = 1992;
  ASSERT_FALSE(comparisons.find(year) == comparisons.end());
  ASSERT_EQ(1u, comparisons[year].size());
  EXPECT_EQ("male+female", comparisons[year][0].category_);
  EXPECT_DOUBLE_EQ(0.35, comparisons[year][0].error_value_);
  EXPECT_DOUBLE_EQ(-22.276816208066737, comparisons[year][0].expected_);
  EXPECT_DOUBLE_EQ(1.50, comparisons[year][0].observed_);
  EXPECT_DOUBLE_EQ(10651.526433300138, comparisons[year][0].score_);

  year = 1993;
  ASSERT_FALSE(comparisons.find(year) == comparisons.end());
  ASSERT_EQ(1u, comparisons[year].size());
  EXPECT_EQ("male+female", comparisons[year][0].category_);
  EXPECT_DOUBLE_EQ(0.35, comparisons[year][0].error_value_);
  EXPECT_DOUBLE_EQ(-22.273330558515898, comparisons[year][0].expected_);
  EXPECT_DOUBLE_EQ(1.1000000000000001, comparisons[year][0].observed_);
  EXPECT_DOUBLE_EQ(10518.701422461665, comparisons[year][0].score_);

  year = 1996;
  ASSERT_FALSE(comparisons.find(year) == comparisons.end());
  ASSERT_EQ(1u, comparisons[year].size());
  EXPECT_EQ("male+female", comparisons[year][0].category_);
  EXPECT_DOUBLE_EQ(0.35, comparisons[year][0].error_value_);
  EXPECT_DOUBLE_EQ(-22.260979027504177, comparisons[year][0].expected_);
  EXPECT_DOUBLE_EQ(1.53, comparisons[year][0].observed_);
  EXPECT_DOUBLE_EQ(10659.725504899203, comparisons[year][0].score_);

  year = 1998;
  ASSERT_FALSE(comparisons.find(year) == comparisons.end());
  ASSERT_EQ(1u, comparisons[year].size());
  EXPECT_EQ("male+female", comparisons[year][0].category_);
  EXPECT_DOUBLE_EQ(0.35, comparisons[year][0].error_value_);
  EXPECT_DOUBLE_EQ(-22.253873471029078, comparisons[year][0].expected_);
  EXPECT_DOUBLE_EQ(0.68000000000000005, comparisons[year][0].observed_);
  EXPECT_DOUBLE_EQ(10314.104407769524, comparisons[year][0].score_);

  year = 2001;
  ASSERT_FALSE(comparisons.find(year) == comparisons.end());
  ASSERT_EQ(1u, comparisons[year].size());
  EXPECT_EQ("male+female", comparisons[year][0].category_);
  EXPECT_DOUBLE_EQ(0.35, comparisons[year][0].error_value_);
  EXPECT_DOUBLE_EQ(-22.24770655278946, comparisons[year][0].expected_);
  EXPECT_DOUBLE_EQ(1.23, comparisons[year][0].observed_);
  EXPECT_DOUBLE_EQ(10565.926685658847, comparisons[year][0].score_);
}

}  // namespace age
} /* namespace niwa */

#endif /* TESTMODE */
