ENGINE_NAME = "2NK"
PROJECT_NAME = "2NK_editor"

V_MAJOR = 1
V_MINOR = 0


#===================================================

import sys, platform
PLATFORM = sys.platform
for x in platform.uname():
    if "linux" in x.lower():
        PLATFORM = "linux"

    if "microsoft" in x.lower() or "windows" in x.lower():
        PLATFORM = "windows"

    if "darwin" in x.lower():
        PLATFORM = "darwin"


def IsWindows():
    return "windows" == PLATFORM

def IsLinux():
    return "linux" == PLATFORM

def IsMac():
    return "darwin" == PLATFORM