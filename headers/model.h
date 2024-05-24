#ifndef __MODEL__H
#define __MODEL__H
#include "Base.h"

class model : public Base
{
public:
    model(Base *ptr, string name, string type, int volume);
    
    // Info methods
    int get_volume();
    string get_type();

    // Action methods
    bool decrease_volume(int delta);

protected:
    int volume;
    string type;
};

#endif
