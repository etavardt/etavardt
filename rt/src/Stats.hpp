#pragma once

//#include Bob;
class Bob;

class Stats {
private:
    static Bob &bob;

    Stats(){}
public:
    static unsigned long memAllocated;
    static void trackMemoryUsage(unsigned long memSize) { memAllocated += memSize; }
    static void statistics(int line);
    static void yystats(void);
};
