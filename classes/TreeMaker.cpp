#include "../headers/TreeMaker.h"

TreeMaker ::TreeMaker(TreeMaker *ptr, string nm) : Base(ptr, nm)
{
	tick = 1;
}

// Creating object tree
void TreeMaker ::MakeTree()
{
	if (!rename("System"))
	{
		return;
	}
	// Variables
	string message;

	// Creating first objects
	control_panel *my_control_panel = new control_panel(this);
	info_table *my_info_table = new info_table(this);

	// Object activation
	this->set_ready();

	// Connection with info table
	this->set_connect(SIGNAL_D(TreeMaker::signal_to_initialize), my_control_panel, HANDLER_D(control_panel::initialize_handler));
	my_control_panel->set_connect(SIGNAL_D(control_panel::signal_send_settings), this, HANDLER_D(TreeMaker::settings_handler));

	// Initializing coils
	this->emit_signal(SIGNAL_D(TreeMaker::signal_to_initialize), message);

	// Creating other objects
	printer_3d *my_printer_3d = (printer_3d *)(this->getLeaf("3D_Printer"));
	computer *my_computer = new computer(this);

	// Global activation
	this->set_ready();

	// TreeMaker connections
	this->set_connect(SIGNAL_D(TreeMaker::signal_enter_command), my_control_panel, HANDLER_D(control_panel::new_command_handler));
	this->set_connect(SIGNAL_D(TreeMaker::signal_system_display_text), my_info_table, HANDLER_D(info_table::display_text_handler));
	this->set_connect(SIGNAL_D(TreeMaker::signal_do_tick), my_printer_3d, HANDLER_D(printer_3d::doing_tick_handler));

	// Control panel connections
	my_control_panel->set_connect(SIGNAL_D(control_panel::signal_turn_off), this, HANDLER_D(TreeMaker::turn_off_handler));
	my_control_panel->set_connect(SIGNAL_D(control_panel::signal_claim_system_info), my_printer_3d, HANDLER_D(printer_3d::claim_system_info_handler));
	my_control_panel->set_connect(SIGNAL_D(control_panel::signal_claim_filament_info), my_printer_3d, HANDLER_D(printer_3d::claim_filament_info_handler));
	my_control_panel->set_connect(SIGNAL_D(control_panel::signal_claim_pc_info), my_computer, HANDLER_D(computer::info_for_info_table_handler));
	my_control_panel->set_connect(SIGNAL_D(control_panel::signal_add_model), my_computer, HANDLER_D(computer::add_new_model_handler));

	// 3D Printer connections
	my_printer_3d->set_connect(SIGNAL_D(printer_3d::signal_3d_printer_display_text), my_info_table, HANDLER_D(info_table::display_text_handler));
	my_printer_3d->set_connect(SIGNAL_D(printer_3d::signal_claim_pc_info), my_computer, HANDLER_D(computer::info_for_3d_printer_handler));
	my_printer_3d->set_connect(SIGNAL_D(printer_3d::signal_get_model), my_computer, HANDLER_D(computer::send_model_handler));
	my_printer_3d->set_connect(SIGNAL_D(printer_3d::signal_copy_model), my_computer, HANDLER_D(computer::move_model_handler));

	// Computer connections
	my_computer->set_connect(SIGNAL_D(computer::signal_pc_display_text), my_info_table, HANDLER_D(info_table::display_text_handler));
	my_computer->set_connect(SIGNAL_D(computer::signal_for_3d_printer), my_printer_3d, HANDLER_D(printer_3d::claim_pc_info_handler));
	my_computer->set_connect(SIGNAL_D(computer::signal_try_send_model), my_printer_3d, HANDLER_D(printer_3d::try_grab_model_handler));

	// Reporting about readiness
	message = "Ready to work";
	this->emit_signal(SIGNAL_D(TreeMaker::signal_system_display_text), message);
}

// Starting application
int TreeMaker ::Start()
{
	while (this->state != 0)
	{
		string message = "";
		this->emit_signal(SIGNAL_D(TreeMaker::signal_enter_command), message);
		this->emit_signal(SIGNAL_D(TreeMaker::signal_do_tick), message);
		++tick;
	}
	return 0;
}

// Signals
void TreeMaker ::signal_to_initialize(string &message) {}
void TreeMaker ::signal_enter_command(string &message) {}
void TreeMaker ::signal_system_display_text(string &message) {}
void TreeMaker ::signal_do_tick(string &message) {}

// Handlers
void TreeMaker ::settings_handler(string message)
{
	stringstream command(message);
	int n, print_speed, temp_speed, temp;
	command >> n >> print_speed >> temp_speed >> temp;
	printer_3d *my_printer_3d = new printer_3d(this, print_speed, temp_speed, temp);

	my_printer_3d->set_state(5);

	filament *cur_filament;

	string coil;
	int print_temp, length;
	for (int i = 0; i < n; ++i)
	{
		command >> coil >> print_temp >> length;
		if (this->find_branch(coil))
		{
			message = "Failed to create filament coil";
			this->emit_signal(SIGNAL_D(TreeMaker::signal_system_display_text), message);
		}
		else
		{
			cur_filament = new filament(my_printer_3d, coil, print_temp, length);
			my_printer_3d->set_connect(SIGNAL_D(printer_3d::signal_refill_coil), cur_filament, HANDLER_D(filament::refill_coil_handler));
			cur_filament->set_connect(SIGNAL_D(filament::signal_refill_end), my_printer_3d, HANDLER_D(printer_3d::refill_end_handler));
		}
	}
}

void TreeMaker ::turn_off_handler(string message)
{
	this->emit_signal(SIGNAL_D(TreeMaker::signal_system_display_text), message);
	this->state = 0;
}

// Methods
int TreeMaker ::get_tick()
{
	return this->tick;
}