#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <numeric>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>
#include <math.h>

class Team;  // Forward declaration of the Team class
double compute_average(const std::vector<double>& v);
void addValuetoAvgList(std::vector<double>& vData, double dnewValue);
void ReadCSV(const std::string& filename, std::map<std::string, Team*>& teamMap);
int PoissonRandom(double mean);
void SimulateGame(Team* team1, Team* team2,double dTH, double dTA,double dxGH,double dxGA);

std::default_random_engine generator(static_cast<unsigned int>(std::time(nullptr))); // Initialize once globally
int iAnzS=0,iAnzO=0;


class Team {
public:
    std::string name;
    std::vector<double> dTH;
    std::vector<double> dTA;
    std::vector<double> dxGH;
    std::vector<double> dxGA;
    std::vector<double> dGTH;
    std::vector<double> dGTA;
    std::vector<double> dxGGH;
    std::vector<double> dxGGA;

    Team(const std::string& teamName) : name(teamName) {}

    double getdTH() { return compute_average(dTH); }
    void setdTH(double dValue) { addValuetoAvgList(dTH, dValue); }

    double getdTA() { return compute_average(dTA); }
    void setdTA(double dValue) { addValuetoAvgList(dTA, dValue); }

    double getdxGH() { return compute_average(dxGH); }
    void setdxGH(double dValue) { addValuetoAvgList(dxGH, dValue); }

    double getdxGA() { return compute_average(dxGA); }
    void setdxGA(double dValue) { addValuetoAvgList(dxGA, dValue); }

    double getdGTH() { return compute_average(dGTH); }
    void setdGTH(double dValue) { addValuetoAvgList(dGTH, dValue); }

    double getdGTA() { return compute_average(dGTA); }
    void setdGTA(double dValue) { addValuetoAvgList(dGTA, dValue); }

    double getdxGGH() { return compute_average(dxGGH); }
    void setdxGGH(double dValue) { addValuetoAvgList(dxGGH, dValue); }

    double getdxGGA() { return compute_average(dxGGA); }
    void setdxGGA(double dValue) { addValuetoAvgList(dxGGA, dValue); }

    void Display() {
        std::cout << "Team name: " << name << std::endl;
    }
};

int main() {
    // Create instances of Team
    std::vector<Team*> teams = {
        new Team("SVW"), new Team("FCB"), new Team("LEV"), new Team("RBL"), 
        new Team("WOL"), new Team("FCH"), new Team("STU"), new Team("BOC"), 
        new Team("FCA"), new Team("GLB"), new Team("TSG"), new Team("SCF"), 
        new Team("BVB"), new Team("KOL"), new Team("UNI"), new Team("MAI"),
        new Team("SVD"), new Team("FRA")
    };

    // Create a map of Team pointers
    std::map<std::string, Team*> teamMap;
    for (auto& team : teams) {
        teamMap[team->name] = team;
    }

    ReadCSV("Data-23-24.csv", teamMap);

    // Access and display team names from the map
    //for (const auto& entry : teamMap) {
    //    std::cout << "Team key: " << entry.first << ", ";
    //    entry.second->Display();
    //}

    // Clean up memory (delete the instances)
    for (auto& team : teams) {
        delete team;
    }
   //double dErg=iAnz/306
    
        std::cout <<"Erg" <<(double)iAnzS/(306-45)<< std::endl;
        std::cout <<"Over"<<(double)iAnzO/(306-45)<<std::endl;

    return 0;
}

double compute_average(const std::vector<double>& v) {
    if (v.empty()) {
        return 0.0; // Handle empty vector case
    }
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    return sum / v.size();
}

void addValuetoAvgList(std::vector<double>& vData, double dnewValue) {
    if (vData.size() >= 10) { // Use >= to ensure we only keep last 7 elements
        vData.erase(vData.begin());
    }
    vData.push_back(dnewValue);
}

