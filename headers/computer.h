#ifndef __COMPUTER__H
#define __COMPUTER__H
#include "Base.h"
#include "model.h"

using namespace std;

class computer : public Base
{
public:
    computer(Base *ptr);

    // Action signals
    void signal_pc_display_text(string &message);
    void signal_for_3d_printer(string &message);
    void signal_try_send_model(string &message);

    // Info handlers
    void info_for_3d_printer_handler(string message);
    void info_for_info_table_handler(string message);

    // Action handlers
    void add_new_model_handler(string message);
    void send_model_handler(string message);
    void move_model_handler(string message);
};

#endif
