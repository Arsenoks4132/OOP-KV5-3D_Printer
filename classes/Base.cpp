#include "../headers/Base.h"

// KV_1______________________________________
Base ::Base(Base *header, string l_name)
{
	this->root = header;
	this->name = l_name;
	if (root)
	{
		root->leaves.push_back(this);
	}
};

bool Base ::rename(string l_name)
{
	if (root == nullptr)
	{
		name = l_name;
		return true;
	}
	if (root->getLeaf(l_name) == nullptr)
	{
		name = l_name;
		return true;
	}
	return false;
};

string Base ::getName()
{
	return name;
};

Base *Base ::getRoot()
{
	return root;
};

Base *Base ::getLeaf(string l_name)
{
	for (int i = 0; i < leaves.size(); ++i)
	{
		if (leaves[i]->getName() == l_name)
		{
			return leaves[i];
		}
	}
	return nullptr;
};

Base ::~Base()
{
	for (int i = 0; i < leaves.size(); ++i)
	{
		delete leaves[i];
	}
}
// KV_1______________________________________

// KV_2______________________________________
Base *Base ::find_branch(string f_name)
{
	Base *this_found = nullptr;
	if (name == f_name)
	{
		this_found = this;
	}
	Base *found;
	Base *child_found;
	int count = 0;
	for (int i = 0; i < leaves.size(); ++i)
	{
		found = leaves[i]->find_branch(f_name);
		if (found)
		{
			++count;
			if (count == 1)
			{
				child_found = found;
			}
			if (this_found || count > 1)
			{
				if (found->getName() == "/")
				{
					if (child_found->getName() == "/")
					{
						delete child_found;
					}
					child_found = found;
				}
				else
				{
					if (child_found->getName() != "/")
					{
						child_found = new Base(nullptr, "/");
					}
				}
			}
		}
	}
	if (count == 0)
	{
		return this_found;
	}
	return child_found;
}

Base *Base ::find_tree(string f_name)
{
	Base *old_root = root;
	if (old_root)
	{
		while (old_root->getRoot())
		{
			old_root = old_root->getRoot();
		}
		return old_root->find_branch(f_name);
	}
	else
	{
		return find_branch(f_name);
	}
}

void Base ::printTree()
{
	Base *old_root = root;
	while (old_root)
	{
		cout << "    ";
		old_root = old_root->getRoot();
	}
	cout << name;
	for (int i = 0; i < leaves.size(); ++i)
	{
		cout << endl;
		leaves[i]->printTree();
	}
}

void Base ::printReady()
{
	Base *old_root = root;
	while (old_root)
	{
		cout << "    ";
		old_root = old_root->getRoot();
	}
	cout << name;
	if (state != 0)
	{
		cout << " is ready";
	}
	else
	{
		cout << " is not ready";
	}
	for (int i = 0; i < leaves.size(); ++i)
	{
		cout << endl;
		leaves[i]->printReady();
	}
}

void Base ::set_state(int state)
{
	if (state == 0)
	{
		this->state = 0;
		for (int i = 0; i < leaves.size(); ++i)
		{
			leaves[i]->set_state(0);
		}
		return;
	}
	if (!root)
	{
		this->state = state;
		return;
	}
	if (root->state != 0)
	{
		this->state = state;
	}
}
// KV_2______________________________________

// KV_3______________________________________
void Base ::delete_leaf(string l_name)
{
	Base *element = getLeaf(l_name);
	if (element)
	{
		leaves.erase(find(leaves.begin(), leaves.end(), element));
		delete element;
	}
}

bool Base ::change_parent(Base *p_point)
{
	if (!p_point || !root || p_point->getLeaf(name))
	{
		return false;
	}
	Base *old_root = p_point;
	while (old_root)
	{
		if (old_root == this)
		{
			return false;
		}
		old_root = old_root->getRoot();
	}

	root->leaves.erase(find(root->leaves.begin(), root->leaves.end(), this));
	this->root = p_point;
	p_point->leaves.push_back(this);
	return true;
}

