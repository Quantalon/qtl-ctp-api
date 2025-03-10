#include <nanobind/nanobind.h>

#include "bind_consts.h"
#include "bind_md_api.h"
#include "bind_td_api.h"


namespace nb = nanobind;

NB_MODULE(_ctp, m) {
    bind_consts(m);
    bind_md_api(m);
    bind_td_api(m);
}
