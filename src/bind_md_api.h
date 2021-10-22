#ifndef BIND_MD_API_H
#define BIND_MD_API_H

#include <pybind11/pybind11.h>
#include "md_api.h"

namespace py = pybind11;

void bind_md_api(py::module &);

#endif //BIND_MD_API_H
