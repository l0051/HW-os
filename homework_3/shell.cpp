#include <iostream>
#include <string>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cerrno>

int main()
{
	while (true)
	{
		// recives a command
		std::string args;
		std::getline(std::cin, args);

		// splits command into arguments
		int num_of_args = std::count(args.begin(), args.end(), ' ');
		num_of_args += args.empty() ? 0 : 1;
		char** args_list = new char*[num_of_args];
		std::string command = args.substr(0, args.find(' '));
		for (int i = 0; i < num_of_args; ++i)
		{
			int len_of_arg = (args.find(' ') == std::string::npos) ? args.size() : args.find(' ');
			args_list[i] = new char[len_of_arg];
			for (int j = 0; j < len_of_arg; ++j)
			{
				arg_list[i][j] = args[j];
			}
			args.erase(0, len_of_arg + 1);
		}

		// dublicates the process
		pid_t child_pid = fork();

		// error handling
		if (child_pid < 0)
		{
			std::cerr << "Could not clone process due to error" << errno << std::endl;
			exit(errno);
		}

		// runs a child process
		if (child_pid == 0)
		{
			int exeс = execvp(command.c_str(), args_list);
			if (exeс == -1)
			{
				std::cerr << "Could not clone execute a programm due to error" << errno << std::endl;
				exit(errno);
			}
		}

		// waits for the end of the child process
		else
		{
			int status;
			waitpid(-1, &status, 0);
		}
	}

	return 0;
}
