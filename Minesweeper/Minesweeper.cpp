
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Tile.h"

using namespace std;

// 1) Load config file

 vector<int> loadConfig() {
    // Returns vector of ints
    string filename = "../boards/config.cfg";
    ifstream inFile(filename);

    vector<int> configData;

    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            configData.push_back(stoi(line));
        }
    }
    else {
        cout << "Error reading file" << endl;
    }
    
    inFile.close();

    return configData;
};

void loadTextures(map<string, sf::Texture>& textures) {
    sf::Texture texMine, texTileHidden, texTileRevealed, texNumber1, texNumber2, texNumber3, texNumber4, texNumber5, texNumber6, texNumber7, texNumber8, texFlag, texFaceHappy, texFaceWin, texFaceLose, texDigits, texDebug, texTest1, texTest2, texTest3;
    
    texMine.loadFromFile("../images/mine.png");
    textures.insert(make_pair("mine", texMine));

    texTileHidden.loadFromFile("../images/tile_hidden.png");
    textures.insert(make_pair("tile_hidden", texTileHidden));

    texTileRevealed.loadFromFile("../images/tile_revealed.png");
    textures.insert(make_pair("tile_revealed", texTileRevealed));

    texNumber1.loadFromFile("../images/number_1.png");
    textures.insert(make_pair("number1", texNumber1));

    texNumber2.loadFromFile("../images/number_2.png");
    textures.insert(make_pair("number2", texNumber2));

    texNumber3.loadFromFile("../images/number_3.png");
    textures.insert(make_pair("number3", texNumber3));

    texNumber4.loadFromFile("../images/number_4.png");
    textures.insert(make_pair("number4", texNumber4));

    texNumber5.loadFromFile("../images/number_5.png");
    textures.insert(make_pair("number5", texNumber5));

    texNumber6.loadFromFile("../images/number_6.png");
    textures.insert(make_pair("number6", texNumber6));

    texNumber7.loadFromFile("../images/number_7.png");
    textures.insert(make_pair("number7", texNumber7));

    texNumber8.loadFromFile("../images/number_8.png");
    textures.insert(make_pair("number8", texNumber8));

    texFlag.loadFromFile("../images/flag.png");
    textures.insert(make_pair("flag", texFlag));

    texFaceHappy.loadFromFile("../images/face_happy.png");
    textures.insert(make_pair("face_happy", texFaceHappy));

    texFaceWin.loadFromFile("../images/face_win.png");
    textures.insert(make_pair("face_win", texFaceWin));

    texFaceLose.loadFromFile("../images/face_lose.png");
    textures.insert(make_pair("face_lose", texFaceLose));

    texDigits.loadFromFile("../images/digits.png");
    textures.insert(make_pair("digits", texDigits));

    texDebug.loadFromFile("../images/debug.png");
    textures.insert(make_pair("debug", texDebug));

    texTest1.loadFromFile("../images/test_1.png");
    textures.insert(make_pair("test_1", texTest1));

    texTest2.loadFromFile("../images/test_2.png");
    textures.insert(make_pair("test_2", texTest2));

    texTest3.loadFromFile("../images/test_3.png");
    textures.insert(make_pair("test_3", texTest3));
 }

void setFlag(Board& gameBoard, sf::RenderWindow& window) {
    sf::Vector2i pos = sf::Mouse::getPosition(window);

    int xCoord = pos.x;
    int yCoord = pos.y;

    // Check if in tile area --> 0 <= y <= rows*32
    if (yCoord <= gameBoard.getRows() * 32) {
        
        Tile** tiles = gameBoard.getBoard();

        for (int i = 0; i < gameBoard.getRows(); i++) {
            for (int j = 0; j < gameBoard.getColumns(); j++) {

                int tileX = tiles[i][j].getXOffset();
                int tileY = tiles[i][j].getYOffset();

                if (tileX <= xCoord && xCoord <= tileX + 32) {
                    if (tileY <= yCoord && yCoord <= tileY + 32) {

                        tiles[i][j].toggleFlag();
                        
                    }
                }

            }
        }

    }
}

void revealTile(Board& gameBoard, sf::RenderWindow& window) {
    sf::Vector2i pos = sf::Mouse::getPosition(window);

    int xCoord = pos.x;
    int yCoord = pos.y;

    // Check if in tile area --> 0 <= y <= rows*32
    if (yCoord <= gameBoard.getRows() * 32) {

        Tile** tiles = gameBoard.getBoard();

        for (int i = 0; i < gameBoard.getRows(); i++) {
            for (int j = 0; j < gameBoard.getColumns(); j++) {

                int tileX = tiles[i][j].getXOffset();
                int tileY = tiles[i][j].getYOffset();

                if (tileX <= xCoord && xCoord <= tileX + 32) {
                    if (tileY <= yCoord && yCoord <= tileY + 32) {

                        if (!tiles[i][j].canShow() && !tiles[i][j].getFlag()) {
                            tiles[i][j].toggleShow();
                            gameBoard.addClick();
                        }
                    }
                }
            }
        }

    }
    else {

        if (yCoord >= gameBoard.getRows() * 32 && yCoord <= gameBoard.getRows()*32+64) {
            if (xCoord >= gameBoard.getColumns() * 16 - 32 && xCoord <= gameBoard.getColumns() * 16 + 32) {
                // face
                gameBoard.resetBoard();
            }
            else if (xCoord >= gameBoard.getColumns() * 32 - (32 * 6) && xCoord <= gameBoard.getColumns() * 32 - (32 * 6) + 64) {
                // test 1
                gameBoard.resetBoard();
                gameBoard.setLayoutFromFile("../boards/testboard1.brd");
            }
            else if (xCoord >= gameBoard.getColumns() * 32 - (32 * 4) && xCoord <= gameBoard.getColumns() * 32 - (32 * 4) + 64) {
                // test 2
                gameBoard.resetBoard();
                gameBoard.setLayoutFromFile("../boards/testboard2.brd");
            }
            else if (xCoord >= gameBoard.getColumns() * 32 - (32 * 2) && xCoord <= gameBoard.getColumns() * 32 - (32 * 2) + 64) {
                // test 3
                gameBoard.resetBoard();
                gameBoard.setLayoutFromFile("../boards/testboard3.brd");
            }
            else if (xCoord >= gameBoard.getColumns() * 32 - (32 * 8) && xCoord <= gameBoard.getColumns() * 32 - (32 * 8) + 64) {
                // debug
                gameBoard.toggleDebug();
            }
        }
    }
}

int main()
{
  
    vector<int> boardData = loadConfig();
    Board gameBoard(boardData);

    int windowWidth = gameBoard.getColumns() * 32;
    int windowHeight = (gameBoard.getRows() * 32) + 88;

    // gameBoard.printLayout();

    map<string, sf::Texture> images;
    loadTextures(images);

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Minesweeper");

    gameBoard.initializeBoard();

    while (window.isOpen())
    {
        // vector<Tile> flaggedTiles;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                revealTile(gameBoard, window);
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                setFlag(gameBoard, window);
            }
            
        }


        window.clear();
        gameBoard.drawLayout(images, window);
        window.display();
    }

    return 0;
}
