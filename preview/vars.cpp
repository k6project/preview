#include "vars.hpp"
#include "main.hpp"

#include <map>
#include <vector>
#include <cstdio>

using namespace std;

void set_vec3f(const char* cmd)
{
    vec3f res = {};
    if (sscanf(cmd, "%f%f%f", &res.x, &res.y, &res.z) < 3)
    {
    }
}
