import globals
import os, sys, subprocess

# run the executable

CONFIG = "Debug"
EXE_PATH = "{}/bin/{}/{}/".format(os.getcwd(), CONFIG, globals.PROJECT_NAME)

ret = 0
ret = subprocess.call(["{}{}{}".format(EXE_PATH, globals.PROJECT_NAME, ".exe" if globals.IsWindows() else "")], cwd=os.getcwd())

sys.exit(ret)