#include "../headers/info_table.h"

info_table ::info_table(Base *ptr) : Base(ptr, "Info_Table") {}

// Handlers
void info_table::display_text_handler(string message)
{
    if (this->state == 2)
    {
        message = "\n" + message;
    }
    else
    {
        this->state = 2;
    }
    cout << message;
}