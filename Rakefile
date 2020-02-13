TARGET                 = "qconv_strip_acp"
FPGA_BITSTREAM_FILE    = TARGET + ".bin"
LINUX_KERNEL_RELEASE   = /^(\d+\.\d+)/.match(`uname -r`)[1]
DEVICE_TREE_FILE       = TARGET + "_" + LINUX_KERNEL_RELEASE + ".dts"
DEVICE_TREE_NAME       = "qconv_strip"
DEVICE_TREE_DIRECTORY  = "/config/device-tree/overlays/" + DEVICE_TREE_NAME
UIO_DEVICE_NAMES       = ["uio_qconv_strip"]
UDMABUF_DEVICE_NAMES   = ["udmabuf-qconv-in", "udmabuf-qconv-out", "udmabuf-qconv-k", "udmabuf-qconv-th"]

CC                     = "g++"
CFLAGS                 = "-I ./include -Wpointer-arith"

def find_uio_device(name)
  found_device_name = nil
  Dir::entries("/sys/class/uio").map{ |device_name|
    if device_name =~ /^uio/
      File.open("/sys/class/uio/#{device_name}/name"){|file|
        if name.eql?(file.gets.chop)
          found_device_name = device_name
        end
      }
    end
  }
  return found_device_name
end

desc "Install fpga and devicetrees"
task :install => ["/lib/firmware/#{FPGA_BITSTREAM_FILE}", DEVICE_TREE_FILE] do
  begin
    sh "./dtbocfg.rb --install #{DEVICE_TREE_NAME} --dts #{DEVICE_TREE_FILE}"
  rescue => e
    print "error raised:"
    p e
    abort
  end
  if (Dir.exist?(DEVICE_TREE_DIRECTORY) == false)
    abort "can not #{DEVICE_TREE_DIRECTORY} installed."
  end

  UIO_DEVICE_NAMES.each do |device_name|
    device_file = find_uio_device(device_name)
    if (device_file.nil?)
      abort "can not find uio device file named #{device_name}"
    end
    if (File.exist?("/dev/" + device_file) == false)
      abort "can not /dev/#{device_file} installed."
    end
    File::chmod(0666, "/dev/" + device_file)
  end    

  UDMABUF_DEVICE_NAMES.each do |device_file|
    if (File.exist?("/dev/" + device_file) == false)
      abort "can not /dev/#{device_file} installed."
    end
    File::chmod(0666, "/dev/" + device_file)
    File::chmod(0666, "/sys/class/udmabuf/" + device_file + "/sync_mode")
    File::chmod(0666, "/sys/class/udmabuf/" + device_file + "/sync_offset")
    File::chmod(0666, "/sys/class/udmabuf/" + device_file + "/sync_size")
    File::chmod(0666, "/sys/class/udmabuf/" + device_file + "/sync_direction")
    File::chmod(0666, "/sys/class/udmabuf/" + device_file + "/sync_owner")
    File::chmod(0666, "/sys/class/udmabuf/" + device_file + "/sync_for_cpu")
    File::chmod(0666, "/sys/class/udmabuf/" + device_file + "/sync_for_device")
  end
end

desc "Uninstall fpga and devicetrees"
task :uninstall do
  if (Dir.exist?(DEVICE_TREE_DIRECTORY) == false)
    abort "can not #{DEVICE_TREE_DIRECTORY} uninstalled: does not already exists."
  end
  sh "./dtbocfg.rb --remove #{DEVICE_TREE_NAME}"
end

file "/lib/firmware/" + FPGA_BITSTREAM_FILE => [ FPGA_BITSTREAM_FILE ] do
  sh "cp #{FPGA_BITSTREAM_FILE} /lib/firmware/#{FPGA_BITSTREAM_FILE}"
end

directory DEVICE_TREE_DIRECTORY do
  Rake::Task["install"].invoke
end

file DEVICE_TREE_FILE => [ "qconv_strip.dts" ] do
  linux_release_number = 0
  /(\d+)\.(\d+)/.match(LINUX_KERNEL_RELEASE)[1..2].each do |n|
    linux_release_number = linux_release_number*100 + n.to_i
  end
  if linux_release_number < 419 then
    clk_name = "clkc"
  else 
    clk_name = "zynqmp_clk"
  end
  File.open(DEVICE_TREE_FILE, "w") do |o_file|
    File.open("qconv_strip.dts") do |i_file|
      i_file.each_line do |line|
        line = line.gsub(/(^\s*firmware-name\s*=\s*)(.*);/){"#{$1}\"#{FPGA_BITSTREAM_FILE}\";"}
        line = line.gsub(/&zynqmp_clk/, "&#{clk_name}")
        o_file.puts(line)
      end
    end
  end
end

file "qconv_data_generator" => ["src/util/qconv_data_generator.cpp"] do
  sh "#{CC} #{CFLAGS} -o qconv_data_generator src/util/qconv_data_generator.cpp"
end

