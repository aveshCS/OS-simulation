#include <iostream>
#include <list>
#include <unordered_map>

class MemoryManager {
    int memorySize;
    int allocatedMemory;
    int numProcesses;
    int numWastedBlocks;
    int numFragments;
    std::list<int> freeList;
    std::unordered_map<int, int> processMap;
    
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
        
        auto it = freeList.begin();
        while (it != freeList.end()) {
            int startIndex = *it;
            int endIndex = startIndex + processSize - 1;
            if (endIndex >= memorySize) {
                break;
            }
            bool isFree = true;
            for (int i = startIndex; i <= endIndex; i++) {
                if (processMap.count(i)) {
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
        auto it = processMap.begin();
        while (it != processMap.end()) {
            if (it->second == processId) {
                int key = it->first;
                it = processMap.erase(it);
                freeList.push_back(key);
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
            std::cout << "Time Unit " << i << ":" << std::endl;
            std::cout << "No. of Processes: " << numProcesses << std::endl;
            std::cout << "No. of Wasted Blocks: " << numWastedBlocks << std::endl;
            std::cout << "No. of Fragments: " << numFragments << std::endl;
            std::cout << "Allocated Memory: " << allocatedMemory << "/" << memorySize << std::endl;
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
