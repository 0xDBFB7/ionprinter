#include "nyion.hpp"
#include "debug_helpers.hpp"


void debug_preamble_printer(int line, std::string file, std::string name){
  std::cout << "\033[1;36m";
  std::cout << "\033[1;36m" << file << "\033[0m | \033[1;36m" << "line " << line << "\033[0m | \033[1;33m" << name;
  std::cout << "\033[0m";
}

// template<typename T>
// void array_stats(T * input, int start, int end, std::string name, std::string name, std::string name){
//
// }

template<typename T>
void pretty_print_array(T * input, int start, int end){
  std::cout << "\033[1;33m" << "[" << end << "]" << "\033[0m = {";

  for(int i = start; i < end-1; i++){
    std::cout << input[i] << ",";
  }
  if(end-start > 0){
    std::cout << input[end-1]; //handle trailing ,
  }

  std::cout << "};\n";
}
template void pretty_print_array(float * input, int start, int length);
template void pretty_print_array(int * input, int start, int length);
