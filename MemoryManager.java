import java.util.*;

public class MemoryManager {
    
    int memorySize;
    int allocatedMemory;
    int numProcesses;
    int numWastedBlocks;
    int numFragments;
    LinkedList<Integer> freeList;
    HashMap<Integer, Integer> processMap;
    
    public MemoryManager(int memorySize) {
        this.memorySize = memorySize;
        allocatedMemory = 0;
        numProcesses = 0;
        numWastedBlocks = 0;
        numFragments = 0;
        freeList = new LinkedList<>();
        freeList.add(0);
        processMap = new HashMap<>();
    }
    
    public boolean allocateMemory(int processId, int processSize) {
        if (processSize > memorySize - allocatedMemory) {
            return false;
        }
        
        ListIterator<Integer> iterator = freeList.listIterator();
        while (iterator.hasNext()) {
            int startIndex = iterator.next();
            int endIndex = startIndex + processSize - 1;
            if (endIndex >= memorySize) {
                break;
            }
            boolean isFree = true;
            for (int i = startIndex; i <= endIndex; i++) {
                if (processMap.containsKey(i)) {
                    isFree = false;
                    break;
                }
            }
            if (isFree) {
                iterator.remove();
                for (int i = startIndex; i <= endIndex; i++) {
                    processMap.put(i, processId);
                }
                allocatedMemory += processSize;
                numProcesses++;
                return true;
            }
        }
        
        numWastedBlocks++;
        return false;
    }
    
    public void deallocateMemory(int processId) {
        Set<Map.Entry<Integer, Integer>> entrySet = processMap.entrySet();
        Iterator<Map.Entry<Integer, Integer>> iterator = entrySet.iterator();
        while (iterator.hasNext()) {
            Map.Entry<Integer, Integer> entry = iterator.next();
            if (entry.getValue() == processId) {
                iterator.remove();
                freeList.add(entry.getKey());
            }
        }
        numProcesses--;
        allocatedMemory -= processMap.size();
    }
    
    public void runSimulation(int numTimeUnits) {
        for (int i = 0; i < numTimeUnits; i++) {
            numFragments += freeList.size();
            System.out.println("Time Unit " + i + ":");
            System.out.println("No. of Processes: " + numProcesses);
            System.out.println("No. of Wasted Blocks: " + numWastedBlocks);
            System.out.println("No. of Fragments: " + numFragments);
            System.out.println("Allocated Memory: " + allocatedMemory + "/" + memorySize);
        }
    }
    
    public static void main(String[] args) {
        MemoryManager memoryManager = new MemoryManager(100);
        memoryManager.allocateMemory(1, 20);
        memoryManager.allocateMemory(2, 30);
        memoryManager.deallocateMemory(1);
        memoryManager.allocateMemory(3, 10);
        memoryManager.runSimulation(5);
    }
}