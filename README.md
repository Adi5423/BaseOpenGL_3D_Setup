# GameWindow

A minimal OpenGL + GLFW project that creates a working 3D-ready window with GLAD for loading OpenGL functions.

This repository gives you a clean and simple starting point to build any OpenGL-based application, engine prototype, or rendering demo.

---

## 🚀 Features

* Modern **OpenGL 4.6 Core Profile**
* **GLFW** for window + context creation
* **GLAD** loader (static)
* Clean CMake-based build system
* Cross-platform structure (Windows/Linux)
* Zero clutter — just run and start coding

---

## 📦 Requirements

* CMake 3.20+
* C++20 compiler
* Ninja / Make / MSVC (any backend supported by CMake)
* Git (for cloning submodules if needed)

---

## 🔧 Build Instructions

Open a terminal inside the project root and run:

```sh
cmake -S . -B build
cmake --build build
```

This generates the executable:

```
build/Sandbox
```

Run it:

```sh
./build/Sandbox
```

You should see a window titled **GameWindow** with a blueish background, created via OpenGL.

---

## 🗂️ Project Structure

```
GameWindow/
│  CMakeLists.txt
│  README.md
│
├─ src/
│   └─ sandbox_main.cpp   # entry point
│
├─ vendor/
│   ├─ glfw/              # GLFW source
│   ├─ glad/              # GLAD loader
│   ├─ glm/               # GLM math library
│   └─ stb/               # stb headers
│
└─ build/ (generated)
```

---

## 🧩 What You Get

When you run the program, it:

* Initializes GLFW
* Creates a 1280×720 window
* Loads OpenGL 4.6 via GLAD
* Prints your GPU's OpenGL version
* Enters a render loop with a clear color

Ready for you to add:

* Shaders
* VAOs/VBOs
* Textures
* 3D scenes
* UI overlays

---

## 📝 Notes

* All dependencies are included in `vendor/` — no external installs required.
* Paths are short and clean to avoid Windows object path issues.

---

## 📄 License

MIT License. See [`LICENSE.txt`](LICENSE.txt)

Feel free to use this as your base for any OpenGL or engine-related project!
