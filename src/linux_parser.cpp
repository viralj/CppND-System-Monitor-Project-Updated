#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
    string line;
    string key;
    string value;
    std::ifstream filestream(kOSPath);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ' ', '_');
            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "PRETTY_NAME") {
                    std::replace(value.begin(), value.end(), '_', ' ');
                    return value;
                }
            }
        }
    }
    return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
    string os, kernel;
    string line;
    std::ifstream stream(kProcDirectory + kVersionFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> os >> kernel;
    }
    return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
    vector<int> pids;
    DIR *directory = opendir(kProcDirectory.c_str());
    struct dirent *file;
    while ((file = readdir(directory)) != nullptr) {
        // Is this a directory?
        if (file->d_type == DT_DIR) {
            // Is every character of the name a digit?
            string filename(file->d_name);
            if (std::all_of(filename.begin(), filename.end(), isdigit)) {
                int pid = stoi(filename);
                pids.push_back(pid);
            }
        }
    }
    closedir(directory);
    return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    float totalMemory, freeMemory, buffers, cached;
    std::string key, value, line;
    std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream stringstream(line);
            while (stringstream >> key >> value) {
                if (key == "totalMemory") totalMemory = std::stof(value);
                if (key == "freeMemory") freeMemory = std::stof(value);
                if (key == "buffers") buffers = std::stof(value);
                if (key == "cached") cached = std::stof(value);
            }
        }
        return (freeMemory + buffers + cached) / totalMemory;
    }
    return 0.0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
    std::ifstream istream(kProcDirectory + kUptimeFilename);
    if (istream.is_open()) {
        string line;
        long upTime = 0;
        std::getline(istream, line);
        std::istringstream linestream(line);
        linestream >> upTime;
        return upTime;
    }
    return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
    std::ifstream istream(kProcDirectory + kStatFilename);
    if (istream.is_open()) {
        string line, cpu;
        long userJiffies, niceJiffies, systemJiffies;
        std::getline(istream, line);
        std::istringstream linestream(line);
        linestream >> cpu >> userJiffies >> niceJiffies >> systemJiffies;
        return systemJiffies;
    }
    return 0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
    string filePath = kProcDirectory + '/' + to_string(pid) + '/' + kStatFilename;
    std::ifstream filestream(filePath);
    if (filestream.is_open()) {
        string line;
        while (std::getline(filestream, line)) {
            std::istringstream stringstream(line);
            std::vector <std::string> results(
                    (std::istream_iterator<std::string>(stringstream)),
                    std::istream_iterator<std::string>());
            return stol(results.at(22));
        }
    }
    return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
    std::ifstream istream(kProcDirectory + kStatFilename);
    if (istream.is_open()) {
        string line, cpu;
        long userJiffies;
        std::getline(istream, line);
        std::istringstream linestream(line);
        linestream >> cpu >> userJiffies;
        return userJiffies;
    }
    return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    std::ifstream istream(kProcDirectory + kStatFilename);
    if (istream.is_open()) {
        string line, cpu;
        long userJiffies, niceJiffies, idleJiffies;
        std::getline(istream, line);
        std::istringstream linestream(line);
        linestream >> cpu >> userJiffies >> niceJiffies >> idleJiffies;
        return idleJiffies;
    }
    return 0;
}

// TODO: Read and return CPU utilization
vector <string> LinuxParser::CpuUtilization() {
    std::ifstream istream(kProcDirectory + kStatFilename);
    if (istream.is_open()) {
        vector <string> cpuStats{};
        string line;
        while (std::getline(istream, line)) {
            cpuStats.emplace_back(line);
        }
        return cpuStats;
    }
    return {};
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    std::ifstream istream(kProcDirectory + kStatFilename);
    if (istream.is_open()) {
        string key, line;
        int totalProcesses;
        while (std::getline(istream, line)) {
            std::istringstream istringstream(line);
            istringstream >> key >> totalProcesses;
            if (key == "processes") return totalProcesses;
        }
    }
    return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    std::ifstream istream(kProcDirectory + kStatFilename);
    if (istream.is_open()) {
        string key, line;
        int totalProcesses;
        while (std::getline(istream, line)) {
            std::istringstream istringstream(line);
            istringstream >> key >> totalProcesses;
            if (key == "procs_running") return totalProcesses;
        }
    }
    return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    string filePath = kProcDirectory + "/" + to_string(pid) + "/" + kCmdlineFilename;
    std::ifstream istream(filePath);
    if (istream.is_open()) {
        string command;
        while (getline(istream, command)) {
            return command;
        }
    }
    return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    string filePath = kProcDirectory + "/" + to_string(pid) + "/" + kStatusFilename;
    std::ifstream istream(filePath);
    if (istream.is_open()) {
        string line, key, memory, memKB;
        while (std::getline(istream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream istringstream(line);
            istringstream >> key >> memory >> memKB;
            if (key == "VmSize") return memory;
        }
    }
    return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    string line, key, r_uid, e_uid, ss_uid, fs_uid;
    string filePath = kProcDirectory + "/" + to_string(pid) + "/" + kStatusFilename;
    std::ifstream istream(filePath);
    if (istream.is_open()) {
        while (std::getline(istream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream istringstream(line);
            istringstream >> key >> r_uid >> e_uid >> ss_uid >> fs_uid;
            if (key == "Uid") return r_uid;
        }
    }

    return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    std::ifstream stream(kPasswordPath);
    if (stream.is_open()) {
        string uid = LinuxParser::Uid(pid);
        string line;
        string user;
        while (std::getline(stream, line)) {
            if (line.find(uid) != std::string::npos) {
                user = line.substr(0, line.find(":"));
                break;
            }
        }
    }
    return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    string filePath = kProcDirectory + "/" + to_string(pid) + "/" + kStatFilename;
    std::ifstream istream(filePath);
    if (istream.is_open()) {
        string line, key, memory, kb;
        vector <string> stats{};
        while (std::getline(istream, line)) {
            std::istringstream istringstream(line);
            while (istringstream >> line) {
                stats.emplace_back(line);
            }
        }
        return stol(stats[21]);
    }

    return 0;
}