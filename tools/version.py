import globals
import sys

print("Tonic tools - v{}.{}".format(globals.V_MAJOR, globals.V_MINOR))
if len(sys.argv) >= 2:
    print(", ".join(sys.argv))