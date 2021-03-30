#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <fstream>
#include "Process.h"
#include "Processor.h"
using namespace P;
using namespace std;
int main() {
	
	//ask for n, scheduling method and quantum if applicable
	cout << "Enter the amount of processes\n";
	int n = 0;
	cin >> n;
	cout << "Enter the scheduling method 1 for fcfs, 2 for RR\n";
	int SA = 0;
	cin >> SA;
	int quantum = 0;

	vector<int> processes;
	map<int, Process> processMap;
	if (SA == 2) {
		cout << "Enter the quantum\n";
		cin >> quantum;
		Process::pquantum = quantum;
		
	}
	else {
		Process::pquantum = -1;
	}
	for (int i = 1; i < n + 1; i++) {
		Process temp = Process(0, "ready", i);
		processMap.insert({ i,temp });
	}
	
	//ask for the Rs and Bs	
	cout << "Now enter R and B in order of :R1.1,B1.1,R1.2,B1.2.... 4 for each Pi \n";
	while(true) {
		cout << "Enter an R,enter -1 to stop\n";
		int temp = 0;
		cin >> temp;
		if (temp == -1) {
			break;
		}
		processes.push_back(temp);
		cout << "Enter an B, enter -1 to stop\n";
		cin >> temp;
		processes.push_back(temp);
		if (temp == -1) {
			break;
		}
	}
	

	if (SA == 1) {
		processor(n, processes, processMap,-1);
	}
	else {
		processor(n, processes, processMap, quantum);
	}
	cout << "Please ignore messages regarding system('pause')\n";
	//system("pause");
	
	return 0;
}