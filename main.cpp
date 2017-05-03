#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <exception>
#include "memory.h"

#include <string>
#include <cstdlib>
#include <functional> 
#include <cctype>
#include <locale>

std::string trim (std::string str){
	int pos=0;
	while (str[pos] == ' '){
		pos++;
	}

	str=str.substr(pos);

	pos = str.length();
	while (str[pos] == ' '){
		pos--;
	}

	str=str.substr(0, pos);

	return str;
}

int readfile(char* file, std::vector<process> processes){

	std::string line;

	std::ifstream ipfile (file);

	if (ipfile.is_open())
	{
		int numProc=-1;

		while (getline (ipfile,line) && numProc!=0){
			//remove whitespace from ends
			line = trim(line);

			// skip comment lines
			if (line[0]=='#'){
				continue;
			}

			// first line is #of processes
			if (numProc==-1){
				numProc = atoi((const char*) &line[0]);
			}

			// read in a single process with all arrivials and departures
			processes.push_back(process(line));

			numProc--;			
		}
			

		ipfile.close();
	}
	else{
		perror("ERROR could not open input file '%s' for reading\n");
		exit(EXIT_FAILURE);
	}

}


void add(memory &nfmem, process &p){
	int result = nfmem.addNC(p);
	int time = 0;
	if (result == 0){
		std::cout << "time "<< time << "ms: Cannot place process " << p.getName() << " -- skipped!\n";
	}
	else if (result == 1){
		std::cout << "time "<< time << "ms: Placed process " << p.getName() << ":\n";
		nfmem.printMem();
	}
	else if (result == 2){
		std::cout << "time "<< time << "ms: Cannot place process " << p.getName() << " -- starting defragmentation\n";
		time += nfmem.defrag(time);
		nfmem.printMem();
		std::cout << "time " << time << "ms: Placed process " << p.getName() << ": \n";
		nfmem.addBF(p);
		nfmem.printMem();
	}
}

void teststuff(){
	//this is all just testing stuff
	memory mem(8, 32);
	std::cout << "done\n";
	process A('A', 45);
	process B('B', 28);
	process C('C', 58);
	process D('D', 86);
	process E('E', 14);
	process F('F', 24);
	process G('G', 13);
	process J('J', 46);


	//all the commands from input 1, just not in input form

	add(mem, A);
	add(mem, B);
	add(mem, C);
	add(mem, D);
	add(mem, E);
	add(mem, F);
	mem.remove(A);
	add(mem, A);
	add(mem, G);
	mem.remove(A);
	mem.remove(F);
	add(mem, F);
	add(mem, J);
	mem.remove(D);
	mem.remove(C);
	mem.remove(F);
	add(mem, C);
	mem.remove(C);
	add(mem, D);
	mem.remove(E);
	mem.remove(J);
	mem.remove(D);
	mem.remove(B);


}
int main(int argc, char** argv){
// 	if (argc < 2){
// 		std::cout << "Usage: ./a.out <file>\n";
// 		return EXIT_FAILURE;
// 	}
//   	std::ifstream inFile;
//   	inFile.open(argv[1]);
//   	if (!inFile.good()) {
//     	std::cerr<<"ERROR: BAD FILE"<<std::endl;
// 		return EXIT_FAILURE;
// 	}

// 	int numprocesses;
// 	char line[128];
// 	inFile.getline(line, 128);

// 	std::cout << "\n\n" << line << "\n\n";
// 	numprocesses = atoi(line);
// //		std::cout << numprocesses;

// 	while (inFile){
// 		inFile.getline(line, 128);
// 		char delim = ' ';
// 		char name = line[0];
// 		int size, start ,end;

// 		process P(name, size);
// 		std::cout << line << "\n";
// 	}

	if(argc < 2) {
		perror("ERROR Invalid arguments\n");
		exit(EXIT_FAILURE);
	}
	int time = 0;
	std::vector<process> processes;

	// take the args and read the file into our vector of proccesses
	std::string line;
	std::ifstream ipfile (argv[1]);

	if (ipfile.is_open())
	{
		int numProc=-1;

		while (getline (ipfile,line) && numProc!=0){
			//remove whitespace from ends
			line = trim(line);

			// skip comment lines
			if (line[0]=='#'){
				continue;
			}

			// first line is #of processes
			if (numProc==-1){
				numProc = atoi((const char*) &line[0]);
				continue;
			}

			// read in a single process with all arrivials and departures
			processes.push_back(process(line));

			numProc--;			
		}
			

		ipfile.close();
	}
	else{
		perror("ERROR could not open input file '%s' for reading\n");
		exit(EXIT_FAILURE);
	}
	// end ip reading

	printf("TEST: GOT %d proccesses!\n", processes.size());

	for (int i=0; i<processes.size(); i++){
		printf("TEST: process %d = %c\n", i, processes[i].getName());
	}

	// //queue or something to do processes
	// //map of maps?? map<int time, map<process, string action> >
	// //action says remove or add
	// //we'll see
	// //or we add arrival times to process class and iterate through time idk

	// //read in number of processes from file
	// int numprocesses = 6;
	// memory nfmem(8, 32);

	// //tbh gonna copy and paste this 4 times once it's correct
	// std::cout << "time " << time << "ms: Simulator started (Contiguous -- Next-Fit)\n";
	// for (int i = 0; i < numprocesses; i++){//not actually the right loop conditions but im tired
	// 	process Test('T', 10);
	// 	process currentprocess = Test;
	// 	std::cout << "time " << time << "ms: Process " << currentprocess.getName() << " arrived (requires " << currentprocess.getSize() << " frames)\n";
	// 	int result = nfmem.addNF(currentprocess);
	// 	if (result == 0){
	// 		std::cout << "time "<< time << "ms: Cannot place process " << currentprocess.getName() << " -- skipped!\n";
	// 	}
	// 	else if (result == 1){
	// 		std::cout << "time "<< time << "ms: Placed process " << currentprocess.getName() << ":\n";
	// 		nfmem.printMem();
	// 	}
	// 	else if (result == 2){
	// 		std::cout << "time "<< time << "ms: Cannot place process " << currentprocess.getName() << " -- starting defragmentation\n";
	// 		time += nfmem.defrag(time);
	// 		nfmem.printMem();
	// 		std::cout << "time " << time << "ms: Placed process " << currentprocess.getName() << ": \n";
	// 		nfmem.addNF(currentprocess);
	// 		nfmem.printMem();
	// 	}


	// }
	std::cout << "time " << time << "ms: Simulator ended (Contiguous -- Next-Fit)\n";

	teststuff();
	return 1;
}