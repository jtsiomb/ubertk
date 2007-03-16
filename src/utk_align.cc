#include "utk_align.h"
#include "utk_container.h"

namespace utk {

Widget *create_spacer(Widget *parent, int sz)
{
	Widget *w = new Widget;

	if(dynamic_cast<VBox*>(parent)) {
		w->set_size(0, sz);
	} else if(dynamic_cast<HBox*>(parent)) {
		w->set_size(sz, 0);
	} else {
		w->set_size(sz, sz);
	}

	parent->add_child(w);
	return w;
}

}	// end of namespace utk
