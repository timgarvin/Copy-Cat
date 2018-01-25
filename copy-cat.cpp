//Tim Garvin

//Compile using: g++ copy-cat.cpp -lrt -o copy-cat
//Execute using: ./copy-cat

#include <iostream>
#include <time.h> //Required for clock_gettime()
#include <unistd.h> //Required for read(), write(), and close() system calls
#include <fcntl.h> //Required for open() system call
#include <sys/types.h> //Required for open() system call
#include <sys/stat.h> //Required for open() system call
#include <string.h> //Required for strcat()
#include <stdlib.h> //Required for realloc()
#include <stdio.h> //Required for fopen(), fread(), fwrite(), and fclose() library functions

using namespace std;

int main()
{
	char *working_path = (char *)"/tmp/"; //Declares a char pointer (string in C) and initializes it to the path the source file to be copied and the resulting copied file must be located
	char *source_path = new char[4096]; //Declares a char pointer (string in C) to store the path name (including filename later) for the source file to be copied and initializes it to the maximum allowed path length for Linux (4096 bytes, as mentioned in /usr/include/linux/limits.h)
	char *copy_path = new char[4096]; //Declares a char pointer (string in C) to store the path name (including filename later) for the new file copied from the source file and initializes it to the maximum allowed path length for Linux (4096 bytes, as mentioned in /usr/include/linux/limits.h)
	char *filename = new char[255]; //Declares a char pointer (string in C) to store the source filename (including extension) to be copied based on user input and initializes it to the maximum allowed filename length for Linux (255 bytes, as mentioned in /usr/include/linux/limits.h)
	char *copy_filename = new char[255]; //Declares a char pointer (string in C) to store the new copy filename (including extension) based on user input and initializes it to the maximum allowed filename length for Linux (255 bytes, as mentioned in /usr/include/linux/limits.h)
	
	//SECTION USING READ() AND WRITE() SYSTEM CALLS
	
	//Initializes the source and copy path to the /tmp/ directory (where the file to be copied must be located)
	strcpy(source_path, working_path);
	strcpy(copy_path, working_path);
	
	//Prompts the user for the filename
	cout<<"Enter the filename (including the extension) of the file you'd like to copy: ";
	cin>>filename;
	
	//Concatenates the filename to the source path
	strcat(source_path, filename);
	
	//Creates a new name for the copy filename (based on user input) and concatenates that new filename to the copy path
	strcpy(copy_filename, "copy1_");
	strcat(copy_filename, filename);
	strcat(copy_path, copy_filename);
	
	//Declares variables for calculating runtime (in nanoseconds)
	long int start_time;
	long int time_difference;
	struct timespec current_time;
	
	//Declares and initializes the source and copy file descriptors
	int source_fd = 0, copy_fd = 0;
	
	source_fd = open(source_path, O_RDONLY); //Opens the source file
	copy_fd = open(copy_path, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR); //Creates a new file to copy data to (in append mode) and opens it with read/write permissions for the current user

	//If the source and copy file descriptors do not throw any errors: start copying the source file
	if(source_fd != -1 && copy_fd != -1)
	{
		//Declares and initializes variables for keeping track of the number of bytes read or to be written
		size_t amount_read = 1;
		size_t amount_written;
		
		//Declares and initializes variables for the buffer
		char *buffer = new char[1];
		size_t buffer_size = 1;
		
		//Declares and initializes a variable to keep track of the current copy iteration
		int copy_num = 1;
		
		cout<<"Using read() and write() system calls:"<<endl;
		
		//While there is something left to read: keep reading from the source file and copying (writing) to the copy file
		while(amount_read > 0)
		{
			//If not the first copy iteration: increase buffer size
			if(copy_num > 1)
			{
				buffer_size *= 2; //Doubles the buffer size

				buffer = (char *)realloc(buffer, buffer_size); //Reallocates memory to double the previous buffer size
				
				//Print an error if there was a problem doubling the buffer size
				if(!buffer)
				{
					cout<<"ERROR increasing buffer size"<<endl;
					break;
				}
			}
			
			//Get starting time (in nanoseconds)
			clock_gettime(CLOCK_REALTIME, &current_time);
			start_time = current_time.tv_nsec;
			
			amount_read = read(source_fd, buffer, buffer_size); //Read from source file and save the number of bytes read into amount_read
			
			//If something was read: write to the copy output file
			if(amount_read > 0)
			{
				amount_written = write(copy_fd, buffer, amount_read); //Write to the copy output file and save the number of bytes written into amount_written
				
				//Print an error if there was a problem writing to the output file
				if(amount_written == -1)
				{
					cout<<"ERROR when writing to the output file"<<endl;
					break;
				}
				
				//Get ending time (in nanoseconds)
				clock_gettime(CLOCK_REALTIME, &current_time);
				time_difference = current_time.tv_nsec - start_time; //Calculated runtime

				if(time_difference < 0)
				{
					time_difference += 1000000000;
				}
				
				cout<<"Copy "<<copy_num<<" read and wrote "<<amount_read<<" bytes and took "<<time_difference<<" nanoseconds."<<endl; //Outputs how many nanoseconds the program took to execute
				copy_num++; //Increments the current copy iteration before looping
			}
			else if(amount_read == -1) //Print an error if there was a problem reading from the source file
			{
				cout<<"ERROR when reading the source file"<<endl;
				break;
			}
		}
		
		cout<<endl;
	}
	else if(source_fd == -1) //Print an error if there was a problem opening the source file
	{
		cout<<"ERROR when opening source file to read from (using open())"<<endl;
	}
	else if(copy_fd == -1) //Print an error if there was a problem creating or opening the output file
	{
		cout<<"ERROR when creating or opening the output file to write to (using open())"<<endl;
	}
	
	//Closes the source and output files
	close(source_fd);
	close(copy_fd);
	
	
	//SECTION USING FREAD() AND FWRITE() LIBRARY FUNCTIONS
	
	strcpy(copy_path, working_path);
	strcpy(copy_filename, "copy2_");
	strcat(copy_filename, filename);
	strcat(copy_path, copy_filename);
		
	FILE *source_fp, *copy_fp; //Declares the source and copy file pointers
		
	source_fp = fopen(source_path, "r"); //Opens the source file
	copy_fp = fopen(copy_path, "a+"); //Creates a new file to copy data to (in append mode) and opens it with read/write permissions for the current user
	
	//If the source and copy file pointers do not throw any errors: start copying the source file
	if(source_fp && copy_fp)
	{
		//Declares and initializes variables for keeping track of the number of items read or to be written
		size_t amount_read = 1;
		size_t amount_written;
		
		//Declares and initializes variables for the buffer
		char *buffer = new char[1];
		size_t buffer_size = 1;
		int copy_num = 1;
		
		cout<<"Using fread() and fwrite() system calls:"<<endl;
		
		//While there is something left to read: keep reading from the source file and copying (writing) to the copy file
		while(amount_read > 0)
		{
			//If not the first copy iteration: increase buffer size
			if(copy_num > 1)
			{
				buffer_size *= 2; //Doubles the buffer size
		
				buffer = (char *)realloc(buffer, buffer_size); //Reallocates memory to double the previous buffer size
				
				//Print an error if there was a problem doubling the buffer size
				if(!buffer)
				{
					cout<<"ERROR increasing buffer size"<<endl;
					break;
				}
			}
			
			//Get starting time (in nanoseconds)
			clock_gettime(CLOCK_REALTIME, &current_time);
			start_time = current_time.tv_nsec;
			
			amount_read = fread(buffer, 1, buffer_size, source_fp); //Read from source file and save the number of items read into amount_read
			
			//If something was read: write to the copy output file
			if(amount_read > 0)
			{
				amount_written = fwrite(buffer, 1, strlen(buffer), copy_fp); //Write to the copy output file and save the number of items written into amount_written
			
				//Print an error if there was a problem writing to the output file
				if(ferror(copy_fp))
				{
					cout<<"ERROR when writing to the output file"<<endl;
					break;
				}
				
				//Get ending time (in nanoseconds)
				clock_gettime(CLOCK_REALTIME, &current_time);
				time_difference = current_time.tv_nsec - start_time; //Calculated runtime

				if(time_difference < 0)
				{
					time_difference += 1000000000;
				}
				
				cout<<"Copy "<<copy_num<<" read and wrote "<<strlen(buffer)<<" bytes and took "<<time_difference<<" nanoseconds."<<endl; //Outputs how many nanoseconds the program took to execute
				copy_num++; //Increments the current copy iteration before looping
			}
			else if(ferror(source_fp)) //Print an error if there was a problem reading from the source file
			{
				cout<<"ERROR when reading the source file"<<endl;
				break;
			}
		}
		
		cout<<endl;
	}
	else if(!source_fp) //Print an error if there was a problem opening the source file
	{
		cout<<"ERROR when opening source file to read from (using fopen())"<<endl;
	}
	else if(!copy_fp) //Print an error if there was a problem creating or opening the output file
	{
		cout<<"ERROR when creating or opening the output file to write to (using fopen())"<<endl;
	}
	
	//Closes the source and output files
	fclose(source_fp);
	fclose(copy_fp);
	
	return 0;
}