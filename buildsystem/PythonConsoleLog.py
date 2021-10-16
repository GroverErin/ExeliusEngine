import platform
import ctypes

class Log:
    _verbose = False
    RED_FG      = '\033[91m'
    GREEN_FG    = '\033[92m'
    YELLOW_FG   = '\033[93m'
    WHITE_FG    = '\033[97m'
    NORMAL      = '\033[0m'

    # This function may need to be called multiple times,
    # VT100 emulation stops on some function calls like
    # git or premake executions.
    @classmethod
    def EnableWindowsColorLog(self):
        if platform.system() == "Windows":
            # This code allows windows terminals to print colors logs.
            # Found in comment on StackOverflow question below:
            # https://stackoverflow.com/questions/287871/how-to-print-colored-text-to-the-terminal
            kernel32 = ctypes.WinDLL('kernel32')
            hStdOut = kernel32.GetStdHandle(-11)
            mode = ctypes.c_ulong()
            kernel32.GetConsoleMode(hStdOut, ctypes.byref(mode))
            mode.value |= 4
            kernel32.SetConsoleMode(hStdOut, mode)

    @classmethod
    def SetVerbose(self, isVerbose):
        self._verbose = isVerbose

    @classmethod
    def Log(self, text):
        if not self._verbose:
            return
        print(self.WHITE_FG + "[Python] " + text + self.NORMAL)

    @classmethod
    def Info(self, text):
        print(self.GREEN_FG + "[Python] " + text + self.NORMAL)

    @classmethod
    def Warn(self, text):
        print(self.YELLOW_FG + "[Python] " + text + self.NORMAL)

    @classmethod
    def Error(self, text):
        print(self.RED_FG + "[Python] " + text + self.NORMAL)