Base *Base ::find_coord(string coord)
{
	if (coord.length() == 0)
	{
		return nullptr;
	}
	Base *old_root = root;
	if (old_root)
	{
		while (old_root->getRoot())
		{
			old_root = old_root->getRoot();
		}
	}
	else
	{
		old_root = this;
	}

	Base *found;
	bool is_found = false;
	if (coord[0] == '.')
	{
		if (coord == ".")
		{
			return this;
		}
		else
		{
			found = find_branch(coord.substr(1, coord.length() - 1));
			is_found = true;
		}
	}

	stringstream spliter;
	if (coord[0] == '/')
	{
		spliter = stringstream(coord.substr(1, coord.length() - 1));
	}
	else
	{
		spliter = stringstream(coord);
	}
	string segment;
	vector<string> cords;
	while (getline(spliter, segment, '/'))
	{
		cords.push_back(segment);
	}

	if (coord[0] == '/')
	{
		if (coord == "/")
		{
			return old_root;
		}
		if (coord[1] == '/')
		{
			found = find_tree(coord.substr(2, coord.length() - 2));
			is_found = true;
		}
	}

	if (!is_found)
	{
		if (coord[0] == '/')
		{
			found = old_root->getLeaf(cords[0]);
		}
		else
		{
			found = this->getLeaf(cords[0]);
		}
		int i = 1;
		while (found && i < cords.size())
		{
			found = found->getLeaf(cords[i]);
			++i;
		}
	}
	if (found)
	{
		if (found->getName() == "/")
		{
			delete found;
			found = nullptr;
		}
	}
	return found;
}
// KV_3______________________________________

// KV_4______________________________________
void Base::set_connect(TYPE_SIGNAL p_signal, Base *p_object, TYPE_HANDLER p_ob_handler)
{
	for (int i = 0; i < connects.size(); ++i)
	{
		if (connects[i]->p_signal == p_signal &&
			connects[i]->p_cl_base == p_object &&
			connects[i]->p_handler == p_ob_handler)
		{
			return;
		}
	}

	o_sh *p_value = new o_sh();

	p_value->p_signal = p_signal;
	p_value->p_cl_base = p_object;
	p_value->p_handler = p_ob_handler;

	this->connects.push_back(p_value);
}

void Base::emit_signal(TYPE_SIGNAL p_signal, string &s_command)
{
	if (this->state == 0)
	{
		return;
	}

	(this->*p_signal)(s_command);

	TYPE_HANDLER p_handler;
	Base *p_object;

	for (int i = 0; i < connects.size(); ++i)
	{
		if (connects[i]->p_signal == p_signal)
		{
			p_handler = connects[i]->p_handler;
			p_object = connects[i]->p_cl_base;
			if (p_object->state != 0)
			{
				(p_object->*p_handler)(s_command);
			}
		}
	}
}

void Base::delete_connect(TYPE_SIGNAL p_signal, Base *p_object, TYPE_HANDLER p_ob_handler)
{
	for (int i = 0; i < connects.size(); ++i)
	{
		if (connects[i]->p_signal == p_signal &&
			connects[i]->p_cl_base == p_object &&
			connects[i]->p_handler == p_ob_handler)
		{
			delete connects[i];
			connects.erase(connects.begin() + i);
			return;
		}
	}
}

string Base::absolute_path()
{
	string s = "/";
	if (this->root)
	{
		s += this->name;
	}
	Base *obj = this;
	while (obj->getRoot())
	{
		obj = obj->getRoot();
		if (obj->getRoot())
		{
			s = "/" + obj->getName() + s;
		}
	}
	return s;
}

void Base::set_ready()
{
	this->state = 1;
	for (int i = 0; i < leaves.size(); ++i)
	{
		leaves[i]->set_ready();
	}
}