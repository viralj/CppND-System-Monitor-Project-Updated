#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
public:
    float Utilization();  // TODO: See src/processor.cpp

    // TODO: Declare any necessary private members
private:
    float cpuJiffies[10]{0};
    float idleJiffies, nonIdleJiffies, totalJiffies, cpuPercentage;
};

#endif