
#include <iostream>
#include <vector>
#include <cmath>
#include <string.h>
using namespace std;

struct tAsianHand{ //für Gebrauch im ganzen Code

 double qSieg;
 double qRemis;
 double qNied;
 double qDNB1;
 double qDNB2;
 double qAHC_1_15;
 double qAHC_2_15;
};

int fakultaet(int iNumber);
double poisson_probability(double dMean,int iAnz);
double getKumValue(double dMean,int iMax);
//int pow(iZahl,iExp);
//double eTest = exp(1);
vector <double> getp_Over_Under(double dOffH, double dOffA);
void getq_Over_Under(vector <double> dToreIngKum);
void berechneTorP(double dOff, double dTore[], double dToreKum[], int size);
tAsianHand get_1X2_Q(double dOffH,double dOffA);



int main()
{

  //hier team als klasse interesante:
  //dTH,dGTH, methods stärken errechene
  //offensiv und defensivwerte errechen
  double dTFra =1.75,dGTFra = 1.25,dTLev =2.0,dGTLev =1.667;
  //Daten Heim
  double dOffH,dOffA,dDefH,dDefA;
  dOffH=(dTFra+dGTLev)/2;
  //dDefH=(dGTFra+dTLev)/2;

  //cout << "Off Fra: " << to_string(dOffH) << endl;
  //cout << "Def Fra: " << to_string(dDefH) << endl;

  //Daten Ausw
  dOffA=(dTLev+dGTFra)/2;
  //dDefA=(dGTLev+dTFra)/2;
  //cout << "Off Lev: " << to_string(dOffA) << endl;
  //cout << "Def Lev: " << to_string(dDefA) << endl;



  vector <double>  dToreIng,dToreIngKum;

cout << "Under/Over:" << endl;
 dToreIngKum = getp_Over_Under(dOffH,dOffA);
 getq_Over_Under(dToreIngKum);

 tAsianHand AktDaten;
 
 
 AktDaten = get_1X2_Q(dOffH,dOffA);
cout << "als struct" << endl;
cout << AktDaten.qSieg << "/" << AktDaten.qRemis << "/" << AktDaten.qNied << endl;
cout << "DNB:" << AktDaten.qDNB1 << "/" << AktDaten.qDNB2 << endl;
cout << "AHC1 -1.5: " << AktDaten.qAHC_1_15 << endl;
cout << "AHC2 -1.5: " << AktDaten.qAHC_1_15 << endl;

  return 0;
}

void getMatchOdds(string strH,string strA,double dOffH,double dOffA){

}


int fakultaet(int iNumber) {

	int iTotal =iNumber;

  if (iNumber <0) {
  	cout << "ungültige Zahl" << endl;
  	return 0;
  	}
  if (iNumber == 0) {
  	 return 1;
  	}
  else {
		//cout << to_string(iNumber) << endl;
		for (int i =iNumber-1;i >0;i--) {
		iTotal = iTotal * i;
		//cout << to_string(iTotal) << endl;
		}
	}

	return iTotal;
}

// Poisson Wahrscheinlichkeit für ein Tor ereignis anhand 
// der Durschnittswerte

double poisson_probability(double dMean,int iAnz) {
	double dProb;
	dProb = (pow(dMean,iAnz)/fakultaet(iAnz))*exp(-dMean);
    //cout << dProb<< endl;
  return dProb;

}

//***
//Brief: Berechnung der kumulierten Warscheinlichkeit, dass bis zu iMax Tore fallen
//Detail: Errechnet die Poisson-Warscheinlichkeit jeder Toranzahl und Addiert alle zusammen.
//Par[IN]dMean: Durchschnitt
//Par[IN]iMax: maximale Anzahl an Toren 
//Return: Kummulierte Warscheinlichkeit z.B. 1 tor=1:0,0:1 
//***

double getKumValue(double dMean,int iMax)	{
	double dProb =0;
	for (int i = 0; i <=iMax;i++) {
		dProb= dProb + poisson_probability(dMean,i);
	}


  	return dProb;

  }
  
//***
//Brief: Berechnung der Over-Under Warscheinlichkeit
//Detail: Errechnet die kummulierten Poisson-Warscheinlichkeiten für 0,1 ...6 Tore im Spiel
//Par[IN]dOffH: Durchschnittlich erzielte Tore Heim
//Par[IN]dOffA: Durchschnittlich erzielte Tore Gast 
//Return: Vector mit den Warscheinlichkeiten für 0-6 Tore
//***  

vector <double> getp_Over_Under(double dOffH, double dOffA) {

vector <double> dToreIng,dToreIngKum;

	//cout << "für Over/Under" << endl;
//in Schleife
 for (int i =0;i<7;i++) {
 	if (i<6) {
 	dToreIng.push_back(poisson_probability(dOffA +dOffH,i));
dToreIngKum.push_back(getKumValue(dOffA+dOffH,i));
 	}
 	else {
 		dToreIng.push_back(1-dToreIngKum[i-1]);
 		dToreIngKum.push_back(dToreIng[i]);
 	}
 	//cout << "p= " << to_string(dToreIng[i]) << endl;
 	//cout << "kum= " << to_string(dToreIngKum[i]) << endl;
 	//cout << "Quote " << to_string(1/dToreIngKum[i]) << endl;
 }
 return dToreIngKum;
  }
  
  
