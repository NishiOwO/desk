#include <Desk/Desk.h>

void DeskInit(void) {}

void DeskMainLoop(DeskWidget w) {}

DeskWidget DeskGetRoot(DeskWidget w) {
	if(w->parent == NULL) return w;
	return DeskGetRoot(w->parent);
}

void DeskDestroy(DeskWidget w) {
	int i;
	for(i = 0; i < arrlen(w->children); i++) {
		DeskDestroy(w->children[i]);
	}
	arrfree(w->children);
	if(w->parent != NULL) {
		DeskWidget p = w->parent;
		for(i = 0; i < arrlen(p->children); i++) {
			if(p->children[i] == w) {
				arrdel(p->children, i);
				break;
			}
		}
	}
	if(w->wclass->destroy != NULL) w->wclass->destroy(w);
	free(w);
}
