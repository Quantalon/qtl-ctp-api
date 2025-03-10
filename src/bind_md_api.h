#ifndef BIND_MD_API_H
#define BIND_MD_API_H

#include <nanobind/nanobind.h>
#include "md_api.h"

namespace nb = nanobind;

void bind_md_api(nb::module_ &);

#endif //BIND_MD_API_H
