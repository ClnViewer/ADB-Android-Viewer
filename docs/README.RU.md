
[![Download MSI](https://img.shields.io/badge/Download-MSI-brightgreen.svg?style=flat)](https://clnviewer.github.io/ADB-Android-Viewer/dist/Android-ADB-Viewer.msi)
[![DOC EN](https://img.shields.io/badge/Features-EN-brightgreen.svg?style=flat)](https://github.com/ClnViewer/ADB-Android-Viewer/blob/master/docs/README.EN.md)
[![Promo EN](https://img.shields.io/badge/Promo-EN-brightgreen.svg?style=flat)](https://clnviewer.github.io/Code-Blocks-Android-NDK/ADBANDROIDVIEWER.EN.html)
[![DOC RU](https://img.shields.io/badge/Features-RU-brightgreen.svg?style=flat)](https://github.com/ClnViewer/ADB-Android-Viewer/blob/master/docs/README.RU.md)
[![Promo RU](https://img.shields.io/badge/Promo-RU-brightgreen.svg?style=flat)](https://clnviewer.github.io/Code-Blocks-Android-NDK/ADBANDROIDVIEWER.RU.html)
[![License MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg?style=flat)](https://github.com/ClnViewer/ADB-Android-Viewer/blob/master/LICENSE)

# ADB Android Viewer

> Android remote Viewer разработан для просмотра и управления устройством Android с ПК.


![adbviewer](images/adbviewer-2-terminal-en.png)

>[`ADB Android Viewer` инсталлятор](https://clnviewer.github.io/ADB-Android-Viewer/dist/Android-ADB-Viewer.msi) `Bender Rodriguez edition :)`   
> ADB Android Viewer - графическое приложение отображающее экран мобильного устройства с возможностью управления. Поддержка масштабного просмотра, ввод касания с помощью мыши, пролистывание ввода с клавиатуры, скриншоты, выбор устройств, управление АДБ менеджером и другие функции ..  

Описание и скриншоты: [EN](https://clnviewer.github.io/Code-Blocks-Android-NDK/ADBANDROIDVIEWER.EN.html) | [RU](https://clnviewer.github.io/Code-Blocks-Android-NDK/ADBANDROIDVIEWER.RU.html)  

## Возможности

- Основные характеристики:
  - использует ADB для доступа к устройству  
  - поддерживает устройства с версиями Android от `5.x` до `9.x` и `Android API` от `20` версии  
  - доступ и получение изображения производиться через собственные драйвера временно копируемые на устройство  
  - поддерживает [конфигурационный файл](https://github.com/ClnViewer/ADB-Android-Viewer/blob/master/src/ADBViewer/ADBViewer.ini) с основными настройками интерфейса  
  - встроенный терминал для доступа к консольным командам удаленного устройства  
  - поддержка [скриптов `LUA`](https://github.com/ClnViewer/ADB-Android-Viewer/blob/master/src/ADBViewer/src/App/plugin/plugin-lua/plugin-lua.lua.example) для интерактивного взаимодействия с устройством и приложениями на нем  
  - встроенный графический [редактор/генератор](https://clnviewer.github.io/Code-Blocks-Android-NDK/ADBANDROIDVIEWER.EN.html) скриптов `LUA`  
  - система подгружаемых [плагинов](https://github.com/ClnViewer/ADB-Android-Viewer/tree/master/src/ADBViewer/src/App/plugin/plugin-template), в собранный пакет включен плагин управления `LUA` скриптами  
  - режим отображения абсолютных координат как в пикселях, так и позиции в видео буфере  
  - сохранение скриншотов в форматах `.bmp` или `.png`  
  - возможность копировать экран устройства в буфер обмена  
  - графический интерфейс поддерживает языки: `Russian`, `English`, `German`, `Chinese`  
  - набор типовых команд Android для быстрого вызова из меню  
  - установка APK пакетов  
- Экран устройства:
  - полноэкранный режим  
  - маштабирование `1:1` - `1:2`  
  - зеркальное отображение  
- Навигация:
  - клик мыши преобразуется в клик на устройстве  
  - буквенно-цифровая клавиатура преобразуется в нажатие системных ключей, включая ввод текста  
  - отдельное редактируемое поле ввода текста, поодерживает копирование и вставку текста  
  - клавиши `Up`, `Down`, `Left`, `Right` используются для движения экрана (swipe)  
  - ввод на национальных языках не поддерживается ADB  
- Встроенный терминал:
  - поддерживает режим консольного терминала на удаленном устройстве  
  - редактируемое поле ввода текста, поодерживает копирование и вставку текста, историю команд  
  - клавиши `Up`, `Down`, используются для выбора из истории введенных команд  
  - клавиши `PgUp`, `PgDown` используются для прокрутки экрана  
  - клавиша `Tab` используются для последовательного перебора заранее заданных команд быстрого доступа  
  - настраеваемый отступ от нижнего края экрана при открытии терминала  

## Требования и совместимость

- необходим смартфон/планшет, работающий на Android с версией `4.1.1` и выше  
- минимально поддерживаемое `Android API` от `20` версии  
- использует в качестве драйвера [AScreencap](https://github.com/ClnViewer/Android-fast-screen-capture)  
- проверена работа на версиях Android `5.1.1`, `6.1.1`, `8.0.1`, `9.1.0`  
- используемые сторонние библиотеки: `SDL2 32bit`, `LUA 5.35 32bit`  
- на текущий момент доступна готовая сборка под `Windows 10 32bit` в формате архива [MSI](https://clnviewer.github.io/ADB-Android-Viewer/dist/Android-ADB-Viewer.msi)  


## License

 _MIT_

