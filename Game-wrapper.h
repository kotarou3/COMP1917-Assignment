#ifndef GAME_WRAPPER_H
#define GAME_WRAPPER_H

#include "Game-redefines.h"
#include "Map.h"
#include "University.h"

#define UNI_A_START_CAMPUS_0 ((VertexLocation){{-1, 3}, {0, 2}, {0, 3}})
#define UNI_A_START_CAMPUS_1 ((VertexLocation){{0, -3}, {0, -2}, {1, -3}})
#define UNI_B_START_CAMPUS_0 ((VertexLocation){{-3, 2}, {-3, 3}, {-2, 2}})
#define UNI_B_START_CAMPUS_1 ((VertexLocation){{2, -2}, {3, -3}, {3, -2}})
#define UNI_C_START_CAMPUS_0 ((VertexLocation){{-3, 0}, {-2, -1}, {-2, 0}})
#define UNI_C_START_CAMPUS_1 ((VertexLocation){{2, 0}, {2, 1}, {3, 0}})

#define KPI_PER_ARC 2
#define KPI_PER_CAMPUS 10
#define KPI_PER_GO8 20
#define KPI_PER_PATENT 10
#define KPI_FOR_MOST_PUBS 10
#define KPI_FOR_MOST_ARCS 10

#define STUPID_DICE_VALUE_RULE 7

struct _Game {
    size_t currentTurn;
    Map map;
    University universities[NUM_PLAYERS];

    size_t mostPublications;
    PlayerId mostPublicationsPlayer;

    size_t mostArcs;
    PlayerId mostArcsPlayer;
};

/* Game.h interface
Game* newGame(DegreeType regionDegreeTypes[], DiceValue regionDiceValues[]);
void disposeGame(Game* game);

int getTurnNumber(Game* game);
PlayerId getWhoseTurn(Game* game);

int getKPIpoints(Game* game, PlayerId player);
PlayerId getMostPublications(Game* game);
PlayerId getMostARCs(Game* game);

bool isLegalAction(Game* game, Action action);
void makeAction(Game* game, Action action);
void throwDice(Game* game, DiceValue diceValue);
*/

// Helper functions
void constructGame(Game* game, DegreeType* regionDegreeTypes, DiceValue* regionDiceValues);
void destroyGame(Game* game);

University* getOwnedUniversity(Game* game, PlayerId player, bool isFatalOnNotFound);

#endif
