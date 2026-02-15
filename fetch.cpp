#include <iostream>
#include <fstream>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <vector>
#include <cmath>
#include <sstream>

using namespace std;

#define RESET         "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_WHITE   "\033[37m"
#define COLOR_BLACK   "\033[30m"
#define COLOR_RED     "\033[31m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"

struct utsname buffer;

string GetNameHost() {
    stringstream ret;

    struct passwd *pw = getpwuid(getuid());
    ret << COLOR_GREEN << (pw ? pw->pw_name : "Unknown") << "@";
    
    char hostname[HOST_NAME_MAX + 1];
    if (gethostname(hostname, HOST_NAME_MAX) == 0) {
        ret << hostname << RESET;
    } else {
        ret << "Unknown" << RESET;
    }

    return ret.str();
}

string GetOS() {
    ifstream file("/etc/os-release");
    string line;
    string distro = "Unknown";
    stringstream ret;
    
    while (getline(file, line)) {
        if (line.find("PRETTY_NAME=") == 0) {
            distro = line.substr(12);
            if (distro.front() == '"' && distro.back() == '"') {
                distro = distro.substr(1, distro.length() - 2);
            }
            break;
        }
    }
    
    ret << COLOR_GREEN << "os      " << COLOR_WHITE << distro << " ";

    if (uname(&buffer) == 0) {
        ret << buffer.machine;
    } else {
        ret << "Unknown";
    }

    return ret.str();
}

string GetKernel() {
    stringstream ret;

    ret << COLOR_GREEN << "Kernel  " << COLOR_WHITE;

    if (uname(&buffer) == 0) {
        ret << buffer.release;
    } else {
        ret << "Unknown";
    }
    
    return ret.str();
}

string GetUptime() {
    ifstream uptime_file("/proc/uptime");
    stringstream ret;

    double uptime_seconds;
    
    if (uptime_file.is_open()) {
        uptime_file >> uptime_seconds;
        uptime_file.close();
        
        int days = uptime_seconds / 86400;
        int hours = (uptime_seconds - days * 86400) / 3600;
        int minutes = (uptime_seconds - days * 86400 - hours * 3600) / 60;
        int seconds = fmod(uptime_seconds, 60);
        
        ret << COLOR_GREEN << "Uptime  " << COLOR_WHITE;
        if (days > 0) ret << days << "d ";
        if (hours > 0) ret << hours << "h ";
        if (minutes > 0) ret << minutes << "m ";
        ret << seconds << "s";
    }
    
    return ret.str();
}

int main() {
    vector<string> logo = {
        "   ___   ",
        "  ('\033[33mv\033[0m')  ",
        " ((___)) ",
        "  \033[33m'\033[0m   \033[33m'\033[0m  "
    };

    vector<string> info;
    info.push_back(GetNameHost());
    info.push_back(GetOS());
    info.push_back(GetKernel());
    info.push_back(GetUptime());
    
    for (size_t i = 0; i < logo.size() || i < info.size(); i++) {
        if (i < logo.size())
            cout << logo[i];
        else
            cout << string(logo[0].length(), ' ');
            
        if (i < info.size())
            cout << "  " << info[i];
        
        cout << endl;
    }

    return 0;
}
