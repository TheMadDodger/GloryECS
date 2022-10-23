#pragma once

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
