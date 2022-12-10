# CS1D Pacman

## Installing the Application

You can download the binaries for Windows, Linux, or macOS in the bin folder of the project and run the game directly
using the binaries. If your machine is unable to run the application, you can try compiling the code yourself to run the
application in the Godot Engine editor.

## Running the project in Godot

You can follow the more generalized, official tutorial
[here](https://docs.godotengine.org/en/stable/tutorials/scripting/gdnative/gdnative_cpp_example.html) if you need any
extra help.

To run the project directly in the Godot Engine, you must first download the engine from their
[website](https://godotengine.org/download) and then do the following:

* Follow the official Godot documentation to set up compiling for either
[Windows](https://docs.godotengine.org/en/stable/development/compiling/compiling_for_windows.html),
[Linux](https://docs.godotengine.org/en/stable/development/compiling/compiling_for_x11.html), or
[macOS](https://docs.godotengine.org/en/stable/development/compiling/compiling_for_osx.html).

* Clone this repository with the following command:

    ```
    git clone --recursive https://github.com/Decrypted-X/CS1D-Pacman.git
    ```

* Then compile the godot-cpp bindings with the following commands. Replace ```<platform>``` with windows, linux, or osx
depending on your OS.

    ```
    cd godot-cpp
    scons platform=<platform> generate_bindings=yes target=release
    cd ..
    ```

* Once the godot-cpp bindings are done compiling, you can compile the project with the following commands:

    ```
    cd cs1d_pacman
    scons platform=<platform> target=release
    cd ..
    ```

After the godot-cpp bindings and the project are compiled, you can then run the project in the Godot Engine. You just
run the Godot Engine and import the project by selecting the ```project.godot``` file in the ```cs1d_pacman/project/```
directory. Click the Run button at the top right of the Godot Engine editor to run the game.

# Kruskal's Algorithm Maze Generator

This code uses Kruskal's algorithm to generate a perfect maze. Kruskal's algorithm is a method for generating a maze by randomly selecting walls and removing them to create a path, ensuring that the maze remains connected.

## Getting Started

Once the code is compiled, you can run it by executing the `maze` executable.

## Code Overview

The code includes the following functions:

- `randomInd`: This function generates a random valid index for the maze.
- `randomDir`: This function generates a random direction for the maze.
- `reGroup`: This function regroups cells in the maze.
- `isPerfectMaze`: This function checks whether the maze is a perfect maze.
- `isConnected`: This function checks whether the maze is connected.
- `krusMaze`: This is the main function that generates the maze using Kruskal's algorithm.

## Dependencies

This code depends on the following libraries:

- `vector`
- `algorithm`
- `iostream`
- `utility`
- `cmath`
- `random`
- `chrono`
- `set`

## Contributing

If you would like to contribute to this code, please fork the repository and submit a pull request.

## License

This code is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
To use this code, you will need to have a C++ compiler installed on your system. You can then clone the repository and compile the code using the following commands:

git clone https://github.com/user/repo.git
cd repo
g++ -o maze main.cpp


Once the code is compiled, you can run it by executing the `maze` executable.

## Code Overview

The code includes the following functions:

- `randomInd`: This function generates a random valid index for the maze.
- `randomDir`: This function generates a random direction for the maze.
- `reGroup`: This function regroups cells in the maze.
- `isPerfectMaze`: This function checks whether the maze is a perfect maze.
- `isConnected`: This function checks whether the maze is connected.
- `krusMaze`: This is the main function that generates the maze using Kruskal's algorithm.

## Dependencies

This code depends on the following libraries:

- `vector`
- `algorithm`
- `iostream`
- `utility`
- `cmath`
- `random`
- `chrono`
- `set`

## Contributing

If you would like to contribute to this code, please fork the repository and submit a pull request.

## License

This code is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
