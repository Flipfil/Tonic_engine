import subprocess
import os, sys
import globals

# executes makes the binary file
args = globals.ProcessArguments(sys.argv)
CONFIG = globals.GetArgumentValue(args, "config", "debug")
print("Building config: " + CONFIG)

ret = 0
if globals.IsWindows():
    VS_CODE_BUILD_PATH = "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe"
    ret = subprocess.call(["cmd.exe", "/c" , VS_CODE_BUILD_PATH, "{}.sln".format(globals.ENGINE_NAME), "/property:Configuration={}".format(CONFIG)])
else:
    ret = subprocess.call(["make", "config={}".format(CONFIG)])

sys.exit(ret)