// Footy2.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <iostream>
//#include <ifstream> //zum einlesen von Dateien 
#include <fstream>
#include <string>
#include <vector> //vector-klasse
#include "strfuncsc++.cpp"
#include "betfuncs.cpp"
#include "FileFuncc++.cpp"
#include <stdlib.h>
#include "sqlite3.h"//sqlite3.h>
//#include "mysql_connection.h"
//#include <cppconn/driver.h>
//#include <cppconn/exception.h>
//#include <cppconn/prepared_statement.h>
using namespace std;


//Funcs
string GetMatchDate(string strline);
string GetMatchDay(string strline);
string GetTeams(string strline);
string ReadData(string strPath,int iMatch);
int GetFromDB();
string GetPlayerData(string strline,string strTeams);
string GetTeamID(string strTeamName);
string GetTeamData(string strline, string strTeams, int iRound);
string GetResultSQL(string strTeamDataH, string strTeamDataA, int iMatch, string strMLiga);

//Vars
int iMatch = 0;

//Funktion zum Einlesen 1 ST, 6, alle 
//Spieltag pro Datei

int main() {
	//cout << "Version 1.0.0" << endl;
	cout << "Bitte Modus waehlen" << endl;
	cout << "(0) Daten einlesen (1) Quoten anzeigen (2) Abbruch" << endl;
	int iWahl = 0, iErg = 0, iHighIndex = 27; //iHighIndex dynamisch in Funktion bestimmen
	string strPath, strSQLResults,strSDay;
	vector<string> strSkript;
	int iMatch = iHighIndex + 1, j = 1;
	cin >> iWahl; 
	switch (iWahl)
	{
	case 0:
		cout << "Bitte Spieltag wählen" << endl;
		cin >> strSDay;
		for (int i = iHighIndex + 1; i < iHighIndex + 10; i++) {
			strPath = "BL_2023_2024//BL-2023-2024-" + strSDay + "-0" + to_string(j) + ".txt";
			strSQLResults = ReadData(strPath, iMatch);
			strSkript.push_back(strSQLResults);
			iMatch++;
			j++;
		}
		ExportinCSV(strSkript, "test2.txt");
		//ReadData();
		break;
	case 1:
		//GetQuotes();
		//WriteInputinVector();
		//iErg = GetFromDB();

		break;
	case 2:
		return 1;
	default:
		break;
	}

}

