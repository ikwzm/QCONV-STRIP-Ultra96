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
#include <sys/mman.h>
#include "udmabuf.hpp"
#include "uio.hpp"

class QconvStrip
{
public:
  uint8_t* regs;
  bool     status;

  bool  initialize(void* _regs, bool acp)
  {
    if (_regs == MAP_FAILED) {
      regs   = 0;
      status = false;
    } else {
      regs   = (uint8_t*)_regs;
      status = true;
    }
    return status;
  }
  
  QconvStrip(void* _regs)
  {
    initialize(_regs, false);
  }

  QconvStrip()
  {
    status = false;
  }

  bool setup(uint64_t in_addr , uint64_t out_addr, uint64_t k_addr, uint64_t th_addr,
             unsigned in_w , unsigned in_h , unsigned in_c_by_word,
             unsigned out_w, unsigned out_h, unsigned out_c,
             unsigned k_w,   unsigned k_h, unsigned pad, unsigned use_threshold)
  {
    if (status == false)
      return false;


    *(uint32_t*)(regs + 0x20) = (uint32_t)((in_addr  >>  0) & 0xFFFFFFFF);
    *(uint32_t*)(regs + 0x24) = (uint32_t)((in_addr  >> 32) & 0xFFFFFFFF);
    *(uint32_t*)(regs + 0x28) = (uint32_t)((out_addr >>  0) & 0xFFFFFFFF);
    *(uint32_t*)(regs + 0x2C) = (uint32_t)((out_addr >> 32) & 0xFFFFFFFF);
    *(uint32_t*)(regs + 0x30) = (uint32_t)((k_addr   >>  0) & 0xFFFFFFFF);
    *(uint32_t*)(regs + 0x34) = (uint32_t)((k_addr   >> 32) & 0xFFFFFFFF);
    *(uint32_t*)(regs + 0x38) = (uint32_t)((th_addr  >>  0) & 0xFFFFFFFF);
    *(uint32_t*)(regs + 0x3C) = (uint32_t)((th_addr  >> 32) & 0xFFFFFFFF);
    *(uint32_t*)(regs + 0x40) = (uint32_t)(in_w);
    *(uint32_t*)(regs + 0x48) = (uint32_t)(in_h);
    *(uint32_t*)(regs + 0x50) = (uint32_t)(in_c_by_word);
    *(uint32_t*)(regs + 0x58) = (uint32_t)(out_w);
    *(uint32_t*)(regs + 0x60) = (uint32_t)(out_h);
    *(uint32_t*)(regs + 0x68) = (uint32_t)(out_c);
    *(uint32_t*)(regs + 0x70) = (uint32_t)(k_w);
    *(uint32_t*)(regs + 0x78) = (uint32_t)(k_h);
    *(uint32_t*)(regs + 0x80) = (uint32_t)(pad);
    *(uint32_t*)(regs + 0x88) = (uint32_t)(use_threshold);
    return true;
  }
  bool start(void)
  {
    if (status == false)
      return false;
    else {
      *(uint32_t*)(regs + 0x08) = (uint32_t)1;
      return true;
    }
  }
  bool interrupt_enable(void)
  {
    if (status == false)
      return false;
    else {
      *(uint32_t*)(regs + 0x10) = (uint32_t)1;
      return true;
    }
  }
  bool interrupt_disable(void)
  {
    if (status == false)
      return false;
    else {
      *(uint32_t*)(regs + 0x10) = (uint32_t)0;
      return true;
    }
  }
  bool clear_status(void)
  {
    if (status == false)
      return false;
    else {
      *(uint32_t*)(regs + 0x18) = (uint32_t)1;
      return true;
    }
  }
  bool wait_for_done(void)
  {
    volatile uint32_t done_flag = 0;
    if (status == false)
      return false;
    while (!(done_flag & 0x2)) { done_flag = *(volatile uint32_t*)(regs + 0x18); }
    return true;
  }
  bool run(uint64_t in_addr , uint64_t out_addr, uint64_t k_addr, uint64_t th_addr,
           unsigned in_w , unsigned in_h , unsigned in_c_by_word,
           unsigned out_w, unsigned out_h, unsigned out_c,
           unsigned k_w,   unsigned k_h, unsigned pad, unsigned use_threshold)
  {
    if (status == false)
      return false;

    setup(in_addr, out_addr, k_addr, th_addr, in_w, in_h, in_c_by_word, out_w, out_h, out_c, k_w, k_h, pad, use_threshold);
    interrupt_disable();
    start();
    wait_for_done();
    return true;
  }
};
