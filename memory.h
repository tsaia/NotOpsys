#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <list>

class process{
public:
	process();
	process(char n, int s): size(s), name(n){};
	process(std::string);

	int getSize(){return size;}
	int getTimesSize(){return arrivalTimes.size();}
	char getName(){return name;}
	int arrivals(){return arrivalTimes.size();}
	int departures(){return departureTimes.size();}
	int getArrival(int i) {return arrivalTimes[i];}
	int getDeparture(int i) {return departureTimes[i];}
	int getCurDep(){return currentDepart;}
	int lastD(){return departureTimes[departureTimes.size()-1];}
	void setDepart(int i){currentDepart = i;}
	void departCount(){currentDepart--;}
	void removeArrival();
	void removeDeparture();
	void update(int time);


private:
	int size;
	char name;
	int currentDepart;
	std::vector<int> arrivalTimes;
	std::vector<int> departureTimes;
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

		queue.push_back('.');
	};

	void printMem();
	int defrag(int time);
	void test();
	void updateDisplay();
	int remove(process p);
	int addNF2(process p);
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

	//add a queueueue for most recent memory
	std::list<char> queue;
	//remind me to actually clear the queue sometime lol

	
};
