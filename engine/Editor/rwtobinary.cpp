#include "rwtobinary.h"

#include <wx/log.h>

/*main()
{
	Student me(11321, "Myself", 4.3);
	ofstream myfile;
	myfile.open("silly.dat", ios::binary | ios::out);
	me.save(myfile);
	myfile.close();
	return(0);
}*/

Student::Student()
{
}

void Student::saveToBinary(std::string fileName)
{
	Data myData;
	myData.age = 29;
	myData.name = "David";
	myData.randomNumber = 5.78f;

	FILE *file = fopen(fileName.c_str(), "wb");
	fwrite(&myData, sizeof(Data), 1, file);
	fflush(file);
	fclose(file);
}

void Student::loadFromBinary(std::string fileName)
{
	Data myData;

	FILE *file = fopen(fileName.c_str(), "rb");
	fread(&myData, sizeof(Data), 1, file);
	fclose(file);

	wxLogMessage("File contains = %i, %s, %f!", myData.age, myData.name, myData.randomNumber);
}