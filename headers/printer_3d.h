#ifndef __PRINTER_3D__H
#define __PRINTER_3D__H
#include "Base.h"

#include "TreeMaker.h"
#include "filament.h"
#include "model.h"

class printer_3d : public Base
{
public:
    printer_3d(Base *ptr, int print_speed, int temp_speed, int temp);

    // Info signals
    void signal_claim_pc_info(string &message);

    // Action signals
    void signal_3d_printer_display_text(string &message);
    void signal_get_model(string &message);
    void signal_copy_model(string &message);
    void signal_refill_coil(string &message);

    // Info handlers
    void claim_system_info_handler(string message);
    void claim_pc_info_handler(string message);
    void claim_filament_info_handler(string message);

    // Action handlers
    void doing_tick_handler(string message);
    void try_grab_model_handler(string message);
    void refill_end_handler(string message);

protected:
    int print_speed;
    int temp_speed;
    int temp;
    string buffer;
    string current_model;
};

#endif