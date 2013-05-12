#include <assert.h>

#include "Game-wrapper.h"
#include "Map.h"

static void constructRegions(Region* regions, DegreeType* generatedDegrees, DiceValue* diceValues);
static void destroyRegions(Region* regions);
static void constructEdges(Edge* edges);
static void destroyEdges(Edge* edges);
static void constructVertices(Vertex* vertices);
static void destroyVertices(Vertex* vertices);

static void constructRegion(Region* region, RegionLocation location, DegreeType generatedDegree, DiceValue diceValue);
static void destroyRegion(Region* region);
static void constructEdge(Edge* edge, EdgeLocation location);
static void destroyEdge(Edge* edge);
static void constructVertex(Vertex* vertex, VertexLocation location);
static void destroyVertex(Vertex* vertex);

static bool isLand(RegionLocation location);
static bool isRegionsEqual(RegionLocation a, RegionLocation b);
static bool isEdgesEqual(EdgeLocation a, EdgeLocation b);
static bool isVerticesEqual(VertexLocation a, VertexLocation b);

bool isSea(Game* game, RegionLocation location) {
    return getRegion(&game->map, location)->isSea;
}

DegreeType getDegree(Game* game, RegionLocation location) {
    return getRegion(&game->map, location)->generatedDegree;
}

DiceValue getDiceValue(Game* game, RegionLocation location) {
    return getRegion(&game->map, location)->diceValue;
}

ArcType getARC(Game* game, EdgeLocation location) {
    Edge* edge = getEdge(&game->map, location);
    ArcType arcType = VACANT_ARC;

    if (edge->isOwned) {
        if (edge->owner == UNI_A) {
            arcType = ARC_A;
        } else if (edge->owner == UNI_B) {
            arcType = ARC_B;
        } else if (edge->owner == UNI_C) {
            arcType = ARC_C;
        }
    }

    return arcType;
}

CampusType getCampus(Game* game, VertexLocation location) {
    Vertex* vertex = getVertex(&game->map, location);
    CampusType campusType = VACANT_VERTEX;

    if (vertex->isOwned) {
        if (vertex->owner == UNI_A) {
            if (vertex->isGo8Campus) {
                campusType = GO8_CAMPUS_A;
            } else {
                campusType = CAMPUS_A;
            }
        } else if (vertex->owner == UNI_B) {
            if (vertex->isGo8Campus) {
                campusType = GO8_CAMPUS_B;
            } else {
                campusType = CAMPUS_B;
            }
        } else if (vertex->owner == UNI_C) {
            if (vertex->isGo8Campus) {
                campusType = GO8_CAMPUS_C;
            } else {
                campusType = CAMPUS_C;
            }
        }
    }

    return campusType;
}

void constructMap(Map* map, DegreeType* regionDegreeTypes, DiceValue* regionDiceValues) {
    constructRegions(map->regions, regionDegreeTypes, regionDiceValues);
    constructEdges(map->edges);
    constructVertices(map->vertices);
}

void destroyMap(Map* map) {
    destroyRegions(map->regions);
    destroyEdges(map->edges);
    destroyVertices(map->vertices);
}


Region* getRegion(Map* map, RegionLocation location) {
    size_t r = 0;
    while (r < NUM_ALL_REGIONS) {
        if (isRegionsEqual(map->regions[r].location, location)) {
            return &map->regions[r];
        }
        r++;
    }
    assert(!"Invalid region location");
}

Edge* getEdge(Map* map, EdgeLocation location) {
    size_t e = 0;
    while (e < NUM_EDGES) {
        if (isEdgesEqual(map->edges[e].location, location)) {
            return &map->edges[e];
        }
        e++;
    }
    assert(!"Invalid edge location");
}

Vertex* getVertex(Map* map, VertexLocation location) {
    size_t v = 0;
    while (v < NUM_VERTICES) {
        if (isVerticesEqual(map->vertices[v].location, location)) {
            return &map->vertices[v];
        }
        v++;
    }
    assert(!"Invalid vertex location");
}

RegionLocation getAdjacentRegion(RegionLocation location, Direction direction) {
    assert(direction & UP || direction & DOWN);

    RegionLocation result;
    result.x = location.x;
    result.y = location.y;

    if (direction & LEFT) {
        --result.x;
    } else if (direction & RIGHT) {
        result.x++;
    }

    if (direction == DOWN || direction == DOWN_RIGHT) {
        --result.y;
    } else if (direction == UP || direction == UP_LEFT) {
        result.y++;
    }

    return result;
}

