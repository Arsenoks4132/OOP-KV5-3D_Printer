#include "../headers/model.h"

model ::model(Base *ptr, string name, string type, int volume) : Base(ptr, name)
{
    this->volume = volume;
    this->type = type;
}

// Methods
int model ::get_volume()
{
    return this->volume;
}

string model ::get_type()
{
    return this->type;
}

bool model ::decrease_volume(int delta)
{
    this->volume -= delta;
    if (this->volume <= 0)
    {
        return true;
    }
    return false;
}