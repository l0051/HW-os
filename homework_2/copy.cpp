#include <iostream>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 16

int main(int argc, char** argv)
{
	// check if amount of arguments is right
	if (argc < 3)
	{
		std::cerr << "No file to read" << std::endl;
		exit(1);
	}

	// get source file path
	const char* sourcePath = argv[1];

	// get destination file path
	const char* destinationPath = argv[2];

	// open source file only for reading
	int source = open(sourcePath, O_RDONLY);

	// check if file was not opened due to error
	if (source < 0)
	{
		std::cerr << "Could not open source file because of error " << errno << std::endl;
		exit(errno);
	}

	// open destination file only for writing
	int destination = open(destinationPath, O_WRONLY);

	// check if destination file was not opened due to error
	if (destination < 0)
	{
		std::cerr << "Could not open destination file because of error " << errno << std::endl;
		exit(errno);
	}

	// allocate a buffer to read from file
	char* buffer = new char[BUFFER_SIZE];

	// start copying
	while (true)
	{
		// read BUFFER_SIZE bytes from source file into buffer
		int readBytes = read(source, buffer, BUFFER_SIZE);

		// check if could not read due to error
		if (readBytes < 0)
		{
			std::cerr << "Could not read from source file due to error " << errno << std::endl;
			exit(errno);
		}

		// stop reading because reached the end
		if (readBytes == 0)
		{
			break;
		}

		// write buffer into destination file
		int writeBytes = write(destination, buffer, readBytes);

		//check if could not write due to error
		if (write < 0)
		{
			std::cerr << "Could not write into destination file due to error " << errno << std::endl;
			exit(errno);
		}
	}

	// delete allocated memory
	delete[] buffer;

	// close source file
	close(source);

	//close destination file
	close(destination);

	return 0;
}
