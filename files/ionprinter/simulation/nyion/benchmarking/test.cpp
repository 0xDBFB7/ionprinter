#include <vector>
#include <chrono>
#include <iostream>
#include <omp.h>
#include <cstdint>
#define SIZE 100

int main(){

	std::vector<float> potentials(SIZE*SIZE*SIZE);
	std::vector<float> potentials_out(SIZE*SIZE*SIZE);
	potentials[100] = 1;

	auto t1 = std::chrono::high_resolution_clock::now();
	#pragma omp parallel for
	for(int x=1; x < SIZE-1; x++){
		for(int y=1; y < SIZE-1; y++){
			for(int z=1; z < SIZE-1;z++){
				int coord = (z*SIZE*SIZE) + y*SIZE + x;
				potentials[coord] = (potentials[coord+1] +
						     potentials[coord-1] +
						     potentials[coord+SIZE] +
						     potentials[coord-SIZE] +
						     potentials[coord+(SIZE*SIZE)] +
						     potentials[coord-(SIZE*SIZE)])/6.0;
			}
		}
	}
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << duration;

}
