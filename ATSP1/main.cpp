#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "Pugixml/pugixml.hpp"

using namespace std;
using namespace pugi;
using namespace chrono;

int verNum = 0, minimum = INT_MAX;
auto timeAverage = 0;
double st;

void ATSP(vector<vector<double>>& graph, vector<bool>& visited, int start, int cost);

int main() {

    for (int i = 0; i < 100; i++) {

        //ładowanie pliku
        xml_document doc;
        if (!doc.load_file("../XML_files/ftv33.xml")) return -1;
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

        //ustawienie wszystkich wierzchołków jako nieodwiedzone
        vector<bool> visited;
        visited.reserve(verNum);
        for (int j = 0; j < verNum; j++) visited.push_back(false);


        //część kodu wykonująca ATSP
        int start = (rand() % verNum);
        st = start;
        auto t1 = steady_clock::now();

        //obliczanie nakrótszej możliwej ścieżki
        ATSP(graph, visited, start, 0);

        //wyświetlenie kosztu nakrótszej drogi
        //printf("Minimal cost equals: %d\n", minimum);

        auto t2 = steady_clock::now();
        timeAverage = timeAverage + duration_cast<microseconds>(t2 - t1).count();

        //resetting the variables for further use
        graph.clear();
        visited.clear();
        verNum = 0;
        minimum = INT_MAX;

    }
    printf("\nTime average for 100 tries equals: %dus\n", timeAverage / 100);
    timeAverage = 0;

    return 0;
}


void ATSP(vector<vector<double>>& graph, vector<bool>& visited, int start, int cost) {
    visited[start] = true;
    int helper = cost;

    if (find(begin(visited), end(visited), false) == end(visited)) {
        helper += graph[start][st];
        minimum = helper;
        return;
    }
    else {
        for (int i = 0; i < verNum; i++) {
            if (!visited[i]) {
                helper += graph[start][i];
                if (helper >= minimum)
                    return;
                start = i;
                ATSP(graph, visited, start, helper);
            }
        }
        return;
    }
}



