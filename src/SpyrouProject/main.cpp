
#include "CIni.h"




int main() {

	CIni iniObj;
    
	std::string* userInput = NULL;

	iniObj.writeFile(userInput,iniObj.getHeaderValues());
    if (iniObj.validateFile())
    {
        iniObj.writeLogFile(iniObj.getValidatedMsg());

    }
    else
    {
        iniObj.writeLogFile(iniObj.getNotValidatedMsg());
    }


	return 0;
}