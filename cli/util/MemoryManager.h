//
// Created by freddy on 08/05/19.
//

#ifndef OSARRACINO_MEMORYMANAGER_H
#define OSARRACINO_MEMORYMANAGER_H

const long REQUIRED_STACK_SIZE = 64L * 1024L * 1024L;

class MemoryManager {
public:
    static void increase_stack_size();
    static long get_stack_size();
};


#endif //OSARRACINO_MEMORYMANAGER_H
