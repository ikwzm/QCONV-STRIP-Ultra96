#include "argparse.hpp"

int main(int argc, char** argv) {
  argparse::ArgumentParser parser("qconv_data_generator", "Quntaized Convolution Data Generator", "Apache License 2.0");

  parser.addArgument({"type"                       }, "Generate Type <sequential|random|all_1>");
  parser.addArgument({"--kw"  , "--kernel-width"   }, "Kernel Width  <1|3> default=1" );
  parser.addArgument({"--kh"  , "--kernel-height"  }, "Kernel Height <1|3> default=1" );
  parser.addArgument({"--oc"  , "--output-channels"}, "Output Channels     default=32");
  parser.addArgument({"--ic"  , "--input-channels" }, "Input Channels      default=32");
  parser.addArgument({"--iw"  , "--input-width"    }, "Input Width         default=1" );
  parser.addArgument({"--ih"  , "--input-height"   }, "Input Height        default=1" );
  parser.addArgument({"--th"  , "--use-threshold"  }, "Use Threshold <0|1> default=0" );
  parser.addArgument({"--of"  , "--output-file"    }, "Output File Name    default=data.yml");

  auto args = parser.parseArgs(argc, argv);

  std::string   generate_type    = args.get<std::string>("type");
  unsigned int  kw               = args.safeGet<unsigned int>("kw",  1);
  unsigned int  kh               = args.safeGet<unsigned int>("kh",  1);
  unsigned int  oc               = args.safeGet<unsigned int>("oc", 32);
  unsigned int  ic               = args.safeGet<unsigned int>("ic", 32);
  unsigned int  iw               = args.safeGet<unsigned int>("iw",  1);
  unsigned int  ih               = args.safeGet<unsigned int>("ih",  1);
  unsigned int  th               = args.safeGet<unsigned int>("th",  0);
  std::string   output_file_name = args.safeGet<std::string> ("of",  "data.yml");

  std::cout << std::boolalpha;
  std::cout << "type:   " << generate_type << std::endl;
  std::cout << "kw:     " << kw <<std::endl;
  std::cout << "kh:     " << kh <<std::endl;
  std::cout << "oc:     " << oc <<std::endl;
  std::cout << "ic:     " << ic <<std::endl;
  std::cout << "iw:     " << iw <<std::endl;
  std::cout << "ih:     " << ih <<std::endl;
  std::cout << "th:     " << th <<std::endl;
  std::cout << "output: " << output_file_name <<std::endl;
}
