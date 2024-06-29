# Snake (written C)

During my process of learing C, I've decided to challenge myself with building a snake game in C.

I will use the [GLFW library](https://www.glfw.org/) for creating the windows and use OpenGL(3.4) for drawing on the screen.

## Disclaimer
There ar no guaranties that this program wil run on your device.

(Oh and the code is quite crappy at the moment)

## Dependencies
 - OpenGL drivers. (in my case i used [mesa](https://www.mesa3d.org/), but this might differ depending on your system)
 - [GLFW](https://www.glfw.org/) For creating windows and handeling input.
 - [gcc](https://gcc.gnu.org/) For compiling the code.

If you are using other OpenGL drivers than mesa, you might need to tweak the `build.sh` script.

## Controls
- [ESC] Close game
- [ArrowLeft] | [A] Move left
- [ArrowRight] | [D] Move right
- [ArrowUp] | [W] Move up
- [ArrowDown] | [S] Move down
