import globals
import os, subprocess

CONFIG = "Debug"
EXE_PATH = "{}/bin/{}/{}/".format(os.getcwd(), CONFIG, globals.PROJECT_NAME, globals.PROJECT_NAME)

if globals.IsWindows():
    subprocess.call(["{}/{}{}".format(EXE_PATH, globals.PROJECT_NAME, ".exe")], cwd=os.getcwd())