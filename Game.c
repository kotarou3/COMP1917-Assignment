#include <stdlib.h>

#include "Game-wrapper.h"

Game* newGame(DegreeType regionDegreeTypes[], DiceValue regionDiceValues[]) {
    Game* game = malloc(sizeof(Game));
    constructGame(game, regionDegreeTypes, regionDiceValues);
    return game;
}

void disposeGame(Game* game) {
    destroyGame(game);
    free(game);
}

int getTurnNumber(Game* game) {
    // TODO
}

PlayerId getWhoseTurn(Game* game) {
    // TODO
}

int getKPIpoints(Game* game, PlayerId player) {
    // TODO
}

PlayerId getMostARCs(Game* game) {
    // TODO
}

PlayerId getMostPublications(Game* game) {
    // TODO
}

bool isLegalAction(Game* game, Action action) {
    // TODO
}

void makeAction(Game* game, Action action) {
    // TODO
}

void throwDice(Game* game, DiceValue diceValue) {
    // TODO
}

void constructGame(Game* game, DegreeType* regionDegreeTypes, DiceValue* regionDiceValues) {
    // TODO
}

void destroyGame(Game* game) {
    // TODO
}

University* getOwnedUniversity(Game* game, PlayerId player) {
    // TODO
}