//***
//Brief: Berechnung der Quoten für Over-Under 
//Detail: Errechnet aus den kummulierten Poisson-Warscheinlichkeiten für 0,1 ...6 Tore im Spiel die entsprechende Quote
//Par[IN]dToreIngKum: Vector mit den Warscheinlichkeiten für 0-6 Tore
//Return: Textoutput der Warscheinlichkeiten
//***

  
void getq_Over_Under(vector <double> dToreIngKum) {

vector <double> dAktOver,dAktUnder,dUndGanz,dOverGanz;
double pAkt;
//Schleife over/Under
for (int i =0;i<6;i++) {
	//cout << to_string(dToreIngKum[i]) << endl;
	pAkt = dToreIngKum[i];
	dAktUnder.push_back(1/pAkt);
	cout << "Under " << to_string(i) << ".5: " <<dAktUnder[i];
	dAktOver.push_back(1/(1-pAkt));
	cout << "/ Over " << to_string(i) << ".5: " << dAktOver[i] << endl;


}

}

void berechneTorP(double dOff, double dTore[], double dToreKum[], int size) {
    for (int i = 0; i < size; i++) {
        if (i < size - 1) {
            dTore[i] = poisson_probability(dOff, i);
            dToreKum[i] = getKumValue(dOff, i);
        } else {
            dTore[i] = 1 - dToreKum[i - 1];
            dToreKum[i] = dTore[i];
        }
    }
}

//***
//Brief: Berechnung der 3Weg Warscheinlichkeit
//Detail: Errechnet die kummulierten Poisson-Warscheinlichkeiten für 0,1 ...6 Tore im Spiel
//Par[IN]dOffH: Durchschnittlich erzielte Tore Heim
//Par[IN]dOffA: Durchschnittlich erzielte Tore Gast 
//Return: Vector mit den Warscheinlichkeiten für 0-6 Tore
//***  

tAsianHand get_1X2_Q(double dOffH,double dOffA) {
 tAsianHand AktDaten;
   //Wahrscheinlichkeit Tore jeTeam/Tabelle nachbauen

 double dToreH[10], dToreHKum[10];
 berechneTorP(dOffH, dToreH, dToreHKum, 10);
    
 double dToreA[10], dToreAKum[10];
 berechneTorP(dOffA, dToreA, dToreAKum, 10);

    //for (int i = 0; i < 10; i++) {
    //    cout << "p= " << to_string(dToreA[i]) << endl;
    //    cout << "kum= " << to_string(dToreAKum[i]) << endl;
    //}
 


  //Berechning Standard 1x2
  double dSieg,dRemis,dNied;
  double dP_AHC_1_1,dP_AHC_1_2;
  double dP_AHC_0,dP_AHC_1;
  double dP_AHC_0_1,dP_AHC_0_2;

  for (int i=0;i<9;i++) {
  	int iAktAnzTore = i;
  	for (int z=0;z<9;z++) {
  		int iAnzG = z;
  		if (i > z) {
  			dSieg = dSieg + dToreH[i] * dToreA[z];
  			//cout << dSieg << endl;
  		}
  		if (i-1 > z) { // AHC 1 -1.5
  				dP_AHC_0_1 = dP_AHC_0_1 + dToreH[i] * dToreA[z];
  			}
  		if (i+1 > z) { // 1x
  				dP_AHC_1_1 = dP_AHC_1_1 + dToreH[i] * dToreA[z];
  			}
  		if (i == z) {
  			dRemis = dRemis + dToreH[i] * dToreA[z];
  		}
  		if (i < z) {
  			dNied = dNied + dToreH[i] * dToreA[z];
  		}
  		if ( z-1 > i) { //AHC 2 -1.5
  				dP_AHC_0_2 = dP_AHC_0_2 + dToreH[i] * dToreA[z];
  			}
  		if (z+1 > i) { // x2
  				dP_AHC_1_2 = dP_AHC_1_2 + dToreH[i] * dToreA[z];
  			}
  	}
  }

  //+ 0.5 1 ->1X
  dP_AHC_0 = dSieg + dRemis;
  //cout << 1/dP_AHC_0 << endl;
  // +0.5 2 ->X2
  dP_AHC_1 = dNied + dRemis;
  //cout << 1/dP_AHC_1 << endl;



  //1x2 quoten errechnen
  //cout << 1/dSieg << "/" << 1/dRemis << "/" << 1/dNied << endl;
  //cout << 1/dP_AHC_0_1 << endl;
  //cout << 1/dP_AHC_0_2 << endl;
  //cout << 1/dP_AHC_1_2 << endl;
     //cout << dSieg + dRemis + dNied << endl;

  //dnb errechen
  double dNenn,dDNB1,dDNB2;
  dNenn = dSieg + dNied;
  dDNB1 = dSieg/dNenn;
  dDNB2 = dNied/dNenn;
  //cout << 1/dDNB1 << "/" << 1/dDNB2 << endl;
  AktDaten.qSieg = 1/dSieg;
  AktDaten.qRemis = 1/dRemis;
  AktDaten.qNied = 1/dNied;
  AktDaten.qDNB1 = 1/dDNB1;
  AktDaten.qDNB2 = 1/dDNB2;
  AktDaten.qAHC_1_15 = 1/dP_AHC_0_1;
  AktDaten.qAHC_2_15 = 1/dP_AHC_0_2;
    return AktDaten;
}