RegionLocation getAdjacentRegionFromEdge(EdgeLocation location, Direction direction) {
    // TODO
}

RegionLocation getAdjacentRegionFromVertex(VertexLocation location, Direction direction) {
    // TODO
}

EdgeLocation getAdjacentEdgeFromRegion(RegionLocation location, Direction direction) {
    // TODO
}

EdgeLocation getAdjacentEdgeFromVertex(VertexLocation location, Direction direction) {
    // TODO
}

VertexLocation getAdjacentVertexFromRegion(RegionLocation location, Direction direction) {
    // TODO
}

VertexLocation getAdjacentVertexFromEdge(EdgeLocation location, Direction direction) {
    // TODO
}

static void constructRegions(Region* regions, DegreeType* generatedDegrees, DiceValue* diceValues) {
    // Set up regions by looping through array bottom to top, left to right
    // populating each region with the correct dice value and student type.
    // This is the order that generatedDegrees and diceValues happen to be in.
    size_t r = 0;
    size_t landRegionIndex = 0;
    RegionLocation location;
    location.x = -3;
    while (location.x <= 3) {
        location.y = -3;
        while (location.y <= 3) {
            if (-3 <= location.x + location.y && location.x + location.y <= 3) {
                assert(r < NUM_ALL_REGIONS);

                if (landRegionIndex == NUM_LAND_REGIONS) {
                    constructRegion(&regions[r], location, 0, 0);
                } else {
                    constructRegion(&regions[r], location, generatedDegrees[landRegionIndex], diceValues[landRegionIndex]);
                }

                if (!regions[r].isSea) {
                    assert(landRegionIndex < NUM_LAND_REGIONS);
                    landRegionIndex++;
                }

                r++;
            }
            location.y++;
        }
        location.x++;
    }
    assert(r == NUM_ALL_REGIONS);
    assert(landRegionIndex == NUM_LAND_REGIONS);
}

static void destroyRegions(Region* regions) {
    size_t r = 0;
    while (r < NUM_ALL_REGIONS) {
        destroyRegion(&regions[r]);
        r++;
    }
}

static void constructEdges(Edge* edges) {
    // By looking at diagram on open learning, each region can define three edges:
    //  - One with the region below
    //  - One with the region below-right
    //  - One with the region up-right
    // except regions in the last column and row

    // Loop through regions (bottom + 1) to top, then left to (right - 1)
    // and add the edges for each region
    size_t e = 0;
    RegionLocation anchorRegion;
    anchorRegion.y = -3 + 1;
    while (anchorRegion.y <= 3) {
        anchorRegion.x = -3;
        while (anchorRegion.x <= 3 - 1) {
            EdgeLocation location;
            location.region0 = anchorRegion;
            location.region1 = getAdjacentRegion(anchorRegion, DOWN);

            // Make sure edge is adjacent to land
            if (isLand(location.region0) || isLand(location.region1)) {
                assert(e < NUM_EDGES);
                constructEdge(&edges[e], location);
                e++;
            }

            // Repeat two times for the other edges
            location.region1 = getAdjacentRegion(anchorRegion, DOWN_RIGHT);
            if (isLand(location.region0) || isLand(location.region1)) {
                assert(e < NUM_EDGES);
                constructEdge(&edges[e], location);
                e++;
            }

            location.region1 = getAdjacentRegion(anchorRegion, UP_RIGHT);
            if (isLand(location.region0) || isLand(location.region1)) {
                assert(e < NUM_EDGES);
                constructEdge(&edges[e], location);
                e++;
            }

            anchorRegion.x++;
        }
        anchorRegion.y++;
    }
    assert(e == NUM_EDGES);
}

static void destroyEdges(Edge* edges) {
    size_t e = 0;
    while (e < NUM_EDGES) {
        destroyEdge(&edges[e]);
        e++;
    }
}