void ReadCSV(const std::string& filename, std::map<std::string, Team*>& teamMap) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << filename << std::endl;
        return;
    }

    int i = 1;
    std::string line;
    std::string strTeamH,strTeamA;
    double dTH=0,dTA=0,dxGH=0, dxGA=0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> allValues;

        // Reading values and adding them to the vector
        while (std::getline(ss, cell, ';')) {
            allValues.push_back(cell);
        }

        // Ensure enough columns are present
        if (allValues.size() < 10) {
            std::cerr << "Invalid row in CSV file." << std::endl;
            continue;
        }

        // Werte abgreifen
        try{
        strTeamH = allValues[4];
        strTeamA = allValues[9];
        dTH =std::stod(allValues[6]); 
        dTA = std::stod(allValues[7]);
        dxGH = std::stod(allValues[5]); 
        dxGA = std::stod(allValues[8]);}
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid data in CSV file: " << e.what() << std::endl;
            continue;
        }
        //std::cout << strTeamH << ":" << strTeamA << std::endl;

        // Teams bestimmen
        Team* TeamH = teamMap[strTeamH];
        Team* TeamA = teamMap[strTeamA];
        
        // Check if the teams are found in the map
        if (TeamH == nullptr || TeamA == nullptr) {
            std::cerr << "Team not found in map: " << strTeamH << " or " << strTeamA << std::endl;
            continue;
        }

        // Simulation
        if (i > 54 && TeamH->getdGTH() > 0.0 && TeamA->getdGTA() > 0.0) {
            SimulateGame(TeamH, TeamA, dTH, dTA,dxGH,dxGA);
        }

        // Update team data
        if (TeamH) {
            TeamH->setdTH(dTH);
            TeamH->setdxGH(dxGH);
            TeamH->setdGTH(dTA);
            TeamH->setdxGGH(dxGA);
        }
        if (TeamA) {
            TeamA->setdTA(dTA);
            TeamA->setdxGA(dxGA);
            TeamA->setdGTA(dTH);
            TeamA->setdxGGA(dxGH);
        }

        i++;
        // std::cout << strTeamH << " vs " << strTeamA << std::endl;
    }

    file.close();
}

int PoissonRandom(double mean) {
    static std::default_random_engine generator(static_cast<unsigned int>(std::time(nullptr))); // Initialize once
    std::poisson_distribution<int> distribution(mean);
    return distribution(generator);
}

void SimulateGame(Team* team1, Team* team2,double dTH, double dTA,double dxGH,double dxGA) {
    //Heimtore/Auswärtsgg
    double dsH = ((team1->getdTH() + team1->getdxGH()*2)/3 + (team2->getdGTA()+ team2->getdxGGA()*2)/3) / 2;
    //Auswärtstore/Heimggt
    double dsA = ((team2->getdTA()+ team2->getdxGA()*2)/3 + (team1->getdGTH()+ team1->getdxGGH()*2)/3) / 2;
    double dSumA=0,dSumH=0;
    for (int i=1;i<=1000;i++){
      dSumA+=PoissonRandom(dsA);
      dSumH+=PoissonRandom(dsH);
    }
    double goalsTeam1 =(dTH+dxGH*2)/3;
    double goalsTeam2 =(dTA+dxGA*2)/3;
    //meins 48, Tore 37,
    // runden
    int iSumA=dSumA/1000,iSumH=dSumH/1000;
    int igoalsTeam1=goalsTeam1,igoalsTeam2=goalsTeam2;
    
    //std::cout << "est Game result: " << team1->name << " " << iSumH
            //  << " - " <<iSumA<< " " << team2->name << std::endl;

    //std::cout << "Game result: " << team1->name << " " <<igoalsTeam1
         //     << " - " << igoalsTeam2 << " " << team2->name << std::endl;

     if (igoalsTeam1 > igoalsTeam2 && iSumH>iSumA) {
         iAnzS+=1;
    //     std::cout << team1->name << " wins!" << std::endl;
    } else if (igoalsTeam1 < igoalsTeam2 &&iSumH<iSumA) {
    //     std::cout << team2->name << " wins!" << std::endl;
        iAnzS+=1;
    } else if (igoalsTeam1==igoalsTeam2&&iSumH==iSumA) {
        iAnzS+=1;
      } else { //flasch
    //     std::cout << "It's a draw!" << std::endl;
    }
    
    if(igoalsTeam2+igoalsTeam1 ==iSumH+iSumA){
       iAnzO+=1;
    }
    
    //tatsächliche Ergebnisse als Parameter, Abgleich und ausgabe
}
    