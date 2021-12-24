#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Tile.h";

using namespace std;

class Board {
private:
	vector<int> cd;
	int columns;
	int rows;
	int numMines;
	int numTiles;
	bool debugMode;
	bool gameOver;
	bool wonGame;
	int numClicked;
	vector<vector<int>> layout; // 2d vector of 0's and 1's
	vector<vector<Tile>> tiles; // 2d vector of tile objects
	Tile** board; // 2d array of tile pointers
public:
	Board(vector<int> configData) {
		cd = configData;
		columns = configData[0];
		rows = configData[1];
		numMines = configData[2];
		numTiles = columns * rows;
		debugMode = false;
		gameOver = false;
		wonGame = false;
		numClicked = 0;

		// create 2d array of tiles (way easier for pointers)
		board = new Tile*[rows];
		for (int i = 0; i < rows; i++) {
			board[i] = new Tile[columns];
		}

		setRandomLayout();
	}

	~Board() {
		for (int i = 0; i < rows; i++)
		{
			delete[] & board[i];
		}
		delete[] board;
	}

	void toggleDebug() {
		debugMode = !debugMode;
	}

	void addClick() {
		numClicked++;
	}

	bool isDebug() { return debugMode; }

	void setLayoutFromFile(string filename) {
		// check dimensions before clearing board 
		// and setting loaded file
		ifstream inFileCheck(filename);
		if (inFileCheck.is_open()) {
			int c = 0;
			string k;
			while (getline(inFileCheck, k)) {
				c++;
				if (k.length() != columns) {
					return;
				}
			}
			if (c != rows) {
				return;
			}
		}

		layout.clear();

		// Returns vector of ints
		ifstream inFile(filename);

		if (inFile.is_open()) {
			string line;
			while (getline(inFile, line)) {
				// line: 0001001010110 --> split into vector
				vector<int> r;
				for (int i = 0; i < line.length(); i++) {
					int n = line[i] - 48;
					r.push_back(n);
				}
				layout.push_back(r);
			}
		}
		else {
			cout << "Error reading file" << endl;
		}

		inFile.close();
		initializeBoard();
	}

	void setRandomLayout() {
		// Generate numMines unique locations for mines
		const int min = 0;
		const int max = numTiles;
		layout.clear();

		vector<int> values(max);
		generate(values.begin(), values.end(), [value = min]() mutable {
			return value++;
		});
		shuffle(values.begin(), values.end(), mt19937{ random_device{}() });
		values.resize(numMines);

		// Input them in 2d array
		int count = 0;
		for (int i = 0; i < rows; i++) {
			vector<int> currRow;
			for (int j = 0; j < columns; j++) {
				if (find(values.begin(), values.end(), count) != values.end()) {
					currRow.push_back(1); // MINE
				}
				else {
					currRow.push_back(0); // NOT A MINE
				}
				count++;
			}
			layout.push_back(currRow);
		}
		initializeBoard();
	}

	void resetBoard() {
		numTiles = columns * rows; 
		numMines = cd[2];
		debugMode = false;
		gameOver = false;
		wonGame = false;
		numClicked = 0;

		// create 2d array of tiles (way easier for pointers)
		board = new Tile * [rows];
		for (int i = 0; i < rows; i++) {
			board[i] = new Tile[columns];
		}

		setRandomLayout();
	}

	void revealTiles(Tile& currTile, map<string, sf::Texture>& images, sf::RenderWindow& window) {
		if (!currTile.getFlag()) {

			int numAdjMines = 0;

			vector<Tile*> adjTiles = currTile.getAdjacentTiles();
			for (unsigned int k = 0; k < adjTiles.size(); k++) {
				if (adjTiles[k]->getMine()) {
					numAdjMines += 1;
				}
			}

			if (numAdjMines == 0) {
				// show clicked tile, reveal surrounding tiles
				sf::Sprite newClickedTile(images["tile_revealed"]);
				newClickedTile.setPosition(currTile.getXOffset(), currTile.getYOffset());
				window.draw(newClickedTile);

				for (unsigned int i = 0; i < adjTiles.size(); i++) {
					// show each adjacent tile
					// check if flag or mine
					int nm = 0;
					for (unsigned int j = 0; j < adjTiles[i]->getAdjacentTiles().size(); j++) {
						if (adjTiles[i]->getAdjacentTiles()[j]->getMine()) {
							nm += 1;
						}
					}
					//cout << nm << " " << flush;
					if (!adjTiles[i]->getFlag()) {
						if (!adjTiles[i]->canShow()) {

							if (nm == 0) {
								sf::Sprite newClickedTile(images["tile_revealed"]);
								newClickedTile.setPosition(adjTiles[i]->getXOffset(), adjTiles[i]->getYOffset());
								window.draw(newClickedTile);
							}
							else {
								sf::Sprite newClickedTile(images["tile_revealed"]);
								newClickedTile.setPosition(adjTiles[i]->getXOffset(), adjTiles[i]->getYOffset());
								window.draw(newClickedTile);
								string imageNumber = "number" + to_string(nm);
								sf::Sprite numMinesSprite(images[imageNumber]);
								numMinesSprite.setPosition(adjTiles[i]->getXOffset(), adjTiles[i]->getYOffset());
								window.draw(numMinesSprite);
							}
							adjTiles[i]->toggleShow();
							numClicked++;
						}
					}
					else {
						sf::Sprite newC(images["tile_hidden"]);
						newC.setPosition(adjTiles[i]->getXOffset(), adjTiles[i]->getYOffset());
						window.draw(newC);
						sf::Sprite newF(images["flag"]);
						newF.setPosition(adjTiles[i]->getXOffset(), adjTiles[i]->getYOffset());
						window.draw(newF);
						adjTiles[i]->toggleShow();
					}
					
				}
				//cout << endl;
			}
			else {
				sf::Sprite newClickedTile(images["tile_revealed"]);
				newClickedTile.setPosition(currTile.getXOffset(), currTile.getYOffset());
				window.draw(newClickedTile);
				string imageNumber = "number" + to_string(numAdjMines);
				sf::Sprite numMinesSprite(images[imageNumber]);
				numMinesSprite.setPosition(currTile.getXOffset(), currTile.getYOffset());
				window.draw(numMinesSprite);
			}

		}
	}

