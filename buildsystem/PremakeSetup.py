import os
import platform
from pathlib import Path, PosixPath

# Blah, stupid pathing crap.
import BuildSystem.PythonUtilities as Utilities
from BuildSystem.PythonConsoleLog import Log as Log

class PremakeSetup:
    premakeVersion = "5.0.0-alpha16"
    premakeLicenseUrl = "https://raw.githubusercontent.com/premake/premake-core/master/LICENSE.txt"
    premakeDirectory = "./tools/thirdparty/premake"

    @classmethod
    def Validate(self):
        if (not self.__ValidatePremake()):
            Log.Error("[Python] Premake Not Installed.")
            return False
        return True

    @classmethod
    def __ValidatePremake(self):
        if platform.system() == "Windows":
            premakeExecutablePath = Path(f"{self.premakeDirectory}/premake5.exe")
        elif platform.system() == "Linux":
            premakeExecutablePath = PosixPath(f"{self.premakeDirectory}/premake5.sh")
        else:
            Log.Error(f"[Python] OS '{platform.system()}' Not Supported.")

        if (not premakeExecutablePath.exists()):
            Log.Warn(f"[Python] Premake Not Found at Location: {self.premakeDirectory}")
            return self.__InstallPremake()

        Log.Info("[Python] Premake Validation Succeeded.")
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
            Log.Error(f"[Python] OS '{platform.system()}' Not Supported.")

        if not premakeZipUrl:
            Log.Error("[Python] Premake Download URL Failed.")

        Log.Log("[Python] Downloading {0:s} to {1:s}".format(premakeZipUrl, premakePath))
        Utilities.DownloadFile(premakeZipUrl, premakePath)
        Log.Log(f"[Python] Extracting {premakePath}")
        Utilities.UnzipFile(premakePath, deleteZipFile=True)
        Log.Log(f"[Python] Premake {self.premakeVersion} Downloaded to '{self.premakeDirectory}'")

        premakeLicensePath = f"{self.premakeDirectory}/LICENSE.txt"
        Log.Log("[Python] Downloading {0:s} to {1:s}".format(self.premakeLicenseUrl, premakeLicensePath))
        Utilities.DownloadFile(self.premakeLicenseUrl, premakeLicensePath)
        Log.Log(f"[Python] Premake License Downloaded to '{self.premakeDirectory}'")

        Log.Log(f"[Python] Premake Installed at {os.path.abspath(self.premakeDirectory)}")
        return True
