#include "CIni.h"
#include "COptions.h"

int main() 
{

	COptions options;
	CIni iniObj;

	iniObj.readFile();
	iniObj.writeFile(0);

	if (!options.validate())
	{
		iniObj.writeFile(1);
	}
	if(!options.validate())

	return 0;
}
