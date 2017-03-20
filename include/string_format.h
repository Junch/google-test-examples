#include <memory>
#include <string>
#include <stdarg.h>

//http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    size_t size = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

std::string string_format2(const char* fmt_str, ...) {
    va_list args;
    va_start(args, fmt_str);
    size_t size = std::vsnprintf(nullptr, 0, fmt_str, args) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    std::vsnprintf( buf.get(), size, fmt_str, args);
    va_end(args);
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}
