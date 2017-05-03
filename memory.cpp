#include "memory.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>

#define t_memmove 1

process::process(std::string string){
	//the input string is in the format: 'A 45 0/350 400/50'...
	int pos;
	std::string delim = " ";
	std::string delim2 = "/";

	// get the label/name char
	
	pos = string.find(delim);
	std::string strname = string.substr(0, pos);
	name=strname[0];
	string.erase(0, pos+delim.length());
	

	// get the size
	pos = string.find(delim);
	std::string strsize = string.substr(0, pos);
	std::string::size_type sz;  
  	size = atoi(strsize.c_str());
	string.erase(0, pos+delim.length());


	// get arrivial and departure times
	while ((pos = string.find(delim)) != (int)std::string::npos) {
    	std::string token = string.substr(0, pos);
    	// breakup arrival and departure times
    	int pos2 = token.find(delim2);
    	std::string strArrival = token.substr(0, pos2);
    	std::string strDeparture = token.substr(pos2+delim2.length(), token.length());
    	string.erase(0, pos + delim.length());

    	arrivalTimes.push_back(atoi(strArrival.c_str()));
    	departureTimes.push_back(atoi(strDeparture.c_str()));
	}

	//get the last one

	// breakup arrival and departure times
	int pos2 = string.find(delim2);
	std::string strArrival = string.substr(0, pos2);
	std::string strDeparture = string.substr(pos2+delim2.length(), string.length());
	////string.erase(0, pos + delim.length());

	arrivalTimes.push_back(atoi(strArrival.c_str()));
	departureTimes.push_back(atoi(strDeparture.c_str()));
}


void memory::test(){//testing function until inputs are handled
	flat[6] = 'A';
	flat[7] = 'A';
	flat[8] = 'A';
	flat[9] = 'A';
	flat[10] = 'A';
	flat[11] = 'A';

	flat[23] = 'B';
	flat[24] = 'B';
	flat[25] = 'B';
	flat[26] = 'B';

	flat[27] = 'C';
	flat[28] = 'C';

	flat[251] = 'D';
	flat[252] = 'D';
	flat[253]= 'D';

	flat[151] = 'F';
	flat[152] = 'F';
	flat[153]= 'F';
	flat[154] = 'F';
	flat[155] = 'F';
	flat[156]= 'F';

	flat[161] = 'H';
	flat[162] = 'H';
	flat[163]= 'H';
	updateDisplay();
}


void memory::printMem(){
	//border
	int e;
	for (e = 0; e < frames; e++){
		std::cout << "=";
	}
	std::cout << std::endl;

	//actual data
	for (int i = 0; i < lines; i++){
		for (int j = 0; j < frames; j++){
			std::cout << display[i][j];
		}
		std::cout << std::endl;
	}

	//border
	for (e = 0; e < frames; e++){
		std::cout << "=";
	}
	std::cout << std::endl;
}



//copied and pasted the old method for a specific case rather than fixing a bug :^}
int memory::addNF2(process p){
	//RETURN VALUES: 0 - CAN'T ADD, 1 - ADDED, 2 - DEFRAG NEEDED
	//assume process has size and name
	char toAdd = p.getName();
	int size = p.getSize();
	if (size > openFrames){
		return 0;
	}
	int start = 0, test = 0;
	while (true){//I guess it could be while (start < totalFrames) but eh
		while (flat[start]!='.'){
			start++;
		}
		bool canAdd = true;
		while (test < size){
			if (test+start >= totalFrames || flat[start+test] != '.'){
				canAdd = false;
				break;
			}	
			test++;
		}
		if (canAdd){
			test = 0;
			while (test < size){
				flat[start+test] = toAdd;
				test++;
				openFrames--;
			}
			updateDisplay();
			return 1;
		}else{
			start++;
			if (start == totalFrames){
				return 0;
			}
		}
	}
}

//next fit
int memory::addNF(process p){
	//RETURN VALUES: 0 - CAN'T ADD, 1 - ADDED, 2 - DEFRAG NEEDED
	//assume process has size and name
	char prev = queue.back();
	char toAdd = p.getName();
	int size = p.getSize();
	if (size > openFrames){
		return 0;
	}
	int start = 0, test = 0;
	while (true){//I guess it could be while (start < totalFrames) but eh
		if(prev != '.'){
			while (start < totalFrames && flat[start]!=prev){
				start++;
			}
			while (start < totalFrames && flat[start]==prev){
				start++;				
			}
		}
		if (start == totalFrames){
			queue.push_back('.');
			std::cout << "here\n\n";
			if (addNF2(p) == 1){
				return 1;
			}
			return 2;
		}
		bool canAdd = true;
		while (test < size){
			if (test+start >= totalFrames || flat[start+test] != '.'){
				canAdd = false;
				break;
			}	
			test++;
		}
		if (canAdd){
			test = 0;
			while (test < size){
				flat[start+test] = toAdd;
				test++;
				openFrames--;
			}

			queue.push_back(toAdd);
			updateDisplay();
			return 1;
		}else{
			start++;
			if (start == totalFrames){
				queue.push_back('.');
				std::cout << "here\n\n";
				if (addNF2(p) == 1){
					return 1;
				}
				return 2;
			}
		}
	}
}

