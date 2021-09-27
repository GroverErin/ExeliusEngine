import os
import sys
import time
import platform
from zipfile import ZipFile
from tarfile import TarFile

from buildsystem.PythonConsoleLog import Log as log

class Utilities:
    @classmethod
    def UnpackFile(self, packedFilePath, unpackFilePath):
        if platform.system() == "Windows":
            self.__UnzipFile(packedFilePath, unpackFilePath)
        elif platform.system() == "Linux":
            self.__UnpackFile(packedFilePath, unpackFilePath)
        else:
            log.Error("OS '{0:s}' Not Supported.".format(platform.system()))
            return False
        return True

    @classmethod
    def __UnzipFile(self, packedFilePath, unpackFilePath):
        zipFilePath = packedFilePath # get full path of files
        zipFileLocation = unpackFilePath

        zipFileContent = dict()
        zipFileContentSize = 0
        with ZipFile(zipFilePath, 'r') as zipFileFolder:
            for name in zipFileFolder.namelist():
                zipFileContent[name] = zipFileFolder.getinfo(name).file_size
            zipFileContentSize = sum(zipFileContent.values())
            extractedContentSize = 0
            startTime = time.time()
            for zippedFileName, zippedFileSize in zipFileContent.items():
                UnzippedFilePath = os.path.abspath(f"{zipFileLocation}/{zippedFileName}")
                os.makedirs(os.path.dirname(UnzippedFilePath), exist_ok=True)
                if os.path.isfile(UnzippedFilePath):
                    zipFileContentSize -= zippedFileSize
                else:
                    zipFileFolder.extract(zippedFileName, path=zipFileLocation, pwd=None)
                    extractedContentSize += zippedFileSize
                try:
                    done = int(50*extractedContentSize/zipFileContentSize)
                    percentage = (extractedContentSize / zipFileContentSize) * 100
                except ZeroDivisionError:
                    done = 50
                    percentage = 100
                elapsedTime = time.time() - startTime
                try:
                    avgKBPerSecond = (extractedContentSize / 1024) / elapsedTime
                except ZeroDivisionError:
                    avgKBPerSecond = 0.0
                avgSpeedString = '{:.2f} KB/s'.format(avgKBPerSecond)
                if (avgKBPerSecond > 1024):
                    avgMBPerSecond = avgKBPerSecond / 1024
                    avgSpeedString = '{:.2f} MB/s'.format(avgMBPerSecond)
                sys.stdout.write('\r[{}{}] {:.2f}% ({})     '.format('█' * done, '.' * (50-done), percentage, avgSpeedString))
                sys.stdout.flush()
        sys.stdout.write('\n')

    @classmethod
    def __UnpackFile(self, packedFilePath, unpackFilePath):
        tarFilePath = packedFilePath # get full path of files
        tarFileLocation = unpackFilePath

        tarFileContent = dict()
        tarFileContentSize = 0
        tarFileFolder = TarFile.open(tarFilePath, 'r')
        for name in tarFileFolder.getnames():
            tarFileContent[name] = tarFileFolder.getmember(name).size
        tarFileContentSize = sum(tarFileContent.values())
        extractedContentSize = 0
        startTime = time.time()
        for tarFileName, tarFileSize in tarFileContent.items():
            UnpackedFilePath = os.path.abspath(f"{tarFileLocation}/{tarFileName}")
            os.makedirs(os.path.dirname(UnpackedFilePath), exist_ok=True)
            if os.path.isfile(UnpackedFilePath):
                tarFileContentSize -= tarFileSize
            else:
                tarFileFolder.extract(tarFileName, path=tarFileLocation)
                extractedContentSize += tarFileSize
            try:
                done = int(50*extractedContentSize/tarFileContentSize)
                percentage = (extractedContentSize / tarFileContentSize) * 100
            except ZeroDivisionError:
                done = 50
                percentage = 100
            elapsedTime = time.time() - startTime
            try:
                avgKBPerSecond = (extractedContentSize / 1024) / elapsedTime
            except ZeroDivisionError:
                avgKBPerSecond = 0.0
            avgSpeedString = '{:.2f} KB/s'.format(avgKBPerSecond)
            if (avgKBPerSecond > 1024):
                avgMBPerSecond = avgKBPerSecond / 1024
                avgSpeedString = '{:.2f} MB/s'.format(avgMBPerSecond)
            sys.stdout.write('\r[{}{}] {:.2f}% ({})     '.format('█' * done, '.' * (50-done), percentage, avgSpeedString))
            sys.stdout.flush()
        sys.stdout.write('\n')
