-- Create a new module for premake and get it.
premake.modules.exeliusConsoleLog = {}
local exeliusLog = premake.modules.exeliusConsoleLog

-- Sets the default logging color
local defaultColor = term.getTextColor()
local isVerbose = false

if _OPTIONS["verbosity"] == "high" then
    isVerbose = true
end

function exeliusLog.Log(text)
    if isVerbose then
        term.setTextColor(term.white)
        print(text)
        term.setTextColor(defaultColor)
    end
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