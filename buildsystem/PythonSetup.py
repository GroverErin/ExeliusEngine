import importlib.util as importlib_util
import subprocess
import sys

from buildsystem.PythonConsoleLog import Log as log

class PythonSetup:
    packages = ["requests"]

    @classmethod
    def ValidateVersion(self):
        return self.__ValidatePython()

    @classmethod
    def InstallRequiredPackages(self):
        for packageName in self.packages:
            if not self.__ValidatePackage(packageName):
                return False# cannot validate further
        return True

    @classmethod
    def __ValidatePython(self, versionMajor = 3, versionMinor = 3):
        if sys.version is not None:
            log.Log("Version {0:d}.{1:d}.{2:d} Detected.".format(sys.version_info.major, sys.version_info.minor, sys.version_info.micro))
            if sys.version_info.major < versionMajor or (sys.version_info.major == versionMajor and sys.version_info.minor < versionMinor):
                log.Error("Version Too Low. Expected Version {0:d}.{1:d} or Higher.".format(versionMajor, versionMinor))
                return False
            return True
        else:
            log.Error("System Version is None.")
            return False

    @classmethod
    def __ValidatePackage(self, packageName):
        if importlib_util.find_spec(packageName) is None:
            log.Error(f"Package '{packageName}' Not Found.")
            return self.__InstallPackage(packageName)
        return True

    @classmethod
    def __InstallPackage(self, packageName):

        permissionGranted = False
        while not permissionGranted:
            reply = str(input("Install Python package '{0:s}'? [Y/N]: ".format(packageName))).lower().strip()[:1]
            if reply == 'n':
                return False
            permissionGranted = (reply == 'y')
        
        log.Log(f"Installing '{packageName}' Package.")

        # When debugging, ensure you have the packages installed, otherwise this
        # can result in an infinite loop. This is likely due to VSCode possibly
        # running in a special "sandboxed" environment. Installing packages also
        # requires Admin privilages, which may be another issue.
        # TODO: See if possible to ask for admin privilages here.
        subprocess.check_call(['python', '-m', 'pip', 'install', packageName])

        # Package should be installed here, otherwise check_call would throw an
        # error message.
        return True