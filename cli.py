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
    cmd[0] = "{}/{}/{}.py".format(os.getcwd(), TOOLS_DIR, cmd[0])
    if os.path.exists(cmd[0]): # check valid command
        cmd.insert(0, "python3")
        ret = subprocess.call(cmd)
    else:
        print("Invalid command: ", cmd)
        ret = -1

    return ret

# execute commands in order
argc = len(sys.argv)
i = 1
while i < argc:
    cmd = [sys.argv[i]]
    while True:
        if i < argc - 1 and sys.argv[i + 1][0] == "-":
            cmd.append(sys.argv[i+1][1:])
            i = i + 1
        else:
            break

    print("\n----------------------")
    print("Executing: ", cmd[0])
    if len(cmd) > 1:
        print("With arguments: {}".format(", ".join(cmd[1:])))

    if RunCmd(cmd) != 0:
        break

    i = i + 1

