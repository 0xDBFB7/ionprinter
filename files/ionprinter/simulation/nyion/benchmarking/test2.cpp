#include <vector>
#include <chrono>
#include <iostream>
#include <omp.h>
#include <cstdint>

#define SIZE 250

int main(){

	std::vector<float> potentials(SIZE*SIZE*SIZE);
	std::vector<float> potentials_out(SIZE*SIZE*SIZE);
	potentials[100] = 1;
  //
	auto t1 = std::chrono::high_resolution_clock::now();

  for(int i=0; i < 10; i++){
  #pragma omp parallel for
	for(int coord=SIZE*SIZE; coord < SIZE*SIZE*SIZE-(SIZE*SIZE); coord++){
    potentials_out[coord] = (potentials[coord+1] +
             potentials[coord-1] +
             potentials[coord+SIZE] +
             potentials[coord-SIZE] +
             potentials[coord+(SIZE*SIZE)] +
             potentials[coord-(SIZE*SIZE)])/6.0;
	}


  }
  auto t2 = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/100.0;
  std::cout << duration;

}
