/*********************************************************************************
 *
 *       Copyright (C) 2019,2020 Ichiro Kawazome
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
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

class Udmabuf {

public:
  std::string   name;
  void*         buf;
  unsigned int  buf_size;
  uint64_t      phys_addr;
  bool          status;

private:
  int           _device_file;
  int           _sys_sync_for_cpu_file;
  int           _sys_sync_for_dev_file;
  std::string   _sys_path;

  bool set_sys_path(const std::string& _name)
  {
    char        file_name[1024];
    struct stat st;

    sprintf(file_name, "/sys/class/u-dma-buf/%s", name.c_str());
    if (stat(file_name, &st) == 0) {
        _sys_path = "/sys/class/u-dma-buf/" + name;
        return true;
    }
    sprintf(file_name, "/sys/class/udmabuf/%s"  , name.c_str());
    if (stat(file_name, &st) == 0) {
        _sys_path = "/sys/class/udmabuf/"   + name;
        return true;
    }
    return false;
  }
  
public:
  bool initialize(const std::string& device_name)
  {
    char        file_name[1024];
    int         fd;
    char        attr[1024];
    bool        _status = true;

    name = device_name;

    if ((_status = set_sys_path(device_name)) == false) {
      printf("Can not found %s in /sys/class/u-dma-buf/ or /sys/class/udmabuf/\n", name.c_str());
      goto done;
    }
    
    sprintf(file_name, "%s/phys_addr", _sys_path.c_str());
    if ((fd  = open(file_name, O_RDONLY)) < 0) {
      printf("Can not open %s\n", file_name);
      _status = false;
      goto done;
    } else {
      unsigned long long value;
      read(fd, attr, 1024);
      sscanf(attr, "%llx", &value);
      phys_addr = (uint64_t)value;
      close(fd);
    }
    
    sprintf(file_name, "%s/size", _sys_path.c_str());
    if ((fd  = open(file_name, O_RDONLY)) < 0) {
      printf("Can not open %s\n", file_name);
      _status = false;
      goto done;
    } else {
      read(fd, attr, 1024);
      sscanf(attr, "%d", &buf_size);
      close(fd);
    }

    sprintf(file_name, "/dev/%s", name.c_str());
    if ((_device_file = open(file_name, O_RDWR)) < 0) {
      printf("Can not open %s\n", file_name);
      _status = false;
      goto done;
    } else {
      buf = mmap(NULL, buf_size, PROT_READ|PROT_WRITE, MAP_SHARED, _device_file, 0);
      if (buf == MAP_FAILED) {
        printf("Can not mmap %s\n", file_name);
        _status = false;
        goto done;
      }
    }

    sprintf(file_name, "%s/sync_for_cpu", _sys_path.c_str());
    if ((_sys_sync_for_cpu_file = open(file_name, O_RDWR)) < 0) {
      printf("Can not open %s\n", file_name);
      _status = false;
      goto done;
    }

    sprintf(file_name, "%s/sync_for_device", _sys_path.c_str());
    if ((_sys_sync_for_dev_file = open(file_name, O_RDWR)) < 0) {
      printf("Can not open %s\n", file_name);
      _status = false;
      goto done;
    }

  done:
    if (_status == false)
      _done();
    status = _status;

    return status;
  }

  void _done(void)
  {
    if (buf != MAP_FAILED          ) {munmap((void *)buf, buf_size);}
    if (_device_file           >= 0) {close(_device_file          );}
    if (_sys_sync_for_dev_file >= 0) {close(_sys_sync_for_dev_file);}
    if (_sys_sync_for_cpu_file >= 0) {close(_sys_sync_for_cpu_file);}
    buf      = MAP_FAILED;
    buf_size = 0;
    _device_file           = -1;
    _sys_sync_for_dev_file = -1;
    _sys_sync_for_cpu_file = -1;
    status = false;
  }

  bool _set_sys_value(const std::string& sys_name, unsigned int value)
  {
    char      file_name[1024];
    int       fd;
    char      attr[1024];
    if (status == false)
      return false;
    else {
      sprintf(file_name, "%s/%s", _sys_path.c_str(), sys_name.c_str());
      if ((fd = open(file_name, O_RDWR)) < 0) {
        printf("Can not open %s\n", file_name);
        return false;
      } else {
        ssize_t attrlen = sprintf(attr, "%d", value);
        ssize_t written = write(fd, attr, attrlen);
        close(fd);
        return(written == attrlen);
      }
    }
  }

public:
  Udmabuf(const std::string& device_name) :
    _device_file(-1),
    _sys_sync_for_cpu_file(-1),
    _sys_sync_for_dev_file(-1),
    buf(MAP_FAILED),
    buf_size(0),
    phys_addr(0)
  {
    status = initialize(device_name);
  }

  Udmabuf() :
    _device_file(-1),
    _sys_sync_for_cpu_file(-1),
    _sys_sync_for_dev_file(-1),
    buf(MAP_FAILED),
    buf_size(0),
    phys_addr(0)
  {
    status = false;
  }

  ~Udmabuf()
  {
    _done();
  }

  bool sync_for_cpu()
  {
    if (status == true) {
      ssize_t written = write(_sys_sync_for_cpu_file, "1", 1);
      return written == strlen("1");
    } else {
      return false;
    }
  }

  bool sync_for_device()
  {
    if (status == true) {
      ssize_t written = write(_sys_sync_for_dev_file, "1", 1);
      return written == strlen("1");
    } else {
      return false;
    }
  }

  bool set_sync_size(unsigned int size)
  {
    return _set_sys_value("sync_size", size);
  }

  bool set_sync_offset(unsigned int offset)
  {
    return _set_sys_value("sync_offset", offset);
  }

  bool set_sync_dma_bidirection(void)
  {
    return _set_sys_value("sync_direction", 0);
  }

  bool set_sync_dma_to_device(void)
  {
    return _set_sys_value("sync_direction", 1);
  }

  bool set_sync_dma_from_device(void)
  {
    return _set_sys_value("sync_direction", 2);
  }
};
