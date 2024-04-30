#pragma once
#include <list>
#include <vector>
#include "Hand.hpp"
#include <fstream>
class AI{
private:
	std::string filename = "data/train5000_6.txt";
	int numFilas;
	int numPlayers;
	int type;
	static const int NUM_CARTAS = 104;
	static const int NUM_PARTITIONS = 6;
	int reward = 0;
	int lastPuntuation = 0;
	int lastState = -1;
	int lastPos = 0;
	vector<vector<int>> qTable;
	vector<vector<int>> TrainedqTable;
	vector<string> spliter(string text, string delimiter) {
		vector<string> content;
		size_t pos = 0;
		string token;
		while ((pos = text.find(delimiter)) != string::npos) {
			token = text.substr(0, pos);
			content.push_back(token);
			text.erase(0, pos + delimiter.length());
		}
		content.push_back(text);
		return content;
	}
public:
	vector<string> listaT{ "Miedica","Lanzada","MonteCarlo","Aleatoria","TrainQ(Usada para entrenar)","ModeloQ-Lerning"};
	AI() {};
	vector<vector<int>> getQtable() { return qTable; }

	AI(int NUM_PLAYERS, int NUM_FILAS, int t) { 
		vector<int> particiones;
		for (int i = 0; i < NUM_PARTITIONS; i++)
		{
			particiones.push_back(0);
		}
		for (int i = 0; i < NUM_CARTAS; i++)
		{
			qTable.push_back(particiones);
		}
		
		std::ifstream archivo(filename);
		if (!archivo.is_open()) {
			std::cerr << "No se pudo abrir el archivo " << filename << std::endl;
		}
		string tp;
		while (getline(archivo, tp)) {
			int i = 0;
			for (auto carta : spliter(tp, "]")) {
				vector<int> particiones;
				for (auto pos : spliter(carta, ",")) {
					if (pos.size() > 0) 
						particiones.push_back(stoi(pos));
				}
				TrainedqTable.push_back(particiones);
				i++;
			}
		}
		 lastPuntuation = 0; numPlayers = NUM_PLAYERS; numFilas = NUM_FILAS; type = t; }
	int swapFila(list<Card> filas[]);
	int smallestRowValue(list<Card> filas[]);
	int playSmallest(Hand myHand, list<Card> filas[]);
	int playNearest(Hand myHand, list<Card> filas[]);
	float calculaProb(int actual, int ref, vector<int> playedCards);
	int playMontecarlo(Hand myHand, list<Card> filas[], vector<int>);
	int playRand(Hand myHand);
	int TrainQ(Hand myHand, list<Card> filas[], vector<int>, int actualPuntuation);
	int playQTable(Hand myHand, list<Card> filas[]);
	int playCard(Hand, list<Card> filas[],vector<int>, int actualPuntuation);

};