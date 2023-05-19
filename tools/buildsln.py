import subprocess
import os
import globals

CONFIG = "Debug"



print("Debug or release? D/R: ")
input = input()
if input in ["r", "R", "Release", "release"]:
    CONFIG = "Release"

print("Creating project..")
subprocess.call(["cmake", "-B build/" + CONFIG, "-DMAKE_DEBUG_TYPE=" + CONFIG])
print("done.\n")
