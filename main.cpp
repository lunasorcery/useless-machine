#include <cstdio>
#include <cstdint>
#include <csignal>
#include <ctime>
#include <random>
#include <cstring>
#include <string>
#include <thread>
#include <chrono>

using namespace std::chrono;

#define arrSize(x) (sizeof(x)/sizeof(x[0]))

void signalHandler(int signo)
{
	// no
}

void print(char const* msg)
{
	fputs(msg, stdout);
}

void printAndFlush(char const* msg)
{
	fputs(msg, stdout);
	fflush(stdout);
}

void wait(int ms)
{
	std::this_thread::sleep_for(milliseconds(ms));
}

std::string readFile(char const* filepath)
{
	std::string buffer;
	FILE* fh = fopen(filepath, "rb");
	if (fh)
	{
		fseek(fh, 0, SEEK_END);
		size_t const length = ftell(fh);
		rewind(fh);
		buffer.resize(length);
		fread(buffer.data(), 1, length, fh);
		fclose(fh);
	}
	return buffer;
}

int main(int argc, char const** argv)
{
	signal(SIGINT, signalHandler);

	srand(time(NULL));

	char const* msgs[]={
		"No.",
		"I refuse.",
		"Stop it.",
		"What are you trying to achieve?"
	};

	int const untypingSpeed = 75;
	char const* msg = msgs[rand()%arrSize(msgs)];


	// run a dummy bash session to capture the prompt text
	system(
		// run a fake command in a bash session, redirecting stderr to stdout
		"echo fakecommanddoesntexist | bash -li 2>&1 "
		// grep for the fake command and pick out the first line
		"| grep fakecommanddoesntexist | head -1 "
		// remove the fake command text and the trailing newline
		// leaving just the prompt being written to stdout
		"| sed 's/fakecommanddoesntexist//g' | tr -d '\n' "
		// and capture it into a file so we can use it later
		"> .useless");

	// read back the prompt text
	std::string promptText = readFile(".useless");

	// clean up the file
	system("rm .useless");


	// say no
	printf("%s\n", msg);
	wait(1500);

	// move cursor to the end of the message line
	printf("\033[A\033[%luG", strlen(msg)+1);
	fflush(stdout);
	wait(untypingSpeed);

	// untype the message
	for (int i = 0; i < strlen(msg); ++i)
	{
		printAndFlush("\033[D\033[K");
		wait(untypingSpeed);
	}

	// move to the prompt line
	print("\033[A");

	// re-print the prompt
	print(promptText.c_str());

	// re-print the command since the prompt will likely have cleared it
	printAndFlush(argv[0]);
	wait(untypingSpeed);

	// untype the command
	for (int i = 0; i < strlen(argv[0]); ++i)
	{
		printAndFlush("\033[D\033[K");
		wait(untypingSpeed);
	}

	// reset the cursor to the left so the new *real* prompt covers our dummy one
	print("\033[G");
}
