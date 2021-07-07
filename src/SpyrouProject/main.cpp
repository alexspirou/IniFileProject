#include "CIni.h"
#include "COptions.h"

int main() 
{
	//Main Branch
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
		std::cout << "Validation Failed\n";
		std::cout << "Enter new options\n";
		iniObj.writeIniFile(1);
	}
	else
	{
		std::cout << "Validation Succeed\n";
	}
	//If fail again, set default values
	if (!options.isValid())
	{
		std::cout << "Default options are entered\n";
	}
	iniObj.writeLogFile("Program has stopped", 1, 1);

	return 0;
}
