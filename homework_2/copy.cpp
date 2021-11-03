#include <iostream>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 16

// copying segment of data
void copy_segment(int source, int destination, off_t begin, off_t end)
{
	// set cursor in source file
	int cursor = lseek(source, begin, SEEK_SET);

	// check if could not set cursor in source file
	if (cursor < 0)
	{
		std::cerr << "Could not set cursor in source file due to error " << errno << std::endl;
		exit(errno);
	}

	// set cursor in destination file
	cursor = lseek(destination, begin, SEEK_SET);

	// check if could not set cursor in destination file
	if (cursor < 0)
	{
		std::cerr << "Could not set cursor in destination file due to error " << errno << std::endl;
		exit(errno);
	}


	// allocate a buffer to read from file
	char* buffer = new char[MAX_BUFFER_SIZE];

	// start copying
	while (true)
	{
		// set buffer_size
		int buffer_size = std::min((int)(end - lseek(source, 0, SEEK_CUR)), MAX_BUFFER_SIZE);

		// read buffer_size bytes from source file into buffer
		int readBytes = read(source, buffer, buffer_size);

		// check if could not read due to error
		if (readBytes < 0)
		{
			std::cerr << "Could not read from source file due to error " << errno << std::endl;
			exit(errno);
		}

		// stop reading because reached the end
		if ((int)lseek(source, 0, SEEK_CUR) == end)
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

}

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

	// get first data segment begin
	off_t data = lseek(source, 0, SEEK_DATA);

	while (true)
	{
		// get next hole segment begin
		off_t hole = lseek(source, data, SEEK_HOLE);

		// check if something went wrong due to error
		if (hole < 0)
		{
			// check if didn't reached the end of file
			if (errno != ENXIO)
			{
				std::cerr << "Something went wrong, error " << errno << std::endl;
				exit(errno);
			}
			break;
		}

		// copy that segment of data
		copy_segment(source, destination, data, hole);

		// get next data segment begin
		data = lseek(source, hole, SEEK_DATA);

		// check if something went wrong due to error
		if (data < 0)
		{
			// check if didn't reached the end of file
			if (errno != ENXIO)
			{
				std::cerr << "Something went wrong, error " << errno << std::endl;
				exit(errno);
			}
			break;
		}
	}

	// close source file
	close(source);

	//close destination file
	close(destination);

	return 0;
}
