# ZFX

ZFX is a small OpenGL graphics library written in C++ I made to learn some OpenGL basics.
I followed the guides from [learnopengl.com](https://learnopengl.com/) but I did not use all the same libraries or make the examples exactly the same.

---

## ZFX supports (for example)

- FreeType font rendering
- Basic lighting
- Framebuffers
- MSAA
- Loading OBJ models via [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)
- Instancing

A simple demo application (`zfx_demo`) is included.

---

## Screenshots

<p float="left">
  <img src="https://raw.githubusercontent.com/zirkoni/ZFX/main/Images/1_Text_rendering.png" width="45%" />
  <img src="https://raw.githubusercontent.com/zirkoni/ZFX/main/Images/2_Circles.png" width="45%" />
</p>

<p float="left">
  <img src="https://raw.githubusercontent.com/zirkoni/ZFX/main/Images/3_Cube_specular.png" width="45%" />
  <img src="https://raw.githubusercontent.com/zirkoni/ZFX/main/Images/4_Framebuffer.png" width="45%" />
</p>

<p float="left">
  <img src="https://raw.githubusercontent.com/zirkoni/ZFX/main/Images/5_Shadow_mapping.png" width="45%" />
  <img src="https://raw.githubusercontent.com/zirkoni/ZFX/main/Images/6_Instancing.png" width="45%" />
</p>

---

## Build Instructions

### Dependencies

Make sure the following libraries are installed:

- **SDL2**
- **SDL2_Image**
- **GLEW**
- **GLM**
- **FreeType2**
- **OpenGL**

### Arch Linux example

```bash
sudo pacman -S --needed base-devel cmake glew glm freetype2 sdl3 sdl2-compat sdl2_image
```

### Compiling

```bash
git clone https://github.com/zirkoni/ZFX.git
cd ZFX
make
```

This will compile the demo program. To run it:

```bash
./build/Demo/zfx_demo
```

You can also clean the project:

```bash
make clean
```
