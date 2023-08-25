#include <iostream>
#include <string>
#include <vector>
//#include <cctype> for Lcase.. Zeichencheck
using namespace std;


vector<string> Split(string strtosplit, char separator);
vector<string> Split(string strtosplit, char separator, int iMaxIndex, int iMaxChar); //in C++ auto erkannt
int CheckifLetter(char cCheck, int iModus);
bool CheckifNumeric(char cCheck, int iModus);
bool CheckifStrNumeric(string strCheck, int Modus);
string VBAReplace(string strChange, string strPat, string strRep, int iStart, int iCount);
string trim(string strtoclean);
int AltLen(string strCheck);



//***
//Brief: String aussplitten
//Detail: Läuft den eingegeben string char pro char durch und teilt in auf, sobald der separator endeckt wurde
//Par[IN]strtosplit: string der gesplittet werden soll
//Par[IN]separator: char an dem gesplittet werden soll
//Return: string-vector
//***

vector<string> Split(string strtosplit, char separator) { 
	int i = 0;
	vector<string> NewVector;
	
	// Temporary string used to split the string. 
	string strAkt; 
	while (strtosplit[i] != '\0') { 
		if (strtosplit[i] != separator) { 
			// Append the char to the temp string. 
			strAkt += strtosplit[i]; 
			} 
		else { 
			 //cout << s << endl;
			 NewVector.push_back(strAkt);
			 strAkt.clear(); 
			 } 
			 i++;
	} // Output the last stored word. 
	//cout << s << endl;
	NewVector.push_back(strAkt);
  strAkt.clear(); 
	return NewVector;
} 

//***
//Brief: String aussplitten (erweiterte Optionen)
//Detail: Teilt den angegeben string enstsprechend auf. Läuft den eingegeben string char pro char durch und teilt in auf, sobald der separator endeckt wurde. Ist die maximale Indexanzahl erreicht wird die Teilung beendet. 
//Par[IN]strtosplit: string der gesplittet werden soll
//Par[IN]separator: char an dem gesplittet werden soll
//Par[IN]iMaxIndex: Maximale Anzahl an unterteilungen
//Par[IN]iMaxChar: Index des Chars bis wohin gesplittet werden soll
//Return: string-vector
//***

vector<string> Split(string strtosplit, char separator, int iMaxIndex, int iMaxChar) {
	int i = 0;
	int iVecSize = 0;
	vector<string> NewVector;
	if (iMaxChar > 0) {
		strtosplit.substr(0, iMaxChar);
	}

	// Temporary string used to split the string.
	string strAkt;
	while (strtosplit[i] != '\0') {
		if (strtosplit[i] != separator) {
			// Append the char to the temp string.
			strAkt += strtosplit[i];
		}
		else {
			NewVector.push_back(strAkt);
			strAkt.clear();
		}
		i++;
		iVecSize = NewVector.size();
		if (iVecSize == iMaxIndex) {
			return NewVector;
		}
	}
	NewVector.push_back(strAkt);
	strAkt.clear();
	return NewVector;
}

//***
//Brief: Überprüfen, ob Buchstabe
//Detail: Überprüft den eingegeben char, ob Buchstabe 
//Par[IN]cCheck: char der überprüft werden soll
//Par[IN]iModus: 0 -> mit Ausgabe/1 -> ohne
//Return: Bool ob Buchstabe
//***

int CheckifLetter(char cCheck, int iModus) {
	if((cCheck>='a' && cCheck<='z') || (cCheck>='A' && cCheck<='Z')) {
		if (iModus == 0) {
		  cout<<endl<<cCheck<<" is an Alphabet";
		}
		return 0;	
  } 
  else {
  	if (iModus ==0) {
      cout<<endl<<cCheck<<" isn't an Alphabet";      
  	//cout<<endl;
  	 }
  	return 1; 
  }
  
} 

//***
//Brief: Überprüfen, ob Zahl
//Detail: Überprüft den eingegeben char, ob Zahl
//Par[IN]cCheck: char der überprüft werden soll
//Par[IN]iModus: 0 -> mit Ausgabe/1 -> ohne
//Return: Bool ob Zahl
//***

