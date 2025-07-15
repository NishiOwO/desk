#include <Desk/Desk.h>
#include <Desk/Window.h>

int main(int argc, char** argv){
  DeskWidget w;
  DeskInit();
  w = DeskCreateWidget(DeskWindowWidgetClass, 1,
    DeskNtitle, "window"
  );
  DeskMainLoop(w);
}
