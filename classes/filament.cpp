#include "../headers/filament.h"

filament ::filament(Base *ptr, string name, int print_temp, int length) : Base(ptr, name)
{
    this->print_temp = print_temp;
    this->length = length;
    this->cur_length = length;
    this->refill_ticks = 2;
}

// Signals
void filament ::signal_refill_end(string &message) {}

// Handlers
void filament::refill_coil_handler(string message)
{
    if (this->cur_length > 0)
    {
        return;
    }
    this->refill_ticks -= 1;
    if (this->refill_ticks == 0)
    {
        this->cur_length = this->length;
        this->refill_ticks = 2;
        emit_signal(SIGNAL_D(filament::signal_refill_end), message);
    }
}

// Methods
int filament ::getLength()
{
    return this->cur_length;
}

int filament ::getTemp()
{
    return this->print_temp;
}

bool filament ::decrease_length(int delta)
{
    this->cur_length -= delta;
    if (this->cur_length <= 0)
    {
        return true;
    }
    return false;
}
