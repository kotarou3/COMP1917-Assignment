#include <assert.h>

#include "RegionLocation-utils.h"

bool isExistentRegion(RegionLocation location) {
    return -4 < location.x && location.x < 4 &&
        -4 < location.y && location.y < 4 &&
        -4 < location.x + location.y && location.x + location.y < 4;
}

bool isLandRegion(RegionLocation location) {
    return -3 < location.x && location.x < 3 &&
        -3 < location.y && location.y < 3 &&
        -3 < location.x + location.y && location.x + location.y < 3;
}

bool isRegionsEqual(RegionLocation a, RegionLocation b) {
    return a.x == b.x && a.y == b.y;
}

bool isRegionsAdjacent(RegionLocation a, RegionLocation b) {
    return (a.x == b.x && a.y + 1 == b.y) || // Up
        (a.x == b.x && a.y - 1 == b.y) || // Down
        (a.x - 1 == b.x && a.y + 1 == b.y) || // Up-left
        (a.x - 1 == b.x && a.y == b.y) || // Down-left
        (a.x + 1 == b.x && a.y == b.y) || // Up-right
        (a.x + 1 == b.x && a.y - 1 == b.y); // Down-right
}

RegionLocation getAdjacentRegion(RegionLocation location, Direction direction) {
    assert(direction & UP || direction & DOWN);

    RegionLocation result;
    result.x = location.x;
    result.y = location.y;

    if (direction & LEFT) {
        result.x--;
    } else if (direction & RIGHT) {
        result.x++;
    }

    if (direction == DOWN || direction == DOWN_RIGHT) {
        result.y--;
    } else if (direction == UP || direction == UP_LEFT) {
        result.y++;
    }

    return result;
}

RegionLocation getAdjacentRegionFromEdge(EdgeLocation location, Direction direction) {
    /* TODO
    assert((direction != LEFT) && (direction != RIGHT));

    //Deal with non UP or DOWN cases
    if ((direction == UP) ||
        (direction == UP_RIGHT) ||
        (direction == UP_LEFT)) {
        direction = UP;
    } else {
        direction = DOWN;
    }

    region nextRegionID;
    if (direction == UP) {
        if (currentArcID.region1.y < currentArcID.region0.y) {
            nextRegionID = currentArcID.region0;
        } else if (currentArcID.region1.y > currentArcID.region0.y){
            nextRegionID = currentArcID.region1;
        } else if (currentArcID.region1.x < currentArcID.region0.x) {
            nextRegionID = currentArcID.region0;
        } else {
            nextRegionID = currentArcID.region1;
        }
    } else {
        if (currentArcID.region1.y < currentArcID.region0.y) {
            nextRegionID = currentArcID.region1;
        } else if (currentArcID.region1.y > currentArcID.region0.y){
            nextRegionID = currentArcID.region0;
        } else if (currentArcID.region1.x < currentArcID.region0.x) {
            nextRegionID = currentArcID.region1;
        } else {
            nextRegionID = currentArcID.region0;
        }
    }

    return nextRegionID;
    */
}

RegionLocation getAdjacentRegionFromVertex(VertexLocation location, Direction direction) {
    // TODO
}
