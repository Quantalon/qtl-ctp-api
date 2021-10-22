#include <pybind11/pybind11.h>

#include "bind_consts.h"
#include "bind_md_api.h"
#include "bind_td_api.h"


namespace py = pybind11;

PYBIND11_MODULE(_ctp, m) {
    bind_consts(m);
    bind_md_api(m);
    bind_td_api(m);
}
