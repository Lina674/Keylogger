#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <windows.h>
#include <thread>

std::string findUSBDriveLetter() {
    for (char drive = 'D'; drive <= 'Z'; ++drive) {
        std::string drive_path = std::string(1, drive) + ":\\";
                
        // check if removable disk
        if (GetDriveTypeA(drive_path.c_str()) == DRIVE_REMOVABLE) {
            return drive_path;
        }
    }
    return "";
}

std::string getCurrentDateTimeForFilename() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm* localTime = std::localtime(&currentTime);

    std::stringstream ss;
    ss << std::put_time(localTime, "%Y-%m-%d"); // YYYY-MM-DD

    return ss.str();
}

void logToUSB(const std::string& usb_drive, char key) {

    std::string file_name = usb_drive + getCurrentDateTimeForFilename() + ".txt";
    
    std::ofstream logfile(file_name, std::ios_base::app);  // Append to file
 
    if (!logfile.is_open()) {
        std::cerr << "Error opening file for logging!" << std::endl;
    }

    else {
        logfile << key;
        logfile.close();
    }
}

int main() {
    std::string usb_drive_mount = findUSBDriveLetter();
    bool not_mounted = usb_drive_mount.empty(); // true if not mounted

    if (!not_mounted) {
    std::string log_filename = usb_drive_mount + getCurrentDateTimeForFilename() + ".txt";
    std::ofstream logfile(log_filename, std::ios_base::app);
    
        if (!logfile.is_open()) {
            std::cerr << "Error opening file for logging!" << std::endl;
            return 1;
        }

        while (true) {
            if (!std::filesystem::exists(usb_drive_mount)) {
                std::cerr << "USB drive " << usb_drive_mount << " removed!" << std::endl;
                break;
            }

            char key = _getch();
            logfile << key;
            logfile.flush();
        }

    } else {
        std::cerr << "No USB drive detected." << std::endl;
    }

    return 0;
}