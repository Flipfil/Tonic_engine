ENGINE_NAME = "TONIC"
PROJECT_NAME = "TONIC_editor"

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

def ProcessArguments(argv):
    ret = {} # return a key:value dicct
    for arg in argv:
        try:
            k = arg[0:arg.index("=")]
            v = arg[arg.index("=")+1:]
        except:
            k = arg
            v = 0
        ret[k] = v

    print(ret)
    return ret

def GetArgumentValue(args, name, default):
    return args[name] if name in args else default