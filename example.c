#include <Desk/Desk.h>
#include <Desk/Window.h>
#include <Desk/Button.h>

int main(int argc, char** argv){
	DeskWidget w, w2;
	DeskInit();
	w = DeskCreateWidget(DeskWindowWidgetClass, DeskWidgetNone,
		DeskNtitle, "Example window",
		DeskNwidth, 300,
		DeskNheight, 300,
	NULL);
	w2 = DeskCreateWidget(DeskButtonWidgetClass, w,
		DeskNtext, "test",
		DeskNx, 10,
		DeskNy, 10,
		DeskNwidth, 100,
		DeskNheight, 50,
	NULL);
	DeskMainLoop(w);
}
