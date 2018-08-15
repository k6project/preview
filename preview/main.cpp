#include "main.hpp"
#include "vars.hpp"

#include <map>
#include <cstdio>
#include <cctype>
#include <cstdarg>
#include <string>
#include <functional>

using namespace std;

#define SYM_ID_INIT 5381u;

template <typename T, unsigned int N> 
unsigned int length(const T (&arr)[N]){return N;}

void log(const char* fmt, ...)
{
    va_list args;
    printf(fmt, args);
    printf("\n");
}

sym_id id(const char* str)
{
    static unsigned int count = 0;
    char tmp[24];
    if (!str || !*str)
    {
        sprintf(tmp, "NoName_%u", count++);
        str = tmp;
    }
    sym_id checksum = SYM_ID_INIT;
    while (char c = *str++)
    {
        checksum = ((checksum << 5) + checksum) ^ c;
    }
    return checksum;
}

static void prompt(sym_id& sid, const char*& params)
{
    bool capture = false;
    static char line[65536];
    unsigned int length = 0;
    sid = SYM_ID_INIT;
    printf("-> ");
    do
    {
        int c = getc(stdin);
        if (c == 0xA || c == 0xD || isspace(c))
        {
            capture = true;
            if (c == 0xA || c == 0xD)
            {
                break;
            }
        }
        if (!capture)
        {
            sid = ((sid << 5) + sid) ^ c;
        }
        else if (length < (sizeof(line) - 1))
        {
            line[length++] = c;
        }
    } while (true);
    if (length)
    {
        line[length] = 0;
        params = line;
    }
}

static class
{

    typedef function<void(const char*)> callback;

public:

    void add(const char* cmd, callback cb)
    {
        if (cb)
        {
            sym_id sid = id(cmd);
            mappings[sid] = cb;
        }
    }

    void exec(sym_id sid, const char* params)
    {
        const auto pos = mappings.find(sid);
        if (pos != mappings.end())
        {
            pos->second(params);
        }
    }

private:

    map<unsigned int, callback> mappings;

} g_cmdmap;

int main(int argc, const char* argv[])
{
    bool keep_running = true;

    g_cmdmap.add("quit", [&](const char *) { keep_running = false; });
    g_cmdmap.add("vec3f", &set_vec3f);

    while (keep_running)
    {
        sym_id sid = 0;
        const char* params = nullptr;
        prompt(sid, params);
        g_cmdmap.exec(sid, params);
    }

    return 0;
}
