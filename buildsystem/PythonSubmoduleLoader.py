import os
import platform
import subprocess
from pathlib import Path, PosixPath
from buildsystem.PythonConsoleLog import Log as log
import buildsystem.PythonConfig as constants


class SubmoduleLoader:
    @classmethod
    def ValidateSubmodules(self, forceSubmoduleUpdate=False):
        moduleList = []
        log.Info("Validating Submodules.")
        log.Log("Retrieving Submodule List from '{0:s}'".format(os.path.abspath(constants.GITMODULES_FILE_PATH)))
        with open(constants.GITMODULES_FILE_PATH) as mods:
            lines = mods.read().splitlines()
            for line in lines:
                if constants.GITMODULES_PATH_VAR_EXTRACT in line:
                    extractorLength = len(constants.GITMODULES_PATH_VAR_EXTRACT)
                    modulePath = os.path.abspath(line[extractorLength:])
                    moduleList.append(modulePath)

        shouldUpdateSubmodules = forceSubmoduleUpdate
        for path in moduleList:
            if platform.system() == "Windows":
                modulePath = Path(path)
            elif platform.system() == "Linux":
                modulePath = PosixPath(path)
            else:
                log.Error("OS '{0:s}' Not Supported.".format(platform.system()))
                return

            if (modulePath.exists()):
                log.Log("Submodule Found at Location: {0:s}".format(path))
            else:
                log.Log("Submodule Not Found at Location: {0:s}".format(path))
                shouldUpdateSubmodules = True
        log.Info("Submodule Validation Complete.")

        if shouldUpdateSubmodules:
            self.__UpdateSubmodules()

    @classmethod
    def __UpdateSubmodules(self):
        log.Info("Updating Submodules.")
        subprocess.call(["git", "submodule", "update", "--init", "--recursive"])
        log.Info("Submodules Update Complete.")