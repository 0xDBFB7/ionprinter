#include <iostream>
#include <fstream>

#include <string>

#define DEBUG_PRINT

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
//make filename__ preprocessor directive relative - thanks red1ynx!


template<typename T>
void pretty_print_array(T * input, int start, int end);


void debug_preamble_printer(std::string function, int line, std::string file, std::string name);

#ifndef DEBUG_PRINT
  #define pretty_print_named_array(a,b,c) //disable prints
#else
  #define pretty_print_named_array(input, start, end) debug_preamble_printer(__FUNCTION__, __LINE__, __FILENAME__, #input); pretty_print_array(input, start, end);
  // #define pretty_print_named_value(input) debug_preamble_printer(__FUNCTION__, __LINE__, __FILENAME__, #input); std::cout << " = " << input;


#define named_value(input) std::cout << "    \033[1;33m" << #input << "\033[0m" << " = " << input << "\n";
#define named_array(input,len) std::cout << "    \033[1;33m" << #input << \
                                      "\033[0m [" << len << "]" << " = {"; \
                                      for(uint32_t i = 0; i < len-1; i++){std::cout << input[i] << ",";}; \
                                      if(len){std::cout << input[len-1];} \
                                      std::cout << "}\n";


//add pointer location to printed info?

#endif
