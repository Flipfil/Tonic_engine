import sys, os
import subprocess

TOOLS_DIR = "tools"
sys.path.append("{}/../{}".format(os.getcwd(), TOOLS_DIR))
import globals

WIN_SOURCE_PATHS = ["post_build_copy", "post_build_copy_windows"]
NIX_SOURCE_PATHS = ["post_build_copy"]

args = globals.ProcessArguments(sys.argv)
CONFIG = globals.GetArgumentValue(args, "config", "debug")
PROJECT = globals.GetArgumentValue(args, "prj", globals.PROJECT_NAME)

dest = "{}/../bin/{}/{}".format(os.getcwd(), CONFIG, PROJECT)

if globals.IsWindows():
    for source in WIN_SOURCE_PATHS:
        subprocess.call(["cmd.exe", "/c", "robocopy", source, dest, "/E"])
else:
    import shutil

    def rm_r(path):
        if not os.path.exists(dest):
            return
        if os.path.isfile(dest) or os.path.islink(path):
            os.unlink(path)
        else:
            shutil.rmtree(path)

    def copytree(src, dst, symlinks=False, ignore=None):
        for item in os.listdir(src):
            s = os.path.join(src, item)
            d = os.path.join(dst, item)
            if os.path.exists(d):
                rm_r(d)
            if os.path.isdir(s):
                shutil.copytree(s, d, symlinks, ignore)
            else:
                shutil.copy2(s, d)

    for source in NIX_SOURCE_PATHS:
        copytree(source, dest)

print("Post_build: " + CONFIG)


