#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

class process{
public:
	process();
	process(char n, int s): size(s), name(n){};
	int getSize(){return size;}
	char getName(){return name;}

private:
	int size;
	char name;
};

class memory
{
public:
	memory();
	memory(int l, int f): lines(l), frames(f){
		std::vector<char> v(frames, '.');
		for (int i = 0; i < lines; i++){
			display.push_back(v);
		}
		std::vector<char> v2(l*f, '.');
		flat = v2;
		openFrames = l*f;
		totalFrames = l*f;
	};

	void printMem();
	int defrag(int time);
	void test();
	void updateDisplay();
	int remove(process p);
	int addNF(process p);
	int addBF(process p);
	int addWF(process p);
	int addNC(process p);

private:
	int frames;
	int lines;
	int totalFrames;

	//flat is a 1d vector that's easier to work with for actually manipulating the data
	//display is the 2d one that's printed each time
	std::vector<char> flat;
	std::vector<std::vector<char> > display;
	int openFrames;


	
};
