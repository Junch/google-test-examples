#include "logger.h"
#include <ctime>
#include <chrono>

const string CLogger::m_sFileName = "Log.txt";
CLogger* CLogger::m_pThis = nullptr;
ofstream CLogger::m_Logfile;

// // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
// const std::string CurrentDateTime()
// {
//     time_t     now = time(NULL);
//     struct tm  tstruct;
//     char       buf[80];
//     localtime_s(&tstruct, &now);
//     strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
//     return buf;
// }

//http://stackoverflow.com/questions/27136854/c11-actual-system-time-with-milliseconds
//http://en.cppreference.com/w/cpp/chrono/time_point/time_point_cast
const std::string CurrentDateTime()
{
    using namespace std;
    using namespace std::chrono;

    system_clock::time_point now = system_clock::now();
    system_clock::duration tp = now.time_since_epoch(); // Has type milliseconds
    seconds s = duration_cast<seconds>(tp);
    tp -= s;
    milliseconds milli = duration_cast<milliseconds>(tp);

    {
        auto ms = duration_cast<milliseconds>(tp);
        size_t frac = ms.count() % 1000;

        printf("frac=%u, milli=%u\n", frac, milli);
    }
    
    time_t curr_now = system_clock::to_time_t(now);
    struct tm  tstruct;
    localtime_s(&tstruct, &curr_now);

    char  fmt[64];
    char  buf[64];
    strftime(fmt, sizeof(fmt), "%Y-%m-%d %H:%M:%S,%%03u", &tstruct);
    snprintf(buf, sizeof(buf), fmt, milli);
    return buf;
}

CLogger::CLogger()
{
 
}

CLogger* CLogger::GetLogger(){
    if (m_pThis == nullptr){
        m_pThis = new CLogger();
        m_Logfile.open(m_sFileName.c_str(), ios::out | ios::app);
    }
    return m_pThis;
}

void CLogger::Log(const char * format, ...)
{
    va_list args;
    va_start(args, format);
    //  Return the number of characters in the string referenced the list of arguments.
    // _vscprintf doesn't count terminating '\0' (that's why +1)
    int nLength = _vscprintf(format, args) + 1;
    char* sMessage = new char[nLength];
    vsprintf_s(sMessage, nLength, format, args);
    //vsprintf(sMessage, format, args);
    m_Logfile << CurrentDateTime() << " " << sMessage << "\n";
    va_end(args);
 
    delete [] sMessage;
}

void CLogger::Log(const string& sMessage)
{
    m_Logfile <<  CurrentDateTime() << " " << sMessage << "\n";
}

CLogger& CLogger::operator<<(const string& sMessage)
{
    m_Logfile << "\n" << CurrentDateTime() << ":\t";
    m_Logfile << sMessage << "\n";
    return *this;
}