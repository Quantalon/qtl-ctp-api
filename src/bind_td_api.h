#ifndef BIND_TD_API_H
#define BIND_TD_API_H

#include <pybind11/pybind11.h>
#include "td_api.h"

namespace py = pybind11;

void bind_td_api(py::module &);

#endif //BIND_TD_API_H
