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
#include "argparse.hpp"
#include "common/global.h"
#include "common/utils.h"
#include "tb/test_input.h"
#include "cpp.h"
#include "udmabuf.hpp"
#include "uio.hpp"
#include "qconv_strip.hpp"
#include <chrono>

class QconvStripEnv
{
public:
  Udmabuf    in_buf;
  Udmabuf    out_buf;
  Udmabuf    k_buf;
  Udmabuf    th_buf;
  Uio        uio;
  QconvStrip qconv;
  bool       use_acp;
  bool       status;

  bool initialize(
    std::string uio_name,
    std::string in_buf_name,
    std::string out_buf_name,
    std::string k_buf_name,
    std::string th_buf_name,
    bool        _use_acp)
  {
    uio    .initialize(uio_name);
    in_buf .initialize(in_buf_name);
    out_buf.initialize(out_buf_name);
    k_buf  .initialize(k_buf_name);
    th_buf .initialize(th_buf_name);
    use_acp = _use_acp;
    if ((uio.status     == false) ||
        (in_buf.status  == false) ||
        (out_buf.status == false) ||
        (k_buf.status   == false) ||
        (th_buf.status  == false)) {
      status = false;
      if (uio.status     == false)
        std::cerr << "QconvStripEnv::uio     initialize Error" << std::endl;
      if (in_buf.status  == false)
        std::cerr << "QconvStripEnv::in_buf  initialize Error" << std::endl;
      if (out_buf.status == false)
        std::cerr << "QconvStripEnv::out_buf initialize Error" << std::endl;
      if (k_buf.status   == false)
        std::cerr << "QconvStripEnv::k_buf   initialize Error" << std::endl;
      if (th_buf.status  == false)
        std::cerr << "QconvStripEnv::th_buf  initialize Error" << std::endl;
      return status;
    }
    void* regs = uio.memmap(0, 0x1000);
    if (regs == MAP_FAILED){
      std::cerr << "QconvStripEnv::uio.memmap Error" << std::endl;
      status = false;
    } else {
      status = qconv.initialize(regs, use_acp);
      if (status == false) 
        std::cerr << "QconvStripEnv::qconv.initalize Error" << std::endl;
    }
    return status;
  }
  QconvStripEnv(
    std::string uio_name,
    std::string in_buf_name,
    std::string out_buf_name,
    std::string k_buf_name,
    std::string th_buf_name,
    bool        _use_acp)
  {
    initialize(uio_name, in_buf_name, out_buf_name, k_buf_name, th_buf_name, _use_acp);
  }
};

QconvStripEnv qconv_env("uio_qconv_strip", "udmabuf-qconv-in", "udmabuf-qconv-out", "udmabuf-qconv-k", "udmabuf-qconv-th", true);

bool qconv_strip(unsigned k_w, unsigned k_h,
                 T_q in_data_packed[], T_out out_data[], T_q k_data_packed[], T_out th_data[],
                 unsigned in_w, unsigned in_h, unsigned in_c_by_word, 
                 unsigned out_w, unsigned out_h, unsigned out_c, unsigned pad, unsigned use_threshold)
{
  assert((k_h == 1 && k_w == 1) || (k_h == 3 && k_w == 3));

  const unsigned in_size  = in_h * in_w * in_c_by_word * conv_common_params::nbits_in_data;
  const unsigned out_size = out_h * out_w * out_c;
  const unsigned k_size   = k_h * k_w * in_c_by_word * out_c;
  const unsigned th_size  = out_c * conv_common_params::num_thresholds;
  bool status = true;

  memcpy(qconv_env.in_buf.buf, (void *)in_data_packed, in_size*sizeof(T_q));
  memcpy(qconv_env.k_buf.buf , (void *)k_data_packed , k_size *sizeof(T_q));
  if (th_data != NULL) {
    memcpy(qconv_env.th_buf.buf, (void*)th_data, th_size*sizeof(T_out));
  }

  if (qconv_env.use_acp == false) {
    qconv_env.in_buf .set_sync_offset(0);
    qconv_env.in_buf .set_sync_size(in_size*sizeof(T_q));
    qconv_env.in_buf .set_sync_dma_to_device();
    qconv_env.k_buf  .set_sync_offset(0);
    qconv_env.k_buf  .set_sync_size(k_size *sizeof(T_q));
    qconv_env.k_buf  .set_sync_dma_to_device();
    qconv_env.th_buf .set_sync_offset(0);
    qconv_env.th_buf .set_sync_size(th_size*sizeof(T_out));
    qconv_env.th_buf .set_sync_dma_to_device();
    qconv_env.out_buf.set_sync_offset(0);
    qconv_env.out_buf.set_sync_size(out_size*sizeof(T_out));
    qconv_env.out_buf.set_sync_dma_from_device();

    qconv_env.in_buf .sync_for_device();
    qconv_env.k_buf  .sync_for_device();
    qconv_env.th_buf .sync_for_device();
    qconv_env.out_buf.sync_for_device();
  }

  auto start = std::chrono::system_clock::now();
  status = qconv_env.qconv.run(qconv_env.in_buf.phys_addr,
                               qconv_env.out_buf.phys_addr,
                               qconv_env.k_buf.phys_addr,
                               qconv_env.th_buf.phys_addr,
                               in_w, in_h, in_c_by_word, out_w, out_h, out_c, k_w, k_h, pad, use_threshold);
  auto end = std::chrono::system_clock::now();

  if (qconv_env.use_acp == false) {
    qconv_env.in_buf .sync_for_cpu();
    qconv_env.k_buf  .sync_for_cpu();
    qconv_env.th_buf .sync_for_cpu();
    qconv_env.out_buf.sync_for_cpu();
  }

  if        ((use_threshold == 0) || (use_threshold == 1)) {
    memcpy(out_data, qconv_env.out_buf.buf, out_size*sizeof(T_out));
  } else if ((use_threshold == 2)) {
    uint8_t* buf = (uint8_t*)qconv_env.out_buf.buf;
    for (int oc = 0; oc < out_size ; oc++) {
      out_data[oc] = buf[oc];
    }
  }

  auto diff = end - start;
  std::cout << "FPGA exec time ("
            << in_w << "x"
            << in_h << "x"
            << in_c_by_word*32 << "x"
            << out_c << " "
            << k_w << "x"
            << k_h << "): "
            << std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << " [usec]"
            << std::endl;

  return status;
}

