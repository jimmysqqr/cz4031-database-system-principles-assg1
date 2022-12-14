# CZ4031 - Database System Principles (Project 1)

### Building the project using tasks.json in VSCode

1. Create `.vscode` directory into your workspace
2. Create `tasks.json` file (below) into that directory
3. Add the following parameters into `tasks.json`
   ```
   {
   "tasks": [
   	{
   		"type": "cppbuild",
   		"label": "C/C++: g++.exe build active file",
   		"command": "C:\\msys64\\mingw64\\bin\\g++.exe",
   		"args": [
   			"-fdiagnostics-color=always",
   			"-g",
                /** Input files **/
   			"${workspaceFolder}\\*.cpp",
   			"${workspaceFolder}\\bp_tree\\*.cpp",
   			"${workspaceFolder}\\bp_tree\\*.h",
   			"${workspaceFolder}\\storage\\*.cpp",
   			"${workspaceFolder}\\storage\\*.h",
   			"-o",
                /** Output file **/
   			"${workspaceFolder}\\output.exe" // default is "${fileDirname}\\${fileBasenameNoExtension}.exe"
   		],
   		"options": {
   			"cwd": "${fileDirname}"
   		},
   		"problemMatcher": ["$gcc"],
   		"group": {
   			"kind": "build",
   			"isDefault": true
   		},
   		"detail": "Task generated by Debugger."
   	}
   ],
   "version": "2.0.0"
    }
   ```
4. Check the line ```"command": "C:\\msys64\\mingw64\\bin\\g++.exe"```. Provide the path to your g++ here.
5. At the top row of buttons in VSCode, click Terminal -> Run Build Task (or do Ctrl + Shift + B)
6. There should be an `output.exe` file in your workspace
7. In the terminal, type `./output` and press Enter to run the executable
