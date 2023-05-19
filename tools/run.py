import globals
import os, subprocess

CONFIG = "Debug"
EXE_PATH = "{}/bin/{}/{}/".format(os.getcwd(), CONFIG, globals.PROJECT_NAME)

subprocess.call(["{}{}{}".format(EXE_PATH, globals.PROJECT_NAME, ".exe" if globals.IsWindows() else "")], cwd=os.getcwd())
