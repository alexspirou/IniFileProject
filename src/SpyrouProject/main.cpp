#include "CIni.h"

int main() {

	CIni iniObj;
	int errorHandler = 0;
	//Write file
	iniObj.writeFile(0, iniObj.getHeaderValues());

	//Check for validation
	if (iniObj.validateFile())
	{
		//Print the result in console
		std::cout << "Validation completed successfully  " << std::endl;
		iniObj.writeLogFile("Validation completed successfully ", 1, 0);
	}
	else
	{
		//Print the result in console
		std::cout << "Validation failed " << std::endl;
		iniObj.writeLogFile("Validation failed", 1, 0);

		//Read file again
		iniObj.readFile();
		//Ask the user for new values
		iniObj.writeLogFile("User's inputs ", 1, 0);
		try
		{
			std::cout << "Enter the values : " << std::endl;
			iniObj.writeFile(1, iniObj.getHeaderValues());
			//Increase errorHandler and if !=0 throw -1
			errorHandler++;
			if (iniObj.getErrorHandler() == 0) { throw - 1; }

		}
		catch (int e)
		{
			//If error handler != 0 pass default values
			iniObj.writeLogFile("File read / write error occurred", 1, 0);
			iniObj.writeFile(0, iniObj.getDefaultValues());
			iniObj.writeLogFile("Default values", 1, 0);
		}

		//Check for validation
		if (!iniObj.validateFile())
		{
			std::cout << "Validation failed after the user's input" << std::endl;
				
			//If it's not validated write default values
			iniObj.writeLogFile("Validation failed after the user's input", 1, 0);
			iniObj.readFile();
			iniObj.writeFile(0, iniObj.getDefaultValues());
			iniObj.writeLogFile("Default values", 1, 0);
		}
		else
		{
			//Validation succeed
			std::cout << "Validation completed successfully after the user's input " << std::endl;
			iniObj.writeLogFile("Validation completed successfully after the user's input ", 1, 0);
		}
	
}


	return 0;
}


//Try for basic parameters string to double per comma
// 
//	std::string s ="100,2,3,45,6";
//	std::string n{  };
//	std::vector<std::string> sV{ 8 };
//	int i = 0;
//	int a = 0;
//	int previous_a = 0;
//	while (i < s.size()) 
//	{
//		//find the comma
//		auto it = std::find(s.begin(), s.end(), ',');
//		//std::cout << *it << std::endl;
//		//store the index
//		previous_a = a;

//		a = it - s.begin();
//		if (a != s.size()) {
//			s.replace(s.begin() + a, s.begin() + a +1 , "");
//			std::cout << n << std::endl;
//			sV.push_back(n);
//			std::cout << sV[i] << std::endl;
//		}
//		i++;

//	}
//	std::cout << "HERE" << std::endl;
//		
//}
