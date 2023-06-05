import globals
import os, sys, subprocess

# run the executable

args = globals.ProcessArguments(sys.argv)
config  = globals.GetArgumentValue(args, "config", "debug")
prj = globals.GetArgumentValue(args, "prj", globals.PROJECT_NAME)
EXE_PATH = "{}/bin/{}/{}/".format(os.getcwd(), config, prj)

print("EXE_PATH: " + EXE_PATH)

ret = 0
ret = subprocess.call(["{}{}{}".format(EXE_PATH, prj, ".exe" if globals.IsWindows() else "")], cwd=os.getcwd())

sys.exit(ret)