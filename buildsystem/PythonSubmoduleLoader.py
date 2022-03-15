import os
import platform
import subprocess
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
            if (os.path.isdir(path) and (len(os.listdir(path)) != 0)):
                log.Log("Submodule Found at Location: {0:s}".format(path))
            else:
                log.Log("Submodule Not Found at Location: {0:s}".format(path))
                shouldUpdateSubmodules = True
        log.Info("Submodule Validation Complete.")

        if shouldUpdateSubmodules:
            self._UpdateSubmodules()

    @classmethod
    def _UpdateSubmodules(self):
        log.Info("Updating Submodules.")
        log.Warn("Some failure messages are expected, particularly with EASTL.")
        subprocess.call(["git", "submodule", "update", "--init", "--recursive", "--quiet"])

        # Does nothing on non-windows platforms.
        # This is necessary because calling git
        # functions disables vt100 emulation, and
        # it needs to be manually reenabled.
        log.EnableWindowsColorLog()
        log.Info("Submodules Update Complete.")