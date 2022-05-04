#include "EXEPCH.h"

#include "source/engine/settings/ConfigFile.h"
#include "source/debug/LogManager.h"
#include "source/utility/io/File.h"

#include <EASTL/vector.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	ConfigFile::ConfigFile()
		: m_pFileName("engine_config.ini")
		, m_isOpen(false)
	{
		//
	}

	bool ConfigFile::OpenConfigFile()
	{
		EXE_ASSERT(m_pFileName);
		File configFile;

		if (!configFile.Open(m_pFileName, File::AccessPermission::kReadOnly, File::CreationType::kOpenFile))
		{
			EXE_LOG_WARN("Failed to open '{}'.", m_pFileName);
			m_isOpen = false;
			return false;
		}

		// Read in the data.
		eastl::vector<std::byte> fileData(configFile.GetSize());
		size_t readBytes = configFile.Read(fileData);

		if (readBytes != configFile.GetSize())
		{
			EXE_LOG_WARN("Failed to read '{}'. Read {} of {} bytes.", m_pFileName, readBytes, configFile.GetSize());
			m_isOpen = false;
			return false;
		}

		const eastl::string stringData(reinterpret_cast<const char*>(fileData.data()), configFile.GetSize());

		if (m_parsedData.Parse(stringData.c_str()).HasParseError())
		{
			auto errorCode = m_parsedData.GetParseError();
			EXE_LOG_WARN("Failed to Parse JSON for file '{}'. Error Code: {}", m_pFileName, errorCode);
			m_isOpen = false;
			return false;
		}

		EXE_ASSERT(m_parsedData.IsObject());
		m_isOpen = true;
		return true;
	}

	bool ConfigFile::PopulateLogData(FileLogDefinition& fileLog, ConsoleLogDefinition& consoleLog, eastl::vector<LogData>& logData) const
	{
		if (!m_isOpen)
		{
			EXE_LOG_ERROR("Failed to populate log data: Config File is not open or parsed correctly.");
			return false;
		}

		bool populationResult = true;
		if (!PopulateFileLogDefinition(fileLog))
		{
			EXE_LOG_WARN("Failed to populate the file log definition. Some defaults may have been used.");
			populationResult = false;
		}
		if (!PopulateConsoleLogDefiniton(consoleLog))
		{
			EXE_LOG_WARN("Failed to populate the console log definition. Some defaults may have been used.");
			populationResult = false;
		}
		if (!PopulateLogs(logData, "EngineLogs"))
		{
			EXE_LOG_WARN("Failed to populate Engine logs correctly. Some defaults may have been used.");
			populationResult = false;
		}
		if (!PopulateLogs(logData, "ClientLogs"))
		{
			EXE_LOG_WARN("Failed to populate Client logs. Some defaults may have been used.");
			populationResult = false;
		}

		return populationResult;
	}

	bool ConfigFile::PopulateWindowData(WindowProperties& windowProperties) const
	{
		if (!m_isOpen)
		{
			EXE_LOG_ERROR("Failed to populate window data: Config File is not open or parsed correctly.");
			return false;
		}

		bool populationResult = true;
		if (!PopulateWindowTitle(windowProperties.m_title))
		{
			EXE_LOG_WARN("Failed to populate window title. Some defaults may have been used.");
			populationResult = false;
		}
		if (!PopulateWindowSize(windowProperties.m_windowSize))
		{
			EXE_LOG_WARN("Failed to populate the window size. Some defaults may have been used.");
			populationResult = false;
		}
		if (!PopulateWindowVSync(windowProperties.m_isVSync))
		{
			EXE_LOG_WARN("Failed to populate window vsync setting. Some defaults may have been used.");
			populationResult = false;
		}

		return true;
	}

	//---------------------------------------------------------------------------------------------------------------
	// Private
	//---------------------------------------------------------------------------------------------------------------

	bool ConfigFile::PopulateFileLogDefinition(FileLogDefinition& fileLog) const
	{
		// Traverse tree to "Log".
		if (!m_parsedData.HasMember("Log"))
		{
			EXE_LOG_WARN("'Log' member not found in config file.\nDefaulting Pattern to: {}\nDefaulting Output Directory to: {}\nDefaulting Max File Size to: {}\nDefaulting Num Files to: {}\nDefaulting File Rotation to: {}", fileLog.m_pattern.c_str(), fileLog.m_outputDirectory.c_str(), fileLog.m_maxFileSize, fileLog.m_numFiles, fileLog.m_rotateOnOpen);
			return false;
		}
		if (!m_parsedData["Log"].IsObject())
		{
			EXE_LOG_WARN("'Log' member in config file is not an Object.\nDefaulting Pattern to: {}\nDefaulting Output Directory to: {}\nDefaulting Max File Size to: {}\nDefaulting Num Files to: {}\nDefaulting File Rotation to: {}", fileLog.m_pattern.c_str(), fileLog.m_outputDirectory.c_str(), fileLog.m_maxFileSize, fileLog.m_numFiles, fileLog.m_rotateOnOpen);
			return false;
		}

		// Traverse tree to "Definitions".
		if (!m_parsedData["Log"].HasMember("Definitions"))
		{
			EXE_LOG_WARN("'Definitions' member not found in 'Log'.\nDefaulting Pattern to: {}\nDefaulting Output Directory to: {}\nDefaulting Max File Size to: {}\nDefaulting Num Files to: {}\nDefaulting File Rotation to: {}", fileLog.m_pattern.c_str(), fileLog.m_outputDirectory.c_str(), fileLog.m_maxFileSize, fileLog.m_numFiles, fileLog.m_rotateOnOpen);
			return false;
		}
		auto definitionsMember = m_parsedData["Log"].FindMember("Definitions");
		EXE_ASSERT(definitionsMember != m_parsedData["Log"].MemberEnd());
		if (!definitionsMember->value.IsObject())
		{
			EXE_LOG_WARN("'Definitions' member in 'Log' is not an Object.\nDefaulting Pattern to: {}\nDefaulting Output Directory to: {}\nDefaulting Max File Size to: {}\nDefaulting Num Files to: {}\nDefaulting File Rotation to: {}", fileLog.m_pattern.c_str(), fileLog.m_outputDirectory.c_str(), fileLog.m_maxFileSize, fileLog.m_numFiles, fileLog.m_rotateOnOpen);
			return false;
		}

		// Traverse tree to "File".
		if (!definitionsMember->value.HasMember("File"))
		{
			EXE_LOG_WARN("'File' member not found in 'Definitions'.\nDefaulting Pattern to: {}\nDefaulting Output Directory to: {}\nDefaulting Max File Size to: {}\nDefaulting Num Files to: {}\nDefaulting File Rotation to: {}", fileLog.m_pattern.c_str(), fileLog.m_outputDirectory.c_str(), fileLog.m_maxFileSize, fileLog.m_numFiles, fileLog.m_rotateOnOpen);
			return false;
		}
		auto fileMember = definitionsMember->value.FindMember("File");
		EXE_ASSERT(fileMember != definitionsMember->value.MemberEnd());
		if (!fileMember->value.IsObject())
		{
			EXE_LOG_WARN("'File' member in 'Definitions' is not an Object.\nDefaulting Pattern to: {}\nDefaulting Output Directory to: {}\nDefaulting Max File Size to: {}\nDefaulting Num Files to: {}\nDefaulting File Rotation to: {}", fileLog.m_pattern.c_str(), fileLog.m_outputDirectory.c_str(), fileLog.m_maxFileSize, fileLog.m_numFiles, fileLog.m_rotateOnOpen);
			return false;
		}

		bool successResult = true;
		if (fileMember->value.HasMember("Pattern") && fileMember->value["Pattern"].IsString())
		{
			fileLog.m_pattern = fileMember->value["Pattern"].GetString();
		}
		else
		{
			EXE_LOG_WARN("'Pattern' member in 'File' was not found or is not a string. Defaulting Pattern to: {}", fileLog.m_pattern.c_str());
			successResult = false;
		}

		if (fileMember->value.HasMember("OutDir") && fileMember->value["OutDir"].IsString())
		{
			fileLog.m_outputDirectory = fileMember->value["OutDir"].GetString();
		}
		else
		{
			EXE_LOG_WARN("'OutDir' member in 'File' was not found or is not a string. Defaulting Output Directory to: {}", fileLog.m_outputDirectory.c_str());
			successResult = false;
		}

		if (fileMember->value.HasMember("MaxSize") && fileMember->value["MaxSize"].IsUint())
		{
			fileLog.m_maxFileSize = fileMember->value["MaxSize"].GetUint();
		}
		else
		{
			EXE_LOG_WARN("'MaxSize' member in 'File' was not found or is not an unsigned integer type. Defaulting Max File Size to: {}", fileLog.m_maxFileSize);
			successResult = false;
		}

		if (fileMember->value.HasMember("NumFiles") && fileMember->value["NumFiles"].IsUint())
		{
			fileLog.m_numFiles = fileMember->value["NumFiles"].GetUint();
		}
		else
		{
			EXE_LOG_WARN("'NumFiles' member in 'File' was not found or is not an unsigned integer type. Defaulting Num Files to: {}", fileLog.m_numFiles);
			successResult = false;
		}

		if (fileMember->value.HasMember("RotateOnOpen") && fileMember->value["RotateOnOpen"].IsBool())
		{
			fileLog.m_rotateOnOpen = fileMember->value["RotateOnOpen"].GetBool();
		}
		else
		{
			EXE_LOG_WARN("'RotateOnOpen' member in 'File' was not found or is not a boolean type. Defaulting File Rotation to: {}", fileLog.m_rotateOnOpen);
			successResult = false;
		}

		return successResult;
	}

	bool ConfigFile::PopulateConsoleLogDefiniton(ConsoleLogDefinition& consoleLog) const
	{
		// Traverse tree to "Log".
		if (!m_parsedData.HasMember("Log"))
		{
			EXE_LOG_WARN("'Log' member not found in config file. Defaulting Pattern to: {}", consoleLog.m_pattern.c_str());
			return false;
		}
		if (!m_parsedData["Log"].IsObject())
		{
			EXE_LOG_WARN("'Log' member in config file is not an Object. Defaulting Pattern to: {}", consoleLog.m_pattern.c_str());
			return false;
		}

		// Traverse tree to "Definitions".
		if (!m_parsedData["Log"].HasMember("Definitions"))
		{
			EXE_LOG_WARN("'Definitions' member not found in 'Log'. Defaulting Pattern to: {}", consoleLog.m_pattern.c_str());
			return false;
		}
		auto definitionsMember = m_parsedData["Log"].FindMember("Definitions");
		EXE_ASSERT(definitionsMember != m_parsedData["Log"].MemberEnd());
		if (!definitionsMember->value.IsObject())
		{
			EXE_LOG_WARN("'Definitions' member in 'Log' is not an Object. Defaulting Pattern to: {}", consoleLog.m_pattern.c_str());
			return false;
		}

		// Traverse tree to "Console".
		if (!definitionsMember->value.HasMember("Console"))
		{
			EXE_LOG_WARN("'Console' member not found in 'Definitions'. Defaulting Pattern to: {}", consoleLog.m_pattern.c_str());
			return false;
		}
		auto consoleMember = definitionsMember->value.FindMember("Console");
		EXE_ASSERT(consoleMember != definitionsMember->value.MemberEnd());
		if (!consoleMember->value.IsObject())
		{
			EXE_LOG_WARN("'Console' member in 'Definitions' is not an Object. Defaulting Pattern to: {}", consoleLog.m_pattern.c_str());
			return false;
		}

		bool successResult = true;
		if (consoleMember->value.HasMember("Pattern") && consoleMember->value["Pattern"].IsString())
		{
			consoleLog.m_pattern = consoleMember->value["Pattern"].GetString();
		}
		else
		{
			EXE_LOG_WARN("'Pattern' member in 'Console' was not found or is not a string. Defaulting Pattern to: {}", consoleLog.m_pattern.c_str());
			successResult = false;
		}

		return true;
	}

	bool ConfigFile::PopulateLogs(eastl::vector<LogData>& logData, const char* pCategoryName) const
	{
		// Traverse tree to "Log".
		if (!m_parsedData.HasMember("Log"))
		{
			EXE_LOG_WARN("'Log' member not found in config file.");
			return false;
		}
		if (!m_parsedData["Log"].IsObject())
		{
			EXE_LOG_WARN("'Log' member in config file is not an Object.");
			return false;
		}

		// Traverse tree to Category Name.
		if (!m_parsedData["Log"].HasMember(pCategoryName))
		{
			EXE_LOG_WARN("'{}' member not found in 'Log'.", pCategoryName);
			return false;
		}
		auto logCategoryMember = m_parsedData["Log"].FindMember(pCategoryName);
		EXE_ASSERT(logCategoryMember != m_parsedData["Log"].MemberEnd());
		if (!logCategoryMember->value.IsArray())
		{
			EXE_LOG_WARN("'{}' member in 'Log' is not an Array.", pCategoryName);
			return false;
		}

		// Add the log members.
		bool successResult = true;
		for (rapidjson::SizeType i = 0; i < logCategoryMember->value.Size(); ++i)
		{
			if (!logCategoryMember->value[i].IsObject())
			{
				EXE_LOG_WARN("Member at index {} in '{}' is not an Object.", static_cast<size_t>(i), pCategoryName);
				successResult = false;
			}
			if (!logCategoryMember->value[i].HasMember("Name"))
			{
				EXE_LOG_WARN("Object at index {} in '{}' has no member 'Name'. Defaulting to: {}", static_cast<size_t>(i), pCategoryName, "UnnamedLog" + std::to_string(static_cast<size_t>(i)));
				successResult = false;
			}
			if (!logCategoryMember->value[i].FindMember("Name")->value.IsString())
			{
				EXE_LOG_WARN("'Name' member of Object at index {} in '{}' is not a string. Defaulting to: {}", static_cast<size_t>(i), pCategoryName, "UnnamedLog" + std::to_string(static_cast<size_t>(i)));
				successResult = false;
			}
			if (!logCategoryMember->value[i].HasMember("LogLocation"))
			{
				EXE_LOG_WARN("Object at index {} in '{}' has no member 'LogLocation'. Defaulting to: {}", static_cast<size_t>(i), pCategoryName, static_cast<int>(LogLocation::kConsole));
				successResult = false;
			}
			if (!logCategoryMember->value[i].FindMember("LogLocation")->value.IsUint())
			{
				EXE_LOG_WARN("'LogLocation' member of Object at index {} in '{}' is not an unsigned integer type. Defaulting to: {}", static_cast<size_t>(i), pCategoryName, static_cast<int>(LogLocation::kConsole));
				successResult = false;
			}
			if (!logCategoryMember->value[i].HasMember("LogLevel"))
			{
				EXE_LOG_WARN("Object at index {} in '{}' has no member 'LogLevel'. Defaulting to: {}", static_cast<size_t>(i), pCategoryName, static_cast<int>(LogLevel::kTrace));
				successResult = false;
			}
			if (!logCategoryMember->value[i].FindMember("LogLevel")->value.IsUint())
			{
				EXE_LOG_WARN("'LogLevel' member of Object at index {} in '{}' is not an unsigned integer type. Defaulting to: {}", static_cast<size_t>(i), pCategoryName, static_cast<int>(LogLevel::kTrace));
				successResult = false;
			}

			LogData newLogData;
			newLogData.m_logName = logCategoryMember->value[i].FindMember("Name")->value.GetString();

			unsigned int logLocation = logCategoryMember->value[i].FindMember("LogLocation")->value.GetUint();
			if (static_cast<LogLocation>(logLocation) >= LogLocation::kMax)
			{
				logLocation = (static_cast<int>(LogLocation::kMax) - 1);
				EXE_LOG_WARN("'LogLocation' member of Object at index {} in '{}' is out of bounds. Defaulting to: {}", static_cast<size_t>(i), pCategoryName, static_cast<int>(LogLocation::kConsole));
				successResult = false;
			}

			unsigned int logLevel = logCategoryMember->value[i].FindMember("LogLevel")->value.GetUint();

			if (static_cast<LogLevel>(logLevel) >= LogLevel::kMax)
			{
				logLevel = static_cast<int>(LogLevel::kFatal);
				EXE_LOG_WARN("'LogLevel' member of Object at index {} in '{}' is out of bounds. Defaulting to: {}", static_cast<size_t>(i), pCategoryName, static_cast<int>(LogLevel::kFatal));
				successResult = false;
			}

			newLogData.m_logLocation = static_cast<LogLocation>(logLocation);
			newLogData.m_logLevel = static_cast<LogLevel>(logLevel);

			logData.emplace_back(newLogData);
		}

		return successResult;
	}

	bool ConfigFile::PopulateWindowTitle(eastl::string& windowTitle) const
	{
		// Traverse tree to "Window".
		if (!m_parsedData.HasMember("Window"))
		{
			EXE_LOG_WARN("'Window' member not found in config file. Defaulting Window Title to: {}", windowTitle.c_str());
			return false;
		}
		if (!m_parsedData["Window"].IsObject())
		{
			EXE_LOG_WARN("'Window' member in config file is not an Object. Defaulting Window Title to: {}", windowTitle.c_str());
			return false;
		}

		// Traverse tree to "WindowTitle".
		if (!m_parsedData["Window"].HasMember("WindowTitle"))
		{
			EXE_LOG_WARN("'WindowTitle' member not found in 'Window'. Defaulting Window Title to: {}", windowTitle.c_str());
			return false;
		}
		auto windowTitleMember = m_parsedData["Window"].FindMember("WindowTitle");
		EXE_ASSERT(windowTitleMember != m_parsedData["Window"].MemberEnd());
		if (!windowTitleMember->value.IsString())
		{
			EXE_LOG_WARN("'WindowTitle' is not a String. Defaulting Window Title to: {}", windowTitle.c_str());
			return false;
		}

		windowTitle = windowTitleMember->value.GetString();

		return true;
	}

	bool ConfigFile::PopulateWindowSize(glm::vec2& windowSize) const
	{
		// Traverse tree to "Window".
		if (!m_parsedData.HasMember("Window"))
		{
			EXE_LOG_WARN("'Window' member not found in config file. Defaulting Window Size to: ({}, {})", windowSize.x, windowSize.y);
			return false;
		}
		if (!m_parsedData["Window"].IsObject())
		{
			EXE_LOG_WARN("'Window' member in config file is not an Object. Defaulting Window Size to: ({}, {})", windowSize.x, windowSize.y);
			return false;
		}

		// Traverse tree to "WindowWidth".
		if (!m_parsedData["Window"].HasMember("WindowWidth"))
		{
			EXE_LOG_WARN("'WindowWidth' member not found in 'Window'. Defaulting Window Size to: ({}, {})", windowSize.x, windowSize.y);
			return false;
		}
		auto windowWidthMember = m_parsedData["Window"].FindMember("WindowWidth");
		EXE_ASSERT(windowWidthMember != m_parsedData["Window"].MemberEnd());
		if (!windowWidthMember->value.IsUint())
		{
			EXE_LOG_WARN("'WindowWidth' is not an unsigned int type. Defaulting Window Size to: ({}, {})", windowSize.x, windowSize.y);
			return false;
		}

		// Traverse tree to "WindowHeight".
		if (!m_parsedData["Window"].HasMember("WindowHeight"))
		{
			EXE_LOG_WARN("'WindowHeight' member not found in 'Window'. Defaulting Window Size to: ({}, {})", windowSize.x, windowSize.y);
			return false;
		}
		auto windowHeightMember = m_parsedData["Window"].FindMember("WindowHeight");
		EXE_ASSERT(windowHeightMember != m_parsedData["Window"].MemberEnd());
		if (!windowHeightMember->value.IsUint())
		{
			EXE_LOG_WARN("'WindowHeight' is not an unsigned int type. Defaulting Window Size to: ({}, {})", windowSize.x, windowSize.y);
			return false;
		}

		windowSize.x = windowWidthMember->value.GetFloat();
		windowSize.y = windowHeightMember->value.GetFloat();

		return true;
	}

	bool ConfigFile::PopulateWindowVSync(bool& isVsyncEnabled) const
	{
		// Traverse tree to "Window".
		if (!m_parsedData.HasMember("Window"))
		{
			EXE_LOG_WARN("'Window' member not found in config file. Defaulting VSync to: {}", isVsyncEnabled);
			return false;
		}
		if (!m_parsedData["Window"].IsObject())
		{
			EXE_LOG_WARN("'Window' member in config file is not an Object. Defaulting VSync to: {}", isVsyncEnabled);
			return false;
		}

		// Traverse tree to "VSyncEnabled".
		if (!m_parsedData["Window"].HasMember("VSyncEnabled"))
		{
			EXE_LOG_WARN("'VSyncEnabled' member not found in 'Window'. Defaulting VSync to: {}", isVsyncEnabled);
			return false;
		}
		auto vsyncMember = m_parsedData["Window"].FindMember("VSyncEnabled");
		EXE_ASSERT(vsyncMember != m_parsedData["Window"].MemberEnd());
		if (!vsyncMember->value.IsBool())
		{
			EXE_LOG_WARN("'VSyncEnabled' is not a boolean type. Defaulting VSync to: {}", isVsyncEnabled);
			return false;
		}

		isVsyncEnabled = vsyncMember->value.GetBool();

		return true;
	}
}