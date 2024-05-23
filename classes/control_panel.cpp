#include "../headers/control_panel.h"

control_panel ::control_panel(Base *ptr) : Base(ptr, "Control_Panel") {}

// Signals

void control_panel ::signal_send_settings(string &message) {}
void control_panel ::signal_turn_off(string &message) {}
void control_panel ::signal_claim_system_info(string &message) {}
void control_panel ::signal_claim_pc_info(string &message) {}
void control_panel ::signal_claim_filament_info(string &message) {}
void control_panel ::signal_add_model(string &message) {}

// Handlers
void control_panel ::initialize_handler(string message)
{
    string text;
    while (true)
    {
        getline(cin, message);
        if (message == "End of settings")
        {
            break;
        }
        text += message + " ";
    }
    this->set_state(1);
    emit_signal(SIGNAL_D(control_panel::signal_send_settings), text);
}

void control_panel ::new_command_handler(string message)
{
    string command;
    getline(cin, message);
    if (message.length() > 0)
    {
        if (message == "Turn off the system")
        {
            this->emit_signal(SIGNAL_D(control_panel::signal_turn_off), message);
            return;
        }
        stringstream console(message);
        console >> command;
        if (command == "System")
        {
            emit_signal(SIGNAL_D(control_panel::signal_claim_system_info), message);
            return;
        }
        if (command == "PC")
        {
            console >> command;
            if (command == "condition")
            {
                emit_signal(SIGNAL_D(control_panel::signal_claim_pc_info), message);
            }
            else
            {
                getline(console, message);
                message = command + message;
                emit_signal(SIGNAL_D(control_panel::signal_add_model), message);
            }
            return;
        }
        if (command == "Filament")
        {
            console >> command >> command >> command;
            emit_signal(SIGNAL_D(control_panel::signal_claim_filament_info), command);
            return;
        }
        if (command == "SHOWTREE")
        {
            this->root->printReady();
        }
    }
}
