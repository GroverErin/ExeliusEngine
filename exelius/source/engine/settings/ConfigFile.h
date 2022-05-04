#pragma once
#include "source/utility/containers/Vector2.h"
#include "source/render/WindowProperties.h"

#include <rapidjson/document.h>
#include <EASTL/vector.h>
#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct FileLogDefinition;
	struct ConsoleLogDefinition;
	struct LogData;

	class ConfigFile
	{
		const char* m_pFileName;
		rapidjson::Document m_parsedData;
		bool m_isOpen;

	public:
		ConfigFile();

		bool OpenConfigFile();

		bool PopulateLogData(FileLogDefinition& fileLog, ConsoleLogDefinition& consoleLog, eastl::vector<LogData>& logData) const;

		bool PopulateWindowData(WindowProperties& windowProperties) const;

	private:
		bool PopulateFileLogDefinition(FileLogDefinition& fileLog) const;

		bool PopulateConsoleLogDefiniton(ConsoleLogDefinition& consoleLog) const;

		bool PopulateLogs(eastl::vector<LogData>& logData, const char* pCategoryName) const;

		bool PopulateWindowTitle(eastl::string& windowTitle) const;

		bool PopulateWindowSize(glm::vec2& windowSize) const;

		bool PopulateWindowVSync(bool& isVsyncEnabled) const;
	};
}