#include <assert.h>

#include "EdgeLocation-utils.h"
#include "RegionLocation-utils.h"
#include "VertexLocation-utils.h"

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

EdgeLocation getEdgeFromTwoVertices(VertexLocation a, VertexLocation b) {
    RegionLocation resultRegions[2];
    size_t r = 0;

    RegionLocation aRegions[3] = {a.region0, a.region1, a.region2};
    RegionLocation bRegions[3] = {b.region0, b.region1, b.region2};
    size_t ar = 0;
    while (ar < 3) {
        size_t br = 0;
        bool isFound = false;
        while (br < 3 && !isFound) {
            if (isRegionsEqual(aRegions[ar], bRegions[br])) {
                assert(r < 2);
                resultRegions[r] = aRegions[ar];
                r++;
                isFound = true;
            }
            br++;
        }
        ar++;
    }
    assert(r == 2);

    EdgeLocation result;
    result.region0 = resultRegions[0];
    result.region1 = resultRegions[1];
    return result;
}

SurroundingEdgesFromVertex getSurroundingEdgesFromVertex(VertexLocation location) {
    assert(NUM_SURROUNDING_EDGES_FROM_VERTEX == NUM_SURROUNDING_VERTICES_FROM_VERTEX);

    SurroundingEdgesFromVertex result;
    SurroundingVerticesFromVertex vertices = getSurroundingVerticesFromVertex(location);
    size_t v = 0;
    while (v < NUM_SURROUNDING_VERTICES_FROM_VERTEX) {
        result.locations[v] = getEdgeFromTwoVertices(location, vertices.locations[v]);
        v++;
    }

    return result;
}