bool CheckifNumeric(char cCheck, int iModus) {
	if (isdigit(cCheck) == false) {
		if (iModus ==0){
			cout << "input ist keine Zahl";
			}
		return false;
	} 
	if (isdigit(cCheck) == true) {
		if (iModus ==0){
			cout << "input ist Zahl";
			}
		return true;
	} 		
} 

//***
//Brief: Überprüfen, ob string numerisch
//Detail: Überprüft die einzelnen chars des eingegebenen strings ob zahl, wenn alle Zahl , dann ist der string numerisch
//Par[IN]strCheck: string analysiert werden soll
//Par[IN]iModus: 0 -> mit Ausgabe/1 -> ohne
//Return: Bool ob string
//***

bool CheckifStrNumeric(string strCheck, int iModus) {
	int iLen =0, i =0;
	iLen = strCheck.length();
	bool bTest = false;
	char cCheck;
	
	for (int i =0;i<iLen;i++) {
		//cCheck = strCheck[i];
		//cout << cCheck + to_string(i) + "\n";
		if (isdigit(strCheck[i]) ==false) {
			if (iModus == 0) {
				cout << "ist nicht numerisch!";
				} 
			return false;
			} 
		} 
	
	if (iModus == 0) {
	  cout << "ist numerisch";
	} 
	return true;
	
	
} 	



//***
//Brief: Überprüfen, ob string im string
//Detail: Teilt den angegeben string enstsprechend auf. Läuft den eingegeben string char pro char durch und teilt in auf, sobald der separator endeckt wurde. Ist die maximale Indexanzahl erreicht wird die Teilung beendet. 
//Par[IN]iStart: Index an dem die Suche gestartet werden soll
//Par[IN]strInput: zu überprüfender String
//Par[IN]strSearch: string nach dem gesucht werden soll
//Return: index wo gefunden, sonst -1
//***


int instr(int iStart, string strInput, string strSearch) {
	int iLen = strInput.length() - iStart;
	string strCheck = strInput.substr(iStart, iLen);
	if (strCheck.find(strSearch) != strCheck.npos) {
		return strCheck.find(strSearch);
	}
	else {
		return -1;
	}

}

//***
//Brief: Teile eines strings ersetzen
//Detail: Zunächst wird die Länge des strings gemäß des Startindexes angepasst. 
//Detail: Danach wird das gesuchte Muster solange ersetzt wie es gefunden wird, solange keine Anzahl angegeben wurde
//Par[IN]strChange: string der angepasst werden soll
//Par[IN]strPat: string der ersetzt werden soll
//Par[IN]strRep: string mit dem ersetzt werden soll
//Par[IN]iStart: Index an dem die Suche gestartet werden soll //0 = Anfang des Wortes
//Par[IN]iStart: Anzahl an ersetzungen //0 = Alle 
//Return: angepasster string
//***

string VBAReplace(string strChange, string strPat, string strRep, int iStart, int iCount) {
	int i = 0, iRow = 0;
	//int iLen = strPat.length;
	int iLen = AltLen(strPat);
	strChange = strChange.substr(iStart, strChange.length() - iStart);
	i = instr(0, strChange, strPat);
	while (i != -1) {
		//cout << to_string(i) << endl;
		cout << strChange << endl;
		strChange.replace(i, iLen, strRep);
		i = instr(0, strChange, strPat);
		iRow += 1;
		if (iRow == iCount) {
			goto Ende;
		}
		cout << to_string(iRow) << endl;
	}

Ende:
	//cout << strtoclean << endl;
	return strChange;

}

//wie vba replace nur mit leerzeichen
string trim(string strtoclean) {
	int i = 0;
	string strNew;
	//cout << strtoclean << endl;
	i = instr(0, strtoclean, "  ");
	while (i != -1) {
		cout << to_string(i) << endl;
		cout << strtoclean << endl;
		strtoclean.replace(i, 2, " ");
		i = instr(0, strtoclean, "  ");
	}

	//cout << strtoclean << endl;
	return strtoclean;

}


int AltLen(string strCheck) {
	int i, count = 0;
	for (i = 0; strCheck[i]; i++) {
		count++;
	}

	return count;
}