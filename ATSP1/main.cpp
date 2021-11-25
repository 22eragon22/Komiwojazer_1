#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "Pugixml/pugixml.hpp"

using namespace std;
using namespace pugi;
using namespace chrono;

#define MAX 999999

vector<int> solutions;
int verNum = 0, cost = 0;
auto timeAverage = 0;

int CalcPath(vector<vector<double>> graph, vector<bool> visited, int start);
void ATSP(vector<vector<double>> graph, vector<bool> visited, int start);


int main() {

    for (int i = 0; i < 100; i++) {

        //ładowanie pliku
        xml_document doc;
        if (!doc.load_file("../XML_files/br17.xml")) return -1;
        xml_node gr = doc.child("travellingSalesmanProblemInstance").child("graph");

        //liczenie wierzchołków grafu
        for (auto &it1 : gr) verNum++;

        //umieszczanie wag krawędzi w macierzy zrobionej z wektorów
        vector<vector<double>> graph;
        vector<double> helpMe;
        for (auto &it1 : gr) {
            for (auto &it2: it1) {
                helpMe.push_back(it2.attribute("cost").as_double());
            }
            graph.push_back(helpMe);
            helpMe.clear();
        }

        //wybranie startowego punktu
        //printf("Choose starting node from numbers between: 1 and %d\n", verNum);
        //int start; cin >> start; start -= 1;

        //ustawienie wszystkich wierzchołków jako nieodwiedzone
        vector<bool> visited;
        visited.reserve(verNum);
        for (int j = 0; j < verNum; j++) visited.push_back(false);


        //część kodu wykonująca ATSP
        int start = (rand() % verNum);
        auto t1 = steady_clock::now();


        //obliczanie nakrótszej możliwej ścieżki
        ATSP(graph, visited, start);

        //wyświetlanie ścieżki
        //for (auto &it : solutions) printf("%d-->", it);

        //wyświetlenie kosztu nakrótszej drogi
        //printf("Minimal cost equals: %d\n", cost);

        auto t2 = steady_clock::now();
        timeAverage = timeAverage + duration_cast<microseconds>(t2 - t1).count();

        //resetting the variables for further use
        solutions.clear();
        graph.clear();
        visited.clear();
        verNum = 0;
        cost = 0;

    }



    printf("\nTime average for 100 tries equals: %dus\n", timeAverage / 100);
    timeAverage = 0;

    return 0;
}


int CalcPath(vector<vector<double>> graph, vector<bool> visited, int start) {
    int nNode = MAX, minimum = MAX, path;

    for (int i = 0; i < verNum; i++)                            //przechodzenie przez wszystkie wierzchołki
        if (graph[start][i] != MAX && !visited[i])              //jeżeli wierzchołek nie był ani odwiedzony
            if (graph[start][i] + graph[i][start] < minimum) {  // ani nie próbuje odwiedzić siebie samego
                minimum = graph[start][i] + graph[i][start];    //próbujemy znaleźć minimum
                path = graph[start][i];
                nNode = i;
            }

    if (minimum != MAX) cost += path;                           //dodajemy koszt podróży do ogólnego kosztu

    return nNode;
}

void ATSP(vector<vector<double>> graph, vector<bool> visited, int start) {
    int nNode;
    visited[start] = true;
    solutions.push_back(start);

    nNode = CalcPath(graph, visited, start);

    if (nNode == MAX) {                                          //jeżeli dotarliśmy do ostatniego wierzchołka
        nNode = 0;                                               //dodajemy do ogólnego kosztu
        solutions.push_back(nNode);                              //koszt do wierzchołka początkowego
        cost += graph[start][nNode];
        return;
    }

    ATSP(graph, visited, nNode);                    //w innym wypadku, kontynuujemy
}