file "unit_test" => ["src/tb/unit_test.cpp", "src/cpp/conv1x1.cpp", "src/cpp/conv3x3.cpp", "include/udmabuf.hpp", "include/uio.hpp", "include/qconv_strip.hpp" ] do |task|
  cpp_file_list = task.prerequisites.select{|file_name| File.extname(file_name) == '.cpp'}.join(' ')
  sh "#{CC} #{CFLAGS} -o #{task.name} #{cpp_file_list}"
end

#                  type      iw  ih  ic  oc  kw  kh  use_th
TEST_SCENARIOS = [["random",  1,  1, 32, 32,  1,  1, 0],
                  ["random",  1,  1, 32, 32,  1,  1, 1],
                  ["random",  8,  8, 32, 32,  1,  1, 0],
                  ["random",  8,  8, 32, 32,  1,  1, 1],
                  ["random", 32, 32, 32, 32,  1,  1, 0],
                  ["random", 32, 32, 32, 32,  1,  1, 1],
                  ["random",  1,  1, 64, 64,  1,  1, 0],
                  ["random",  1,  1, 64, 64,  1,  1, 1],
                  ["random", 32, 32,128,128,  1,  1, 0],
                  ["random", 32, 32,128,128,  1,  1, 1],
                  ["random", 64, 64, 64, 64,  1,  1, 0],
                  ["random", 64, 64, 64, 64,  1,  1, 1],
                  ["random",  1,  1, 32, 32,  3,  3, 0],
                  ["random",  1,  1, 32, 32,  3,  3, 1],
                  ["random",  8,  8, 32, 32,  3,  3, 0],
                  ["random",  8,  8, 32, 32,  3,  3, 1],
                  ["random", 32, 32, 32, 32,  3,  3, 0],
                  ["random", 32, 32, 32, 32,  3,  3, 1],
                  ["random",  1,  1, 64, 64,  3,  3, 0],
                  ["random",  1,  1, 64, 64,  3,  3, 1],
                  ["random", 32, 32,128,128,  3,  3, 0],
                  ["random", 32, 32,128,128,  3,  3, 1],
                  ["random", 64, 64, 64, 64,  3,  3, 0],
                  ["random", 64, 64, 64, 64,  3,  3, 1]]
test_task_list =  []

TEST_SCENARIOS.each_with_index do |scenario_param, index|
  type = scenario_param[0]
  iw   = scenario_param[1]
  ih   = scenario_param[2]
  ic   = scenario_param[3]
  oc   = scenario_param[4]
  kw   = scenario_param[5]
  kh   = scenario_param[6]
  uth  = scenario_param[7]
  task_name = sprintf("test_%03d", index).to_sym
  test_task_list.push(task_name)
  desc "unit_test type=#{type} iw=#{iw} ih=#{ih} ic=#{ic} oc=#{oc} kw=#{kw} kh=#{kh} use_th={uth}"
  task task_name => ["unit_test"] do
    sh "./unit_test -iw #{iw} -ih #{ih} -ic #{ic} -oc #{oc} -kw #{kw} -kh #{kh} -th #{uth} #{type}"
  end
end

desc "unit_test_all"
task :unit_test_all => test_task_list do
end

#                   type      iw  ih   ic  oc  kw  kh  use_th
TEST2_SCENARIOS = [["random",160,160,  64, 32,  1,  1, 1],
                   ["random",160,160,  32,  8,  3,  3, 1],
                   ["random", 80, 80,  32, 16,  3,  3, 1],
                   ["random", 40, 40,  64, 32,  3,  3, 1],
                   ["random", 20, 20, 128, 64,  3,  3, 1],
                   ["random", 10, 10, 256,128,  3,  3, 1],
                   ["random",  5,  5, 512,128,  3,  3, 1],
                   ["random",  5,  5, 128,256,  3,  3, 1],
                   ["random", 10, 10, 128, 32,  1,  1, 1],
                   ["random", 20, 20,  64, 16,  1,  1, 1],
                   ["random", 40, 40,  32,  4,  1,  1, 1],
                   ["random",  5,  5,1024,256,  3,  3, 1],
                   ["random",  5,  5, 256,128,  3,  3, 1],
                   ["random",  5,  5, 128,256,  3,  3, 1],
                   ["random",  5,  5, 256,128,  3,  3, 1],
                   ["random",  5,  5, 128,256,  3,  3, 1]]
test2_task_list =  []

TEST2_SCENARIOS.each_with_index do |scenario_param, index|
  type = scenario_param[0]
  iw   = scenario_param[1]
  ih   = scenario_param[2]
  ic   = scenario_param[3]
  oc   = scenario_param[4]
  kw   = scenario_param[5]
  kh   = scenario_param[6]
  uth  = scenario_param[7]
  task_name = sprintf("test_2_%03d", index).to_sym
  test2_task_list.push(task_name)
  desc "unit_test type=#{type} iw=#{iw} ih=#{ih} ic=#{ic} oc=#{oc} kw=#{kw} kh=#{kh} use_th={uth}"
  task task_name => ["unit_test"] do
    sh "./unit_test -iw #{iw} -ih #{ih} -ic #{ic} -oc #{oc} -kw #{kw} -kh #{kh} -th #{uth} #{type}"
  end
end

desc "unit_test2_all"
task :unit_test2_all => test2_task_list do
end

task :default => ["unit_test"]
