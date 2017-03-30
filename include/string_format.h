#include <memory>
#include <string>
#include <stdarg.h>

//http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template<typename ... Args>
std::string string_format2( const std::string& format, Args ... args )
{
    size_t size = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

// http://stackoverflow.com/questions/23157530/vsnprintf-on-mac-gives-exc-bad-access
std::string string_format(const char* fmt, ...);

std::string string_format(const char* fmt, va_list vlist);
