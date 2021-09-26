import os
import platform
from pathlib import Path, PosixPath

# Blah, stupid pathing crap.
import buildsystem.PythonUtilities as utilities
from buildsystem.PythonConsoleLog import Log as log

class PremakeSetup:
    premakeVersion = "5.0.0-alpha16"
    premakeLicenseUrl = "https://raw.githubusercontent.com/premake/premake-core/master/LICENSE.txt"
    premakeDirectory = "./tools/thirdparty/premake"

    @classmethod
    def Validate(self):
        if (not self.__ValidatePremake()):
            log.Error("[Python] Premake Not Installed.")
            return False
        return True

    @classmethod
    def __ValidatePremake(self):
        if platform.system() == "Windows":
            premakeExecutablePath = Path(f"{self.premakeDirectory}/premake5.exe")
        elif platform.system() == "Linux":
            premakeExecutablePath = PosixPath(f"{self.premakeDirectory}/premake5.sh")
        else:
            log.Error(f"[Python] OS '{platform.system()}' Not Supported.")

        if (not premakeExecutablePath.exists()):
            log.Warn(f"[Python] Premake Not Found at Location: {self.premakeDirectory}")
            return self.__InstallPremake()

        log.Info("[Python] Premake Validation Succeeded.")
        return True

    @classmethod
    def __InstallPremake(self):
        permissionGranted = False
        while not permissionGranted:
            reply = str(input("[Python] Download Premake {0:s}? [Y/N]: ".format(self.premakeVersion))).lower().strip()[:1]
            if reply == 'n':
                return False
            permissionGranted = (reply == 'y')

        premakeZipUrl = ""
        if platform.system() == "Windows":
            premakePath = f"{self.premakeDirectory}/premake-{self.premakeVersion}-windows.zip"
            premakeZipUrl = f"https://github.com/premake/premake-core/releases/download/v{self.premakeVersion}/premake-{self.premakeVersion}-windows.zip"
        elif platform.system() == "Linux":
            premakePath = f"{self.premakeDirectory}/premake-{self.premakeVersion}-linux.tar.gz"
            premakeZipUrl = f"https://github.com/premake/premake-core/releases/download/v{self.premakeVersion}/premake-{self.premakeVersion}-linux.tar.gz"
        else:
            log.Error(f"[Python] OS '{platform.system()}' Not Supported.")

        if not premakeZipUrl:
            log.Error("[Python] Premake Download URL Failed.")

        log.Log("[Python] Downloading {0:s} to {1:s}".format(premakeZipUrl, premakePath))
        utilities.DownloadFile(premakeZipUrl, premakePath)
        log.Log(f"[Python] Extracting {premakePath}")
        utilities.UnzipFile(premakePath, deleteZipFile=True)
        log.Log(f"[Python] Premake {self.premakeVersion} Downloaded to '{self.premakeDirectory}'")

        premakeLicensePath = f"{self.premakeDirectory}/LICENSE.txt"
        log.Log("[Python] Downloading {0:s} to {1:s}".format(self.premakeLicenseUrl, premakeLicensePath))
        utilities.DownloadFile(self.premakeLicenseUrl, premakeLicensePath)
        log.Log(f"[Python] Premake License Downloaded to '{self.premakeDirectory}'")

        log.Log(f"[Python] Premake Installed at {os.path.abspath(self.premakeDirectory)}")
        return True
