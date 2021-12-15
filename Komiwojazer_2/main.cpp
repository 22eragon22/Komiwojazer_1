#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "Pugixml/pugixml.hpp"

using namespace std;
using namespace pugi;
using namespace chrono;

//deklaracja zmiennych oraz funkcji

#define MAX 999999

vector<int> startingPath;
vector<vector<double>> graph;
int verNum = 0, minCost = 0, start;
auto timeAverage = 0;

void createPath(int st);
vector<int> swapVertexes(vector<int> path, int i, int k);
int calcCost(vector<int> path);
void opt2Algorithm();
bool opt2AlgorithmHelper();

int main() {

    for (int i = 0; i < 100; i++) {


        //ładowanie pliku
        xml_document doc;
        if (!doc.load_file("../XML_files/br17.xml")) return -1;
        xml_node gr = doc.child("travellingSalesmanProblemInstance").child("graph");

        //liczenie wierzchołków grafu
        for (auto &it1 : gr) verNum++;

        //umieszczanie wag krawędzi w macierzy zrobionej z wektorów
        vector<double> helpMe;
        for (auto &it1 : gr) {
            for (auto &it2: it1) {
                helpMe.push_back(it2.attribute("cost").as_double());
            }
            graph.push_back(helpMe);
            helpMe.clear();
        }


        //generowanie punktu startowego
        start = (rand() % verNum);

        //rozpoczęcie pomiaru czasu
        auto t1 = steady_clock::now();

        //wywołanie algorytmu 2-opt
        createPath(start);
        opt2Algorithm();

        //zakończenie pomiaru czasu
        auto t2 = steady_clock::now();
        timeAverage = timeAverage + duration_cast<microseconds>(t2 - t1).count();

        //resetowanie zmiennych
        minCost = 0;
        verNum = 0;
        graph.clear();
        startingPath.clear();
    }

    //zliczanie średniej wyników
    printf("\nTime average for 100 tries equals: %dus\n", timeAverage / 100);
    timeAverage = 0;

    return 0;
}


//funkcja wykorzystująca algorytm najbliższego sąsiada do
//rekurencyjnego utworzenia wstępnej ścieżki przez graf
void createPath(int st) {

    startingPath.push_back(st);

    int min = MAX;
    int newSt;

    //jeżeli przeszliśmy wszystkie wierzchołki, dodajemy
    //do ścieżki drogę z ostatniego punktu do punktu startowego
    if (startingPath.size() == verNum) {
        startingPath.push_back(start);
        return;
    }

    //tak długo jak mamy jeszcze wierzchołki do przejścia
    for (int i = 0; i < verNum; i++)

        //jeżeli waga odwiedzonego wierzchołka jest mniejsza od minimum
        //oraz jeszcze nie odwiedziliśmy tego wierzchołka
        if (graph[st][i] < min && (find(startingPath.begin(), startingPath.end(), i) == startingPath.end())) {

            //ustalamy ten wierzchołek jako nowe minimum
            min = graph[st][i];
            newSt = i;
        }

    //wywołujemy funkcje dla nowego minimum
    createPath(newSt);
}


//funkcja odpowiadająca za zamianę fragmentów ścieżki
vector<int> swapVertexes(vector<int> path, int i, int k) {

    vector<int> new_path;
    new_path.reserve(verNum);

    //do nowego wektora wklejamy wierzchołki od 0 do i-2
    for (int a = 0; a < i - 1; a++) new_path.push_back(path[a]);

    //wierzchołki od i - 1 do k - 1 obracamy i wklejamy do nowego wektora
    for (int a = k - 1; a >= i - 1; a--) new_path.push_back(path[a]);

    //wierzchołki od k do końca wklejamy do nowego wektora
    for (int a = k; a < verNum; a++) new_path.push_back(path[a]);

    //na końcu doklejamy jeszcze do wektora drogę z końca spowrotem do początku
    new_path.push_back(start);


    //zwracamy nowy wektor
    return new_path;
}


//funkcja zliczająca aktualny koszt drogi obecnej w zmiennej path
int calcCost(vector<int> path) {

    //zwykłe obliczenie kosztu aktualnej drogi
    int cost = 0;
    for (int i = 1; i <= verNum; i++) {
        int a = path[i-1];
        int b = path[i];
        cost += graph[a][b];
    }

    return cost;
}


//algorytm 2-opt, część 1/2
void opt2Algorithm() {

    //pętla wykonująca się tak długo, jak
    //koszt drogi się zmniejsza
    bool result = false;
    do{
        //minCost = calcCost(startingPath);
        //printf("Cost: %d\n", minCost);
        //for (auto &it1 : startingPath) printf("%d->", it1);
        //cout << endl;

        //wywołanie ,,rdzenia" algorytmu
        result = opt2AlgorithmHelper();

    }while (result);
}


//algorytm 2-opt, część 2/2
bool opt2AlgorithmHelper() {

    //tak długo jak są wierzchołki do odwiedzenia, przy czym
    //pomijamy wierzchołek startowy oraz końcowy
    for (int j = 2; j <= verNum - 1; j++)
        for (int k = j + 1; k <= verNum; k++) {

            //zamieniamy miejscami wierzchołki
            vector<int> new_path = swapVertexes(startingPath, j, k);

            //liczymi koszt drogi
            int new_cost = calcCost(new_path);

            //jak się poprawił, zapisujemy
            if (new_cost < minCost) {
                startingPath = new_path;
                minCost = new_cost;
                return true;
            }
        }

    //w innym wypadku zwracamy fałsz
    return false;
}