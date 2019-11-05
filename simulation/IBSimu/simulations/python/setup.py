#!/usr/bin/env python3
# encoding: utf-8

from distutils.core import setup, Extension
import pkgconfig
pkg = pkgconfig.parse('ibsimu-1.0.6 umfpack')
hello_module = Extension('IBSimulate', sources = ['einzel.cpp'], include_dirs=pkg["include_dirs"], libraries=pkg['libraries'], library_dirs=pkg['library_dirs'])

setup(name='IBSimulate',
      version='0.1.0',
      description='Simulate an ion beam',
      ext_modules=[hello_module])
