# -*- coding: utf-8 -*-
import os
import sys
import time
import platform
import requests
import urllib
from zipfile import ZipFile
from tarfile import TarFile

import importlib.util as importlib_util
import subprocess

from buildsystem.PythonConsoleLog import Log as log

class Utilities:
    @classmethod
    def Validate(self):
        log.Info("Validating Python Version")
        if not self.__ValidatePython():
            return # cannot validate further

        for packageName in ["requests"]:
            if not self.__ValidatePackage(packageName):
                return # cannot validate further

    @classmethod
    def __ValidatePython(self, versionMajor = 3, versionMinor = 3):
        if sys.version is not None:
            log.Log("Python version {0:d}.{1:d}.{2:d} detected.".format(sys.version_info.major, sys.version_info.minor, sys.version_info.micro))

            if sys.version_info.major < versionMajor or (sys.version_info.major == versionMajor and sys.version_info.minor < versionMinor):
                log.Error("Python version too low, expected version {0:d}.{1:d} or higher.".format(versionMajor, versionMinor))
                return False
            return True

    @classmethod
    def __ValidatePackage(self, packageName):
        if importlib_util.find_spec(packageName) is None:
            return self.__InstallPackage(packageName)
        return True

    @classmethod
    def __InstallPackage(self, packageName):
        permissionGranted = False
        while not permissionGranted:
            reply = str(input("Would you like to install Python package '{0:s}'? [Y/N]: ".format(packageName))).lower().strip()[:1]
            if reply == 'n':
                return False
            permissionGranted = (reply == 'y')
        
        log.Info(f"Installing {packageName} module...")
        subprocess.check_call(['python', '-m', 'pip', 'install', packageName])

        return self.__ValidatePackage(packageName)

    @classmethod
    def DownloadFile(self, url, filepath):
        filepath = os.path.abspath(filepath)
        os.makedirs(os.path.dirname(filepath), exist_ok=True)
                
        if (type(url) is list):
            for url_option in url:
                print("Downloading", url_option)
                try:
                    self.DownloadFile(url_option, filepath)
                    return
                except urllib.error.URLError as e:
                    print(f"URL Error encountered: {e.reason}. Proceeding with backup...\n\n")
                    os.remove(filepath)
                    pass
                except urllib.error.HTTPError as e:
                    print(f"HTTP Error  encountered: {e.code}. Proceeding with backup...\n\n")
                    os.remove(filepath)
                    pass
                except:
                    print(f"Something went wrong. Proceeding with backup...\n\n")
                    os.remove(filepath)
                    pass
            raise ValueError(f"Failed to download {filepath}")
        if not(type(url) is str):
            raise TypeError("Argument 'url' must be of type list or string")

        with open(filepath, 'wb') as f:
            headers = {'User-Agent': "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.97 Safari/537.36"}
            response = requests.get(url, headers=headers, stream=True)
            total = response.headers.get('content-length')

            if total is None:
                f.write(response.content)
            else:
                downloaded = 0
                total = int(total)
                startTime = time.time()
                for data in response.iter_content(chunk_size=max(int(total/1000), 1024*1024)):
                    downloaded += len(data)
                    f.write(data)
                    
                    try:
                        done = int(50*downloaded/total) if downloaded < total else 50
                        percentage = (downloaded / total) * 100 if downloaded < total else 100
                    except ZeroDivisionError:
                        done = 50
                        percentage = 100
                    elapsedTime = time.time() - startTime
                    try:
                        avgKBPerSecond = (downloaded / 1024) / elapsedTime
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
                UnzippedFilePath = os.path.abspath("{0:s}/{1:s}".format(zipFileLocation, zippedFileName))
                
                if not os.path.isdir(zipFileLocation):
                    os.makedirs(os.path.dirname(UnzippedFilePath))
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
            UnpackedFilePath = os.path.abspath("{0:s}/{1:s}".format(tarFileLocation, tarFileName))
            
            if not os.path.isdir(tarFileLocation):
                os.makedirs(os.path.dirname(UnpackedFilePath))
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
