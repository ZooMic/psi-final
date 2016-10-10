#ifndef perceptron_h
#define perceptron_h
class Perceptron{
public:
    int n; //liczba wejść
    double* wektorWag;
    double* wektorWejsc;
    double suma;
    double sumaDyskretna;
    double wspolczynnikUczenia;
    double bladPrzyblizenia;
    void trenujNeutron(double**ciagUczacy, int size);
    void odpowiedz(double pytanie);
    double odpowiedz2(double p1, double p2);
    Perceptron(int liczbaWejsc);
private:
    int licznikCiagu;
    void losowanieWag();
    void ustawWejscia(double* ciagUczacy);
    void wynikDyskretny();

};
#endif
