#include "CIni.h"
#include "COptions.h"

int main() 
{

	COptions options;
	CIni iniObj;

	iniObj.readIniFile();
	iniObj.writeIniFile(0);

	if (!options.isValid())
	{
		iniObj.writeIniFile(1);
	}
	if(!options.isValid())

	return 0;
}
