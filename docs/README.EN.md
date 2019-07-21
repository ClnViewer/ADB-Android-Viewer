
[![Download MSI](https://img.shields.io/badge/Download-MSI-brightgreen.svg?style=flat)](https://clnviewer.github.io/ADB-Android-Viewer/dist/Android-ADB-Viewer.msi)
[![DOC EN](https://img.shields.io/badge/Features-EN-brightgreen.svg?style=flat)](https://github.com/ClnViewer/ADB-Android-Viewer/blob/master/docs/README.EN.md)
[![Promo EN](https://img.shields.io/badge/Promo-EN-brightgreen.svg?style=flat)](https://clnviewer.github.io/Code-Blocks-Android-NDK/ADBANDROIDVIEWER.EN.html)
[![DOC RU](https://img.shields.io/badge/Features-RU-brightgreen.svg?style=flat)](https://github.com/ClnViewer/ADB-Android-Viewer/blob/master/docs/README.RU.md)
[![Promo RU](https://img.shields.io/badge/Promo-RU-brightgreen.svg?style=flat)](https://clnviewer.github.io/Code-Blocks-Android-NDK/ADBANDROIDVIEWER.RU.html)
[![License MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg?style=flat)](https://github.com/ClnViewer/ADB-Android-Viewer/blob/master/LICENSE)

# ADB Android Viewer

> Android remote Viewer 
developed to view and control your android device from a PC.


![adbviewer](images/adbviewer-2-terminal-en.png)


>[`ADB Android Viewer` installer](https://clnviewer.github.io/ADB-Android-Viewer/dist/Android-ADB-Viewer.msi) `Bender Rodriguez edition :)`   
> ADB Android Viewer is a graphical application that displays the screen of a mobile device with the ability to control. Support for large-scale viewing, touch input with the mouse, flipping through keyboard input, screenshots, device selection, ADB manager management, and other functions ..  
Description and screenshots: [EN](https://clnviewer.github.io/Code-Blocks-Android-NDK/ADBANDROIDVIEWER.EN.html) | [RU](https://clnviewer.github.io/Code-Blocks-Android-NDK/ADBANDROIDVIEWER.RU.html)  

## Features overview

- Main characteristics:  
  - uses ADB to access the device  
  - supports devices with Android versions from `5.x` to `9.x` and `Android API` from` 20` version  
  - access and receipt of images made through their own drivers temporarily copied to the device  
  - supports [configuration file](https://github.com/ClnViewer/ADB-Android-Viewer/blob/master/src/ADBViewer/ADBViewer.ini) with basic interface settings  
  - built-in terminal for access to console commands of a remote device  
  - support [scripts `LUA`](https://github.com/ClnViewer/ADB-Android-Viewer/blob/master/src/ADBViewer/src/App/plugin/plugin-lua/plugin-lua.lua.example) for interactive interaction with the device and applications on it  
  - integrated graphic [editor / generator](https://clnviewer.github.io/Code-Blocks-Android-NDK/ADBANDROIDVIEWER.EN.html) of the `LUA` scripts  
  - system of loadable [plug-ins](https://github.com/ClnViewer/ADB-Android-Viewer/tree/master/src/ADBViewer/src/App/plugin/plugin-template), the LUA control plugin is included in the assembled package scripts  
  - mode of displaying absolute coordinates both in pixels and positions in the video buffer  
  - save screenshots in `.bmp` or `.png` formats  
  - the ability to copy the device screen to the clipboard  
  - GUI supports languages: `Russian`, `English`, `German`, `Chinese`  
  - a set of typical Android commands for quick access from the menu  
  - install APK packages  
- Device Screen:  
  - full screen mode  
  - scaling `1: 1` - `1: 2`  
  - mirror reflection  
- Navigation:  
  - mouse click is converted to a click on the device  
  - the alphanumeric keyboard is converted to pressing system keys, including text input  
  - a separate editable text input field that supports copying and pasting text  
  - keys `Up`, `Down`, `Left`, `Right` are used to move the screen (swipe)  
  - input in national languages characters​is not supported by ADB manager  
- Built-in terminal:  
  - supports the console terminal mode on the remote device  
  - editable text input field, copy and paste text, command history  
  - keys `Up`, `Down`, used to select from the history of entered commands  
  - the keys `PgUp`, `PgDown` are used to scroll the screen  
  - the `Tab` key is used to iterate through pre-defined shortcut commands  
  - adjustable indent from the bottom edge of the screen when opening the terminal  
   
## Requirements and compatibility

- you need a smartphone / tablet running on Android with version `4.1.1` and higher  
- minimally supported `Android API` from `20` version  
- uses as a driver [AScreencap](https://github.com/ClnViewer/Android-fast-screen-capture)  
- work on Android versions `5.1.1`, `6.1.1`, `8.0.1`, `9.1.0` has been checked  
- used third-party libraries: `SDL2 32bit`, `LUA 5.35 32bit`  
- currently available ready assembly under `Windows 10 32bit` in [MSI](https://clnviewer.github.io/ADB-Android-Viewer/dist/Android-ADB-Viewer.msi) archive format  

## License

 _MIT_

