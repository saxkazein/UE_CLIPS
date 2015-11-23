#include <string.h>
#include <sstream>

class CLIPS::CLIPSCPPEnv;

class CustomFileRouter : public CLIPS::CLIPSCPPRouter
{
private:
	char* fName = "unreal";
	std::stringstream outputStream;

public:
	virtual int Query(CLIPS::CLIPSCPPEnv *, const char *);
	virtual int Print(CLIPS::CLIPSCPPEnv *, const char *, const char *);
	std::string GetOutput();
	void ClearOutput();
};

int CustomFileRouter::Query(CLIPS::CLIPSCPPEnv *cEnv, const char *logicalName)
{
	int n = strcmp(logicalName, fName);
	if (strcmp(logicalName, fName) == 0)
		return 1;
	else
		return 0;
}

int CustomFileRouter::Print(CLIPS::CLIPSCPPEnv *cEnv, const char *logicalName, const char *output)
{
	outputStream << output;
	return 1;
}

std::string CustomFileRouter::GetOutput()
{
	return outputStream.str();
}

void CustomFileRouter::ClearOutput()
{
	outputStream.str("");
}
