#include <Desk/Desk.h>
#include <Desk/Window.h>
#include <Desk/Button.h>
#include <Desk/Label.h>

#include <stdio.h>

void pressed(DeskWidget w){
	printf("pressed %p\n", w);
}

int main(int argc, char** argv){
	DeskWidget w, w2, w3, w4, w5;
	DeskInit();
	w = DeskCreateWidget(DeskWindowWidgetClass, DeskWidgetNone,
		DeskNtitle, "Example window",
		DeskNwidth, 300,
		DeskNheight, 300,
		DeskNfontSize, 16,
	NULL);
	w2 = DeskCreateWidget(DeskButtonWidgetClass, w,
		DeskNtext, "test 1",
		DeskNx, 10,
		DeskNy, 10,
		DeskNwidth, 100,
		DeskNheight, 50,
		DeskNpressCallback, pressed,
	NULL);
	w3 = DeskCreateWidget(DeskButtonWidgetClass, w,
		DeskNtext, "test 2",
		DeskNx, 110,
		DeskNy, 10,
		DeskNwidth, 100,
		DeskNheight, 25,
		DeskNpressCallback, pressed,
	NULL);
	w4 = DeskCreateWidget(DeskButtonWidgetClass, w,
		DeskNtext, "test 3",
		DeskNx, 110,
		DeskNy, 35,
		DeskNwidth, 100,
		DeskNheight, 25,
		DeskNpressCallback, pressed,
	NULL);
	w5 = DeskCreateWidget(DeskLabelWidgetClass, w,
		DeskNtext, "あのイーハトーヴォのすきとほった風、\n夏でも底に冷たさをもつ青いそら、\nうつくしい森で飾られたモーリオ市、\n郊外のぎらぎらひかる草の波",
		DeskNx, 10,
		DeskNy, 60,
		DeskNwidth, 350,
		DeskNheight, 250,
	NULL);
	DeskMainLoop(w);
}
