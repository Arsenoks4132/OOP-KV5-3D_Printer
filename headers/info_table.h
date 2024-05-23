#ifndef __INFO_TABLE__H
#define __INFO_TABLE__H
#include "Base.h"

using namespace std;

class info_table : public Base
{
public:
    info_table(Base *ptr);

    // Action handler
    void display_text_handler(string message);
};

#endif
