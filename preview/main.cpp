#include <map>
#include <cstdio>
#include <cctype>
#include <functional>

using namespace std;

#define CHECKSUM_INIT 5381u;

template <typename T, unsigned int N> 
unsigned int length(const T (&arr)[N]){return N;}

void prompt(unsigned int& checksum, const char*& params)
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
			unsigned int checksum = CHECKSUM_INIT;
			while (char c = *command++)
			{
				checksum = ((checksum << 5) + checksum) ^ c;
			}
			mappings[checksum] = cb;
		}
	}

	void exec(unsigned int checksum, const char* params)
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
		unsigned int checksum = 0;
		const char* params = nullptr;
		prompt(checksum, params);
		cmdMap.exec(checksum, params);
	}

	return 0;
}
