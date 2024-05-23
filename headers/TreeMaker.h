#ifndef __TREEMAKER__H
#define __TREEMAKER__H
#include "Base.h"

#include "computer.h"
#include "control_panel.h"
#include "filament.h"
#include "info_table.h"
#include "model.h"
#include "printer_3d.h"

class TreeMaker : public Base
{
public:
	TreeMaker(TreeMaker *ptr, string nm = "StandartName");

	// Basic methods
	void MakeTree();
	int Start();

	// Info methods
	int get_tick();

	// Action signals
	void signal_to_initialize(string &message);
	void signal_system_display_text(string &message);
	void signal_enter_command(string &message);
	void signal_do_tick(string &message);

	// Action handlers
	void settings_handler(string message);
	void turn_off_handler(string message);

protected:
	int tick;
};

#endif
