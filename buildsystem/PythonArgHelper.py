import sys

from buildsystem.PythonConsoleLog import Log as log
import buildsystem.PythonConfig as const

class ArgHelper:
    _availableArgs = {
        const.HELP_COMMAND_SHORT            : '\t\t: Display this help menu.',
        const.HELP_COMMAND                  : '\t\t: Display this help menu.',
        const.VERBOSE_COMMAND_SHORT         : '\t\t: Perform build with verbose logging enabled.',
        const.VERBOSE_COMMAND               : '\t: Perform build with verbose logging enabled.',
        const.FORCE_SUBMODULE_COMMAND_SHORT : '\t\t: Force Submodules to update during build. Submodules will always update if any are missing.',
        const.FORCE_SUBMODULE_COMMAND       : '\t\t: Force Submodules to update during build. Submodules will always update if any are missing.',
        const.ONLY_SUBMODULE_COMMAND_SHORT  : '\t\t: Only perform submodule updates.',
        const.ONLY_SUBMODULE_COMMAND        : '\t: Only perform submodule updates.'
    }
    @classmethod
    def HandleHelpCommand(self):
        for param in sys.argv:
            if const.HELP_COMMAND_SHORT == param or const.HELP_COMMAND == param:
                self.__PrintHelpMenu()
                return True
        return False

    @classmethod
    def DetectAndSetVerbosity(self):
        for param in sys.argv:
            if const.VERBOSE_COMMAND_SHORT == param or const.VERBOSE_COMMAND == param:
                log.SetVerbose(True)
                log.Log("The Exelius engines build-system log verbosity has now been elevated to the highest of tiers.")
        
    @classmethod
    def ShouldForceSubmoduleUpdates(self):
        for param in sys.argv:
            if const.FORCE_SUBMODULE_COMMAND_SHORT == param or const.FORCE_SUBMODULE_COMMAND == param:
                return True
        return False

    @classmethod
    def OnlyUpdateSubmodules(self):
        for param in sys.argv:
            if const.ONLY_SUBMODULE_COMMAND_SHORT == param or const.ONLY_SUBMODULE_COMMAND == param:
                return True
        return False

    @classmethod
    def __PrintHelpMenu(self):
        log.SetVerbose(True)
        log.Log("Exelius Engine Build System Usage:")
        log.Log("\tpython Setup.py [<options>]")
        log.Log("Options:")
        for key, value in self._availableArgs.items():
            log.Log(key + value)
        log.Log("Example:")
        log.Log("\tpython Setup.py -v -f")
        log.Log("\t(Force submodules to update and build engine with verbose logging)")