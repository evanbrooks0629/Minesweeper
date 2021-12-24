#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <SFML/Graphics.hpp>

using namespace std;

class Tile {
private:
	int xOffset; // pixel location
	int yOffset;
	int x; // location in array
	int y;
	bool isMine;
	bool showValue;
	bool hasFlag;
	bool canReveal;
	int rows;
	int cols;
	vector<Tile*> adjacentTiles;
public:
	Tile(int xPosition, int yPosition, int xCoord, int yCoord, bool mine, bool flag, int r, int c) {
		xOffset = xPosition;
		yOffset = yPosition;
		x = xCoord;
		y = yCoord;
		isMine = mine;
		hasFlag = flag;
		rows = r;
		cols = c;
		showValue = false;
		canReveal = false;
	}

	Tile() {}

	void toggleFlag() {
		hasFlag = !hasFlag; 
		//showValue = !showValue;
	}

	void toggleShow() {
		showValue = !showValue;
	}

	void toggleReveal() {
		canReveal = true;
	}

	void setAdjacentTiles(Tile** board) {
		adjacentTiles.clear();

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if ((x == i) || (x == i - 1) | (x == i + 1)) {
					if ((y == j) || (y == j - 1) | (y == j + 1)) {
						if (!(x == i && y == j)) {
							adjacentTiles.push_back(&board[i][j]);
						}
					}
				}
			}
		}
		
	};

	int getXOffset() { return xOffset; }
	int getYOffset() { return yOffset; }
	int getX() { return x; }
	int getY() { return y; }
	bool getMine() { return isMine; }
	bool getFlag() { return hasFlag; }
	bool canShow() { return showValue; }
	bool isRevealed() { return canReveal; }
	vector<Tile*> getAdjacentTiles() { return adjacentTiles; }
};
