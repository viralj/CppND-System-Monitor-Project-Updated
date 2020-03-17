#include "processor.h"
#include <string>
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    auto cpuStats = LinuxParser::CpuUtilization();
    if (!cpuStats[0].empty()) {
        std::istringstream stringstream(cpuStats[0]);
        std::string line;
        int jiffiesCounter = 0;
        while (stringstream) {
            stringstream >> line;
            if (line != "cpu") {
                cpuJiffies[jiffiesCounter] = std::stof(line);
                jiffiesCounter++;
            }
        }
        idleJiffies = cpuJiffies[3] + cpuJiffies[4];
        nonIdleJiffies = cpuJiffies[0] + cpuJiffies[1] + cpuJiffies[2] + cpuJiffies[5] + cpuJiffies[6] + cpuJiffies[7];
        totalJiffies = idleJiffies + nonIdleJiffies;
        cpuPercentage = (totalJiffies - idleJiffies) / totalJiffies;
        return cpuPercentage;
    }
    return 0.0;
}