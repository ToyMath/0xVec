import sys
from setuptools import setup, Extension
import pybind11

cpp_args = ['-std=c++11']

ext_modules = [
    Extension(
        'oxvec',
        ['bindings.cpp', 'oxvec.cpp'],
        include_dirs=[
            pybind11.get_include(),
        ],
        language='c++',
        extra_compile_args=cpp_args,
    ),
]

setup(
    name='oxvec',
    version='0.2',
    author='Aakash Apoorv',
    description='0xVec: In-Memory Vector Database with improved numerical stability',
    ext_modules=ext_modules,
)
