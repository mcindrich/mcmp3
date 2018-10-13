#include <mcmp3/mp-log.h>
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>

namespace mp {
    Log::Log() {
        m_file.open("mcmp3.log", std::ios::app);
    }

    int Log::logMessage(const char* msg) {
        // get the date and log the date plus the message
        if(m_file.is_open()) {
        time_t now = time( 0 );
            struct tm* loc_t = localtime ( &now );
            // [y-m-d h:m]
            m_file << "[";
            m_file << loc_t->tm_year + 1900 << "-";
            m_file << loc_t->tm_mon + 1 << "-";
            m_file << loc_t->tm_mday << " ";
            m_file << std::setw(2) << std::setfill('0') << loc_t->tm_hour + 1 
            << ":";
            
            m_file << std::setw(2) << std::setfill('0') << loc_t->tm_min + 1 
            << "]\t" << msg << std::endl;

        } else {
            return 1;
        }
        return 0;
    }

    Log::~Log() {
        if(m_file.is_open()) {
            m_file.close();
        }
    }
}