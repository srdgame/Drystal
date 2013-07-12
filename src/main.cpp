#include "engine.hpp"

int main(int argc, const char* argv[])
{
    const char* filename = "data/main.lua";
    if (argc == 2) {
        filename = argv[1];
    }

    Engine e(filename, 42);
    e.loop();
    return 0;
}
