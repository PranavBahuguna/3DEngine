# 3DEngine
A custom 3D rendering engine, written in C++ with OpenGL. Will be steadily adding more features to
this over time.

# Example screenshot (as of 14/07/2020)
![](example.png)

# Cloning the repo
There are several third-party libraries that are used in this project. They are stored in the
ThirdParty repo, which is a submodule of this repository. Simply cloning the 3DEngine repo isn't
enough as the ThirdParty submodule is not automatically cloned with it.

To clone the ThirdParty submodule as well, the following set of commands can be run:

```
git clone https://github.com/PranavBahuguna/3DEngine
git submodule init
git submodule update
```

Or to clone the repo and submodules in one go:

```
git clone --recurse-submodules https://github.com/PranavBahuguna/3DEngine
```

# Features
- Model loading system (using wavefront .obj format).
- Model texturing and materials system.
- Real-time lighting (point, directional and cone lights).
- 2D font and text rendering.
- Game object scripting with Lua.

# Controls
- **A** - Left
- **D** - Right
- **W** - Forwards
- **S** - Backwards
- **Space** - Up
- **Ctrl** - Down
- **Left arrow** - Turn left
- **Right arrow** - Turn right
- **Up arrow** - Turn up
- **Down arrow** Turn down
- **Mouse** - Look around
- **Esc** - Exit
- **M** - Toggle HUD
- **O / Mouse Scroll Down** - Decrease FOV (zoom in)
- **P / Mouse Scroll Up** - Increase FOV (zoom out)
