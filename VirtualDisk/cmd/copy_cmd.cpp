#include "copy_cmd.h"
#include "../file_system/copy_tool.h"
#include "../file_system/path.h"
#include "../file_system/virtual_file_system.h"

void CopyCmd::execute(CommandParser& cmdParser, VirtualConsole& virtualConsole)
{
	std::ostream& output = virtualConsole.getOutput();

	std::set<std::string> optionals;
	std::vector<std::string> positionalOptionals;

	cmdParser.parseArg(optionals, positionalOptionals);

	if(positionalOptionals.size() != 2)
	{
		output << "命令语法不正确" << std::endl; 
	}
	else
	{
		std::string srcPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), positionalOptionals[0]);
		std::string destPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), positionalOptionals[1]);

		CopyTool copyTool(VirtualFileSystem::getInstPtr(), srcPath, destPath);

		copyTool.prepare();
		while(true)
		{
			CopyTool::Error e = copyTool.getError();
			if(e == CopyTool::FileNotExists)
			{
				output << "系统找不到指定的文件" << std::endl;
				break;
			}
			else if(e == CopyTool::SameFile)
			{
				while(true)
				{
					std::string sameFileName = copyTool.getSameFileName();
					Path::transformSeparator(sameFileName, virtualConsole.getDisplaySeparator());
					output << "覆盖 " << sameFileName << " 吗？（Yes/No）：";

					std::string result;
					virtualConsole.getLine(result);
					if(result == "Yes")
					{
						copyTool.overrideCurrent();
						break;
					}
					else if(result == "No")
					{
						copyTool.skipCurrent();
						break;
					}
				}
			}
			else if(e == CopyTool::SrcIsDir)
			{
				copyTool.skipCurrent();
			}

			bool isSuccess = copyTool.execute();
			if(isSuccess && !copyTool.haveNext())
			{
				break;
			}
		}
		output << "已复制           " << copyTool.getTotalCopyFile() << "个文件。" << std::endl;
	}
}