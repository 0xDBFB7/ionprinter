#include "nyion.hpp"


#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
//make filename__ preprocessor directive relative - thanks red1ynx!


template<typename T>
void pretty_print_array(T * input, int start, int end);

void debug_preamble_printer(int line, std::string file, std::string name);

#define pretty_print_named_array(input, start, end) debug_preamble_printer(__LINE__, __FILENAME__, #input); pretty_print_array(input, start, end);
