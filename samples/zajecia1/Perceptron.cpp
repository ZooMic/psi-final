#include "Perceptron.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Perceptron::Perceptron(int liczbaWejsc)
{
    n = liczbaWejsc;
    wektorWag = new double[n + 1];
    wektorWejsc = new double[n];
    wektorWag[n] = ((double)rand()) / (RAND_MAX);
    wspolczynnikUczenia = 0.00001;
    bladPrzyblizenia = 0.001;
}

void Perceptron::trenujNeutron(double **ciagUczacy, int size) {
    losowanieWag();
    // Licznik przerobienia ciagu uczacego
    int licznikCiagu = 0;
    int licznikPrzerwania = 1000000;
    int diff = 0;
    // Główna pętla
    while(licznikCiagu != size && licznikPrzerwania > 0) {
        ustawWejscia(ciagUczacy[licznikCiagu]);
        wynikDyskretny();
        // Aktualizowanie wag
        cout << licznikPrzerwania << ". ";
        cout << "Wektor uczacy |";
        for(int i = 0; i <= n; i++) {
            cout << ciagUczacy[licznikCiagu][i] << " | ";
        }
        cout << " -> "<< suma << " == " << sumaDyskretna << endl;
        diff = ciagUczacy[licznikCiagu][n] - sumaDyskretna;
        if (diff == 0) {
            licznikCiagu++;
        } else if (diff < 0) {
            for(int i = 0; i < n; i++) {
                wektorWag[i] -= wspolczynnikUczenia;
            }
            licznikCiagu = 0;
            cout << endl;
        } else {
            for(int i = 0; i < n; i++) {
                wektorWag[i] += wspolczynnikUczenia;
            }
            licznikCiagu = 0;
            cout << endl;
        }
        licznikPrzerwania--;
    }
}

void Perceptron::losowanieWag() {
    for(int i = 0; i < n; i++) {
        wektorWag[i] = ((double)rand()) / (RAND_MAX);
    }
    wektorWag[n] = 0.0;
    cout << "Losowy wektorWag: [ ";
    for(int i = 0; i <= n; i++) {
        cout << wektorWag[i];
        if(i != n) cout << " | ";
    }
    cout << " ]" << endl;
}

void Perceptron::ustawWejscia(double * wektorUczacy) {
    for(int i = 0; i < n; i++) {
        wektorWejsc[i] = wektorUczacy[i];
    }
}

void Perceptron::wynikDyskretny() {
    suma = 0;
    for(int i = 0; i < n; i++) {
        suma += wektorWejsc[i] * wektorWag[i];
    }
    suma -= wektorWag[n];
    //cout << suma << endl;
    // dyskretyzacja
    if(suma >= 0) {
        sumaDyskretna = 1;
    } else {
        sumaDyskretna = 0;
    }
}

void Perceptron::odpowiedz(double pytanie) {
    // Ustawianie wejscia
    wektorWejsc[0] = pytanie;
    wynikDyskretny();
    cout << "Wartość " << pytanie << " należy do klasy o wartości " << sumaDyskretna << endl;
}

double Perceptron::odpowiedz2(double p1, double p2) {
    // Ustawianie wejscia
    wektorWejsc[0] = p1;
    wektorWejsc[1] = p2;
    wynikDyskretny();
    return sumaDyskretna;
}
