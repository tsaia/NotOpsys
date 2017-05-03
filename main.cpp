
/*
  Tim Saia        661258600   saiat@rpi.edu
  Sean Waclawik   661414058   waclas@rpi.edu
  Nate Wheeler    661486023   wheeln@rpi.edu
*/

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
	return 0;
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

void updateTimes(std::vector<process> &processes, int add){
	for (unsigned int i = 0; i <processes.size(); i++){
		processes[i].update(add);
	}
}

void nf(std::vector<process> processes){
	std::cout << "time 0ms: Simulator started (Contiguous -- Next-Fit)\n";
	memory mem(8,32);
	int time = 0, result = 0;
	process current('.', 0);
	int lastDepart = 0;
	for (unsigned int j = 0; j < processes.size(); j++){
		lastDepart = std::max(lastDepart, processes[j].lastD());
	}
	while (time < lastDepart+1){//temp for now
		for (unsigned int i = 0; i < processes.size(); i++){
			current = processes[i];
		//		std::cout << "TIME: " << time << "\tPROCESS: " << current.getName() << "\tARRIVAL: "<< current.getArrival(0) << "\n";
			for (int q = 0; q < current.arrivals(); q++){
				if (current.getArrival(q) == time){
					std::cout << "time " << time << "ms: Process " << current.getName() << " arrived (requires "
						<< current.getSize() << " frames)\n";
					result = mem.addNF(current);
					if (result == 0){
						std::cout << "time "<< time << "ms: Cannot place process " << current.getName() << " -- skipped!\n";
					}
					else if (result == 1){
						std::cout << "time "<< time << "ms: Placed process " << current.getName() << ":\n";
						mem.printMem();
					}
					else if (result == 2){
						std::cout << "time "<< time << "ms: Cannot place process " << current.getName() << " -- starting defragmentation\n";
						int def = mem.defrag(time);//prints the defrag statement here
						time += def;
						mem.printMem();
						std::cout << "time " << time << "ms: Placed process " << current.getName() << ":\n";
					//	time+=def;
						mem.addNF(current);
						mem.printMem();
						lastDepart += def;

						updateTimes(processes, def);
					}
					current.removeArrival();
				}
			}
			for (int q = 0; q < current.departures(); q++){
				if (current.getDeparture(q) == time){
					int removed = mem.remove(current);
					current.removeDeparture();
					if (removed > 0){
						std::cout << "time " << time << "ms: Process " << current.getName() << " removed:\n";
						mem.printMem();
					}
				}
			}
		}
	time++;
	}
	std::cout << "time " << time-1 << "ms: Simulator ended (Contiguous -- Next-Fit)\n\n";
}

void bf(std::vector<process> processes){

	std::cout << "time 0ms: Simulator started (Contiguous -- Best-Fit)\n";
	memory mem(8,32);
	int time = 0, result = 0;
	process current('.', 0);
	int lastDepart = 0;
	for (unsigned int j = 0; j < processes.size(); j++){
		lastDepart = std::max(lastDepart, processes[j].lastD());
	}
	while (time < lastDepart+1){//temp for now
		for (unsigned int i = 0; i < processes.size(); i++){
			current = processes[i];
		//		std::cout << "TIME: " << time << "\tPROCESS: " << current.getName() << "\tARRIVAL: "<< current.getArrival(0) << "\n";
			for (int q = 0; q < current.arrivals(); q++){
				if (current.getArrival(q) == time){
					std::cout << "time " << time << "ms: Process " << current.getName() << " arrived (requires "
						<< current.getSize() << " frames)\n";
					result = mem.addBF(current);
					if (result == 0){
						std::cout << "time "<< time << "ms: Cannot place process " << current.getName() << " -- skipped!\n";
					}
					else if (result == 1){
						std::cout << "time "<< time << "ms: Placed process " << current.getName() << ":\n";
						mem.printMem();
					}
					else if (result == 2){
						std::cout << "time "<< time << "ms: Cannot place process " << current.getName() << " -- starting defragmentation\n";
						int def = mem.defrag(time);//prints the defrag statement here
						time += def;
						mem.printMem();
						std::cout << "time " << time << "ms: Placed process " << current.getName() << ":\n";
					//	time+=def;
						mem.addBF(current);
						mem.printMem();

						lastDepart += def;

						updateTimes(processes, def);
					}
					current.removeArrival();
				}
			}
			for (int q = 0; q < current.departures(); q++){
				if (current.getDeparture(q) == time){
					int removed = mem.remove(current);
					current.removeDeparture();
					if (removed > 0){//lazy fix
						std::cout << "time " << time << "ms: Process " << current.getName() << " removed:\n";
						mem.printMem();
					}
				}
			}
		}
	time++;
	}
	std::cout << "time " << time-1 << "ms: Simulator ended (Contiguous -- Best-Fit)\n\n";
}


