# coding=UTF-8
"""
@Description:
@Author: Ziang Wang
@Date: 2025-08-24 23:16:33
@LastEditTime: 2025-08-24 23:16:33
"""
import os
import importlib
from setuptools import setup, find_packages
from os import path
from setuptools import Extension
from setuptools.command.build_ext import build_ext
import shutil
import subprocess


def load_version_module(pkg_path):
    spec = importlib.util.spec_from_file_location(
        'version', os.path.join(pkg_path, 'version.py'))
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


project_name = 'grpfpy'
_version_module = load_version_module(project_name)
__version__ = _version_module._get_version_for_build()

here = path.abspath(path.dirname(__file__))

with open("README.md", "r") as fh:
    long_description = fh.read()


# Custom build_ext to use CMake
class CMakeBuild(build_ext):

    def run(self):
        build_dir = os.path.join(here, 'build')
        os.makedirs(build_dir, exist_ok=True)
        # Run CMake
        subprocess.check_call(['cmake', '..'], cwd=build_dir)
        subprocess.check_call(['cmake', '--build', '.', '--config', 'Release'],
                              cwd=build_dir)
        # Find the .so file
        so_name = None
        for root, dirs, files in os.walk(build_dir):
            for f in files:
                if f.startswith('_grpfpy.cpython'):
                    so_name = os.path.join(root, f)
                    break
        if not so_name:
            raise RuntimeError("Could not find built cpython file")
        print(f"Found built cpython file: {so_name}")
        # Copy .so to build/lib.../grpfpy/
        for build_lib in os.listdir(os.path.join(here, 'build')):
            lib_path = os.path.join(here, 'build', build_lib, 'grpfpy')
            if os.path.isdir(lib_path):
                shutil.copy2(so_name, os.path.join(lib_path, os.path.basename(so_name)))
                print(f"Copied {so_name} to {lib_path}")
                break


setup(
    name='grpfpy',
    version=__version__,
    author='Ziang Wang',
    url='https://github.com/allegro0132/grpfc',
    description='the Global complex Root and Pole Finding (GRPF) algorithm.',
    long_description=long_description,
    long_description_content_type="text/markdown",
    packages=find_packages(),
    platforms='any',
    install_requires=['numpy'],
    extras_require={'test': ['pytest']},
    include_package_data=True,
    package_data={"grpfpy": ["*.so"]},
    ext_modules=[Extension('_grpfpy',
                           sources=[])],  # sources empty, built by cmake
    cmdclass={'build_ext': CMakeBuild},
    zip_safe=False,
)
