This is my attempt at a crossplatform game engine + my game collection using this engine.

=============================================================================================
BUILDING PROJECTS
=============================================================================================
As core build tool I chose premake5 over CMake because of its simplicity. The other building 
tools are very simple python scripts with one core script "cli.py". Run the core script with
arguments as names of the other scripts in the tools dir (without ".py"). It also supports flags 
specifying release/debug version or target project. Commands can be chained.

Following example generates a platfrom solution for the Pong project and runs Tonic_editor:
python3 ./cli.py gensln -prj=Pong run -prj=Tonic_editor

Tools: 
	gensln   - generate a solution, for example *.sln for windows
	buildsln - builds the binaries of the project
	run      - execute the binaries
Flags:
	-prj=[PROJECT NAME]	- command targets specified project. Default applies to all projects - current working projects: Pong, SpaceInvaders, Tonic_editor, Tonic_engine (is just a .lib - cannot be run)
	-config=[CONFIGURATION]	- to which configuration of a project/s is command applied to. Default is "debug" other option is "release".

How to build a project from source:
python3 ./cli.py gensln buildsln -prj=[PROJECT NAME]

Note - If you are working on Windows platform, then running the code from VisualStudio 
	may result in incorrect file paths. The code is written as if "post_build_copy"
	directory does not exist and VS runs the code from the project root. To avoid
	this create a symbolic link in prj root to your files/directories from 
	the "post_build_copy" OR run the program from the "bin" folder.
