#ifndef BIND_TD_API_H
#define BIND_TD_API_H

#include <nanobind/nanobind.h>
#include "td_api.h"

namespace nb = nanobind;

void bind_td_api(nb::module_ &);

#endif //BIND_TD_API_H
