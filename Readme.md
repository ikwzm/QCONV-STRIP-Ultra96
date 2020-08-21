Quantized Convolution (strip) for Ultra96
=========================================

Quantized Convolution (strip) binary and project and test code for Ultra96/Ultra96-V2.

Quantized Convolution is a convolution method published by LeapMind Inc(https://leapmind.io) on Blueoil(https://github.com/blue-oil/blueoil).

### Requirement

* Board: Ultra96, Ultra96-V2
* OS: https://github.com/ikwzm/ZynqMP-FPGA-Linux

## Install

### Install ZynqMP-FPGA-Linux

See https://github.com/ikwzm/ZynqMP-FPGA-Linux

### Boot ZynqMP-FPGA-Linux

### Login fpga user

### Download QCONV-STRIP-Ultra96 to Ultra96

```console
fpga@debian-fpga:~/$ git clone https://github.com/ikwzm/QCONV-STRIP-Ultra96.git
fpga@debian-fpga:~/$ cd QCONV-STRIP-Ultra96
```

### Expand the CMA area

Add ```cma=256M``` to linux_boot_args_cma in /mnt/boot/uEnv.txt

```
########################################################################
# Linux Boot Argments
#  * linux_boot_args_console : ex) console=tty1
#                                  console=ttyPS0,115200
#  * linux_boot_args_rootfs  : ex) root=/dev/mmcblk0p2 rw rootwait
#  * linux_boot_args_systemd : ex) systemd.unit=graphical.target
#                                  systemd.unit=multi-user.target
#  * linux_boot_args_cpuidle : ex) cpuidle.off=1
#  * linux_boot_args_cma     : ex) cma=256M
#  * linux_boot_args_uio     : ex) uio=uio_pdrv_genirq.of_id=generic-uio
#  * linux_boot_args_other   :
########################################################################
linux_boot_args_console=console=ttyPS0,115200
linux_boot_args_rootfs=root=/dev/mmcblk0p2 rw rootwait
linux_boot_args_systemd=
linux_boot_args_cpuidle=cpuidle.off=1
linux_boot_args_cma=cma=256M
linux_boot_args_uio=uio_pdrv_genirq.of_id=generic-uio
linux_boot_args_other=
```

### Reboot ZynqMP-FPGA-Linux

```console
fpga@debian-fpga:~/QCONV-STRIP-Ultra96$ sudo reboot
```

### Install FPGA Bitstream file and Device Tree

```console
fpga@debian-fpga:~/QCONV-STRIP-Ultra96$ sudo rake install
cp qconv_strip_acp.bin /lib/firmware/qconv_strip_acp.bin
/tmp/dtovly20200821-813-onecke: Warning (unit_address_vs_reg): /fragment@2/__overlay__/uio_qconv_strip: node has a reg or ranges property, but no unit name
/tmp/dtovly20200821-813-onecke: Warning (avoid_unnecessary_addr_size): /fragment@2: unnecessary #address-cells/#size-cells without "ranges" or child "reg" property

[ 2322.715342] fpga_manager fpga0: writing qconv_strip_acp.bin to Xilinx ZynqMP FPGA Manager
[ 2322.871319] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /fpga-full/firmware-name
[ 2322.889122] fclkcfg amba_pl@0:fclk0: driver version : 1.7.1
[ 2322.894727] fclkcfg amba_pl@0:fclk0: device name    : amba_pl@0:fclk0
[ 2322.901174] fclkcfg amba_pl@0:fclk0: clock  name    : pl0_ref
[ 2322.906930] fclkcfg amba_pl@0:fclk0: clock  rate    : 249999998
[ 2322.912873] fclkcfg amba_pl@0:fclk0: clock  enabled : 1
[ 2322.918091] fclkcfg amba_pl@0:fclk0: remove rate    : 1000000
[ 2322.923828] fclkcfg amba_pl@0:fclk0: remove enable  : 0
[ 2322.929053] fclkcfg amba_pl@0:fclk0: driver installed.
[ 2322.938227] u-dma-buf udmabuf-qconv-in: driver version = 3.2.0
[ 2322.944066] u-dma-buf udmabuf-qconv-in: major number   = 241
[ 2322.949723] u-dma-buf udmabuf-qconv-in: minor number   = 0
[ 2322.955208] u-dma-buf udmabuf-qconv-in: phys address   = 0x0000000070400000
[ 2322.962170] u-dma-buf udmabuf-qconv-in: buffer size    = 4194304
[ 2322.968175] u-dma-buf amba_pl@0:udmabuf_qconv_in: driver installed.
[ 2322.979305] u-dma-buf udmabuf-qconv-out: driver version = 3.2.0
[ 2322.985227] u-dma-buf udmabuf-qconv-out: major number   = 241
[ 2322.990976] u-dma-buf udmabuf-qconv-out: minor number   = 1
[ 2322.996553] u-dma-buf udmabuf-qconv-out: phys address   = 0x0000000070800000
[ 2323.003603] u-dma-buf udmabuf-qconv-out: buffer size    = 8388608
[ 2323.009696] u-dma-buf amba_pl@0:udmabuf_qconv_out: driver installed.
[ 2323.019803] u-dma-buf udmabuf-qconv-k: driver version = 3.2.0
[ 2323.025582] u-dma-buf udmabuf-qconv-k: major number   = 241
[ 2323.031153] u-dma-buf udmabuf-qconv-k: minor number   = 2
[ 2323.036577] u-dma-buf udmabuf-qconv-k: phys address   = 0x0000000071000000
[ 2323.043452] u-dma-buf udmabuf-qconv-k: buffer size    = 4194304
[ 2323.049372] u-dma-buf amba_pl@0:udmabuf_qconv_k: driver installed.
[ 2323.057021] u-dma-buf udmabuf-qconv-th: driver version = 3.2.0
[ 2323.062871] u-dma-buf udmabuf-qconv-th: major number   = 241
[ 2323.068531] u-dma-buf udmabuf-qconv-th: minor number   = 3
[ 2323.074011] u-dma-buf udmabuf-qconv-th: phys address   = 0x0000000070060000
[ 2323.080974] u-dma-buf udmabuf-qconv-th: buffer size    = 65536
[ 2323.086804] u-dma-buf amba_pl@0:udmabuf_qconv_th: driver installed.
```

## Run 

### Run Unit Test

```console
fpga@debian-fpga:~/QCONV-STRIP-Ultra96$ rake unit_test2_all
./unit_test -iw 160 -ih 160 -ic 64 -oc 32 -kw 1 -kh 1 -th 1 random
FPGA exec time (160x160x64x32 1x1): 1281 [usec]
[qconv_strip] test success!!!
./unit_test -iw 160 -ih 160 -ic 32 -oc 8 -kw 3 -kh 3 -th 1 random
FPGA exec time (160x160x32x8 3x3): 452 [usec]
[qconv_strip] test success!!!
./unit_test -iw 80 -ih 80 -ic 32 -oc 16 -kw 3 -kh 3 -th 1 random
FPGA exec time (80x80x32x16 3x3): 151 [usec]
[qconv_strip] test success!!!
./unit_test -iw 40 -ih 40 -ic 64 -oc 32 -kw 3 -kh 3 -th 1 random
FPGA exec time (40x40x64x32 3x3): 82 [usec]
[qconv_strip] test success!!!
./unit_test -iw 20 -ih 20 -ic 128 -oc 64 -kw 3 -kh 3 -th 1 random
FPGA exec time (20x20x128x64 3x3): 68 [usec]
[qconv_strip] test success!!!
./unit_test -iw 10 -ih 10 -ic 256 -oc 128 -kw 3 -kh 3 -th 1 random
FPGA exec time (10x10x256x128 3x3): 172 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 512 -oc 128 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x512x128 3x3): 108 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 128 -oc 256 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x128x256 3x3): 58 [usec]
[qconv_strip] test success!!!
./unit_test -iw 10 -ih 10 -ic 128 -oc 32 -kw 1 -kh 1 -th 1 random
FPGA exec time (10x10x128x32 1x1): 11 [usec]
[qconv_strip] test success!!!
./unit_test -iw 20 -ih 20 -ic 64 -oc 16 -kw 1 -kh 1 -th 1 random
FPGA exec time (20x20x64x16 1x1): 15 [usec]
[qconv_strip] test success!!!
./unit_test -iw 40 -ih 40 -ic 32 -oc 4 -kw 1 -kh 1 -th 1 random
FPGA exec time (40x40x32x4 1x1): 19 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 1024 -oc 256 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x1024x256 3x3): 528 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 256 -oc 128 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x256x128 3x3): 56 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 128 -oc 256 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x128x256 3x3): 64 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 256 -oc 128 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x256x128 3x3): 70 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 128 -oc 256 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x128x256 3x3): 65 [usec]
[qconv_strip] test success!!!
```

## Uninstall

### Uninstall Device Tree

```console
fpga@debian-fpga:~/QCONV-STRIP-Ultra96$ sudo rake uninstall
./dtbocfg.rb --remove qconv_strip
[ 2460.022306] u-dma-buf amba_pl@0:udmabuf_qconv_th: driver removed.
[ 2460.029678] u-dma-buf amba_pl@0:udmabuf_qconv_k: driver removed.
[ 2460.037550] u-dma-buf amba_pl@0:udmabuf_qconv_out: driver removed.
[ 2460.044698] u-dma-buf amba_pl@0:udmabuf_qconv_in: driver removed.
[ 2460.051848] fclkcfg amba_pl@0:fclk0: driver removed.
```

## Build Bitstream file

### Requirement

* Xilinx Vivado 2018.3
* Xilinx Vivado 2019.1

### Download QCONV-STRIP-Ultra96

```console
shell$ git clone https://github.com/ikwzm/QCONV-STRIP-Ultra96.git
shell$ cd QCONV-STRIP-Ultra96
shell$ git submodule update --init --recursive
```

### Build qconv_strip_axi.bin

#### Create Project

```
Vivado > Tools > Run Tcl Script > qconv_strip_acp/create_project.tcl
```

#### Implementation

```
Vivado > Tools > Run Tcl Script > qconv_strip_acp/implementation.tcl
```

#### Convert from Bitstream File to Binary File

```console
vivado% cp qconv_strip_acp/project.runs/impl_1/design_1_wrapper.bit qconv_strip_acp.bit
vivado% bootgen -image qconv_strip_acp.bif -arch zynqmp -w -o qconv_strip_acp.bin
```

## Licensing

Distributed under the BSD 2-Clause License.

