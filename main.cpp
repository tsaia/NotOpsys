#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <exception>
#include "memory.h"

int main(){
	//this is all just testing stuff
	memory mem(8, 32);
	std::cout << "done\n";
	process A(10, 'A');
	process B(15, 'B');
	process C(50, 'C');
	process Q(3, 'Q');
	process E(2, 'E');
	process T(126, 'T');

	mem.test();
	mem.printMem();
	mem.addWF(E);
	mem.printMem();
		mem.addWF(Q);
	mem.printMem();
	mem.addNC(T);
	mem.printMem();
	// mem.addBF(A);
	// mem.printMem();
	// mem.addNF(B);
	// mem.printMem();
	// mem.addNF(C);it 
	// mem.printMem();
	// mem.remove(A);
	// mem.remove(B);
	// mem.addNF(D);
	// mem.printMem();
	// mem.addNF(E);
	// mem.printMem();
	return 1;
}