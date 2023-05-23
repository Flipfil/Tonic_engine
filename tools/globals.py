ENGINE_NAME = "TNK"
PROJECT_NAME = "TNK_editor"

CONFIG="debug"

V_MAJOR = 1
V_MINOR = 0


#===================================================

import sys, platform
PLATFORM = sys.platform
for x in platform.uname():
    if "linux" in x.lower():
        PLATFORM = "linux"
        break

    if "microsoft" in x.lower() or "windows" in x.lower():
        PLATFORM = "windows"
        break

    if "darwin" in x.lower():
        PLATFORM = "darwin"
        break


def IsWindows():
    return "windows" == PLATFORM

def IsLinux():
    return "linux" == PLATFORM

def IsMac():
    return "darwin" == PLATFORM