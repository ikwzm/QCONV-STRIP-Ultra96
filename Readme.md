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
./dtbocfg.rb --install qconv_strip --dts qconv_strip_acp_4.19.dts
/tmp/dtovly20200213-4112-vcp96b: Warning (unit_address_vs_reg): /fragment@2/__overlay__/uio_qconv_strip: node has a reg or ranges property, but no unit name
/tmp/dtovly20200213-4112-vcp96b: Warning (avoid_unnecessary_addr_size): /fragment@2: unnecessary #address-cells/#size-cells without "ranges" or child "reg" property
[22336.175676] fpga_manager fpga0: writing qconv_strip_acp.bin to Xilinx ZynqMP FPGA Manager
[22336.338190] fclkcfg amba_pl@0:fclk0: driver installed.
[22336.343379] fclkcfg amba_pl@0:fclk0: device name    : amba_pl@0:fclk0
[22336.349830] fclkcfg amba_pl@0:fclk0: clock  name    : pl0_ref
[22336.355577] fclkcfg amba_pl@0:fclk0: clock  rate    : 249999998
[22336.361519] fclkcfg amba_pl@0:fclk0: clock  enabled : 1
[22336.366749] fclkcfg amba_pl@0:fclk0: remove rate    : 1000000
[22336.372494] fclkcfg amba_pl@0:fclk0: remove enable  : 0
[22336.382257] udmabuf udmabuf-qconv-in: driver version = 1.4.6
[22336.387952] udmabuf udmabuf-qconv-in: major number   = 241
[22336.393442] udmabuf udmabuf-qconv-in: minor number   = 0
[22336.398754] udmabuf udmabuf-qconv-in: phys address   = 0x0000000070400000
[22336.405535] udmabuf udmabuf-qconv-in: buffer size    = 4194304
[22336.411372] udmabuf udmabuf-qconv-in: dma device     = amba_pl@0:udmabuf_qconv_in
[22336.418850] udmabuf udmabuf-qconv-in: dma coherent   = 0
[22336.424156] udmabuf amba_pl@0:udmabuf_qconv_in: driver installed.
[22336.436213] udmabuf udmabuf-qconv-out: driver version = 1.4.6
[22336.441974] udmabuf udmabuf-qconv-out: major number   = 241
[22336.447558] udmabuf udmabuf-qconv-out: minor number   = 1
[22336.452957] udmabuf udmabuf-qconv-out: phys address   = 0x0000000070800000
[22336.459833] udmabuf udmabuf-qconv-out: buffer size    = 8388608
[22336.465752] udmabuf udmabuf-qconv-out: dma device     = amba_pl@0:udmabuf_qconv_out
[22336.473407] udmabuf udmabuf-qconv-out: dma coherent   = 0
[22336.478804] udmabuf amba_pl@0:udmabuf_qconv_out: driver installed.
[22336.488989] udmabuf udmabuf-qconv-k: driver version = 1.4.6
[22336.494590] udmabuf udmabuf-qconv-k: major number   = 241
[22336.499987] udmabuf udmabuf-qconv-k: minor number   = 2
[22336.505210] udmabuf udmabuf-qconv-k: phys address   = 0x0000000071000000
[22336.511912] udmabuf udmabuf-qconv-k: buffer size    = 4194304
[22336.517664] udmabuf udmabuf-qconv-k: dma device     = amba_pl@0:udmabuf_qconv_k
[22336.524975] udmabuf udmabuf-qconv-k: dma coherent   = 0
[22336.530207] udmabuf amba_pl@0:udmabuf_qconv_k: driver installed.
[22336.539455] udmabuf udmabuf-qconv-th: driver version = 1.4.6
[22336.545129] udmabuf udmabuf-qconv-th: major number   = 241
[22336.550639] udmabuf udmabuf-qconv-th: minor number   = 3
[22336.555954] udmabuf udmabuf-qconv-th: phys address   = 0x00000000700a0000
[22336.562742] udmabuf udmabuf-qconv-th: buffer size    = 65536
[22336.568399] udmabuf udmabuf-qconv-th: dma device     = amba_pl@0:udmabuf_qconv_th
[22336.575883] udmabuf udmabuf-qconv-th: dma coherent   = 0
[22336.581193] udmabuf amba_pl@0:udmabuf_qconv_th: driver installed.
```

## Run 

### Run Unit Test

```console
fpga@debian-fpga:~/QCONV-STRIP-Ultra96$ rake unit_test2_all
./unit_test -iw 160 -ih 160 -ic 64 -oc 32 -kw 1 -kh 1 -th 1 random
FPGA exec time (160x160x64x32 1x1): 1337 [usec]
[qconv_strip] test success!!!
./unit_test -iw 160 -ih 160 -ic 32 -oc 8 -kw 3 -kh 3 -th 1 random
FPGA exec time (160x160x32x8 3x3): 333 [usec]
[qconv_strip] test success!!!
./unit_test -iw 80 -ih 80 -ic 32 -oc 16 -kw 3 -kh 3 -th 1 random
FPGA exec time (80x80x32x16 3x3): 154 [usec]
[qconv_strip] test success!!!
./unit_test -iw 40 -ih 40 -ic 64 -oc 32 -kw 3 -kh 3 -th 1 random
FPGA exec time (40x40x64x32 3x3): 85 [usec]
[qconv_strip] test success!!!
./unit_test -iw 20 -ih 20 -ic 128 -oc 64 -kw 3 -kh 3 -th 1 random
FPGA exec time (20x20x128x64 3x3): 66 [usec]
[qconv_strip] test success!!!
./unit_test -iw 10 -ih 10 -ic 256 -oc 128 -kw 3 -kh 3 -th 1 random
FPGA exec time (10x10x256x128 3x3): 93 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 512 -oc 128 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x512x128 3x3): 103 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 128 -oc 256 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x128x256 3x3): 55 [usec]
[qconv_strip] test success!!!
./unit_test -iw 10 -ih 10 -ic 128 -oc 32 -kw 1 -kh 1 -th 1 random
FPGA exec time (10x10x128x32 1x1): 9 [usec]
[qconv_strip] test success!!!
./unit_test -iw 20 -ih 20 -ic 64 -oc 16 -kw 1 -kh 1 -th 1 random
FPGA exec time (20x20x64x16 1x1): 13 [usec]
[qconv_strip] test success!!!
./unit_test -iw 40 -ih 40 -ic 32 -oc 4 -kw 1 -kh 1 -th 1 random
FPGA exec time (40x40x32x4 1x1): 25 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 1024 -oc 256 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x1024x256 3x3): 522 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 256 -oc 128 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x256x128 3x3): 82 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 128 -oc 256 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x128x256 3x3): 90 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 256 -oc 128 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x256x128 3x3): 54 [usec]
[qconv_strip] test success!!!
./unit_test -iw 5 -ih 5 -ic 128 -oc 256 -kw 3 -kh 3 -th 1 random
FPGA exec time (5x5x128x256 3x3): 74 [usec]
[qconv_strip] test success!!!
```

## Uninstall

### Uninstall Device Tree

```console
fpga@debian-fpga:~/QCONV-STRIP-Ultra96$ sudo rake uninstall
./dtbocfg.rb --remove qconv_strip
[22509.384069] udmabuf amba_pl@0:udmabuf_qconv_th: driver removed.
[22509.390959] udmabuf amba_pl@0:udmabuf_qconv_k: driver removed.
[22509.398528] udmabuf amba_pl@0:udmabuf_qconv_out: driver removed.
[22509.405560] udmabuf amba_pl@0:udmabuf_qconv_in: driver removed.
[22509.413403] fclkcfg amba_pl@0:fclk0: driver unloaded
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

