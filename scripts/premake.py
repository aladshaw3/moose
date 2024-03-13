#!/usr/bin/env python3
#* This file is part of the MOOSE framework
#* https://www.mooseframework.org
#*
#* All rights reserved, see COPYRIGHT for full restrictions
#* https://github.com/idaholab/moose/blob/master/COPYRIGHT
#*
#* Licensed under LGPL 2.1, please see LICENSE for details
#* https://www.gnu.org/licenses/lgpl-2.1.html

import subprocess, os, sys
from versioner import Versioner

class PreMake:
    def __init__(self):
        self.conda_env = self.getCondaEnv()
        self.versioner_meta = Versioner().version_meta()

    @staticmethod
    def printColored(msg, color, **kwargs):
        """
        Prints a colored message
        """
        color_vals = {'red': 31, 'green': 32, 'yellow': 33}
        if color not in color_vals:
            raise Exception('Unknown prefix color {}'.format(color))
        print(f'\033[{color_vals[color]}m{msg}\033[0m', **kwargs)

    @staticmethod
    def warn(msg):
        """
        Prints a warning in yellow
        """
        PreMake.printColored(f'WARNING: {msg}', 'yellow', file=sys.stderr)

    @staticmethod
    def condaList():
        """
        Gets the conda package list in JSON form

        Separated out on purpose so that we can mock it in unit tests,
        running tests even when conda doesn't exist.
        """
        cmd = ['conda', 'list', '--json']
        process = subprocess.run(cmd, stdout=subprocess.PIPE,
                                    stderr=subprocess.PIPE,
                                    encoding='utf-8')
        if process.returncode != 0:
            raise Exception(f"Failed to get conda env: '{' '.join(cmd)}'\n{process.stdout}")

        try:
            import json
        except Exception as e:
            raise Exception(f"Failed to import json for conda list") from e

        return json.loads(process.stdout)

    @staticmethod
    def getCondaEnv():
        """
        Gets the conda package environment in the form of a dict of
        package name to information about that package.

        The keys that we require to exist for each package entry:
        version, build_string

        Will return None if conda is not available.
        """
        conda_env = None

        if os.environ.get('CONDA_PREFIX') is not None:
            conda_list = PreMake.condaList()

            conda_env = {}
            for entry in conda_list:
                conda_env[entry['name']] = entry

                # Make sure the keys exist that we expect to be there
                for key in ['base_url', 'build_string', 'version']:
                    if key not in entry:
                        raise Exception(f'Missing expected key {key} in conda package list')

        return conda_env

    class CondaVersionMismatch(Exception):
        """
        Exception that denotes the mismatch of a version or build
        in a conda packages
        """
        def __init__(self, package, version, required_version, build, required_build, msg=None):
            self.package = package
            self.version = version
            self.required_version = required_version
            self.build = build
            self.required_build = required_build

            current = f'{version}'
            required = f'{required_version}'
            # Same version, different build, include build string
            if version == required_version:
                current += f'={build}'
                required += f'={required_build}'

            full_message = "Conda package 'moose-dev' is currently at version "
            full_message += f"'{current}', but the required version is '{required}'."
            if msg:
                full_message += f'\n{msg}'
            Exception.__init__(self, full_message)

    def check(self):
        """
        Checks for build issues
        """
        try:
            self._check()
        except self.CondaVersionMismatch as e:
            self.warn(str(e))

    def _checkCondaPackage(self, package_name, versioner_name=None):
        """
        Internal method for checking if a given conda package is up to date
        """
        package = self.conda_env.get(package_name)
        if package:
            if versioner_name is None:
                versioner_name = package_name
            version = package['version']
            versioner_version = self.versioner_meta[versioner_name]['conda']['version']
            build = package['build_string']
            # we store the integer in Version, not the full string
            versioner_build = f"build_{self.versioner_meta[versioner_name]['conda']['build']}"
            if version != versioner_version or (versioner_build is not None and build != versioner_build):
                raise self.CondaVersionMismatch(package_name, version, versioner_version,
                                                build, versioner_build)


    def _check(self):
        """
        Internal check method
        """
        # We have conda available, check those environments
        if self.conda_env:
            self._checkCondaPackage('moose-dev')
            self._checkCondaPackage('moose-libmesh', 'libmesh')
            self._checkCondaPackage('moose-petsc', 'petsc')
            self._checkCondaPackage('moose-mpich', 'mpich')
            self._checkCondaPackage('moose-wasp', 'wasp')

if __name__ == '__main__':
    PreMake().check()
