#pragma once
#include <list>
#include <vector>
#include "Hand.hpp"
#include <fstream>
class AI{
private:
	std::string filename = "data/train5000_6.txt";
	std::string filenameCard = "data/trainCard50000.txt";
	std::string filenameCardCount = "data/trainCardCount50000.txt";
	int numFilas;
	int numPlayers;
	int type;
	static const int NUM_CARTAS = 104;
	static const int NUM_PARTITIONS = 6;
	int reward = 0;
	int lastPuntuation = 0;
	int lastState = -1;
	int lastPos = 0;
	vector<int> lastFilas;
	vector<int> lastCount;
	vector<vector<int>> qTable;
	vector<vector<int>> TrainedqTable;
	vector<vector<int>> qTableCard;
	vector<vector<int>> TrainedqTableCard;
	vector<vector<vector<int>>> qTableCardCount;
	vector<vector<vector<int>>> TrainedqTableCardCount;
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
	vector<string> listaT{ "Miedica","Lanzada","MonteCarlo","Aleatoria","TrainQ(Usada para entrenar)","ModeloQ-Lerning","TrainQCard(Usada para entrenar)","ModeloQ-LerningCard","TrainQCardCount(Usada para entrenar)","ModeloQ-LerningCardCount","Quimera"};
	AI() {};
	vector<vector<int>> getQtable() { return qTable; }
	vector<vector<int>> getQtableCard() { return qTableCard; }
	vector<vector<vector<int>>> getQtableCardCount() { return qTableCardCount; }

	AI(int NUM_PLAYERS, int NUM_FILAS, int t) { 
		type = t;
		vector<int> particiones;
		if (type == 5) {
			//Empty builder QTable
			for (int i = 0; i < NUM_PARTITIONS; i++)
			{
				particiones.push_back(0);
			}
			for (int i = 0; i < NUM_CARTAS; i++)
			{
				qTable.push_back(particiones);
			}
		}
		//Q table loader
		if (type == 6) {
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
		}
		//Empty builder QTableCArd
		if (type == 7) {
			for (int i = 0; i < 4; i++)
			{
				lastFilas.push_back(0);
			}
			for (int i = 0; i < NUM_CARTAS; i++)
			{
				particiones.push_back(0);
			}
			for (int i = 0; i < NUM_CARTAS; i++)
			{
				qTableCard.push_back(particiones);
			}
		}
		//Q table CARD loader
		if (type == 8) {
			std::ifstream archivoCarta(filenameCard);
			if (!archivoCarta.is_open()) {
				std::cerr << "No se pudo abrir el archivo " << filenameCard << std::endl;
			}
			string tpCarta;
			while (getline(archivoCarta, tpCarta)) {
				int i = 0;
				for (auto carta : spliter(tpCarta, "]")) {
					vector<int> particiones;
					for (auto pos : spliter(carta, ",")) {
						if (pos.size() > 0)
							particiones.push_back(stoi(pos));
					}
					TrainedqTableCard.push_back(particiones);
					i++;
				}
			}
		}
		//Empty builder QTableCArdCount
		if (type == 9) {
			for (int i = 0; i < 4; i++)
			{
				lastFilas.push_back(0);
				lastCount.push_back(0);
			}
			for (int i = 0; i < 5; i++)
			{
				particiones.push_back(0);
			}
			vector<vector<int>> cantidad;
			for (int i = 0; i < NUM_CARTAS; i++)
			{
				cantidad.push_back(particiones);
			}
			for (int i = 0; i < NUM_CARTAS; i++)
			{
				qTableCardCount.push_back(cantidad);
			}
		}
		//Q table QTableCArdCount loader
		if (type == 10 || type == 11) {
			std::ifstream archivoCartaCount(filenameCardCount);
			if (!archivoCartaCount.is_open()) {
				std::cerr << "No se pudo abrir el archivo " << filenameCardCount << std::endl;
			}
			string tpCartaCount;
			while (getline(archivoCartaCount, tpCartaCount)) {
				int i = 0;
				for (auto carta : spliter(tpCartaCount, "]")) {
					vector<vector<int>> rows;
					for (auto Row : spliter(carta, "|")) {
						if (Row.size() > 0) {
							vector<int> particiones;
							for (auto pos : spliter(Row, ",")) {
								if (pos.size() > 0)
									particiones.push_back(stoi(pos));
							}
							rows.push_back(particiones);
						}
					}
					TrainedqTableCardCount.push_back(rows);
					i++;
				}
			}
		}
		 lastPuntuation = 0; numPlayers = NUM_PLAYERS; numFilas = NUM_FILAS;  }
	int swapFila(list<Card> filas[]);
	int smallestRowValue(list<Card> filas[]);
	int playSmallest(Hand myHand, list<Card> filas[]);
	int playNearest(Hand myHand, list<Card> filas[]);
	float calculaProb(int actual, int ref, vector<int> playedCards);
	int playMontecarlo(Hand myHand, list<Card> filas[], vector<int>);
	int playRand(Hand myHand);
	int TrainQ(Hand myHand, list<Card> filas[], vector<int>, int actualPuntuation);
	int TrainQCard(Hand myHand, list<Card> filas[], vector<int>, int actualPuntuation, int posJugada);
	int TrainQCardCount(Hand myHand, list<Card> filas[], vector<int>, int actualPuntuation, int posJugada);
	int playQTable(Hand myHand, list<Card> filas[]);
	int playQTableCard(Hand myHand, list<Card> filas[]);
	int playQTableCardCount(Hand myHand, list<Card> filas[]);
	int playCard(Hand, list<Card> filas[],vector<int>, int actualPuntuation, int posJugada);
	int playQuimera(Hand myHand, list<Card> filas[]);

};