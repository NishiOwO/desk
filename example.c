#include <Desk/Desk.h>
#include <Desk/Window.h>
#include <Desk/Button.h>

int main(int argc, char** argv){
  DeskWidget w, w2;
  DeskInit();
  w = DeskCreateWidget(DeskWindowWidgetClass, DeskNoParent, 1,
    DeskNtitle, "window"
  );
  w2 = DeskCreateWidget(DeskButtonWidgetClass, w, 5,
    DeskNtext, "test",
    DeskNx, 10,
    DeskNy, 10,
    DeskNwidth, 100,
    DeskNheight, 50
  );
  DeskMainLoop(w);
}
