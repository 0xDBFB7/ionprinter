#include <vector>
#include <chrono>
#include <iostream>
#include <omp.h>
#include <cstdint>

#define SIZE 250

int main(){

	std::vector<float> potentials(SIZE*SIZE*SIZE);
	std::vector<float> potentials_out(SIZE*SIZE*SIZE);
	std::vector<float> source_term(SIZE*SIZE*SIZE);
	std::vector<float> boundaries(SIZE*SIZE*SIZE);

	potentials[(SIZE*SIZE*z)+(SIZE*y)+x] = 1;
  //
	auto t1 = std::chrono::high_resolution_clock::now();

  while(True){


	  #pragma omp parallel for
		for(int x=1;x<SIZE-1;x++){
			for(int y=1;y<SIZE-1;y++){
				for(int z=1;z<SIZE-1;z++){
					int coord = (SIZE*SIZE*z)+(SIZE*y)+x;
					if(!boundaries[coord]){
						potentials_out[coord] = (potentials[coord+1] +
						       potentials[coord-1] +
						       potentials[coord+SIZE] +
						       potentials[coord-SIZE] +
						       potentials[coord+(SIZE*SIZE)] +
						       potentials[coord-(SIZE*SIZE)] + source_term[coord])/6.0;
					}
				}
			}
		}

	}

  auto t2 = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/100.0;
  std::cout << duration;

}
