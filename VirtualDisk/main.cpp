#include <iostream>
#include <cstdlib>

#include "virtual_console.h"
#include "file_system/virtual_file_system.h"
#include "cmd/cmd_header.h"

using namespace std;

int main(int argc, char* argv[])
{
	VirtualFileSystem::initWithDriveName("C:");
	VirtualConsole::init();

	VirtualConsole::getInstPtr()->addCommand("dir", new DirCmd());
	VirtualConsole::getInstPtr()->addCommand("mkdir", new MdCmd());
	VirtualConsole::getInstPtr()->addCommand("cd", new CdCmd());
	VirtualConsole::getInstPtr()->addCommand("copy", new CopyCmd());
	VirtualConsole::getInstPtr()->addCommand("del", new DelCmd());
	VirtualConsole::getInstPtr()->addCommand("rmdir", new RdCmd());
	VirtualConsole::getInstPtr()->addCommand("ren", new RenCmd());
	VirtualConsole::getInstPtr()->addCommand("move", new MoveCmd());
	VirtualConsole::getInstPtr()->addCommand("mklink", new MklinkCmd());
	VirtualConsole::getInstPtr()->addCommand("save", new SaveCmd());
	VirtualConsole::getInstPtr()->addCommand("load", new LoadCmd());
	VirtualConsole::getInstPtr()->addCommand("cls",  new ClsCmd());

	VirtualConsole::getInstPtr()->run();

	VirtualConsole::release();
	VirtualFileSystem::release();

	system("pause");

	return 0;
}