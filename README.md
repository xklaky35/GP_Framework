# Grapple - A small custom game engine



## Overview

This project originated from a course at university. The task was to create a minimal game engine to create a single game where I took it a step further and continued to work on it to create a more generic game engine. 


## Features

- Scene trees (Node based hierarchy)
- Scene [editor](./docs/wiki/overview.md#imgui-node-editor)
- Physics simulation


## Building from source

At this point only builds on Linux are supported. The build was only tested on Arch Linux so far.

### Dependencies

- gcc
- cmake

#### Arch

```bash
pacman -S cmake gcc
```

### Build

After downloading the sourcecode run the following commands to build the project:

```bash

mkdir build
cd build
cmake ..
cmake --build .

```

Run the created binary file to start the engine:

```bash

./GP_Framework

```




