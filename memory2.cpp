#include <iostream>
#include <list>
#include <map>
//Firstly create a file in nano editor with the name memory_manager.cpp
// nano memory_manager.cpp
//Command to create an executable
//g++ memory_manager.cpp -o memory_manager
//Run the command to create an executable of name memory_manger
//To run the executable type this command
// ./memory_manager 

using namespace std;

class MemoryManager {
    int memorySize;
    int allocatedMemory;
    int numProcesses;
    int numWastedBlocks;
    int numFragments;
    list<int> freeList;
    map<int, int> processMap;

public:
    MemoryManager(int memorySize) {
        this->memorySize = memorySize;
        allocatedMemory = 0;
        numProcesses = 0;
        numWastedBlocks = 0;
        numFragments = 0;
        freeList.push_back(0);
    }

    bool allocateMemory(int processId, int processSize) {
        if (processSize > memorySize - allocatedMemory) {
            return false;
        }

        list<int>::iterator it = freeList.begin();
        while (it != freeList.end()) {
            int startIndex = *it;
            int endIndex = startIndex + processSize - 1;
            if (endIndex >= memorySize) {
                break;
            }
            bool isFree = true;
            for (int i = startIndex; i <= endIndex; i++) {
                if (processMap.count(i) > 0) {
                    isFree = false;
                    break;
                }
            }
            if (isFree) {
                it = freeList.erase(it);
                for (int i = startIndex; i <= endIndex; i++) {
                    processMap[i] = processId;
                }
                allocatedMemory += processSize;
                numProcesses++;
                return true;
            } else {
                ++it;
            }
        }

        numWastedBlocks++;
        return false;
    }

    void deallocateMemory(int processId) {
        map<int, int>::iterator it = processMap.begin();
        while (it != processMap.end()) {
            if (it->second == processId) {
                it = processMap.erase(it);
                freeList.push_back(it->first);
            } else {
                ++it;
            }
        }
        numProcesses--;
        allocatedMemory -= processMap.size();
    }

    void runSimulation(int numTimeUnits) {
        for (int i = 0; i < numTimeUnits; i++) {
            numFragments += freeList.size();
            cout << "Time Unit " << i << ":" << endl;
            cout << "No. of Processes: " << numProcesses << endl;
            cout << "No. of Wasted Blocks: " << numWastedBlocks << endl;
            cout << "No. of Fragments: " << numFragments << endl;
            cout << "Allocated Memory: " << allocatedMemory << "/" << memorySize << endl;
        }
    }
};

int main() {
    MemoryManager memoryManager(100);
    memoryManager.allocateMemory(1, 20);
    memoryManager.allocateMemory(2, 30);
    memoryManager.deallocateMemory(1);
    memoryManager.allocateMemory(3, 10);
    memoryManager.runSimulation(5);
    return 0;
}
