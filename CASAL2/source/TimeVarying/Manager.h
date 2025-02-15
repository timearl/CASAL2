/**
 * @file Manager.h
 * @author Scott Rasmussen (scott.rasmussen@zaita.com)
 * @github https://github.com/Zaita
 * @date 27/01/2015
 * @section LICENSE
 *
 * Copyright Casal2 Project 2024 - https://github.com/Casal2/
 *
 * @section DESCRIPTION
 *
 * << Add Description >>
 */
#ifndef TIMEVARYING_MANAGER_H_
#define TIMEVARYING_MANAGER_H_

// Headers
#include "../BaseClasses/Manager.h"
#include "../TimeVarying/TimeVarying.h"

// Namespaces
namespace niwa {
namespace timevarying {

/**
 * Class Definition
 */
class Manager : public niwa::base::Manager<niwa::timevarying::Manager, niwa::TimeVarying> {
  friend class niwa::base::Manager<niwa::timevarying::Manager, niwa::TimeVarying>;
  friend class niwa::Managers;

public:
  // methods
  virtual ~Manager() = default;
  void         Update(unsigned current_year);
  bool         IsTimeVarying(const string& label);
  bool         IsTimeVaryingTarget(const string& label);
  TimeVarying* GetTimeVarying(const string& label);
  unsigned     GetTimeVaryingCount();
  void         RestoreOriginalValue();


protected:
  // methods
  Manager();
};

} /* namespace timevarying */
} /* namespace niwa */
#endif /* TIMEVARYING_MANAGER_H_ */
