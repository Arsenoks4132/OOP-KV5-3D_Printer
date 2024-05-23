#include <stdlib.h>
#include <stdio.h>
#include "headers/TreeMaker.h"

int main()
{
	TreeMaker tree_maker(nullptr);
	tree_maker.MakeTree();
	return tree_maker.Start();
}
