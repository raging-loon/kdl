#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <shellapi.h>
#include <stdbool.h>

typedef bool (*SEARCH_FN)(const unsigned char*, unsigned int);

static int parseOptions(char** argv);

static const char* TargetDLL = NULL;
static const char* RuleList = NULL;
static const char* TargetFile = NULL;

int main(int argc, char** argv)
{
	if (argc != 4)
	{
		printf("Usage: KdlTestDllLoader <rule.dll>"
			   "<target rules to run> <file to analyze>\n");
		return 1;
	}

	HMODULE ruleDLL = LoadLibraryA(argv[1]);
	
	if(ruleDLL == NULL)
	{
		printf("Failed to load DLL\n");
		return 1;
	}

	SEARCH_FN test = (SEARCH_FN)(GetProcAddress(ruleDLL, "kdl_rule_EICAR"));

	if (!test)
	{
		printf("Failed to load kdl_rule_EICAR\n");
		return 1;
	}
	///"X5O!P%@AP[4PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*"
	const unsigned char testStr[] = 
		
		"X5O!P%@AP[4PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST";
	if (test(testStr, sizeof(testStr)) == TRUE)
		printf("Found\n");


	return 0;
}

