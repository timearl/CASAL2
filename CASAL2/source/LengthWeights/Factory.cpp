/**
 * @file Factory.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 24/07/2013
 * @section LICENSE
 *
 * Copyright Casal2 Project 2024 - https://github.com/Casal2/
 *
 */

// headers
#include "Factory.h"

#include "../LengthWeights/Common/Basic.h"
#include "../LengthWeights/Common/None.h"
#include "../LengthWeights/Manager.h"
#include "../Model/Managers.h"
#include "../Model/Model.h"

// namespaces
namespace niwa {
namespace lengthweights {

/**
 * Create the instance of the object as defined by the two parameters
 * object_type and sub_type.
 *
 * @param object_type The type of object to create (e.g., age_size, process)
 * @param sub_type The child type of the object to create (e.g., ageing, schnute)
 * @return shared_ptr to the object
 */
LengthWeight* Factory::Create(shared_ptr<Model> model, const string& object_type, const string& sub_type) {
  LengthWeight* result = nullptr;

  if (object_type == PARAM_LENGTH_WEIGHT || object_type == PARAM_LENGTH_WEIGHTS) {
    if (sub_type == PARAM_NONE)
      result = new None(model);
    else if (sub_type == PARAM_BASIC)
      result = new Basic(model);

    if (result)
      model->managers()->length_weight()->AddObject(result);
  }

  return result;
}

} /* namespace lengthweights */
} /* namespace niwa */
