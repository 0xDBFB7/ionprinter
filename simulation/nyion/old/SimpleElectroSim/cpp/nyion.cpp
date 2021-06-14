#include <stdio.h>
#include <iostream>
#include <fstream>


int main(){
	std::ofstream myfile;
	myfile.open ("positions.csv;
	myfile << "This is the first cell in the first column.\n";
	myfile << "a,b,c,\n";
	myfile << "c,s,v,\n";
	myfile << "1,2,3.456\n";
	myfile << "semi;colon";
	myfile.close();
}
