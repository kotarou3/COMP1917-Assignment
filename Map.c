#include "Map.h"

bool isSea(Game* game, RegionLocation location) {
    // TODO
}

DegreeType getDegree(Game* game, RegionLocation location) {
    // TODO
}

DiceValue getDiceValue(Game* game, RegionLocation location) {
    // TODO
}

ArcType getARC(Game* game, EdgeLocation location) {
    // TODO
}

CampusType getCampus(Game* game, VertexLocation location) {
    // TODO
}

void constructMap(Map* map, DegreeType* regionDegreeTypes, DiceValue* regionDiceValues) {
    // TODO
}

void destroyMap(Map* map) {
    // TODO
}

Region* getRegion(Map* map, RegionLocation location) {
    // TODO
}

Edge* getEdge(Map* map, EdgeLocation location) {
    // TODO
}

Vertex* getVertex(Map* map, VertexLocation location) {
    // TODO
}

RegionLocation getAdjacentRegion(Map* map, RegionLocation location, Direction direction) {
    // TODO
}

RegionLocation getAdjacentRegionFromEdge(Map* map, EdgeLocation location, Direction direction) {
    // TODO
}

RegionLocation getAdjacentRegionFromVertex(Map* map, VertexLocation location, Direction direction) {
    // TODO
}

EdgeLocation getAdjacentEdgeFromRegion(Map* map, RegionLocation location, Direction direction) {
    // TODO
}

EdgeLocation getAdjacentEdgeFromVertex(Map* map, VertexLocation location, Direction direction) {
    // TODO
}

VertexLocation getAdjacentVertexFromRegion(Map* map, RegionLocation location, Direction direction) {
    // TODO
}

VertexLocation getAdjacentVertexFromEdge(Map* map, EdgeLocation location, Direction direction) {
    // TODO
}
