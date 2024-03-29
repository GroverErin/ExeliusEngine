GITMODULES_FILE_PATH = '.gitmodules'
GITMODULES_PATH_VAR_EXTRACT = '\tpath = '

HELP_COMMAND = '--help'
HELP_COMMAND_SHORT = '-h'
VERBOSE_COMMAND = '--verbose'
VERBOSE_COMMAND_SHORT = '-v'
FORCE_SUBMODULE_COMMAND = '--full'
FORCE_SUBMODULE_COMMAND_SHORT = '-f'
ONLY_SUBMODULE_COMMAND = '--submodule'
ONLY_SUBMODULE_COMMAND_SHORT = '-s'

PREMAKE_UNPACK_PATH = './tools/thirdparty/premake/'
PREMAKE_PACKED_PATH = './buildsystem/premakebuilds/'
PREMAKE_WINDOWS_ZIP_FILE = 'premake-5.0.0-alpha16-windows.zip'
PREMAKE_LINUX_TAR_FILE = 'premake-5.0.0-alpha16-linux.tar.gz'
PREMAKE_RPI_TAR_FILE = 'premake-5.0.0-alpha16-rpi.tar.gz'

VULKAN_ENVIRONMENT_VARIABLE = "VULKAN_SDK"
VULKAN_REQUIRED_VERSION = '1.2.170.0'
VULKAN_SDK_PATH = './tools/thirdparty/vulkan'
VULKAN_INSTALL_URL = f'https://sdk.lunarg.com/sdk/download/{VULKAN_REQUIRED_VERSION}/windows/VulkanSDK-{VULKAN_REQUIRED_VERSION}-Installer.exe'
VULKAN_INSTALL_LOCATION = f'{VULKAN_REQUIRED_VERSION}/VulkanSDK-{VULKAN_REQUIRED_VERSION}-Installer.exe'
VULKAN_SHADERC_DEBUG_LIB = f'{VULKAN_SDK_PATH}/Debug/Lib/shaderc_sharedd.lib'
VULKAN_WINDOWS_DEBUG_LIB_URL_A = f'https://sdk.lunarg.com/sdk/download/{VULKAN_REQUIRED_VERSION}/windows/VulkanSDK-{VULKAN_REQUIRED_VERSION}-DebugLibs.zip'
VULKAN_WINDOWS_DEBUG_LIB_URL_B = f'https://files.lunarg.com/SDK-{VULKAN_REQUIRED_VERSION}/VulkanSDK-{VULKAN_REQUIRED_VERSION}-DebugLibs.zip'
VULKAN_SHADERC_DEBUG_LIB_INSTALL = f'{VULKAN_SDK_PATH}/Debug/VulkanSDK-{VULKAN_REQUIRED_VERSION}-DebugLibs.zip'
VULKAN_RELEASE_DLLS = ['shaderc_shared.dll', 'spirv-cross-c-shared.dll', 'SPIRV-Tools-shared.dll']
VULKAN_RELEASE_DLL_INSTALL_PATH = f'{VULKAN_SDK_PATH}/Release/'
