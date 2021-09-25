-- Create a new module for premake and get it.
premake.modules.exeliusConsoleLog = {}
local exeliusLog = premake.modules.exeliusConsoleLog

-- Sets the default logging color
local defaultColor = term.getTextColor()

function exeliusLog.Log(text)
    term.setTextColor(term.white)
    print(text)
    term.setTextColor(defaultColor)
end

function exeliusLog.Info(text)
    term.setTextColor(term.green)
    print(text)
    term.setTextColor(defaultColor)
end

function exeliusLog.Warn(text)
    term.setTextColor(term.yellow)
    print(text)
    term.setTextColor(defaultColor)
end

function exeliusLog.Error(text)
    term.setTextColor(term.red)
    print(text)
    term.setTextColor(defaultColor)
end

return exeliusLog