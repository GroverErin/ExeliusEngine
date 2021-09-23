# TODO: Escape characters don't seem to work on Windows CMD.
#       I could install colorama or termcolor...
class Log:
    Default_FG = '\033[39m'
    Default_BG = '\033[49m'
    Grey_FG    = '\033[90m'
    Grey_BG    = '\033[100m'
    Red_FG     = '\033[91m'
    Red_BG     = '\033[101m'
    Green_FG   = '\033[92m'
    Green_BG   = '\033[102m'
    Yellow_FG  = '\033[93m'
    Yellow_BG  = '\033[103m'
    Blue_FG    = '\033[94m'
    Blue_BG    = '\033[104m'
    Pink_FG    = '\033[95m'
    Pink_BG    = '\033[105m'
    Cyan_FG    = '\033[96m'
    Cyan_BG    = '\033[106m'
    White_FG   = '\033[97m'
    White_BG   = '\033[107m'
    Normal     = '\033[0m'
    Bold       = '\033[1m'
    Italic     = '\033[3m'
    Underline  = '\033[4m'

    @classmethod
    def Log(self, text):
        print(self.White_FG + text + self.Normal)

    @classmethod
    def Info(self, text):
        print(self.Green_FG + text + self.Normal)

    @classmethod
    def Warn(self, text):
        print(self.Yellow_FG + text + self.Normal)

    @classmethod
    def Error(self, text):
        print(self.Red_FG + text + self.Normal)