//best fit
int memory::addBF(process p){
	//RETURN VALUES: 0 - CAN'T ADD, 1 - ADDED, 2 - DEFRAG NEEDED
	//assume process has size and name
	char toAdd = p.getName();
	int size = p.getSize();
	if (size > openFrames){
		return 0;
	}
	int start = 0, test = 0, bestfit = totalFrames+1, beststart = 0;
	bool added = false;
	while (start < totalFrames){
		while (start < totalFrames && flat[start] != '.'){
			start++;
		}
		if (start == totalFrames){
			//double check whoops
			break;
		}
		//beststart = start;
		test = start;
		int fit = 0;
		while (flat[test] == '.'){
			test++;
			fit++;
		}
		if (fit >= size){
			added = true;
			if (fit != bestfit){
				bestfit = std::min(bestfit, fit);
				if (bestfit == fit){
					beststart = start;
				}
			}
		}

		if (test == totalFrames){
			//if the current gap goes all the way to the end, 
			//don't count all the smaller gaps within that
			break;	
		}
		start = test;
		//move to where the current gap ends
	}
	if (added){//can add
		for (int i = beststart; i < size+ beststart; i++){
			flat[i] = toAdd;
			openFrames--;
		}
		updateDisplay();
		return 1;
	}
	else{//need to defrag
		return 2;
	}


}

//worst fit
int memory::addWF(process p){
	//copied from bestfit but max instead of min
	//RETURN VALUES: 0 - CAN'T ADD, 1 - ADDED, 2 - DEFRAG NEEDED
	//assume process has size and name
	char toAdd = p.getName();
	int size = p.getSize();
	if (size > openFrames){
		return 0;
	}
	int start = 0, test = 0, worstfit = 0, worststart = 0;
	bool added = false;
	while (start < totalFrames){
		while (start < totalFrames && flat[start] != '.'){
			start++;
		}
		if (start == totalFrames){
			//double check whoops
			break;
		}
		//beststart = start;
		test = start;
		int fit = 0;
		while (flat[test] == '.'){
			test++;
			fit++;
		}
		if (fit >= size){
			added = true;
			if (fit != worstfit){
				worstfit = std::max(worstfit, fit);
				if (worstfit == fit){
					worststart = start;
				}
			}
		}

		if (test == totalFrames){
			//if the current gap goes all the way to the end, 
			//don't count all the smaller gaps within that
			break;	
		}
		start = test;
		//move to where the current gap ends
	}
	if (added){//can add
		for (int i = worststart; i < size+ worststart; i++){
			flat[i] = toAdd;
			openFrames--;
		}
		updateDisplay();
		return 1;
	}
	else{//need to defrag
		return 2;
	}
}

//noncontiguous
int memory::addNC(process p){
	char toAdd = p.getName();
	int size = p.getSize();
	if (openFrames < size){
		return 0;
	}
	int remaining = size, start = 0;
	while (remaining > 0){
		while (flat[start] != '.'){
			start++;
		}
		while (flat[start] == '.' && remaining > 0){
			flat[start] = toAdd;
			start++;
			remaining--;
			openFrames--;
		}
	}
	updateDisplay();
	return 1;
}

int memory::remove(process p){
	//could be better

	char toRemove = p.getName();
	int removed = 0;
	for (int i = 0; i < totalFrames; i++){
		if (flat[i] == toRemove){
			flat[i] = '.';
			removed++;
			openFrames++;
		}
	}
	updateDisplay();
	std::cout << "Removed process " << p.getName() << "\n";
	printMem();
	//top of the queue always needs to be a period but want to eliminate extra periods
	//added 
	queue.remove(toRemove);
	queue.remove('.');
	queue.push_front('.');
	return removed;
}

int memory::defrag(int time){
	int start = 0, replace, replaceuntil, replacelength, timetaken = 0;
	//add a list of moved processes
	std::vector<char> moved;
	while (start < totalFrames){
		while (flat[start] != '.' && start < totalFrames){
			start++;
		}
	//add a way to check for wrapping around im lazy rn tho
		//wait maybe we dont actually have to im dumb
		replace = start;
		while (flat[replace] == '.' && replace < totalFrames){
			replace++;
		}
		if (replace >= totalFrames){
			break;
			//for when the last frames are dots
		}
		char replaceChar = flat[replace]; 
		moved.push_back(replaceChar);
		replaceuntil = replace;
		while (flat[replaceuntil] == flat[replace]){
			replaceuntil++;
		}
		replacelength = replaceuntil - replace;
		for (int i = 0; i < replacelength; i++){
			flat[start] = replaceChar;
			start++;
			flat[replace] = '.';
			replace++;
			timetaken+=t_memmove;
		}
	}
	//print here instead of in main to get those sick moved frames
	std::cout << "time " << time+timetaken << "ms: Defragmentation complete (moved " << timetaken << " frames:";
	for (int i = 0; i < moved.size(); i++){
		std::cout << " " << moved[i];
		if (i != moved.size() - 1){//formatting is dumb 
			std::cout << ",";
		}
	}
	std::cout << ")	\n";
	updateDisplay();	
	return timetaken;
}

void memory::updateDisplay(){
	//update the display with the contents of the flat memory
	int k = 0;
	for (int i = 0; i < lines; i++){
		for (int j = 0; j < frames; j++){
			display[i][j] = flat[k];
			k++;
		}
	}
}