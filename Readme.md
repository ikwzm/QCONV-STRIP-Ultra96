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
linux_kernel_image=image-4.14.0-xlnx-v2018.2-zynqmp-fpga
linux_fdt_image=devicetree-4.14.0-xlnx-v2018.2-zynqmp-fpga-ultra96.dtb
linux_boot_args=console=ttyPS0,115200 root=/dev/mmcblk0p2 rw rootwait uio_pdrv_genirq.of_id=generic-uio cma=256M

linux_img_load_cmd=fatload mmc ${sdbootdev} ${kernel_addr} ${linux_kernel_image}
linux_fdt_load_cmd=fatload mmc ${sdbootdev} ${fdt_addr}    ${linux_fdt_image}
linux_load_cmd=run linux_img_load_cmd && run linux_fdt_load_cmd
linux_boot_cmd=setenv bootargs ${linux_boot_args} && booti ${kernel_addr} - ${fdt_addr}

uenvcmd=run linux_load_cmd && run linux_boot_cmd

bootmenu_0=Boot Default=boot
```

### Boot ZynqMP-FPGA-Linux

### Login fpga user

### Download QCONV-STRIP-Ultra96 to Ultra96

```console
fpga@debian-fpga:~/$ git clone https://github.com/ikwzm/QCONV-STRIP-Ultra96.git
fpga@debian-fpga:~/$ cd QCONV-STRIP-Ultra96
```

### Install FPGA Bitstream file

```console
fpga@debian-fpga:~/QCONV-STRIP-Ultra96$ sudo rake install
./dtbocfg.rb --install qconv_strip --dts qconv_strip.dts
/tmp/dtovly20190525-4198-1dq5awi: Warning (unit_address_vs_reg): /[ 1552.148082] fpga_manager fpga0: writing qconv_strip_axi.bin to Xilinx ZynqMP FPGA Manager
fragment@2/__overlay__/uio_qconv_strip: node has a reg or ranges property, but no unit name
/tmp/dtovly20190525-4198-1dq5awi: Warning (avoid_unnecessary_addr_size): /fragment@2: unnecessary #address-cells/#size-cells without "ranges" or child "reg" property
[ 1552.307714] fclkcfg amba_pl@0:fclk0: driver installed.
[ 1552.312819] fclkcfg amba_pl@0:fclk0: device name    : fclk0
[ 1552.318365] fclkcfg amba_pl@0:fclk0: clock  name    : pl0_ref
[ 1552.324069] fclkcfg amba_pl@0:fclk0: clock  rate    : 249999998
[ 1552.329990] fclkcfg amba_pl@0:fclk0: clock  enabled : 1
[ 1552.335179] fclkcfg amba_pl@0:fclk0: remove rate    : 1000000
[ 1552.340904] fclkcfg amba_pl@0:fclk0: remove enable  : 0
[ 1552.351081] udmabuf udmabuf-qconv-in: driver version = 1.3.2
[ 1552.356689] udmabuf udmabuf-qconv-in: major number   = 242
[ 1552.362145] udmabuf udmabuf-qconv-in: minor number   = 0
[ 1552.367433] udmabuf udmabuf-qconv-in: phys address   = 0x0000000070100000
[ 1552.374201] udmabuf udmabuf-qconv-in: buffer size    = 4194304
[ 1552.380016] udmabuf udmabuf-qconv-in: dma coherent   = 0
[ 1552.385313] udmabuf amba_pl@0:udmabuf_qconv_in: driver installed.
[ 1552.397118] udmabuf udmabuf-qconv-out: driver version = 1.3.2
[ 1552.402786] udmabuf udmabuf-qconv-out: major number   = 242
[ 1552.408354] udmabuf udmabuf-qconv-out: minor number   = 1
[ 1552.413720] udmabuf udmabuf-qconv-out: phys address   = 0x0000000070500000
[ 1552.420579] udmabuf udmabuf-qconv-out: buffer size    = 8388608
[ 1552.426481] udmabuf udmabuf-qconv-out: dma coherent   = 0
[ 1552.431868] udmabuf amba_pl@0:udmabuf_qconv_out: driver installed.
[ 1552.441905] udmabuf udmabuf-qconv-k: driver version = 1.3.2
[ 1552.447434] udmabuf udmabuf-qconv-k: major number   = 242
[ 1552.452792] udmabuf udmabuf-qconv-k: minor number   = 2
[ 1552.457998] udmabuf udmabuf-qconv-k: phys address   = 0x0000000070d00000
[ 1552.464684] udmabuf udmabuf-qconv-k: buffer size    = 4194304
[ 1552.470409] udmabuf udmabuf-qconv-k: dma coherent   = 0
[ 1552.475621] udmabuf amba_pl@0:udmabuf_qconv_k: driver installed.
[ 1552.482725] udmabuf udmabuf-qconv-th: driver version = 1.3.2
[ 1552.488357] udmabuf udmabuf-qconv-th: major number   = 242
[ 1552.493779] udmabuf udmabuf-qconv-th: minor number   = 3
[ 1552.499075] udmabuf udmabuf-qconv-th: phys address   = 0x0000000070090000
[ 1552.505842] udmabuf udmabuf-qconv-th: buffer size    = 65536
[ 1552.511486] udmabuf udmabuf-qconv-th: dma coherent   = 0
[ 1552.516778] udmabuf amba_pl@0:udmabuf_qconv_th: driver installed.
```

### Run Unit Test

```console
fpga@debian-fpga:~/QCONV-STRIP-Ultra96$ rake unit_test2_all
./unit_test -iw 160 -ih 160 -ic 64 -oc 32 -kw 1 -kh 1 -th 1 random
FPGA exec time (160x160x64x32 1x1): 655 [usec]
[qconv_strip] test success!!!
./unit_test -iw 160 -ih 160 -ic 32 -oc 8 -kw 3 -kh 3 -th 1 random
FPGA exec time (160x160x32x8 3x3): 240 [usec]
[qconv_strip] test success!!!
./unit_test -iw 80 -ih 80 -ic 32 -oc 16 -kw 3 -kh 3 -th 1 random
FPGA exec time (80x80x32x16 3x3): 146 [usec]
[qconv_strip] test success!!!
./unit_test -iw 40 -ih 40 -ic 64 -oc 32 -kw 3 -kh 3 -th 1 random
FPGA exec time (40x40x64x32 3x3): 79 [usec]
[qconv_strip] test success!!!
./unit_test -iw 20 -ih 20 -ic 128 -oc 64 -kw 3 -kh 3 -th 1 random
FPGA exec time (20x20x128x64 3x3): 79 [usec]
[qconv_strip] test success!!!
./unit_test -iw 10 -ih 10 -ic 256 -oc 128 -kw 3 -kh 3 -th 1 random
FPGA exec time (10x10x256x128 3x3): 133 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 512 -oc 128 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x512x128 3x3): 126 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 128 -oc 256 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x128x256 3x3): 73 [usec]
[qconv_strip] test success!!!
./unit_test -iw 10 -ih 10 -ic 128 -oc 32 -kw 1 -kh 1 -th 1 random
FPGA exec time (10x10x128x32 1x1): 14 [usec]
[qconv_strip] test success!!!
./unit_test -iw 20 -ih 20 -ic 64 -oc 16 -kw 1 -kh 1 -th 1 random
FPGA exec time (20x20x64x16 1x1): 25 [usec]
[qconv_strip] test success!!!
./unit_test -iw 40 -ih 40 -ic 32 -oc 4 -kw 1 -kh 1 -th 1 random
FPGA exec time (40x40x32x4 1x1): 34 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 1024 -oc 256 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x1024x256 3x3): 459 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 256 -oc 128 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x256x128 3x3): 87 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 128 -oc 256 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x128x256 3x3): 94 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 256 -oc 128 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x256x128 3x3): 95 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 128 -oc 256 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x128x256 3x3): 80 [usec]
[qconv_strip] test success!!!
```

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

```console
vivado% cp project/project.runs/impl_1/design_1_wrapper.bit qconv_strip_axi.bit
vivado% bootgen -image qconv_strip_axi.bif -arch zynqmp -w -o qconv_strip_axi.bin
```

## Licensing

Distributed under the BSD 2-Clause License.

