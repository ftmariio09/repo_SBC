# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/user2/esp/esp-idf/components/bootloader/subproject"
  "C:/Users/user2/Desktop/esp32-main/esp32-main/03_spi/build/bootloader"
  "C:/Users/user2/Desktop/esp32-main/esp32-main/03_spi/build/bootloader-prefix"
  "C:/Users/user2/Desktop/esp32-main/esp32-main/03_spi/build/bootloader-prefix/tmp"
  "C:/Users/user2/Desktop/esp32-main/esp32-main/03_spi/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/user2/Desktop/esp32-main/esp32-main/03_spi/build/bootloader-prefix/src"
  "C:/Users/user2/Desktop/esp32-main/esp32-main/03_spi/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/user2/Desktop/esp32-main/esp32-main/03_spi/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
