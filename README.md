
<p align="center">
	<img src="https://github.com/imitatehappiness/QtPassManager/blob/main/resources/icon.png" width="128" height="128" />
</p>

# PassManager

![GitHub stars](https://img.shields.io/github/stars/imitatehappiness/QtPassManager?style=social)
![GitHub forks](https://img.shields.io/github/forks/imitatehappiness/QtPassManager?style=social)
![GitHub followers](https://img.shields.io/github/followers/imitatehappiness?style=social)

![Qt version](https://img.shields.io/badge/Qt-5.15.2-151515.svg?Color=EEE&logoColor=EEE)
![MinGW version](https://img.shields.io/badge/MinGW-5.3.0-151515.svg?Color=EEE&logoColor=EEE)

PassManager is a simple application for managing credentials. Suitable for personal use when you need to organize your passwords quickly and easily.

All data is stored locally:
- `C:\Users\<Username>\AppData\Roaming\PassManager\accounts.xml`

[My Custom Title Bar Template](https://github.com/imitatehappiness/QtCustomTitleBar)

## Build

```bash
git clone https://github.com/imitatehappiness/QtPassManager.git
cd QtPassManager/PassManager
qmake PassManager.pro
mingw32-make
cd release
./PassManager.exe
```

## Deploying the Application
Use [windeployqt](https://doc.qt.io/qt-6/windows-deployment.html) to gather all necessary dependencies:

```bash
windeployqt --compiler-runtime QtPassManager/PassManager/release/PassManager.exe
```

After executing this command:
- All required dependencies (Qt libraries, plugins, etc.) will be copied into the directory containing PassManager.exe.
- The application will be self-contained and ready for distribution to other systems.

## UI Demonstration 

<p align="center">
	<img src="https://github.com/imitatehappiness/QtPassManager/blob/main/resources/demo.png"  />
</p>
