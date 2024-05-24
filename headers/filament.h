#ifndef __FILAMENT__H
#define __FILAMENT__H
#include "Base.h"

class filament : public Base
{
public:
    filament(Base *ptr, string name, int print_temp, int length);
    
    // Info methods
    int getLength();
    int getTemp();

    // Action methods
    bool decrease_length(int delta);

    // Action signals
    void signal_refill_end(string &message);

    // Action handlers
    void refill_coil_handler(string message);

protected:
    int print_temp;
    int length;
    int cur_length;
    int refill_ticks;
};

#endif