string ReadData(string strPath, int iMatch) {
	string strline,strAkt, strTeams,strSQLResults;
	string strTeamDataH, strTeamDataA, strResultSQL, strPlayerSQL;
	string strMLiga;
	int iMatchID = iMatch;
    //strPath = "test.txt";
	//Dynamisch
	//cout << "Bitte Dateinamen eingeben" << endl;
	//cin >> strPath;
	//strPath = "BL_2023_2024//" + strPath + ".txt";
	int iAllPlayers = 0;
	int iAktLine = 0, iAktPlayer = 0,iRound = 0;
	vector <string> strSplit;
	vector <string> strSkript;
	ifstream myfile(strPath);
	int iNext = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, strline))
		{
			iAktLine++; // Wo bin ich gerade?
			if (instr(0, strline, "venue") > -1) {
				// Datum + Uhrzeit abgreifen
				string strMDate = GetMatchDate(strline);
				//cout << strMDate << endl;
			}

			if (instr(0, strline, "Matchweek") > -1) {
				// Liga + Spieltag abgreifen
				strMLiga = GetMatchDay(strline);
				//cout << strMLiga << endl;
			}

			if (instr(0,strline,"vs") > -1 ) {
				//iMatchID++;
				

				// Teams abgreifen
				if (instr(0, strline, "Report") == -1) {
					strTeams = GetTeams(strline);
				}
				
				//vector <string> vTeams = Split(strTeams, ';');
				//string strIDH = vTeams[0];
				//string strIDA = vTeams[1];

				//Funktion zum IDs bestimmen
				//cout << strTeams << endl;
			}

			if (instr(0, strline, "Players") > -1) { //alle
				//cout << strline << '\n';
				//cout << "size;" << strSkript.size() << endl;
				//ist 0 Fehler beheben mit übergabe pointer? oder vector als rückgabewert
				iAllPlayers = 0;
				
				
				if (iRound == 0) { //gibt keine Rückgabe?
					strTeamDataH = GetTeamData(strline, strTeams, iRound);
					iRound = 1;
				}
				else {
					strTeamDataA = GetTeamData(strline, strTeams, iRound);
					strSQLResults = GetResultSQL(strTeamDataH, strTeamDataA,iMatchID,strMLiga);
					//strSkript.push_back(strSQLResults);
					//strResultSQL = GetResultSQL(strTeamDataH, strTeamDataA);
					iRound = 0;
				}


			}

			if (iAllPlayers == 1) { //Player //vorher 5
				//Umlaute nur in comsole falsch
				//"├®" angezeigt aber nicht gefunden, é dagegen schon 
				//if (instr(0, strline, "é") > 0) {
				//	cout << "found" << endl;
				//}
				//strline = VBAReplace(strline, "é", "e", 0, -1); //ersetzen möglich
				//cout << strline << endl;
				//vector <string> stroutput;
				//stroutput.push_back(strline);
				//ExportinCSV(stroutput, "out.txt");

				// \121\Q \128\X, 
				
				//cout << "gr\131\er";


				if (strSkript.size() < 1) {
					strSkript.push_back("--Skriptstart");
				}
				strPlayerSQL =GetPlayerData(strline, strTeams);
				strSkript.push_back(strPlayerSQL);

				iAktPlayer++;
			}
			

			if (instr(0, strline, "Player 	# 	Nation") > -1) { //Playerliste Start
				iAllPlayers = 1;
			}
			//strAkt =
			//strSkript.push_back(strAkt);
			//Alternative zu Direktverbindung: Schreiben eines SQL-Statements
			
		}
		myfile.close();
		//ExportinCSV(strSkript, "test2.txt");
		return strSQLResults;
	}

	else cout << "Unable to open file";

	return "";

}

string GetMatchDate(string strline) {
	vector <string> strSplit;
	strSplit = Split(strline, ' ');
	string strWTag = strSplit[0];
	string strDatum, strUhr, strDate,strJahr;
	strDatum = strSplit[2];
	strDatum = strDatum.substr(0, 1);
	//Komma bei Jahr weg
	strJahr = strSplit[3];
	strJahr = strJahr.substr(0, strJahr.length() - 1);
	strDatum = strDatum + "-" + strSplit[1] + "-" + strJahr;
	strUhr = strSplit[4];
	strDate = strDatum + ";" + strUhr;
	return strDate;
}

string GetMatchDay(string strline) {
	vector <string> strSplit;
	strSplit = Split(strline, ' ');
	string strLiga = strSplit[0], strMDay = strSplit[2];
	strMDay = strMDay.substr(0, strMDay.length() - 1);
	return strLiga + ";" + strMDay;
}

string GetTeams(string strline) {
	vector <string> strSplit;
	strSplit = Split(strline, ' ');
	//if (instr(0, strline, "Union") > -1) {
	//	cout << "test" << endl;
	//}
	//if (strSplit.size() == 0) {
		//strSplit = Split(strline, ' ');
	//}
	
	
	int iVS = -1;
	string strAkt;

	for (int i = 0; i < strSplit.size(); i++) {
		strAkt = strSplit[i];
		if (strAkt == "vs.") {
			iVS = i;
		}
	}

	
	// Bayer 04 vs Schalke
	// Bayer vs Schalke 04 -> 
	// Bayer 04 vs Schalke 04
	
	string strTeams;
	//Fehler hier 
	if (strSplit.size() == 5 && iVS ==2) { // Bayer 04 vs Schalke 04 -> s5 i2
		strTeams = strSplit[0] + " " + strSplit[1] + ";" + strSplit[3] + " " + strSplit[4];
		return strTeams;
	}
	if (strSplit.size() == 4 && iVS == 1) { // Bayer vs Schalke 04
		strTeams = strSplit[0] + ";" + strSplit[2] + " " + strSplit[3];
		return strTeams;
	}
	if (strSplit.size() == 4 && iVS == 2) { // Bayer 04 vs Schalke
		strTeams = strSplit[0] + " " + strSplit[1] + ";" + strSplit[3];
		return strTeams;
	}
	if (strSplit.size() == 3 && iVS == 1) { // Bayer vs Schalke
		strTeams = strSplit[0] + ";" + strSplit[2];
		return strTeams;
	}
	else {
		return "";
	}
	
	
}

