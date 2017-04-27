#include "memory.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>

#define t_memmove 1

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

//next fit
int memory::addNF(process p){
	//RETURN VALUES: 0 - CAN'T ADD, 1 - ADDED, 2 - DEFRAG NEEDED
	//assume process has size and name
	char toAdd = p.getName();
	int size = p.getSize();
	if (size > openFrames){
		//move print statement to main 
		std::cout << "Cannot place process " << toAdd << " -- skipped!";
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
				//move print statement to main
				std::cout << "Cannot place process " << toAdd << " -- starting defragmentation\n";
				return 2;
			}
		}
	}
}

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
		while (flat[start] == '.'){
			flat[start] = toAdd;
			start++;
			remaining--;
		}
	}
	updateDisplay();
	return 1;
}

void memory::remove(process p){
	//could be better
	char toRemove = p.getName();
	int start = 0;
	while (flat[start] != toRemove){
		start++;
		if (start == totalFrames){
			std::cout << "process not found\n";
			return;
		}
	}
	while (flat[start] == toRemove){
		flat[start] = '.';
		start++;
		openFrames++;
	}
	updateDisplay();
}

int memory::defrag(){
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
	std::cout << "Defragmentation complete (moved " << timetaken << " frames:";
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
	int k = 0;
	for (int i = 0; i < lines; i++){
		for (int j = 0; j < frames; j++){
			display[i][j] = flat[k];
			k++;
		}
	}
}