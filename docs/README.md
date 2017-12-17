# The Creator 3D	

The Creator 3D is a 3D engine. It's been created for educational purposes by videogame development students studying at CITM. We've used Unity as a reference to create our engine.

## TEAM

![alt text](team_image_path)

Adria Martin:

Sergi Pérez:

## Engine Core

Docking UI: The UI is structured in docks, the user may change the layout by dragging each window.

Gameobject+Component structure: GameObjects follow a hierarchical structure, inheriting the transform matrix of their parent. To add functionality to this GameObjects, components are used. As an example: A GameObject needs a MeshRenderer component, in order to be rendered, or a Camera component, if we want the object to be a camera.

Resource Management: The engine has a resource management window, where the user can manage different types of files. From here, the user can Rename, load, delete, edit various files.

Input: Input keys can be configured through one of the options menu. The engine allows mouse picking for GameObjects that are in the scene, when one object is clicked, a gizmo will appear to help position, rotate or scale the object.

Scenes: The engine allows the user to save "scenes". Scenes are the group of game objects that are currently active at the moment of saving. When saving a scene, all the game objects will also save their respective components. 

Play/Stop/Pause/Step: The engine executes scripts in the scene when the play button is pressed, pressing the play button will save the state of the scene, so if something changes while running, when the stop button is pressed, the scene will go back to its previous state. Pausing the scene allows the user to step, which updates all the gameobjects and their components once.

There are more systems in the engine, including a lot of debug information that might help you understand how the engine works. Feel free to check the code, fork it, or ask us anything you want.

## Scripting System

For this subject we had to create an extra sub-system. In our case we've made a Scripting system.

Ours is a Scripting System uses C#. We are using C# because it is widely used(Unity also implements it) and it provides good performance. It also targets game development.

To implement scripting with C# in our engine we are using Mono. To make the scripting system work, we have made copies of some of our C++ classes, and ported them to C#. This allows mono to compile the scripts we write in C#, and pass the data to our actual C++ classes.

We also implemented a TextEditor, so the user is able to edit and create scripts within the editor.


## Download & Repository
Here you can find a link to the latest release: (link)

You can check the repository here: (link)

If you find any issues you can report them here: (link)

## Credits

The engine has been developed by:

Adrià Martín: https://github.com/TinoTano

Sergi Pérez: https://github.com/sergipa

## License

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means.

In jurisdictions that recognize copyright laws, the author or authors of this software dedicate any and all copyright interest in the software to the public domain. We make this dedication for the benefit of the public at large and to the detriment of our heirs and successors. We intend this dedication to be an overt act of relinquishment in perpetuity of all present and future rights to this software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to http://unlicense.org/

