
#include "CIni.h"




int main() {

	CIni iniObj;

	std::string* userInput = NULL;
	//userInput = new std::string();
	iniObj.writeFile(userInput, iniObj.getHeaderValues());
	if (iniObj.validateFile())
	{
		std::cout << "VALIDATION SUCCEED " << std::endl;
		iniObj.writeLogFile(iniObj.getValidatedMsg(), 1);

	}
	else
	{
		std::cout << "VALIDATION FAILED " << std::endl;
		iniObj.writeLogFile(iniObj.getNotValidatedMsg(), 1);
		iniObj.readFile();
		iniObj.writeFile(NULL, iniObj.getDefaultValues());
	}

	delete userInput;

	//std::string s ="-106.0,100,150.0,10000.0,1,9.0,1,-98.0,2.0,-98.0,3.0,1,0,0";
	//std::string n = s;
	//int i = 0;

	//while (i < n.size()) 
	//{

	//	auto it = std::find(s.begin(), s.end(), ',');
	//	std::cout << *it << std::endl;
	//	int a = it - s.begin();
	//	if(i!=s.size())
	//		s.replace(s.begin()+a, s.begin() + a+1, "");
	//	i++;
	//	std::cout << s << std::endl;
	//	std::cout << a << std::endl;
	//}

	//n.append(s, 0,a);

	return 0;
}