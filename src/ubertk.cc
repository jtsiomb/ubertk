#include "ubertk.h"

namespace utk {

static Container *root_widget;

Container *init(int x, int y)
{
	NullBox *cont = new NullBox;
	cont->set_size(x, y);
	cont->set_pos(0, 0);
	root_widget = cont;
	return cont;
}

void close(Container *root)
{
	if(!root) root = root_widget;
	delete root;
}

void draw(Container *root)
{
	if(!root) root = root_widget;

	size_t sz = root->size();
	for(size_t i=0; i<sz; i++) {
		Drawable *d = dynamic_cast<Drawable*>((*root)[i]);
		if(d) {
			d->draw();
		}
	}
}

Container *get_root_widget()
{
	return root_widget;
}

}	// namespace utk end
