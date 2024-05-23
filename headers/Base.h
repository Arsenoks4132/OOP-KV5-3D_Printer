#ifndef __BASE__H
#define __BASE__H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>

// KV_4_______________________
#define SIGNAL_D(signal_f) (TYPE_SIGNAL)(&signal_f)
#define HANDLER_D(handler_f) (TYPE_HANDLER)(&handler_f)
// KV_4_______________________

using namespace std;

// KV_4_______________________
class Base;
typedef void (Base::* TYPE_SIGNAL)(string &);
typedef void (Base::* TYPE_HANDLER)(string);

struct o_sh
{
	TYPE_SIGNAL p_signal;
	Base *p_cl_base;
	TYPE_HANDLER p_handler;
};

// KV_4_______________________

class Base
{
public:
	// KV_1_______________________
	Base(Base *header, string l_name = "StandartName");
	~Base();

	bool rename(string l_name);
	string getName();
	Base *getRoot();
	Base *getLeaf(string l_name);
	// KV_1_______________________

	// KV_2_______________________
	Base *find_branch(string f_name);
	Base *find_tree(string f_name);
	void printTree();
	void printReady();
	void set_state(int state);
	// KV_2______________________

	// KV_3_____________________
	bool change_parent(Base *p_point);
	void delete_leaf(string l_name);
	Base *find_coord(string coord);
	// KV_3_____________________

	// KV_4_______________________
	void set_connect(TYPE_SIGNAL p_signal, Base *p_object, TYPE_HANDLER p_ob_handler);
	void delete_connect(TYPE_SIGNAL p_signal, Base *p_object, TYPE_HANDLER p_ob_handler);
	void emit_signal(TYPE_SIGNAL p_signal, string &s_command);

	string absolute_path();

	void set_ready();

	// KV_4_______________________

protected:
	// KV_1_______________________
	string name;
	Base *root;
	vector<Base *> leaves;
	// KV_1_______________________

	// KV_2______________________
	int state;
	// KV_2______________________

	// KV_4______________________
	vector<o_sh *> connects;
	// KV_2______________________
};

#endif
