#!/usr/bin/env perl
if(@ARGV < 1){
	die;
}

foreach my $arg (@ARGV){
	open(f, ">", "include/Desk/" . $arg . ".h") or die;
	print(f "#ifndef __DESK_" . uc($arg) . "_H__\n");
	print(f "#define __DESK_" . uc($arg) . "_H__\n");
	print(f "\n");
	print(f "#include <Desk/Desk.h>\n");
	print(f "\n");
	print(f "#ifdef __cplusplus\n");
	print(f "extern \"C\" {\n");
	print(f "#endif\n");
	print(f "\n");
	print(f "DESK_EXPORT DeskWidgetClass Desk" . $arg . "WidgetClass;\n");
	print(f "\n");
	print(f "#ifdef __cplusplus\n");
	print(f "}\n");
	print(f "#endif\n");
	print(f "\n");
	print(f "#endif\n");
	close(f);
	
	open(f, ">", "src/" . $arg . ".c") or die;
	print(f "#include <Desk/Desk.h>\n");
	print(f "#include <Desk/" . $arg . ".h>\n");
	print(f "\n");
	print(f "static void widget_init(DeskWidget w){\n");
	print(f "}\n");
	print(f "\n");
	print(f "static void widget_destroy(DeskWidget w){\n");
	print(f "}\n");
	print(f "\n");
	print(f "static void widget_render(DeskWidget w){\n");
	print(f "}\n");
	print(f "\n");
	print(f "DeskWidgetClassRec Desk" . $arg . "WidgetClassRec = {\n");
	print(f "	widget_init, /* init */\n");
	print(f "	widget_destroy, /* destroy */\n");
	print(f "	widget_render /* render */\n");
	print(f "};\n");
	print(f "\n");
	print(f "DeskWidgetClass Desk" . $arg . "WidgetClass = (DeskWidgetClass)&Desk" . $arg . "WidgetClassRec;\n");
	close(f);
}