static void constructVertices(Vertex* vertices) {
    // By looking at diagram on openlearning, each region can define two vertices:
    //  - One with the region below and below-right
    //  - One with the region above-right and below-right
    // except regions in the last column and row

    // Loop through regions (bottom + 1) to top, then left to (right - 1)
    // and add the vertices for each region
    size_t v = 0;
    RegionLocation anchorRegion;
    anchorRegion.y = -3 + 1;
    while (anchorRegion.y <= 3) {
        anchorRegion.x = -3;
        while (anchorRegion.x <= 3 - 1) {
            VertexLocation location;
            location.region0 = anchorRegion;
            location.region1 = getAdjacentRegion(anchorRegion, DOWN);
            location.region2 = getAdjacentRegion(anchorRegion, DOWN_RIGHT);

            // Make sure vertex is adjacent to land
            if (isLand(location.region0) || isLand(location.region1) || isLand(location.region2)) {
                assert(v < NUM_VERTICES);
                constructVertex(&vertices[v], location);
                v++;
            }

            // Do it again for the other vertex
            location.region1 = getAdjacentRegion(anchorRegion, UP_RIGHT);
            if (isLand(location.region0) || isLand(location.region1) || isLand(location.region2)) {
                assert(v < NUM_VERTICES);
                constructVertex(&vertices[v], location);
                v++;
            }

            anchorRegion.x++;
        }
        anchorRegion.y++;
    }
    assert(v == NUM_VERTICES);
}

static void destroyVertices(Vertex* vertices) {
    size_t v = 0;
    while (v < NUM_VERTICES) {
        destroyVertex(&vertices[v]);
        v++;
    }
}

static void constructRegion(Region* region, RegionLocation location, DegreeType generatedDegree, DiceValue diceValue) {
    region->location = location;
    region->isSea = !isLand(location);

    region->isTraining = false;
    if (isRegionsEqual(location, TRAINING_CENTRE_BPS)) {
        region->isTraining = true;
        region->generatedDegree = DEGREE_BPS;
    } else if (isRegionsEqual(location, TRAINING_CENTRE_BQN)) {
        region->isTraining = true;
        region->generatedDegree = DEGREE_BQN;
    } else if (isRegionsEqual(location, TRAINING_CENTRE_MJ)) {
        region->isTraining = true;
        region->generatedDegree = DEGREE_MJ;
    } else if (isRegionsEqual(location, TRAINING_CENTRE_MTV)) {
        region->isTraining = true;
        region->generatedDegree = DEGREE_MTV;
    } else if (isRegionsEqual(location, TRAINING_CENTRE_MMONEY)) {
        region->isTraining = true;
        region->generatedDegree = DEGREE_MMONEY;
    }

    if (!region->isSea) {
        region->generatedDegree = generatedDegree;
        region->diceValue = diceValue;
    }
}

static void destroyRegion(Region* region) {
    // Do nothing
}

static void constructEdge(Edge* edge, EdgeLocation location) {
    edge->location = location;
    edge->isOwned = false;
}

static void destroyEdge(Edge* edge) {
    // Do nothing
}

static void constructVertex(Vertex* vertex, VertexLocation location) {
    vertex->location = location;
    vertex->isOwned = false;
}

static void destroyVertex(Vertex* vertex) {
    // Do nothing
}

static bool isLand(RegionLocation location) {
    return -3 < location.x && location.x < 3 &&
        -3 < location.y && location.y < 3 &&
        -3 < location.x + location.y && location.x + location.y < 3;
}

static bool isRegionsEqual(RegionLocation a, RegionLocation b) {
    return a.x == b.x && a.y == b.y;
}

static bool isEdgesEqual(EdgeLocation a, EdgeLocation b) {
    // {a.region0, a.region1} ∈ {permutations({b.region0, b.region1})}
    return (isRegionsEqual(a.region0, b.region0) && isRegionsEqual(a.region1, b.region1)) ||
        (isRegionsEqual(a.region0, b.region1) && isRegionsEqual(a.region1, b.region0));
}

static bool isVerticesEqual(VertexLocation a, VertexLocation b) {
    // {a.region0, a.region1, a.region2} ∈ {permutations({b.region0, b.region1, b.region2})}
    return (isRegionsEqual(a.region0, b.region0) && isRegionsEqual(a.region1, b.region1) && isRegionsEqual(a.region2, b.region2)) ||
        (isRegionsEqual(a.region0, b.region0) && isRegionsEqual(a.region1, b.region2) && isRegionsEqual(a.region2, b.region1)) ||
        (isRegionsEqual(a.region0, b.region1) && isRegionsEqual(a.region1, b.region0) && isRegionsEqual(a.region2, b.region2)) ||
        (isRegionsEqual(a.region0, b.region1) && isRegionsEqual(a.region1, b.region2) && isRegionsEqual(a.region2, b.region0)) ||
        (isRegionsEqual(a.region0, b.region2) && isRegionsEqual(a.region1, b.region0) && isRegionsEqual(a.region2, b.region1)) ||
        (isRegionsEqual(a.region0, b.region2) && isRegionsEqual(a.region1, b.region1) && isRegionsEqual(a.region2, b.region0));
}
