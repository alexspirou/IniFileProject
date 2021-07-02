#include "CIni.h"
#include "COptions.h"

int main() 
{
	COptions options;
	CIni iniObj;
	//Create log file
	iniObj.createLogFile();
	iniObj.writeLogFile("\nProgram has started ", 1, 1);
	//Read file and write the file
	iniObj.readIniFile();
	iniObj.writeIniFile(0);
	//If not valid aks for user's inputs
	if (!options.isValid())
	{
		iniObj.writeIniFile(1);
	}
	//If fail again, set default values
	if(!options.isValid())
	
	iniObj.writeLogFile("Program has stopped", 1, 1);

	return 0;
}
