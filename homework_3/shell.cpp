#include<iostream>
#include<string>
#include<algorithm>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<cerrno>
#include<sys/types.h>
#include<filesystem>

void set_stream(int stream, std::string dir, const std::string& file, mode_t mode)
{
	std::filesystem::create_directories(dir);
	dir += file;
	int fd = open(dir.c_str(), mode, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (fd < 0)
	{
		std::cerr << "Error while opening the file" << std::endl;
		exit(errno);
	}
	if (dup2(fd, stream) < 0)
	{
		std::cerr << "Error while changing the stream" << std::endl;
		exit(errno);
	}
}

int main()
{
	while (true)
	{
		// recives a command
		std::string args;
		std::getline(std::cin, args);

		// splits command into arguments

		for (int i = 1; i < args.size(); ++i)
		{
			if (args[i] == ' ' && args[i - 1] == ' ')
			{
				args.erase(args.begin() + i);
				--i;
			}
		}
		if (!args.empty() && args[0] == ' ')
			args.erase(args.begin());
		if (!args.empty() && args[args.size() - 1] == ' ')
			args.pop_back();

		int num_of_args = std::count(args.begin(), args.end(), ' ');

		if (args.empty () || args.size() == num_of_args)
			continue;

		num_of_args += args.empty() ? 0 : 1;
		char** args_list = new char*[num_of_args];
		std::string command = args.substr(0, args.find(' '));
		for (int i = 0; i < num_of_args; ++i)
		{
			int len_of_arg = (args.find(' ') == std::string::npos) ? args.size() : args.find(' ');
			args_list[i] = new char[len_of_arg];
			for (int j = 0; j < len_of_arg; ++j)
			{
				args_list[i][j] = args[j];
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

			set_stream(0, "/opt/silentshell/" + std::to_string(getpid()) + "/", "in.std", O_RDONLY | O_CREAT);
			set_stream(1, "/opt/silentshell/" + std::to_string(getpid()) + "/", "out.std", O_WRONLY | O_CREAT | O_APPEND);
			set_stream(2, "/opt/silentshell/" + std::to_string(getpid()) + "/", "err.std", O_WRONLY | O_CREAT | O_APPEND);

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
