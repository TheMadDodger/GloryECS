#pragma once
#include <Reflection.h>

REFLECTABLE_ENUM(TestEnum, TE_One, TE_Two)

struct MyComponent
{
    MyComponent() : _a(0) {}
    MyComponent(int a) : _a(a) {}

    int _a;
    
};

struct Position
{
    Position() : _x(0.0f), _y(0.0f) {}
    Position(float x, float y) : _x(x), _y(y) {}

    float _x;
    float _y;
};

struct ReflectableComponent
{
    ReflectableComponent() : _f1(0), _i1(0), _cc1(""), _enum(TestEnum::TE_One) {}
    ReflectableComponent(float f1, int i1, const char* cc1) : _f1(f1), _i1(i1), _cc1(cc1), _enum(TestEnum::TE_One) {}

    REFLECTABLE(ReflectableComponent,
        (float) (_f1),
        (int)   (_i1),
        (const char*)   (_cc1),
        (std::vector<float>) (_floats),
        (TestEnum)  (_enum)
    )
};
