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

// get_1X2_Q(dOffH,dOffA,AktDaten);
 //AktDaten = get_1X2_Q(dOffH,dOffA);
// cout << "als struct" << endl;
// cout << AktDaten.qSieg << "/" << AktDaten.qRemis << "/" << AktDaten.qNied << endl;
// cout << "DNB:" << AktDaten.qDNB1 << "/" << AktDaten.qDNB2 << endl;
// cout << "AHC1 -1.5: " << AktDaten.qAHC_1_15 << endl;
// cout << "AHC2 -1.5: " << AktDaten.qAHC_1_15 << endl;

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

// kumulierte Poisson Wahrscheinlichkeit für ein Ereignis
// z.B. 1 tor=1:0,0:1 

double getKumValue(double dMean,int iMax)	{
	double dProb =0;
	for (int i = 0; i <=iMax;i++) {
		dProb= dProb + poisson_probability(dMean,i);
	}


  	return dProb;

  }

//?

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
  
  
 //over under quoten aus Wahrscheinlichkeiten ermitteln  

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
