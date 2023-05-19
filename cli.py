import os, sys
import subprocess

TOOLS_DIR = "tools"

# comand runner script
# usage: python3 ./cli.py [command1] [command2] ...
# available commands are in ./tools directory as separate python scripts

# note: dont use full script file as command use "run" not "run.py"

# execute the command
def RunCmd(cmd):
    ret = 0
    script = "{}/{}/{}.py".format(os.getcwd(), TOOLS_DIR, cmd)

    if os.path.exists(script): # check valid command
        print("Executing: ", cmd)
        ret = subprocess.call(["python3", script])
    else:
        print("Invalid command: ", cmd)
        ret = -1

    return ret

# execute commands in order
for i in range(1,len(sys.argv)):
    cmd = sys.argv[i]

    print("\n--------------------")
    if RunCmd(cmd) != 0:
        break