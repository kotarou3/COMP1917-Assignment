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
