## Linux kernel 3.10.54 (mt6582) for Senseit E400           
![Senseit E400](./arch/arm/mach-mt6582/senseit_e400/e400_silver.jpg)


Компоненты  | Характеристики
-----------:|:-------------------------
Процессор   | 1.3GHz Quad-Core MT6582M
Графика     | Mali-400MP
ОЗУ         | 1GB RAM
Android     | 5.0
ПЗУ         | 8GB
Аккумулятор | 4050 mAh
Дисплей     | 5" 960x540 px
Камера      | 8MPx

* Работает:
  1) Драйвер дисплея (lcm)
  2) Тачскрин
  3) Hotknot
  4) Обе сим карты
  5) Наушники
  6) Радио
  7) Зарядка
  8) Акселлерометр
  9) Датчик света/приближения
  10) Звук
  11) Подстветка
  12) OTG
  13) Главная камера (без интерполяции)
  14) Фронтальная камера
  15) Фонарик
  16) Автофокус

* Инструкция по сборке ядра:
  1) apt-get install git gcc make bc fakeroot dpkg-dev libncurses5-dev libssl-dev subversion
  2) (только для 64 бит):
    dpkg --add-architecture i386
    apt-get install libc6:i386 libstdc++6:i386
    
  3) svn export https://github.com/mohancm/mt6582-alps-mm-3.10.50/trunk/toolchain/linaro-4.9
  4) git clone https://github.com/Ultimate-1/android_kernel_senseit_e400_mt6582.git
  5) export ARCH=arm
  6) export CROSS_COMPILE=$(pwd)/linaro-4.9/bin/arm-linux-androideabi-
  7) cd android_kernel_senseit_e400_mt6582
  8) make senseit_e400_defconfig
  9) make -j3
