#include "../headers/printer_3d.h"

printer_3d ::printer_3d(Base *ptr, int print_speed, int temp_speed, int temp) : Base(ptr, "3D_Printer")
{
    this->print_speed = print_speed;
    this->temp_speed = temp_speed;
    this->temp = temp;
    this->current_model = "";
}

// Signals
void printer_3d ::signal_claim_pc_info(string &message) {}
void printer_3d ::signal_3d_printer_display_text(string &message) {}
void printer_3d ::signal_get_model(string &message) {}
void printer_3d ::signal_copy_model(string &message) {}
void printer_3d ::signal_refill_coil(string &message) {}

// Handlers
void printer_3d ::claim_system_info_handler(string message)
{
    this->buffer = "3D printer tact: " + to_string(((TreeMaker *)this->root)->get_tick());
    this->buffer += "; temp: " + to_string(this->temp);
    this->buffer += "; status: " + to_string(this->state);
    this->buffer += "; print product: ";
    if (this->current_model == "")
    {
        this->buffer += "0";
    }
    else
    {
        this->buffer += to_string(((model *)this->getLeaf(current_model))->get_volume());
    }
    this->buffer += "; ";
    emit_signal(SIGNAL_D(printer_3d::signal_claim_pc_info), message);
    emit_signal(SIGNAL_D(printer_3d::signal_3d_printer_display_text), this->buffer);
}

void printer_3d ::claim_pc_info_handler(string message)
{
    this->buffer += message;
}

void printer_3d ::claim_filament_info_handler(string message)
{
    string text = "Filament coil condition: " + message + " ";
    text += to_string(((filament *)this->getLeaf(message))->getLength());
    emit_signal(SIGNAL_D(printer_3d::signal_3d_printer_display_text), text);
}

void printer_3d ::doing_tick_handler(string message)
{
    if (this->state == 1 || this->state == 3)
    {
        emit_signal(SIGNAL_D(printer_3d::signal_get_model), message);
        return;
    }
    if (this->state == 2)
    {
        model *my_model = (model *)(this->getLeaf(current_model));
        filament *my_coil = (filament *)(this->getLeaf(my_model->get_type()));
        int needed_delta = this->print_speed;
        if (needed_delta > my_model->get_volume())
        {
            needed_delta = my_model->get_volume();
        }
        if (needed_delta > my_coil->getLength())
        {
            needed_delta = my_coil->getLength();
        }
        bool is_printed = my_model->decrease_volume(needed_delta);
        bool is_ran_out = my_coil->decrease_length(needed_delta);
        if (is_printed)
        {
            this->delete_leaf(current_model);
            this->current_model = "";
            this->state = 1;
            return;
        }
        if (is_ran_out)
        {
            this->state = 4;
            return;
        }
        return;
    }
    if (this->state == 4)
    {
        emit_signal(SIGNAL_D(printer_3d::signal_refill_coil), message);
        return;
    }
}

void printer_3d ::try_grab_model_handler(string message)
{
    model *target_model = (model *)this->find_tree(message);
    string model_type = target_model->get_type();
    filament *target_filament = (filament *)this->getLeaf(model_type);
    int target_temp = target_filament->getTemp();
    if (this->temp == target_temp)
    {
        this->state = 2;
        this->current_model = message;
        emit_signal(SIGNAL_D(printer_3d::signal_copy_model), message);
        return;
    }
    this->state = 3;
    if (abs(this->temp - target_temp) <= this->temp_speed)
    {
        this->temp = target_temp;
        return;
    }
    if (this->temp < target_temp)
    {
        this->temp += this->temp_speed;
        return;
    }
    this->temp -= this->temp_speed;
}

void printer_3d ::refill_end_handler(string message)
{
    if (this->current_model == "")
    {
        this->state = 1;
    }
    else
    {
        this->state = 2;
    }
}