bool test_conv(input_type &in_type, unsigned k_w, unsigned k_h, unsigned in_c, unsigned in_w, unsigned in_h, unsigned out_c, unsigned use_threshold, bool verbose, bool generate_scenario)
{
  unsigned pad_w          = (k_w == 3) ? 1 : 0;
  unsigned pad_h          = (k_h == 3) ? 1 : 0;
  unsigned stride_w       = 1;
  unsigned stride_h       = 1;
  unsigned in_c_by_word   = in_c / conv_common_params::nbits_per_word;
  unsigned in_size        = in_h * in_w * in_c;
  unsigned in_size_packed = in_h * in_w * in_c_by_word * conv_common_params::nbits_in_data;
  unsigned in_w_with_pad  = in_w + (2 * pad_w);
  unsigned in_h_with_pad  = in_h + (2 * pad_h);
  unsigned k_c            = in_c;
  unsigned k_n            = out_c;
  unsigned k_c_by_word    = k_c / conv_common_params::nbits_per_word;
  unsigned k_size         = k_h * k_w * k_c;
  unsigned k_size_packed  = k_h * k_w * k_c_by_word * conv_common_params::nbits_k_data;
  unsigned out_h          = (in_h_with_pad - k_h) + 1;
  unsigned out_w          = (in_w_with_pad - k_w) + 1;  
  unsigned out_size       = out_h * out_w * out_c;
  
  T_in *in_data           = new T_in[in_size];
  T_in *in_data_quantized = new T_in[in_size_packed];

  T_k *k_data             = new T_k[k_size * k_n];
  T_q *k_data_quantized   = new T_q[k_size_packed * k_n];

  T_out *out_data         = new T_out[out_size];
  T_out *out_data_fpga    = new T_out[out_size];

  T_out *threshold_data   = NULL;

  bool comp_fpga          = true;
  bool status             = true;

  if (verbose == true) {
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "Conv" << k_h << "x" << k_w << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "Layer info" << std::endl
              << "input: " << std::endl
              << "  height: "  << in_h << std::endl
              << "  width: "   << in_w << std::endl
              << "  channel: " << in_c << std::endl
              << "  packed channel: " << in_c_by_word << std::endl
              << "kernel: " << std::endl
              << "  height: "  << k_h << std::endl
              << "  width: "   << k_w << std::endl
              << "output: " << std::endl
              << "  height: "  << out_h << std::endl
              << "  width: "   << out_w << std::endl
              << "  channel: " << out_c << std::endl
              << "num_pe: " << conv_common_params::num_pe << std::endl
              << "use_threshold: " << use_threshold << std::endl
              << "-------------------------------------------" << std::endl;
  }

  if (in_size_packed*sizeof(T_in) > qconv_env.in_buf.buf_size) {
    std::cerr << "Buffer overflow " << qconv_env.in_buf.name
              << ".size("   << qconv_env.in_buf.buf_size           << "[byte])"
              << " < require size(" << in_size_packed*sizeof(T_in) << "[byte])" << std::endl;
    status = false;
  }
  if (k_size_packed*sizeof(T_q) > qconv_env.k_buf.buf_size) {
    std::cerr << "Buffer overflow " << qconv_env.k_buf.name
              << ".size(" << qconv_env.k_buf.buf_size              << "[byte])"
              << " < require size(" << k_size_packed*sizeof(T_q)   << "[byte])" << std::endl;
    status = false;
  }
  if (((use_threshold == 0) && (out_size*sizeof(T_out) > qconv_env.out_buf.buf_size)) ||
      ((use_threshold == 1) && (out_size*sizeof(T_out) > qconv_env.out_buf.buf_size))) {
    std::cerr << "Buffer overflow " << qconv_env.out_buf.name
              << ".size(" << qconv_env.out_buf.buf_size            << "[byte])"
              << " < require size(" << out_size*sizeof(T_out)      << "[byte])" << std::endl;
    status = false;
  }
  if (((use_threshold == 2) && (out_size*sizeof(uint8_t) > qconv_env.out_buf.buf_size))) {
    std::cerr << "Buffer overflow " << qconv_env.out_buf.name
              << ".size(" << qconv_env.out_buf.buf_size            << "[byte])"
              << " < require size(" << out_size*sizeof(uint8_t)    << "[byte])" << std::endl;
    status = false;
  }
  if (status == false)
    return status;

  if (verbose == true) {
    std::cout << "Data Generate..." << std::endl;
  }

  if (in_type == SEQUENTIAL) {
    for (int i = 0; i < in_size     ; i++) { in_data[i] = (i % 4); }
    for (int i = 0; i < k_size * k_n; i++) { k_data[i]  = (i % 2 == 0) ? 1 : -1; }
  } else if (in_type == RANDOM) {
    for (int i = 0; i < in_size     ; i++) { in_data[i] = gen_random_value<T_in>(4, 1, 0); }
    for (int i = 0; i < k_size * k_n; i++) { k_data[i]  = gen_random_value<T_k>(2, 2, 1); }
  } else if (in_type == ALL_1) {
    for (int i = 0; i < in_size     ; i++) { in_data[i] = 1; }
    for (int i = 0; i < k_size * k_n; i++) { k_data[i]  = 1; }
  }

  if (use_threshold > 0) {
    threshold_data = new T_out[out_c * conv_common_params::num_thresholds];

    for (unsigned oc = 0; oc < out_c; oc++) {
      for (unsigned i = 0; i < conv_common_params::num_thresholds; i++) {
        unsigned idx = oc * conv_common_params::num_thresholds + i;
        if (i == 3) {
          // 1 or -1 means increasing or decreasing function
          threshold_data[idx] = gen_random_value<T_k>(2, 2, 1);
        } else {
          threshold_data[idx] = gen_random_value<T_out>(50, 1, 25);
        }
      }
    }
  }

  if ((k_w == 3) && (k_h == 3)) {
      cpp::conv<3, 3>(in_data, out_data, k_data, threshold_data, in_w, in_h, in_c, out_w, out_h, out_c, pad_w, stride_w);
  } else {
      cpp::conv<1, 1>(in_data, out_data, k_data, threshold_data, in_w, in_h, in_c, out_w, out_h, out_c, pad_w, stride_w);
  }

  pack_input_channel_wise(in_data, in_data_quantized, in_h, in_w, in_c, conv_common_params::nbits_in_data);
  pack_kernel_channel_wise(k_data, k_data_quantized , k_h , k_w , k_c , k_n);
  
  if (verbose == true) {
    std::cout << "Data Generate...done" << std::endl;
    std::cout << "qconv strip start..." << std::endl;
  }

  status = qconv_strip(k_w, k_h, in_data_quantized, out_data_fpga, k_data_quantized, threshold_data,
                       in_w, in_h, in_c_by_word, out_w, out_h, out_c, pad_w, use_threshold);

  if (status == false) {
    std::cout << "qconv strip error(out_size:   " << out_size*sizeof(T_out) << "[byte])" << std::endl;
    return status;
  }

  if (verbose == true) {
    std::cout << "qconv strip success(out_size: " << out_size*sizeof(T_out) << "[byte])" << std::endl;
    std::cout << "qconv strip done" << std::endl;
  }

  comp_fpga = compare_output(out_data_fpga, out_data, "qconv_strip", out_h, out_w, out_c);

  if (generate_scenario == true) {
    printf(  "- I :\n");
    printf(  "  - SET\n");
    printf(  "  - ORG: 0x00000000\n");
    for (int i = 0; i < in_size_packed   ; i+=2) {
      printf("  - DW : [0x%08X, 0x%08X]\n", (uint32_t)in_data_quantized[i], (uint32_t)in_data_quantized[i+1]);
    }
    printf(  "- K :\n");
    printf(  "  - SET\n");
    printf(  "  - ORG: 0x00000000\n");
    for (int i = 0; i < k_size_packed*k_n; i++ ) {
      printf("  - DW : [0x%08X]\n", (uint32_t)k_data_quantized[i]);
    }
    if (use_threshold > 0) {
      printf(  "- T :\n");
      printf(  "  - SET\n");
      printf(  "  - ORG: 0x00000000\n");
      for (int i = 0; i < out_c * conv_common_params::num_thresholds; i+=conv_common_params::num_thresholds ) {
        printf("  - DH : [0x%04X,0x%04X,0x%04X,0x%04X] %-3d\n", (uint16_t)threshold_data[i], (uint16_t)threshold_data[i+1], (uint16_t)threshold_data[i+2], (uint16_t)threshold_data[i+3], i/4 );
      }
    }
    printf(  "- O :\n");
    printf(  "  - CHECK\n");
    printf(  "  - ORG: 0x00000000\n");
    for (int i = 0; i < out_size; i++ ) {
      char mismatch = (out_data[i] != out_data_fpga[i]) ? 'x' : ' ';
      printf("  - DH: 0x%04X # 0x%04X %-3d %c\n", (uint16_t)out_data[i], (uint16_t)out_data_fpga[i], i, mismatch);
    }
  }

  delete[] in_data;
  delete[] in_data_quantized;

  delete[] k_data;
  delete[] k_data_quantized;

  delete[] out_data;
  delete[] out_data_fpga;

  if (threshold_data != NULL) {
        delete[] threshold_data;
  }

  return comp_fpga;
}

