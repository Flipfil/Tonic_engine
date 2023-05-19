import subprocess
import os
import globals

CONFIG = "Debug"

print("Building project..")

if globals.IsWindows():
    VS_CODE_BUILD_PATH = "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe"
    subprocess.call(["cmd.exe", "/c" , VS_CODE_BUILD_PATH, "{}.sln".format(globals.ENGINE_NAME), "/property:Configuration={}".format(CONFIG)])


print("done.\n")
