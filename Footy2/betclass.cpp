#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

class Match {
public:
    string strTeamA, strTeamB;
    double dOffH, dOffA;
private:
    
    vector<double> OverUnder, Result;

public:
    Match(string strA, string strB, double dlamA, double dlamB) {
        strTeamA = strA;
        strTeamB = strB;
        dOffH = dlamA;
        dOffA = dlamB;
    }

    string getName(int iType) {
        if (iType == 0) {
            return strTeamA;
        } else {
            return strTeamB;
        }
    }

    vector<double> getOver_Under() {
        return OverUnder;
    }

    vector<double> getResult() {
        return Result;
    }

private:
    int fakultaet(int iNumber) {
        int iResult = 1;
        for (int i = 1; i <= iNumber; i++) {
            iResult *= i;
        }
        return iResult;
    }

    double poisson_probability(double dMean, int iAnz) {
        return pow(dMean, iAnz) * exp(-dMean) / fakultaet(iAnz);
    }

    double getKumValue(double dMean, int iMax) {
        double dKumValue = 0.0;
        for (int i = iMax; i >= 0; i--) {
            dKumValue += poisson_probability(dMean, i);
        }
        return dKumValue;
    }

    void get_Over_Under() {

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
            dTore[i] = poisson_probability(dOff, i);
            dToreKum[i] = getKumValue(dOff, i);
        }
    }
};
    
int main(){
   Match oMatch1("FRA","LEV",(1.75+1.667)/2,(2+1.25)/2);
   cout << oMatch1.dOffH << endl;
   cout << oMatch1.dOffA << endl;
   oMatch1.getOver_Under();
}    
    