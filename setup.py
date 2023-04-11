import sys

try:
    from skbuild import setup
    import nanobind
except ImportError:
    print("The preferred way to invoke 'setup.py' is via pip, as in 'pip "
          "install .'. If you wish to run the setup script directly, you must "
          "first install the build dependencies listed in pyproject.toml!",
          file=sys.stderr)
    raise


setup(
    name="qtl-ctp-api",
    version="6.6.9.4",
    long_description="QTL CTP Api",
    long_description_content_type='text/markdown',
    packages=['qtl_ctp_api'],
    package_dir={'': 'src'},
    cmake_install_dir='src/qtl_ctp_api',
)
