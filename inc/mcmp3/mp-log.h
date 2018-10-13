#ifndef _MCMP3_LOG_H_
#define _MCMP3_LOG_H_

#include <fstream>

namespace mp {
    class Log {
        public:
            Log();
            int logMessage( const char* );
            ~Log();
        private:
            std::ofstream m_file;
    };
}

#endif