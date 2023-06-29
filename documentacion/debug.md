```bash
PS C:\Users\ftmar\repo_SBC\codigo\no_sleep> C:\Espressif\python_env\idf5.0_py3.8_env\Scripts\python.exe C:\Espressif\frameworks\esp-idf-v5.0\tools\idf_monitor.py -p COM7 -b 115200 --toolchain-prefix xtensa-esp32-elf- --target esp32 c:\Users\ftmar\repo_SBC\codigo\no_sleep\build\no_sleep.elf
--- WARNING: GDB cannot open serial ports accessed as COMx
--- Using \\.\COM7 instead...
--- idf_monitor on \\.\COM7 115200 ---
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
ets Jun  8 2016 00:22:57

rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:2
load:0x3fff0030,len:7000
load:0x40078000,len:15452
ho 0 tail 12 room 4
load:0x40080400,len:3840
0x40080400: _init at ??:?

entry 0x4008064c
I (29) boot: ESP-IDF v5.0-dirty 2nd stage bootloader
I (29) boot: compile time 20:12:01
I (29) boot: chip revision: v1.0
I (32) boot_comm: chip revision: 1, min. bootloader chip revision: 0
I (39) boot.esp32: SPI Speed      : 40MHz
I (44) boot.esp32: SPI Mode       : DIO
I (49) boot.esp32: SPI Flash Size : 4MB
I (53) boot: Enabling RNG early entropy source...
I (59) boot: Partition Table:
I (62) boot: ## Label            Usage          Type ST Offset   Length
I (69) boot:  0 nvs              WiFi data        01 02 00009000 00004000
I (77) boot:  1 otadata          OTA data         01 00 0000d000 00002000
I (84) boot:  2 phy_init         RF data          01 01 0000f000 00001000
I (92) boot:  3 factory          factory app      00 00 00010000 00100000
I (99) boot:  4 ota_0            OTA app          00 10 00110000 00100000
I (107) boot:  5 ota_1            OTA app          00 11 00210000 00100000
I (114) boot: End of partition table
I (119) boot: Defaulting to factory image
I (123) boot_comm: chip revision: 1, min. application chip revision: 0
I (130) esp_image: segment 0: paddr=00010020 vaddr=3f400020 size=27180h (160128) map
I (197) esp_image: segment 1: paddr=000371a8 vaddr=3ffb0000 size=03d80h ( 15744) load
I (203) esp_image: segment 2: paddr=0003af30 vaddr=40080000 size=050e8h ( 20712) load
I (212) esp_image: segment 3: paddr=00040020 vaddr=400d0020 size=9ebech (650220) map
I (447) esp_image: segment 4: paddr=000dec14 vaddr=400850e8 size=12a2ch ( 76332) load
I (479) esp_image: segment 5: paddr=000f1648 vaddr=50000000 size=00050h (    80) load
I (491) boot: Loaded app from partition at offset 0x10000
I (491) boot: Disabling RNG early entropy source...
I (503) cpu_start: Pro cpu up.
I (503) cpu_start: Starting app cpu, entry point is 0x400814f0
0x400814f0: call_start_cpu1 at C:/Espressif/frameworks/esp-idf-v5.0/components/esp_system/port/cpu_start.c:142

I (0) cpu_start: App cpu up.
I (519) cpu_start: Pro cpu start user code
I (519) cpu_start: cpu freq: 160000000 Hz
I (519) cpu_start: Application information:
I (524) cpu_start: Project name:     no_sleep
I (529) cpu_start: App version:      ab59fa4-dirty
I (534) cpu_start: Compile time:     Jun 28 2023 18:54:32
I (540) cpu_start: ELF file SHA256:  082c2e8d1f9922aa...
I (546) cpu_start: ESP-IDF:          v5.0-dirty
I (552) heap_init: Initializing. RAM available for dynamic allocation:
I (559) heap_init: At 3FFAE6E0 len 00001920 (6 KiB): DRAM
I (565) heap_init: At 3FFB7DD0 len 00028230 (160 KiB): DRAM
I (571) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM
I (578) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
I (584) heap_init: At 40097B14 len 000084EC (33 KiB): IRAM
I (592) spi_flash: detected chip: generic
I (595) spi_flash: flash io: dio
I (600) cpu_start: Starting scheduler on PRO CPU.
I (0) cpu_start: Starting scheduler on APP CPU.
I (610) Deep_Sleep_MAIN: Esta el la ultima version de deep_sleep.
I (620) SPI: Esta el la ultima version de deep_sleep.0
I (620) Deep_Sleep_MAIN: Time is not set yet. Connecting to WiFi and getting time over NTP.
I (630) SPI: MAX_RAW.0
I (670) wifi station: ESP_WIFI_MODE_STA
I (690) wifi:wifi driver task: 3ffc1c08, prio:23, stack:6656, core=0
I (690) system_api: Base MAC address is not set
I (690) system_api: read default base MAC address from EFUSE
I (710) wifi:wifi firmware version: 0d470ef
I (710) wifi:wifi certification version: v7.0
I (710) wifi:config NVS flash: enabled
I (710) wifi:config nano formating: disabled
I (710) wifi:Init data frame dynamic rx buffer num: 32
I (720) wifi:Init management frame dynamic rx buffer num: 32
I (720) wifi:Init management short buffer num: 32
I (730) wifi:Init dynamic tx buffer num: 32
I (730) wifi:Init static rx buffer size: 1600
I (730) wifi:Init static rx buffer num: 10
I (740) wifi:Init dynamic rx buffer num: 32
I (740) wifi_init: rx ba win: 6
I (750) wifi_init: tcpip mbox: 32
I (750) wifi_init: udp mbox: 6
I (750) wifi_init: tcp mbox: 6
I (760) wifi_init: tcp tx win: 5744
I (760) wifi_init: tcp rx win: 5744
I (770) wifi_init: tcp mss: 1440
I (770) wifi_init: WiFi IRAM OP enabled
I (770) wifi_init: WiFi RX IRAM OP enabled
I (800) phy_init: phy_version 4670,719f9f6,Feb 18 2021,17:07:07
I (900) wifi:mode : sta (24:6f:28:7b:f2:7c)
I (900) wifi:enable tsf
I (910) wifi station: wifi_init_sta finished.
I (920) wifi:new:<3,0>, old:<1,0>, ap:<255,255>, sta:<3,0>, prof:1
I (8750) wifi:state: assoc -> run (10)
I (8770) wifi:connected with mario-linux-2, aid = 16, channel 3, BW20, bssid = aa:3c:90:44:5e:23
I (8770) wifi:security: WPA2-PSK, phy: bgn, rssi: -55
I (8780) wifi:pm start, type: 1

W (8790) wifi:<ba-add>idx:0 (ifx:0, aa:3c:90:44:5e:23), tid:0, ssn:0, winSize:64
I (8870) wifi:AP's beacon interval = 102400 us, DTIM period = 2
I (9680) esp_netif_handlers: sta ip: 192.168.43.164, mask: 255.255.255.0, gw: 192.168.43.1
I (9680) wifi station: got ip:192.168.43.164
I (10630) SPI: Esta el la ultima version de deep_sleep.0
I (10630) SPI: MAX_RAW.0
I (20630) SPI: Esta el la ultima version de deep_sleep.0
I (20630) SPI: MAX_RAW.0
I (30630) SPI: Esta el la ultima version de deep_sleep.0
I (30630) SPI: MAX_RAW.0
I (30830) DEBUG: Dentro master_begin 2


----------------- TimeStamp --------------------
Seconds: 1687972348
Micro Seconds: 731074

Number of data measurements: 1 / 2
I (40630) SPI: Esta el la ultima version de deep_sleep.0
I (40630) SPI: MAX_RAW.0
I (50630) SPI: Esta el la ultima version de deep_sleep.0
I (50630) SPI: MAX_RAW.0
I (51030) DEBUG: Dentro master_begin 2


----------------- TimeStamp --------------------
Seconds: 1687972368
Micro Seconds: 937482
I (51030) wifi station: ESP_WIFI_MODE_STA
ESP_ERROR_CHECK failed: esp_err_t 0x103 (ESP_ERR_INVALID_STATE) at 0x400dcd03
0x400dcd03: wifi_init_sta at C:/Users/ftmar/repo_SBC/codigo/no_sleep/components/conexion_wifi/conexion_wifi.c:106 (discriminator 1)

file: "./components/conexion_wifi/conexion_wifi.c" line 106
func: wifi_init_sta
expression: esp_event_loop_create_default()

abort() was called at PC 0x4008ab93 on core 0
0x4008ab93: _esp_error_check_failed at C:/Espressif/frameworks/esp-idf-v5.0/components/esp_system/esp_err.c:47



Backtrace: 0x40081fa6:0x3ffbc360 0x4008ab9d:0x3ffbc380 0x4009223e:0x3ffbc3a0 0x4008ab93:0x3ffbc410 0x400dcd03:0x3ffbc440 0x400dcf63:0x3ffbc5c0 0x400d863c:0x3ffbc5e0 0x4008dd3d:0x3ffbc680
0x40081fa6: panic_abort at C:/Espressif/frameworks/esp-idf-v5.0/components/esp_system/panic.c:412

0x4008ab9d: esp_system_abort at C:/Espressif/frameworks/esp-idf-v5.0/components/esp_system/esp_system.c:135

0x4009223e: abort at C:/Espressif/frameworks/esp-idf-v5.0/components/newlib/abort.c:38

0x4008ab93: _esp_error_check_failed at C:/Espressif/frameworks/esp-idf-v5.0/components/esp_system/esp_err.c:47

0x400dcd03: wifi_init_sta at C:/Users/ftmar/repo_SBC/codigo/no_sleep/components/conexion_wifi/conexion_wifi.c:106 (discriminator 1)

0x400dcf63: iniciar_wifi at C:/Users/ftmar/repo_SBC/codigo/no_sleep/components/conexion_wifi/conexion_wifi.c:179 (discriminator 13)

0x400d863c: tarea_Principal at C:/Users/ftmar/repo_SBC/codigo/no_sleep/main/main.c:104

0x4008dd3d: vPortTaskWrapper at C:/Espressif/frameworks/esp-idf-v5.0/components/freertos/FreeRTOS-Kernel/portable/xtensa/port.c:151





ELF file SHA256: 082c2e8d1f9922aa

Rebooting...
ets Jun  8 2016 00:22:57

rst:0xc (SW_CPU_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:2
load:0x3fff0030,len:7000
load:0x40078000,len:15452
ho 0 tail 12 room 4
load:0x40080400,len:3840
0x40080400: _init at ??:?

entry 0x4008064c
I (29) boot: ESP-IDF v5.0-dirty 2nd stage bootloader
I (29) boot: compile time 20:12:01
I (29) boot: chip revision: v1.0
I (32) boot_comm: chip revision: 1, min. bootloader chip revision: 0
I (39) boot.esp32: SPI Speed      : 40MHz
I (44) boot.esp32: SPI Mode       : DIO
I (49) boot.esp32: SPI Flash Size : 4MB
I (53) boot: Enabling RNG early entropy source...
I (59) boot: Partition Table:
I (62) boot: ## Label            Usage          Type ST Offset   Length
I (69) boot:  0 nvs              WiFi data        01 02 00009000 00004000
I (77) boot:  1 otadata          OTA data         01 00 0000d000 00002000
I (84) boot:  2 phy_init         RF data          01 01 0000f000 00001000
I (92) boot:  3 factory          factory app      00 00 00010000 00100000
I (99) boot:  4 ota_0            OTA app          00 10 00110000 00100000
I (107) boot:  5 ota_1            OTA app          00 11 00210000 00100000
I (114) boot: End of partition table
I (119) boot: Defaulting to factory image
I (123) boot_comm: chip revision: 1, min. application chip revision: 0
I (130) esp_image: segment 0: paddr=00010020 vaddr=3f400020 size=27180h (160128) map
I (197) esp_image: segment 1: paddr=000371a8 vaddr=3ffb0000 size=03d80h ( 15744) load
I (203) esp_image: segment 2: paddr=0003af30 vaddr=40080000 size=050e8h ( 20712) load
I (212) esp_image: segment 3: paddr=00040020 vaddr=400d0020 size=9ebech (650220) map
I (447) esp_image: segment 4: paddr=000dec14 vaddr=400850e8 size=12a2ch ( 76332) load
I (479) esp_image: segment 5: paddr=000f1648 vaddr=50000000 size=00050h (    80) load
I (491) boot: Loaded app from partition at offset 0x10000
I (491) boot: Disabling RNG early entropy source...
I (610) Deep_Sleep_MAIN: Esta el la ultima version de deep_sleep.
I (620) SPI: Esta el la ultima version de deep_sleep.0
I (620) SPI: MAX_RAW.0
ClearCommError failed (PermissionError(13, 'Acceso denegado.', None, 5))
```