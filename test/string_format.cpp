#include "gmock/gmock.h"
#include "string_format.h"
using namespace std;

#ifdef _MSC_VER
#define __noinline         __declspec(noinline)
#else
#define __noinline         __attribute__ ((noinline))
#endif

TEST(string_format, test){
    std::string name = "Jun Chen";
    std::string ret = string_format("%s: %d", name.c_str(), 40);
    EXPECT_EQ(ret, "Jun Chen: 40");
}

// 00886089 6a08            push    8
// 0088608b 68e0f78a00      push    offset google_test_examples_test!`string' (008af7e0)
// 00886090 8d4dc0          lea     ecx,[ebp-40h]                         ==> std::string name "Jun Chen"
// 00886093 c745d40f000000  mov     dword ptr [ebp-2Ch],0Fh
// 0088609a c745d000000000  mov     dword ptr [ebp-30h],0
// 008860a1 c645c000        mov     byte ptr [ebp-40h],0
// 008860a5 e846e8ffff      call    google_test_examples_test!std::basic_string<char,std::char_traits<char>,std::allocator<char> >::assign (008848f0)

// 008860aa 6a06            push    6
// 008860ac 68ecf78a00      push    offset google_test_examples_test!`string' (008af7ec)
// 008860b1 8d4dd8          lea     ecx,[ebp-28h]                        ==> std::string "%s: %d"
// 008860b4 c745fc00000000  mov     dword ptr [ebp-4],0
// 008860bb c745ec0f000000  mov     dword ptr [ebp-14h],0Fh
// 008860c2 c745e800000000  mov     dword ptr [ebp-18h],0
// 008860c9 c645d800        mov     byte ptr [ebp-28h],0
// 008860cd e81ee8ffff      call    google_test_examples_test!std::basic_string<char,std::char_traits<char>,std::allocator<char> >::assign (008848f0)

// 008860d2 837dd410        cmp     dword ptr [ebp-2Ch],10h       这里作了比较，2ch放的是string的length,看这个length是否大于16个字节
// 008860d6 8d45c0          lea     eax,[ebp-40h]                 ==> name         
// 008860d9 6a28            push    28h                           ==> arg3: 40
// 008860db 0f4345c0        cmovae  eax,dword ptr [ebp-40h]       如果length >=16 个字节，就修改eax的值，这个值是值char*的地址
// 008860df 50              push    eax                           ==> arg2: "Jun Chen"
// 008860e0 8d45d8          lea     eax,[ebp-28h]                  
// 008860e3 c645fc01        mov     byte ptr [ebp-4],1
// 008860e7 50              push    eax                           ==> arg1: "%s: %d"
// 008860e8 8d45a8          lea     eax,[ebp-58h]
// 008860eb 50              push    eax                           ==> 存放返回值吗?
// 008860ec e8ff080000      call    google_test_examples_test!string_format<char const *,int> (008869f0)

TEST(string_format2, test){
    std::string name = "Jun Chen";
    std::string ret = string_format2("%s: %d", name.c_str(), 40);
    EXPECT_EQ(ret, "Jun Chen: 40");
}

// http://stackoverflow.com/questions/2155730/how-do-c-compilers-implement-functions-that-return-large-structures
// Return Value Optimization
struct Data {
    unsigned values[9] = {1,2,3};
};

__noinline
Data createData(int num) 
{
    Data data;
    data.values[3] = num;

    return data;
}

// 00d97510 55              push    ebp
// 00d97511 8bec            mov     ebp,esp
// 00d97513 8b4508          mov     eax,dword ptr [ebp+8]       ==> hidden arg is the pointer to the return value Data
// 00d97516 0f57c0          xorps   xmm0,xmm0
// 00d97519 8b4d0c          mov     ecx,dword ptr [ebp+0Ch]     ==> first arg is here

// 00d9751c c70001000000    mov     dword ptr [eax],1           ==> Construct values
// 00d97522 c7400402000000  mov     dword ptr [eax+4],2
// 00d97529 c7400803000000  mov     dword ptr [eax+8],3
// 00d97530 0f11400c        movups  xmmword ptr [eax+0Ch],xmm0
// 00d97534 660fd6401c      movq    mmword ptr [eax+1Ch],xmm0

// 00d97539 89480c          mov     dword ptr [eax+0Ch],ecx     ==> Set the num here
// 00d9753c 5d              pop     ebp
// 00d9753d c3              ret

TEST(RVO, test){
    Data a = createData(4);
    EXPECT_EQ(a.values[3], 4);
}
