//
// Created by freddy on 08/05/19.
//

#include <sys/resource.h>
#include <stdio.h>
#include <iostream>
#include "MemoryManager.h"

void MemoryManager::increase_stack_size() {
    struct rlimit rl;
    int result;
    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < REQUIRED_STACK_SIZE)
        {
            rl.rlim_cur = REQUIRED_STACK_SIZE;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                std::cerr << "ERROR: Cannot increase STACK size" << std::endl;
            }
        }
    }
}

long MemoryManager::get_stack_size() {
    struct rlimit rl;
    int result;
    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0) {
        return rl.rlim_cur;
    }

    return -1;
}
