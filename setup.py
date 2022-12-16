import sys

try:
    from skbuild import setup
except ImportError:
    print(
        "Please update pip, you need pip 10 or greater,\n"
        " or you need to install the PEP 518 requirements in pyproject.toml yourself",
        file=sys.stderr,
    )
    raise

from setuptools import find_packages


setup(
    name="qtl-ctp-api",
    version="6.6.9.3",
    long_description="QTL CTP Api",
    long_description_content_type='text/markdown',
    packages=find_packages(where='src'),
    package_dir={"": "src"},
    cmake_install_dir="src/qtl_ctp_api",
)
