Quantized Convolution (strip) for Ultra96
=========================================

Quantized Convolution (strip) binary and project and test code for Ultra96.

Quantized Convolution is a convolution method published by LeapMind Inc(https://leapmind.io) on Blueoil(https://github.com/blue-oil/blueoil).

### Requirement

* Board: Ultra96
* OS: https://github.com/ikwzm/ZynqMP-FPGA-Linux

## Install

### Install ZynqMP-FPGA-Linux

See https://github.com/ikwzm/ZynqMP-FPGA-Linux

### Expand the CMA area

Add ```cma=256M``` to linux_boot_args in uEnv.txt.

```
linux_kernel_image=vmlinuz-4.14.34-armv7-fpga
linux_fdt_image=devicetree-4.14.34-socfpga.dtb
linux_boot_args=console=ttyS0,115200 root=/dev/mmcblk0p2 rw rootwait uio_pdrv_genirq.of_id=generic-uio cma=256M

linux_load_cmd=fatload mmc 0 ${loadaddr} ${linux_kernel_image} && fatload mmc 0 ${fdt_addr} ${linux_fdt_image}
linux_boot_cmd=setenv bootargs ${linux_boot_args} && bootz ${loadaddr} - ${fdt_addr}

uenvcmd=run linux_load_cmd && run linux_boot_cmd

bootmenu_0=Boot linux-4.14.34-armv7-fpga=boot
```

### Boot ZynqMP-FPGA-Linux

### Login fpga user

### Download QCONV-STRIP-Ultra96 to Ultra96

```console
fpga@debian-fpga:~/$ git clone https://github.com/ikwzm/QCONV-STRIP-Ultra96.git
fpga@debian-fpga:~/$ cd QCONV-STRIP-Ultra96
```

### Install FPGA Bitstream file

(T.B.D.)

### Run Unit Test

(T.B.D.)

## Build Bitstream file

### Requirement

* Xilinx Vivado 2018.3

### Download QCONV-STRIP-Ultra96

```console
shell$ git clone https://github.com/ikwzm/QCONV-STRIP-Ultra96.git
shell$ cd QCONV-STRIP-Ultra96
shell$ git submodule update --init --recursive
```

### Build qconv_strip_axi3.bin

#### Download QCONV-STRIP-Ultra96

```console
shell$ git clone https://github.com/ikwzm/QCONV-STRIP-Ultra96.git
shell$ cd QCONV-STRIP-Ultra96
shell$ git submodule update --init --recursive
```

#### Create Project

```
Vivado > Tools > Run Tcl Script > project/create_project.tcl
```

#### Implementation

```
Vivado > Tools > Run Tcl Script > project/implementation.tcl
```

#### Convert from Bitstream File to Binary File

```
shell$ tools/fpga-bit-to-bin.py --flip project/project.runs/impl_1/design_1_wrapper.bit qconv_strip_axi3.bin
```

## Licensing

Distributed under the BSD 2-Clause License.

