#pragma once

class Stats {
private:
    Stats(){}
public:
    static unsigned long memAllocated;
    static void trackMemoryUsage(unsigned long memSize) { memAllocated += memSize; }
    static void statistics(int line);
    static void yystats(void);
};