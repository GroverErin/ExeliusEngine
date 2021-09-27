import platform

class Log:
    _verbose = False
    RED_FG     = '\033[91m'
    GREEN_FG   = '\033[92m'
    YELLOW_FG  = '\033[93m'
    WHITE_FG   = '\033[97m'
    NORMAL     = '\033[0m'

    @classmethod
    def SetVerbose(self, isVerbose):
        self._verbose = isVerbose

    @classmethod
    def Log(self, text):
        if not self._verbose:
            return
        if (platform.system() == "Linux"):
            print(self.WHITE_FG + "[Python] " + text + self.NORMAL)
        else:
            print("[Python] " + text)

    @classmethod
    def Info(self, text):
        if (platform.system() == "Linux"):
            print(self.GREEN_FG + "[Python] " + text + self.NORMAL)
        else:
            print("[Python] " + text)

    @classmethod
    def Warn(self, text):
        if (platform.system() == "Linux"):
            print(self.YELLOW_FG + "[Python] " + text + self.NORMAL)
        else:
            print("[Python] " + text)

    @classmethod
    def Error(self, text):
        if (platform.system() == "Linux"):
            print(self.RED_FG + "[Python] " + text + self.NORMAL)
        else:
            print("[Python] " + text)