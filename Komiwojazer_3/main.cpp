#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <random>
#include "Pugixml/pugixml.hpp"

using namespace std;
using namespace pugi;
using namespace chrono;


vector<vector<double>> graph;
vector<vector<int>> population;
vector<int> solutionRoad;
vector<double> fitness;

int verNum = 0, solution = INT_MAX, start;

auto timeAverage = 0;

void ATSP(int start, int popSize, int popNum);
void dispVec(vector<int> toDisplay);
void dispVec(vector<double> toDisplay);
void dispPop();
vector<int> swap(vector<int> vec, int a, int b);
vector<int> shuffle(vector<int> vec);
vector<int> createBase(int start);
void createPop(vector<int> base, int size);
int calcCost(vector<int> path);
void calcFit();
void normFit();
vector<int> chooseOne(vector<vector<int>> pop, vector<double> fit);
vector<int> mutate(vector<int> vec, double rate);
void nextGen();



int main() {

    for (int i = 0; i < 100; i++) {

        //ładowanie pliku
        xml_document doc;
        if (!doc.load_file("../XML_files/ft53.xml")) return -1;
        xml_node gr = doc.child("travellingSalesmanProblemInstance").child("graph");

        //liczenie wierzchołków grafu
        for (auto &it1: gr) verNum++;

        //umieszczanie wag krawędzi w macierzy zrobionej z wektorów
        vector<double> helpMe;
        for (auto &it1: gr) {
            for (auto &it2: it1) {
                helpMe.push_back(it2.attribute("cost").as_double());
            }
            graph.push_back(helpMe);
            helpMe.clear();
        }


        //generowanie punktu startowego
        srand(time(NULL));
        start = (rand() % verNum);

        //rozpoczęcie pomiaru czasu
        auto t1 = steady_clock::now();


        ATSP(start, 20, 20);



        //zakończenie pomiaru czasu
        auto t2 = steady_clock::now();
        timeAverage = timeAverage + duration_cast<microseconds>(t2 - t1).count();


        population.clear();
        solutionRoad.clear();
        fitness.clear();
        verNum = 0;
        solution = INT_MAX;
    }

    printf("\nTime average for 100 tries equals: %dus\n", timeAverage / 100);

    return 0;
}



void ATSP(int start, int popSize, int popNum) {

    vector<int> base = createBase(start);       //tworzymy bazową, prostą ścieżkę przez wszystkie miasta
    createPop(base, popSize);              //tworzymy populację o rozmiarze popSize przemieszanych ścieżek

    for (int i = 0; i < popNum; i++) {        //tworzymy popNum generacji
        calcFit();                            //obliczamy fitness całej populacji
        normFit();                            //normalizujemy fitness
        nextGen();                            //tworzymy nową generację używając najlepszych osobników

        fitness.clear();
    }
//    printf("\n%d ", solution);               //wyświetlanie wyniku
//    dispVec(solutionRoad);
}

vector<int> createBase(int start) {
    vector<int> base;
    base.push_back(start);

    for (int i = 0; i <= verNum; i++) {
        if (i == start)
            continue;
        else if (i == verNum)
            base.push_back(start);
        else
            base.push_back(i);
    }

    return base;
}

vector<int> swap(vector<int> vec, int a, int b) {
    int temp = vec[a];
    vec[a] = vec[b];
    vec[b] = temp;
    return vec;
}
vector<int> shuffle(vector<int> vec) {
    for (int i = 0; i < verNum; i++) {

        int a = rand() % verNum;
        int b = rand() % verNum;

        if (a == 0 || a == verNum ||
            b == 0 || b == verNum ||
            a == b || b == a)
        {
            i--;
            continue;
        }
        else vec = swap(vec, a, b);
    }
    return vec;
}

void createPop(vector<int> base, int size) {
    for (int i = 0; i < size; i++) {
        vector<int> changed = shuffle(base);
        population.push_back(changed);
    }
}

void dispVec(vector<int> toDisplay) {
    printf("\n");
    for (auto &it : toDisplay)
        printf("%d ", it);
    printf("\n");
}
void dispVec(vector<double> toDisplay) {
    printf("\n");
    for (auto &it : toDisplay)
        printf("%f ", it);
    printf("\n");
}
void dispPop() {
    for (auto &it: population) {
        for (auto &it2 : it){
            printf("%d ", it2);
        }
        printf("\n");
    }
}


int calcCost(vector<int> path) {
    int cost = 0;
    for (int i = 0; i < path.size() - 1; i++)
        cost += graph[path[i]][path[i + 1]];
    return cost;
}
void calcFit() {
    for (int i = 0; i < population.size(); i++) {
        int d = calcCost(population[i]);
        if  (d < solution) {
            solution = d;
            solutionRoad = population[i];
        }
        fitness.push_back((double) 1/d);
    }
}
void normFit() {
    double sum = 0;
    for (double &it : fitness) sum += it;
    for (double &it : fitness) it /= sum;
}


void nextGen() {
    vector<vector<int>> newPop;
    for (int i = 0; i < population.size(); i++) {
        vector<int> help = chooseOne(population, fitness);
        help = mutate(help, 0.5);
        newPop.push_back(help);
    }

    population = newPop;
}
vector<int> chooseOne(vector<vector<int>> pop, vector<double> fit) {
    int i = 0;
    double j = (double) rand() / RAND_MAX;

    while (j > 0){
        j -= fit[i];
        i++;
    }
    i--;
    return pop[i];
}
vector<int> mutate(vector<int> vec, double rate) {
    for (int i = 0; i < verNum; i++) {
        if (rand()/RAND_MAX < rate) {
            int a = rand() % verNum;
            int b = (a + 1) % verNum;
            if (a == 0 || a >= verNum) {
                i--;
                continue;
            }
            vec = swap(vec, a, b);
        }
    }
    return vec;
}

