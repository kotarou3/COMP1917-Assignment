#include <assert.h>

#include "VertexLocation-utils.h"
#include "RegionLocation-utils.h"

bool isValidVertex(VertexLocation location) {
    return isRegionsAdjacent(location.region0, location.region1) &&
        isRegionsAdjacent(location.region1, location.region2) &&
        isRegionsAdjacent(location.region0, location.region2);
}

bool isVerticesEqual(VertexLocation a, VertexLocation b) {
    // {a.region0, a.region1, a.region2} ∈ {permutations({b.region0, b.region1, b.region2})}
    return (isRegionsEqual(a.region0, b.region0) && isRegionsEqual(a.region1, b.region1) && isRegionsEqual(a.region2, b.region2)) ||
        (isRegionsEqual(a.region0, b.region0) && isRegionsEqual(a.region1, b.region2) && isRegionsEqual(a.region2, b.region1)) ||
        (isRegionsEqual(a.region0, b.region1) && isRegionsEqual(a.region1, b.region0) && isRegionsEqual(a.region2, b.region2)) ||
        (isRegionsEqual(a.region0, b.region1) && isRegionsEqual(a.region1, b.region2) && isRegionsEqual(a.region2, b.region0)) ||
        (isRegionsEqual(a.region0, b.region2) && isRegionsEqual(a.region1, b.region0) && isRegionsEqual(a.region2, b.region1)) ||
        (isRegionsEqual(a.region0, b.region2) && isRegionsEqual(a.region1, b.region1) && isRegionsEqual(a.region2, b.region0));
}

VertexType getVertexType(VertexLocation location) {
    assert(isValidVertex(location));
    // From examining the coordinate system, if there is only a single highest value of x,
    // it's a left vertex. Otherwise, it's a right vertex.

    // We first find the largest x value of region0 and region1.
    // If region2's x value is greater than that, then it must single greatest x value.
    // This is because of the only two combinations of regions a valid vertex can have
    // where there are two equal x values and another x value that is one greater or less
    // than the pair that equals.

    int highestX = location.region0.x;
    if (location.region1.x > highestX) {
        highestX = location.region1.x;
    }

    VertexType result = VERTEX_RIGHT;
    if (location.region2.x > highestX) {
        result = VERTEX_LEFT;
    }
    return result;
}

SurroundingVerticesFromRegion getSurroundingVerticesFromRegion(RegionLocation location) {
    SurroundingVerticesFromRegion result;

    // Up-right
    result.locations[0].region0 = location;
    result.locations[0].region1 = getAdjacentRegion(location, UP);
    result.locations[0].region2 = getAdjacentRegion(location, UP_RIGHT);

    // Right
    result.locations[1].region0 = location;
    result.locations[1].region1 = getAdjacentRegion(location, UP_RIGHT);
    result.locations[1].region2 = getAdjacentRegion(location, DOWN_RIGHT);

    // Down-right
    result.locations[2].region0 = location;
    result.locations[2].region1 = getAdjacentRegion(location, DOWN);
    result.locations[2].region2 = getAdjacentRegion(location, DOWN_RIGHT);

    // Down-left
    result.locations[3].region0 = location;
    result.locations[3].region1 = getAdjacentRegion(location, DOWN);
    result.locations[3].region2 = getAdjacentRegion(location, DOWN_LEFT);

    // Left
    result.locations[4].region0 = location;
    result.locations[4].region1 = getAdjacentRegion(location, UP_LEFT);
    result.locations[4].region2 = getAdjacentRegion(location, DOWN_LEFT);

    // Up-left
    result.locations[5].region0 = location;
    result.locations[5].region1 = getAdjacentRegion(location, UP);
    result.locations[5].region2 = getAdjacentRegion(location, UP_LEFT);

    return result;
}

SurroundingVerticesFromVertex getSurroundingVerticesFromVertex(VertexLocation location) {
    // NOTE: Only two possible values for x and y (independently) for a valid vertex
    VertexType type = getVertexType(location);
    SurroundingVerticesFromVertex result;
    if (type == VERTEX_LEFT) {
        RegionLocation topmostRegion = location.region0;
        if (location.region1.y > topmostRegion.y) {
            topmostRegion = location.region1;
        } else if (location.region2.y > topmostRegion.y) {
            topmostRegion = location.region2;
        }

        // Up-right
        result.locations[0].region0 = topmostRegion;
        result.locations[0].region1 = getAdjacentRegion(topmostRegion, UP_RIGHT);
        result.locations[0].region2 = getAdjacentRegion(topmostRegion, DOWN_RIGHT);

        // Left
        result.locations[1].region0 = topmostRegion;
        result.locations[1].region1 = getAdjacentRegion(topmostRegion, DOWN);
        result.locations[1].region2 = getAdjacentRegion(topmostRegion, DOWN_LEFT);

        // Down-right
        result.locations[2].region0 = result.locations[0].region2; // DOWN_RIGHT
        result.locations[2].region1 = result.locations[1].region1; // DOWN
        result.locations[2].region2 = getAdjacentRegion(result.locations[1].region1, DOWN_RIGHT);
    } else {
        RegionLocation topmostRegion = location.region0;
        if ((location.region1.x > topmostRegion.x && location.region1.y == topmostRegion.y) ||
            (location.region1.x == topmostRegion.x && location.region1.y > topmostRegion.y)) {
            topmostRegion = location.region1;
        } else if ((location.region2.x > topmostRegion.x && location.region2.y == topmostRegion.y) ||
            (location.region2.x == topmostRegion.x && location.region2.y > topmostRegion.y)) {
            topmostRegion = location.region2;
        }

        // Up-left
        result.locations[0].region0 = topmostRegion;
        result.locations[0].region1 = getAdjacentRegion(topmostRegion, DOWN_LEFT);
        result.locations[0].region2 = getAdjacentRegion(topmostRegion, UP_LEFT);

        // Right
        result.locations[1].region0 = topmostRegion;
        result.locations[1].region1 = getAdjacentRegion(topmostRegion, DOWN_RIGHT);
        result.locations[1].region2 = getAdjacentRegion(topmostRegion, DOWN);

        // Down-left
        result.locations[2].region0 = result.locations[1].region2; // DOWN
        result.locations[2].region1 = result.locations[0].region1; // DOWN_LEFT
        result.locations[2].region2 = getAdjacentRegion(result.locations[1].region2, DOWN_LEFT);
    }
    return result;
}