	void initializeBoard() {
		// create Tile** array 
		int nm = 0;

		int xOffset = 0;
		int yOffset = 0;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {

				bool isMine = false;
				if (layout[i][j] == 1) {
					isMine = true;
					nm++;
				}

				bool hasFlag = false;
				Tile newTileObj(xOffset, yOffset, i, j, isMine, hasFlag, rows, columns);
				board[i][j] = newTileObj;

				xOffset += 32;
			}

			xOffset = 0;
			yOffset += 32;
		}

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				board[i][j].setAdjacentTiles(board);
			}
		}

		numMines = nm;
	}

	void drawLayout(map<string, sf::Texture>&images, sf::RenderWindow& window) {
		// draw tiles, create Tile objects
		// change the way this works - use the same data to create the board
		// rather than creating a new one each time
		int flags = 0;

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				Tile currTile = board[i][j];

				sf::Sprite newTile(images["tile_hidden"]);
				newTile.setPosition(sf::Vector2f(currTile.getXOffset(), currTile.getYOffset()));
				window.draw(newTile);

				// Turn off, debug mode shows mines
				
				// If it is flagged, it can't be shown
				if (currTile.getFlag() == true) {
					sf::Sprite flag(images["flag"]);
					flag.setPosition(currTile.getXOffset(), currTile.getYOffset());
					window.draw(flag);
					flags++;
					//currTile.toggleShow();
				}
				else {
					if (currTile.canShow()) {
						if (currTile.getMine() == true) {
							// game ends
							sf::Sprite newMine(images["mine"]);
							newMine.setPosition(currTile.getXOffset(), currTile.getYOffset());
							window.draw(newMine);
							gameOver = true;
						}
						else {

							revealTiles(currTile, images, window);

						}
						currTile.toggleShow();
					}
					else {
						if (debugMode || gameOver) {
							if (currTile.getMine() == true) {
								sf::Sprite newMine(images["mine"]);
								newMine.setPosition(currTile.getXOffset(), currTile.getYOffset());
								window.draw(newMine);
							}
						}
						else if (wonGame) {
							if (currTile.getMine() == true) {
								sf::Sprite newMine(images["flag"]);
								newMine.setPosition(currTile.getXOffset(), currTile.getYOffset());
								window.draw(newMine);
								flags++;
							}
						}
					}
				}
				
			}
		}

		// draw bottom

		if (gameOver) {
			sf::Sprite sadTile(images["face_lose"]); // 64x64
			sadTile.setPosition(columns * 16 - 32, rows * 32);
			window.draw(sadTile);
		}
		else if (numClicked == numTiles-numMines) {
			sf::Sprite winTile(images["face_win"]); // 64x64
			winTile.setPosition(columns * 16 - 32, rows * 32);
			window.draw(winTile);
			wonGame = true;
		}
		else {
			sf::Sprite happyTile(images["face_happy"]); // 64x64
			happyTile.setPosition(columns * 16 - 32, rows * 32);
			window.draw(happyTile);
		}
		
		sf::Sprite debugTile(images["debug"]);
		debugTile.setPosition(columns * 32 - (32*8), rows * 32);
		window.draw(debugTile);

		sf::Sprite test1Tile(images["test_1"]);
		test1Tile.setPosition(columns * 32 - (32*6), rows * 32);
		window.draw(test1Tile);

		sf::Sprite test2Tile(images["test_2"]);
		test2Tile.setPosition(columns * 32 - (32*4), rows * 32);
		window.draw(test2Tile);

		sf::Sprite test3Tile(images["test_3"]);
		test3Tile.setPosition(columns * 32 - (32*2), rows * 32);
		window.draw(test3Tile);

		vector<sf::Sprite> nums;
		for (int i = 0; i < 11; i++) {
			sf::Sprite n(images["digits"]);
			n.setTextureRect(sf::IntRect(21*i, 0, 21, 32));
			nums.push_back(n);
		}
		// 0, 1, ... , 9, -
		// all nums at i and - is at 10

		// remaning mines
		int remMines = numMines - flags;
		//remMines = -1;
		string numMinesStr = to_string(remMines);
		
		sf::Sprite z = nums[0];
		z.setPosition(21, rows*32);
		window.draw(z);

		int i = 1;
		for (char& c : numMinesStr) {
			if (c == '-') {
				// access at 10
				sf::Sprite s = nums[10];
				s.setPosition(0, rows*32);
				window.draw(s);
			}
			else {
				// access at i
				int j = (int)c - 48;
				sf::Sprite s = nums[j];
				s.setPosition(21 * (i + 1), rows * 32);
				window.draw(s);
				if (remMines == 0) {
					sf::Sprite t = nums[j];
					t.setPosition(21 * (i + 2), rows * 32);
					window.draw(t);
				}
				i++;
			}
			
		}

	}

	int getColumns() { return columns; }
	int getRows() { return rows; }
	int getMines() { return numMines; }
	int getNumTiles() { return numTiles; }
	vector<vector<int>> getLayout() { return layout; }
	vector<vector<Tile>> getTiles() { return tiles; }
	Tile** getBoard() { return board; }

	void printLayout() {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				cout << layout[i][j] << " " << flush;
			}
			cout << endl;
		}
	}
	
};
