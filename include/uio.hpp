/*********************************************************************************
 *
 *       Copyright (C) 2019 Ichiro Kawazome
 *       All rights reserved.
 * 
 *       Redistribution and use in source and binary forms, with or without
 *       modification, are permitted provided that the following conditions
 *       are met:
 * 
 *         1. Redistributions of source code must retain the above copyright
 *            notice, this list of conditions and the following disclaimer.
 * 
 *         2. Redistributions in binary form must reproduce the above copyright
 *            notice, this list of conditions and the following disclaimer in
 *            the documentation and/or other materials provided with the
 *            distribution.
 * 
 *       THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *       "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *       LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *       A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 *       OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *       SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *       LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *       DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *       THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *       (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *       OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 ********************************************************************************/
#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>

class Uio
{
public:
  std::string   name;
  bool          status;
  std::string   device_name;
  int           device_file;

private:
  bool find_uio_device_name(const std::string& _name, std::string& device_name)
  {
    const std::string sys_uio_path = "/sys/class/uio/";
    DIR* dp;
    status = true;
    dp = opendir(sys_uio_path.c_str());
    if (dp != NULL) {
      dirent* entry;
      do {
        entry = readdir(dp);
        if (entry != NULL) {
          std::string   sys_uio_name_path = sys_uio_path + std::string(entry->d_name) + "/name";
          std::ifstream fin( sys_uio_name_path );
          if (fin) {
            std::stringstream strstream;
            strstream << fin.rdbuf();
            fin.close();
            std::string uio_name;
            std::getline(strstream, uio_name);
            if (uio_name == _name) {
              device_name = std::string(entry->d_name);
              return true;
            }
          }
        }
      } while(entry != NULL);
    }
    return false;
  }
  
public:
  bool initialize(const std::string& _name)
  {
    status = find_uio_device_name(_name, device_name);
    if (status == false) {
      std::cerr << "Can not found "+ _name + " in /sys/class/uio/*/name" << std::endl;
      return false;
    }
    std::string device_path = "/dev/"+device_name;
    if ((device_file = open(device_path.c_str(), O_RDWR)) < 0) {
      std::cerr << "Can not open "+ device_path << std::endl;
      status = false;
      return false;
    }
  }  

  Uio(const std::string& _name) :
    device_file(-1)
  {
    status = initialize(_name);
  }
  Uio() :
    device_file(-1)
  {
    status = false;
  }
  ~Uio()
  {
    if (device_file >= 0)
      close(device_file);
  }
  void* memmap(unsigned int offset, unsigned int size)
  {
    if (device_file >= 0) {
      return mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, device_file, offset);
    } else {
      return MAP_FAILED;
    }
  }
};
