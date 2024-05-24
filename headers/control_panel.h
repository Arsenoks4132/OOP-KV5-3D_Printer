#ifndef __CONTROL_PANEL__H
#define __CONTROL_PANEL__H
#include "Base.h"

class control_panel : public Base
{
public:
    control_panel(Base *ptr);

    // Info signals
    void signal_claim_system_info(string &message);
    void signal_claim_pc_info(string &message);
    void signal_claim_filament_info(string &message);

    // Action signals
    void signal_send_settings(string &message);
    void signal_add_model(string &message);
    void signal_turn_off(string &message);

    // Action handlers
    void initialize_handler(string message);
    void new_command_handler(string message);
};

#endif