string GetPlayerData(string strline, string strTeams) { //+ int für Team1/2

	
	vector<string> strSplit = Split(strline,'\t');
	//for (int i = 0; i < strSplit.size(); i++) {
	//for (int i = 0; i < 15; i++) {
		//cout << strSplit[i] << ";";	
	//}
	string strPlName, strMinPlay,strTore,strRot,strGelb,strXG;
	//if not in list -> AddNewPlayer
	strPlName = strSplit[0];
	//Player - ID bestimmen 
	string strPlayerID;
	//checken ob schon erfasst
	// falls nicht erfassen mit sql sonst ID auslesen
	// aus Datei lesen
	// + Match-ID bekommen aus file
	string strMatchID = "0";
	
	//TeamID bekommen
	vector<string> SplitTeams = Split(strTeams, ';');
	//cout << "size: " << strSplit.size() << endl;
	string strTeamID = GetTeamID(SplitTeams[0]);

	strMinPlay = strSplit[5];
	strTore = strSplit[6];
	strRot = strSplit[13];
	strGelb = strSplit[12];
	strXG = strSplit[18];

	//Statement bauen
	string strSQL;
	strSQL = "INSERT INTO Player_Match_Data VALUES (" + strPlayerID + "," + strMatchID + "," + strMinPlay + "," + strXG +  "," + strTore + "," + strRot + "," + strGelb + "," + strTeamID + ");";
	//cout << strSQL << endl;

	//strSkript.push_back(strSQL);
	//ExportinCSV(strSkript, "test2.txt");

	return strSQL;

}

string GetTeamID(string strTeamName) {
	if (strTeamName == "Eintracht Frankfurt") {
		return "0";
	}
	if (strTeamName == "Bayern Munich") {
		return "1";
	}
	if (strTeamName == "Dortmund") {
		return "2";
	}
	if (strTeamName == "RB Leipzig") {
		return "3";
	}
	if (strTeamName == "Union Berlin") {
		return "4";
	}
	if (strTeamName == "SC Freiburg" || strTeamName == "Freiburg") {
		return "5";
	}
	if (strTeamName == "Bayer Leverkusen") {
		return "6";
	}
	if (strTeamName == "Wolfsburg") {
		return "7";
	}
	if (strTeamName == "Mainz" || strTeamName == "Mainz 05") {
		return "8";
	}
	if (strTeamName == "Köln") {
		return "9";
	}
	if (strTeamName == "Gladbach" || strTeamName == "Mönchengladbach") {
		return "10";
	}
	if (strTeamName == "Hoffenheim") {
		return "11";
	}
	if (strTeamName == "Stuttgart") {
		return "12";
	}
	if (strTeamName == "Werder Bremen") {
		return "13";
	}
	if (strTeamName == "Bochum") {
		return "14";
	}
	if (strTeamName == "Augsburg") {
		return "15";
	}
	if (strTeamName == "Hertha") {
		return "16";
	}
	if (strTeamName == "Schalke 04") {
		return "17";
	}
	if (strTeamName == "Darmstadt" || strTeamName == "Darmstadt 98") {
		return "18";
	}
	if (strTeamName == "Heidenheim") {
		return "19";
	}
	else {
		return "XX";
	}

}

