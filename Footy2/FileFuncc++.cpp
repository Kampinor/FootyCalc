#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
int ExportinCSV(vector<string> vecToPrint, string strPath);
vector<string> ReadstrVecFromFile(string strFilename);

int WriteInputinVector() {
	string strEingabe, strSol, strAns;
	

	// Obtain user input
	cout << "Bitte etwas eingeben: ";
	getline(cin, strEingabe);

	cout << strEingabe;


	return 0;
}

int ExportinCSV(vector<string> vecToPrint, string strPath) {

	// Open the CSV file for writing
	ofstream file(strPath);

	if (file.is_open()) {
		//cout << "File " << strPath << " loaded successfully." << endl;

		for (const auto& element : vecToPrint) {
			file << element << "\n";
		}

		cout << "Vector saved to " + strPath << endl;
		// Close the file
		file.close();
		return 0;

	}
	else {
		cerr << "Error loading file " << strPath << "." << endl;
		// Close the file
		file.close();
		return 1;
	}

}

vector<string> ReadstrVecFromFile(string strFilename) {
		ifstream file(strFilename);

		if (!file.is_open()) {
			cerr << "Error opening file " << strFilename << std::endl;
		}

		vector<string> strcontent;
		string strline;

		while (getline(file, strline)) {
			strcontent.push_back(strline);
		}

		file.close();

		// Print the content of the vector
		for (const auto& str :strcontent) {
			cout << str << std::endl;
		}

		return strcontent;

}


