This is my attempt at a crossplatform game engine and a game 
collection built using this engine. I wanted to make this for a long time and
thanks to Warhorse Studios I finally had the motivation.

As core build tool I chose premake5 because of its compactness over CMake. Also
CMake seemed a little too challenging to learn without help. The other building 
tools arevery simple python scripts with one core script "cli.py". Run this script with
arguments as names of the other scripted tools in the tools dir. It also has 
support for flags specifying release/debug configuration or exact project the
command is meant to target. Commands can be chained.

Following example command generates a platfrom solution for the Pong project and runs Tonic_editor
python3 ./cli.py gensln -prj=Pong run -prj=Tonic_editor

Tools description: not specifying projects applies to all existing projects
	gensln   - generate a solution, for example *.sln for windows
	buildsln - builds the binaries of the project
	run      - execute the binaries