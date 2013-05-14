#include <assert.h>

#include "EdgeLocation-utils.h"
#include "RegionLocation-utils.h"

bool isValidEdge(EdgeLocation location) {
    return isRegionsAdjacent(location.region0, location.region1);
}

bool isEdgesEqual(EdgeLocation a, EdgeLocation b) {
    // {a.region0, a.region1} ∈ {permutations({b.region0, b.region1})}
    return (isRegionsEqual(a.region0, b.region0) && isRegionsEqual(a.region1, b.region1)) ||
        (isRegionsEqual(a.region0, b.region1) && isRegionsEqual(a.region1, b.region0));
}

EdgeType getEdgeType(EdgeLocation location) {
    assert(isValidEdge(location));
    // If one region is above another, the edge is flat.
    // Else if regions have the same y value, the edge has negative gradient.
    // Otherwise, the edge has positive gradient.

    EdgeType result = EDGE_POSITIVE;
    if (isRegionsEqual(getAdjacentRegion(location.region0, UP), location.region1) ||
        isRegionsEqual(getAdjacentRegion(location.region0, DOWN), location.region1)) {
        result = EDGE_FLAT;
    } else if (location.region0.y == location.region1.y) {
        result = EDGE_NEGATIVE;
    }
    return result;
}

EdgeLocation getAdjacentEdgeFromRegion(RegionLocation location, Direction direction) {
    //convert to this code
    /* TODO
     assert((direction == UP) ||
     (direction == UP_RIGHT) ||
     (direction == DOWN_RIGHT) ||
     (direction == DOWN) ||
     (direction == DOWN_LEFT) ||
     (direction == UP_LEFT));

     arc nextArcID;

     nextArcID.region0 = currentRegionID;
     nextArcID.region1 = regionAdjToRegion(currentRegionID, direction);
     //need to check that next arc is on land???
     return nextArcID;
     */
}

EdgeLocation getAdjacentEdgeFromVertex(VertexLocation location, Direction direction) {
    /* TODO
    assert(vertexExists(vertexID));

    arc returnArcID;//id to return

    //each pair of regions in the vertex defines an arc
    arc arc0ID = {vertexID.region0, vertexID.region1};
    arc arc1ID = {vertexID.region0, vertexID.region2};
    arc arc2ID = {vertexID.region0, vertexID.region2};

    int expectedArcOrient;

    //determine expected arc orientation for
    //input direction
    //since for a given vertex, there will be only one arc
    //in each orientation
    if ((direction == UP_RIGHT) ||
        (direction == DOWN_LEFT)) {
        expectedArcOrient = ARC_ORIENT_UP_RIGHT_TO_DOWN_LEFT;
    } else if ((direction == LEFT) ||
               (direction == RIGHT)) {
        expectedArcOrient = ARC_ORIENT_LEFT_TO_RIGHT;
    } else {
        expectedArcOrient = ARC_ORIENT_UP_LEFT_TO_DOWN_RIGHT;
    }

    //set the returnArcID to that of the arc in the
    //correct orientation

    if (arcOrientation(arc0ID) == expectedArcOrient) {
        returnArcID = arc0ID;
    } else if (arcOrientation(arc1ID) == expectedArcOrient) {
        returnArcID = arc1ID;
    } else {
        returnArcID = arc2ID;
    }

    return returnArcID;
    */
}