int GetFromDB() {
	sqlite3 *db; //DB starten
	char  *zErrMsg = 0;
	int rc =0;
	char* VFS = NULL;

	
	//rc = sqlite3_open("C:/Users/kampi/source/repos/Footy2/DB/footdb.db",&db);
	rc = sqlite3_open_v2("file///fottydb.db", &db,SQLITE_OPEN_READWRITE,VFS);

	if (rc) {
		fprintf(stderr, "Kann DB nicht oeffnen");
		return rc;
	}
	else {
		fprintf(stderr, "Kann DB nicht oeffnen");
		return rc;
	}
	sqlite3_close(db);

}

// Besimmen der Daten des gesamten Teams

string GetTeamData(string strline, string strTeams,int iRound) { //iRound für H/A?
	
	vector<string> strSplit = Split(strline, '\t');
	vector<string> strSpTeams = Split(strTeams, ';');
	string strStats;

	//strline = VBAReplace(strline, "\t", ";", 0, 0);
	//cout << strline << endl;
	//nach name index +4 weil leer

	string strAllPlay;
	if (iRound == 0) { //H
		cout << strline << endl;
		//Tore;xG
		//besser Team_ID?
		///strStats = strSpTeams[0] + ";" + strSplit[7] + ";" + strSplit[19];
		strStats = strSpTeams[0] + ";" + strSplit[7] + ";" + strSplit[19] + ";" + strSplit[13];
		cout << strStats << endl;
		
	}
	if (iRound == 1) { //A
		//strStats = strSpTeams[1] + ";" + strSplit[7] + ";" + strSplit[19];
		strStats = strSpTeams[1] + ";" + strSplit[7] + ";" + strSplit[19] + ";" + strSplit[13];;
		cout << strStats << endl;
	}

	return strStats;

}

string GetResultSQL(string strTeamDataH, string strTeamDataA, int iMatch, string strMLiga) { //strTeamDataH ist leer
	string strSQL;
	//Season ID/League ID dynamisch
	vector<string> strHeim = Split(strTeamDataH, ';');
	vector<string> strGast = Split(strTeamDataA, ';');

	// Trainer IDs bekommen später genauso wie team id in map einlesen
	vector<string> strCoaches = ReadstrVecFromFile("Coaches.csv");
	string strCH, strCA;

	for (int i = 0; i < strCoaches.size(); i++) {
		vector<string> strSplit = Split(strCoaches[i], ';');
		if (strSplit[0] == GetTeamID(strHeim[0])) {
			strCH = strSplit[0];
		}
		if (strSplit[0] == GetTeamID(strGast[0])) {
			strCA = strSplit[0];
		}
		//Korrigieren
	}

	//strMLiga //muss gesplittet werden
	vector<string> strMDay = Split(strMLiga, ';');
	//strHeim[3] für rote 

	//SQL bauen
	strSQL = "INSERT INTO Results Values (" + to_string(iMatch) + "," + GetTeamID(strHeim[0]) + "," + GetTeamID(strGast[0]) + ",0,0,"; //GameID/TeamID/Season,Liga
	strSQL= strSQL +strCH + "," + strCA + "," + strHeim[1] + "," + strGast[1] + "," + strHeim[2] + "," + strGast[2]; //Trainer/Tore/xG
	strSQL= strSQL + GetTeamID(strHeim[0]) + "," + GetTeamID(strHeim[0]) + "," + strMDay[1] + "," + strHeim[3] + "," + strGast[3] + ");"; //Stad-ID Dynamiscch!/Spieltag
	cout << strSQL << endl;
	return strSQL;
	//Stad-ID-fehlt!!
	//INSERT INTO Results Values (0-Matchid,13tID,1TID,0SOD,0LID,13CID,1CIID,0TH,4TA,0.6xGHH,2.913xGA,1Spieltagid);
}




// Programm ausführen: STRG+F5 oder Menüeintrag "Debuggen" > "Starten ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
