# 🧩 XML Editor & Social Network Analyzer

A **C++ desktop application** that processes, analyzes, and visualizes XML-based social network data.  
Built with a clean, modular architecture and supports both **CLI** and **GUI** workflows.

---

## 📂 Project Structure
```bash
xml_editor/
├── CMakeLists.txt
├── core/
│   ├── include/
│   │   └── xml_editor/
│   └── src/
│       ├── cli/
│       ├── io/
│       ├── model/
│       ├── sna/
│       ├── util/
│       ├── visualization/
│       └── xml/
├── gui/
│   ├── include/
│   │   ├── gui.h
│   │   └── mainwindow.h
│   └── src/
│       ├── gui.cpp
│       ├── main.cpp
│       ├── mainwindow.cpp
│       └── mainwindow.ui
└── resources/
    └── input.xml
```

---

## 🛠️ Build & Requirements

### Requirements
- **C++ Compiler** (GCC / Clang / MSVC)
- **[CMake](https://cmake.org/)**
- **[Qt](https://www.qt.io/product/development-tools)** (Qt Creator recommended for GUI)  
- **[Graphviz](https://graphviz.org/)** (required for graph visualization using `draw`)  

### Build Instructions
1. Clone the repo
2. Import project in Qt Creator
3. Build -> Run CMake
4. Build -> Build xml_editor (Ctrl/Cmd + B)
5. Output at build/Qt/xml_editor (Run manually/in terminal/Ctrl + R in Qt Creator)
---

## ✨ Key Features

### XML Processing
- XML validation and error correction
- XML parsing into a tree structure
- Formatting (prettifying) and minification
- XML → JSON conversion
- Lossless compression & decompression (Byte Pair Encoding)

### Social Network Analysis
- Graph-based representation of users and followers
- Identify:
  - Most influential user (most followers)
  - Most active user (most connections)
  - Mutual followers
  - Recommended users (friends-of-friends)
- Search posts by word or topic

### Visualization
- Converts the social graph to **DOT format**
- Generates visual graphs (PNG, JPG, SVG, PDF, etc.)
- Uses **Graphviz** under the hood

⚠️ **Note:**  
The `draw` command **requires Graphviz to be installed and available in PATH**.

---

## 🔁 System Modes

### CLI
Designed for automation and testing.

Example commands:
```bash
xml_editor verify     -i input.xml (optionally) -f -o output.xml (fixed xml)
xml_editor format     -i input.xml -o output.xml (optionally) -t 4 (4 spaces per tab)
xml_editor json       -i input.xml -o output.json
xml_editor mini       -i input.xml -o minified.xml
xml_editor compress   -i input.xml -o output.comp
xml_editor decompress -i input.comp -o output.comp

xml_editor most_influencer -i input.xml
xml_editor most_active -i input.xml
xml_editor mutual     -i input.xml -ids 1,2
xml_editor suggest    -i input.xml -id 2
xml_editor search -w word -i input.xml
xml_editor search -t topic -i input.xml
xml_editor draw       -i input.xml -o graph.png (other Graphviz formats are supported)
```

### GUI
- File browsing and manual XML input
- Buttons for all supported operations
- Read-only output preview
- Save results to a user-defined location

<img width="1026" height="840" alt="Screenshot 2025-12-25 at 12 47 10 AM" src="https://github.com/user-attachments/assets/f5f58e14-5fc8-402b-b904-19ccdc2eab37" />

