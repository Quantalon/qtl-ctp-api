import qtl_ctp_api as m


def test_basic():
    print(m)


def test_consts():
    print(m.consts)
    print(f'THOST_FTDC_EXP_Normal: {m.consts.THOST_FTDC_EXP_Normal}')
    print(f'THOST_FTDC_ICT_TaxNo: {m.consts.THOST_FTDC_ICT_TaxNo}')
    print(f'THOST_FTDC_FC_ExitEmergency: {m.consts.THOST_FTDC_FC_ExitEmergency}')


if __name__ == '__main__':
    test_basic()
    test_consts()
