#pragma once

typedef float vec2f[2];
typedef float vec3f[3];
typedef float vec4f[4];
typedef vec4f mat4f[4];

void log(const char* fmt, ...);

typedef unsigned long long SymId;

const char* getSymbol(SymId id, const char* fallback = nullptr);

SymId getSymId(const char* symbol);
