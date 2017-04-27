#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <exception>
#include "memory.h"


void teststuff(){
	//this is all just testing stuff
	memory mem(8, 32);
	std::cout << "done\n";
	process A('A', 10);
	process B('B', 15);
	process C('C', 50);
	process Q('Q', 3);
	process E('E', 2);
	process T('T', 126);

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
	mem.remove(A);
	// mem.remove(B);
	// mem.addNF(D);
	// mem.printMem();
	// mem.addNF(E);
	mem.printMem();
	mem.defrag(5);
	mem.printMem();

}
int main(int argc, char** argv){
	// if (argc < 2){
	// 	std::cout << "Usage: ./a.out <file>\n";
	// 	return EXIT_FAILURE;
	// }
 //  	std::ifstream inFile;
 //  	inFile.open(argv[1]);
 //  	if (!inFile.good()) {
 //    	std::cerr<<"ERROR: BAD FILE"<<std::endl;
	// 	return EXIT_FAILURE;
	// }


	int time = 0;

	//queue or something to do processes
	//map of maps?? map<int time, map<process, string action> >
	//action says remove or add
	//we'll see
	//or we add arrival times to process class and iterate through time idk

	//read in number of processes from file
	int numprocesses = 6;
	memory nfmem(8, 32);

	//tbh gonna copy and paste this 4 times once it's correct
	std::cout << "time " << time << "ms: Simulator started (Contiguous -- Next-Fit)\n";
	for (int i = 0; i < numprocesses; i++){//not actually the right loop conditions but im tired
		process Test('T', 10);
		process currentprocess = Test;
		std::cout << "time " << time << "ms: Process " << currentprocess.getName() << " arrived (requires " << currentprocess.getSize() << " frames)\n";
		int result = nfmem.addNF(currentprocess);
		if (result == 0){
			std::cout << "time "<< time << "ms: Cannot place process " << currentprocess.getName() << " -- skipped!\n";
		}
		else if (result == 1){
			std::cout << "time "<< time << "ms: Placed process " << currentprocess.getName() << ":\n";
			nfmem.printMem();
		}
		else if (result == 2){
			std::cout << "time "<< time << "ms: Cannot place process " << currentprocess.getName() << " -- starting defragmentation\n";
			time += nfmem.defrag(time);
			nfmem.printMem();
			std::cout << "time " << time << "ms: Placed process " << currentprocess.getName() << ": \n"x`;
			nfmem.addNF(currentprocess);
			nfmem.printMem();
		}


	}
	std::cout << "time " << time << "ms: Simulator ended (Contiguous -- Next-Fit)\n";

	//teststuff();
	return 1;
}