int main(int argc, char** argv) {
  argparse::ArgumentParser parser("unit_test", "Quntaized Convolution Strip Unit Test", "BSD 2-Clause License");

  parser.addArgument({"type"                       }, "Generate Type <sequential|random|all_1>");
  parser.addArgument({"-kw"  , "--kernel-width"   }, "Kernel Width  <1|3> default=1" );
  parser.addArgument({"-kh"  , "--kernel-height"  }, "Kernel Height <1|3> default=1" );
  parser.addArgument({"-oc"  , "--output-channels"}, "Output Channels     default=32");
  parser.addArgument({"-ic"  , "--input-channels" }, "Input Channels      default=32");
  parser.addArgument({"-iw"  , "--input-width"    }, "Input Width         default=1" );
  parser.addArgument({"-ih"  , "--input-height"   }, "Input Height        default=1" );
  parser.addArgument({"-th"  , "--use-threshold"  }, "Use Threshold <0|1> default=0" );
  parser.addArgument({"-v"   , "--verbose"        }, "Verbose Flag"          , argparse::ArgumentType::StoreTrue);
  parser.addArgument({"--generate-scenario"       }, "Generate Scenario Flag", argparse::ArgumentType::StoreTrue);

  auto args = parser.parseArgs(argc, argv);

  input_type    in_type;
  std::string   generate_type     = args.get<std::string> ("type");
  unsigned int  kw                = args.safeGet<unsigned int>("kw",  1);
  unsigned int  kh                = args.safeGet<unsigned int>("kh",  1);
  unsigned int  oc                = args.safeGet<unsigned int>("oc", 32);
  unsigned int  ic                = args.safeGet<unsigned int>("ic", 32);
  unsigned int  iw                = args.safeGet<unsigned int>("iw",  1);
  unsigned int  ih                = args.safeGet<unsigned int>("ih",  1);
  unsigned int  th                = args.safeGet<unsigned int>("th",  0);
  bool          verbose           = args.has("v");
  bool          generate_scenario = args.has("generate-scenario");

  if        (generate_type == "sequential") {
    in_type = SEQUENTIAL;
  } else if (generate_type == "random"    ) {
    in_type = RANDOM;
  } else if (generate_type == "all_1"     ) {
    in_type = ALL_1;
  } else {
    cout << "Error: input type is not supported." << endl << "Available input type: <sequential|random|add_1>" << endl;
    return 1;
  }

  bool res_conv = true;
  res_conv &= test_conv(in_type, kw, kh, ic, iw, ih, oc, th, verbose, generate_scenario);

  return (res_conv) ? 0 : 1;
}
