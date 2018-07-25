#include "common.hpp"

#include <map>
#include <cstdio>
#include <cctype>
#include <cstdarg>
#include <string>
#include <functional>

using namespace std;

#define CHECKSUM_INIT 5381u;

template <typename T, unsigned int N> 
unsigned int length(const T (&arr)[N]){return N;}

typedef map<SymId, string> SymbolTable;

void log(const char* fmt, ...)
{
    va_list args;
    printf(fmt, args);
    printf("\n");
}

const char* getSymbol(SymId id, const char* fallback)
{
	static SymbolTable table;
	const auto pos = table.find(id);
	if (pos == table.end())
	{
		if (fallback)
		{
			table.emplace(id, fallback);
		} 
		return fallback;
	}
	return pos->second.c_str();
}

SymId getSymId(const char* symbol)
{
	static unsigned int count = 0;
	const char* original = symbol;
	char tmp[24];
	if (!symbol || !*symbol)
	{
		sprintf(tmp, "NoName_%u", count++);
		symbol = tmp;
	}
	SymId checksum = CHECKSUM_INIT;
    while (char c = *symbol++)
    {
        checksum = ((checksum << 5) + checksum) ^ c;
    }
	getSymbol(checksum, original);
    return checksum;
}

static void prompt(SymId& checksum, const char*& params)
{
    bool capture = false;
    static char line[65536];
    unsigned int length = 0;
    checksum = CHECKSUM_INIT;
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
            checksum = ((checksum << 5) + checksum) ^ c;
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

class CommandMap
{

public:

    typedef function<void(const char*)> Callback;

    void add(const char* command, Callback cb)
    {
        if (cb)
        {
            SymId checksum = getSymId(command);
            mappings[checksum] = cb;
        }
    }

    void exec(SymId checksum, const char* params)
    {
        const auto pos = mappings.find(checksum);
        if (pos != mappings.end())
        {
            pos->second(params);
        }
    }

private:

    map<unsigned int, Callback> mappings;

};

int main(int argc, const char* argv[])
{
    CommandMap cmdMap;
    bool keepRunning = true;

    cmdMap.add("quit", [&](const char *) { keepRunning = false; });

    while (keepRunning)
    {
        SymId checksum = 0;
        const char* params = nullptr;
        prompt(checksum, params);
        cmdMap.exec(checksum, params);
    }

    return 0;
}
