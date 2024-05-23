#include "../headers/computer.h"

computer ::computer(Base *ptr) : Base(ptr, "Computer"){};

// Signals
void computer ::signal_for_3d_printer(string &message) {}
void computer ::signal_pc_display_text(string &message) {}
void computer ::signal_try_send_model(string &message) {}

// Handlers
void computer ::info_for_3d_printer_handler(string message)
{
    string text = "queue products: " + to_string(leaves.size());
    text += "; PC:";
    for (int i = 0; i < leaves.size(); ++i)
    {
        text += " " + leaves[i]->getName() + ":";
        text += to_string(((model *)leaves[i])->get_volume());
    }
    emit_signal(SIGNAL_D(computer::signal_for_3d_printer), text);
}

void computer ::info_for_info_table_handler(string message)
{
    string text = "PC condition turned ";
    if (leaves.size() == 0)
    {
        text += "off";
    }
    else
    {
        text += "on";
        for (int i = 0; i < leaves.size(); ++i)
        {
            text += " " + leaves[i]->getName() + ";";
        }
    }
    emit_signal(SIGNAL_D(signal_pc_display_text), text);
}

void computer ::add_new_model_handler(string message)
{
    stringstream parms(message);
    int volume;
    string type, name;
    parms >> volume >> type;
    parms.ignore();
    getline(parms, name);
    if (this->find_tree(name))
    {
        message = "Failed to create product";
        emit_signal(SIGNAL_D(computer::signal_pc_display_text), message);
        return;
    }
    new model(this, name, type, volume);
}

void computer ::send_model_handler(string message)
{
    if (leaves.size() == 0)
    {
        return;
    }
    message = leaves[0]->getName();
    emit_signal(SIGNAL_D(computer::signal_try_send_model), message);
}

void computer ::move_model_handler(string message)
{
    this->getLeaf(message)->change_parent(this->find_tree("3D_Printer"));
}