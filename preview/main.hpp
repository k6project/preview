#pragma once

// Primitive math types

struct vec2f
{
    float x = 0.f;
    float y = 0.f;
};

struct vec3f
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

struct vec4f
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    float w = 0.f;
};

// Utility functions

void log(const char* fmt, ...);

typedef unsigned long long sym_id;

sym_id id(const char* str);

//
