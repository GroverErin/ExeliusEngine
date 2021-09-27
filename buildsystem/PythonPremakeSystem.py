import os
import platform
import subprocess
from pathlib import Path, PosixPath

# Blah, stupid pathing crap.
from buildsystem.PythonUtilities import Utilities as utilities
from buildsystem.PythonConsoleLog import Log as log
import buildsystem.PythonConfig as const

class PremakeSystem:
    @classmethod
    def Validate(self):
        log.Info("Validating Premake Installation.")
        if (not self.__ValidatePremake()):
            log.Error("Premake Not Installed.")
            return False

        log.Info("Premake Validation Complete.")
        return True

    @classmethod
    def Run(self):
        log.Info("Executing Premake.")

        if platform.system() == "Windows":
            subprocess.call([os.path.abspath("./tools/thirdparty/premake/premake5.exe"), "vs2019", "--file=./buildsystem/PremakeMain.lua", "nopause"])
        elif platform.system() == "Linux":
            log.Info("Linux.")
            #subprocess.call([os.path.abspath("./tools/thirdparty/premake/premake5"), "gmake2", "--file=./buildsystem/PremakeMain.lua", "nopause"])
        else:
            log.Error("OS '{0:s}' Not Supported.".format(platform.system()))
            exit() # Cannot complete setup.

        log.Info("Premake Generation Completed.")

    @classmethod
    def __ValidatePremake(self):
        if platform.system() == "Windows":
            premakeExecutablePath = Path("{0:s}premake5.exe".format(const.PREMAKE_UNPACK_PATH))
        elif platform.system() == "Linux":
            premakeExecutablePath = PosixPath("{0:s}premake5".format(const.PREMAKE_UNPACK_PATH))
        else:
            log.Error("OS '{0:s}' Not Supported.".format(platform.system()))

        if (not premakeExecutablePath.exists()):
            log.Warn("Premake Not Found at Location: {0:s}".format(os.path.abspath(const.PREMAKE_UNPACK_PATH)))
            return self.__UnpackPremake()

        log.Info("Premake Validation Succeeded.")
        return True

    @classmethod
    def __UnpackPremake(self):
        packedPremakeFilename = ""
        if 'armv' in platform.machine():
            packedPremakeFilename = const.PREMAKE_RPI_TAR_FILE
        elif platform.system() == "Windows":
            packedPremakeFilename = const.PREMAKE_WINDOWS_ZIP_FILE
        elif platform.system() == "Linux":
            packedPremakeFilename = const.PREMAKE_LINUX_TAR_FILE
        else:
            log.Error("OS '{0:s}' Not Supported.".format(platform.system()))
            return False

        premakePackedPath = os.path.abspath(const.PREMAKE_PACKED_PATH + packedPremakeFilename)
        premakeUnpackPath = os.path.abspath(const.PREMAKE_UNPACK_PATH)

        return utilities.UnpackFile(premakePackedPath, premakeUnpackPath)