void wf(std::vector<process> processes){

	std::cout << "time 0ms: Simulator started (Contiguous -- Worst-Fit)\n";
	memory mem(8,32);
	int time = 0, result = 0;
	process current('.', 0);
	int lastDepart = 0;
	for (unsigned int j = 0; j < processes.size(); j++){
		lastDepart = std::max(lastDepart, processes[j].lastD());
	}
	while (time < lastDepart+1){//temp for now
		for (unsigned int i = 0; i < processes.size(); i++){
			current = processes[i];
		//		std::cout << "TIME: " << time << "\tPROCESS: " << current.getName() << "\tARRIVAL: "<< current.getArrival(0) << "\n";
			for (int q = 0; q < current.arrivals(); q++){
				if (current.getArrival(q) == time){
					std::cout << "time " << time << "ms: Process " << current.getName() << " arrived (requires "
						<< current.getSize() << " frames)\n";
					result = mem.addWF(current);
					if (result == 0){
						std::cout << "time "<< time << "ms: Cannot place process " << current.getName() << " -- skipped!\n";
					//	current.removeDeparture();
					}
					else if (result == 1){
						std::cout << "time "<< time << "ms: Placed process " << current.getName() << ":\n";
						mem.printMem();
					}
					else if (result == 2){
						std::cout << "time "<< time << "ms: Cannot place process " << current.getName() << " -- starting defragmentation\n";
						int def = mem.defrag(time);//prints the defrag statement here
						time += def;
						mem.printMem();
						std::cout << "time " << time << "ms: Placed process " << current.getName() << ":\n";
					//	time+=def;
						mem.addWF(current);
						mem.printMem();

						lastDepart += def;

						updateTimes(processes, def);
					}
					current.removeArrival();
				}
			}
			for (int q = 0; q < current.departures(); q++){
				if (current.getDeparture(q) == time){
					int removed = mem.remove(current);
					current.removeDeparture();
					if (removed > 0){
						std::cout << "time " << time << "ms: Process " << current.getName() << " removed:\n";
						mem.printMem();
					}
				}
			}
		}
	time++;
	}

	std::cout << "time " << time-1 << "ms: Simulator ended (Contiguous -- Worst-Fit)\n\n";
}



void nc(std::vector<process> processes){

	std::cout << "time 0ms: Simulator started (Non-contiguous)\n";
	memory mem(8,32);
	int time = 0, result = 0;
	process current('.', 0);
	int lastDepart = 0;
	for (unsigned int j = 0; j < processes.size(); j++){
		lastDepart = std::max(lastDepart, processes[j].lastD());
	}
	while (time < lastDepart+1){//temp for now
		for (unsigned int i = 0; i < processes.size(); i++){
			current = processes[i];
		//		std::cout << "TIME: " << time << "\tPROCESS: " << current.getName() << "\tARRIVAL: "<< current.getArrival(0) << "\n";
			for (int q = 0; q < current.arrivals(); q++){
				if (current.getArrival(q) == time){
					std::cout << "time " << time << "ms: Process " << current.getName() << " arrived (requires "
						<< current.getSize() << " frames)\n";
					result = mem.addNC(current);
					if (result == 0){
						std::cout << "time "<< time << "ms: Cannot place process " << current.getName() << " -- skipped!\n";
					//	current.removeDeparture();
					}
					else if (result == 1){
						std::cout << "time "<< time << "ms: Placed process " << current.getName() << ":\n";
						mem.printMem();
					}
					current.removeArrival();
				}
			}
			for (int q = 0; q < current.departures(); q++){
				if (current.getDeparture(q) == time){
					int removed = mem.remove(current);
					current.removeDeparture();
					if (removed > 0){
						std::cout << "time " << time << "ms: Process " << current.getName() << " removed:\n";
						mem.printMem();
					}
				}
			}
		}
	time++;
	}

	std::cout << "time " << time-1 << "ms: Simulator ended (Non-contiguous)\n";
}

int main(int argc, char** argv){

	if(argc < 2) {
		perror("ERROR Invalid arguments\n");
		exit(EXIT_FAILURE);
	}
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

	// printf("TEST: GOT %d proccesses!\n", processes.size());

	// for (int i=0; i<processes.size(); i++){
	// 	printf("TEST: process %d = %c, size=%d\n", i, processes[i].getName(), processes[i].getSize());

	// 	for (int j=0; j<processes[i].getTimesSize(); j++){
	// 		printf("\t%d arrival:'%d' departure:'%d'\n", j, processes[i].getArrival(j), processes[i].getDeparture(j));
	// 	}

	// }
	fflush(stdout);
	fflush (stderr);
	


	nf(processes);
	bf(processes);
	wf(processes);
	nc(processes);

	//teststuff();
	return 1;
}