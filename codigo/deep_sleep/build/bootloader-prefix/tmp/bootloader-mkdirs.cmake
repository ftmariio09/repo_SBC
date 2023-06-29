# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v5.0/components/bootloader/subproject"
  "C:/Users/ftmar/repo_SBC/codigo/deep_sleep/build/bootloader"
  "C:/Users/ftmar/repo_SBC/codigo/deep_sleep/build/bootloader-prefix"
  "C:/Users/ftmar/repo_SBC/codigo/deep_sleep/build/bootloader-prefix/tmp"
  "C:/Users/ftmar/repo_SBC/codigo/deep_sleep/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/ftmar/repo_SBC/codigo/deep_sleep/build/bootloader-prefix/src"
  "C:/Users/ftmar/repo_SBC/codigo/deep_sleep/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/ftmar/repo_SBC/codigo/deep_sleep/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/ftmar/repo_SBC/codigo/deep_sleep/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
