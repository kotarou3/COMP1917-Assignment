#include <stdlib.h>
#include <assert.h>

#include "Game-wrapper.h"
#include "Map.h"
#include "RegionLocation-utils.h"
#include "EdgeLocation-utils.h"
#include "VertexLocation-utils.h"

#define MAX_REGION_DISTANCE_FROM_ORIGIN 2
#define MAX_DISTANCE_FROM_ORIGIN 3

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

bool isSea(Game* game, RegionLocation location) {
    return getRegion(&game->map, location, true)->isSea;
}

DegreeType getDegree(Game* game, RegionLocation location) {
    return getRegion(&game->map, location, true)->generatedDegree;
}

DiceValue getDiceValue(Game* game, RegionLocation location) {
    return getRegion(&game->map, location, true)->diceValue;
}

ArcType getARC(Game* game, EdgeLocation location) {
    Edge* edge = getEdge(&game->map, location, true);
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
    Vertex* vertex = getVertex(&game->map, location, true);
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


Region* getRegion(Map* map, RegionLocation location, bool isFatalOnNotFound) {
    size_t r = 0;
    while (r < NUM_ALL_REGIONS) {
        if (isRegionsEqual(map->regions[r].location, location)) {
            return &map->regions[r];
        }
        r++;
    }
    if (isFatalOnNotFound) {
        assert(!"Invalid region location");
    }
    return NULL;
}

Edge* getEdge(Map* map, EdgeLocation location, bool isFatalOnNotFound) {
    size_t e = 0;
    while (e < NUM_EDGES) {
        if (isEdgesEqual(map->edges[e].location, location)) {
            return &map->edges[e];
        }
        e++;
    }
    if (isFatalOnNotFound) {
        assert(!"Invalid edge location");
    }
    return NULL;
}

Vertex* getVertex(Map* map, VertexLocation location, bool isFatalOnNotFound) {
    size_t v = 0;
    while (v < NUM_VERTICES) {
        if (isVerticesEqual(map->vertices[v].location, location)) {
            return &map->vertices[v];
        }
        v++;
    }
    if (isFatalOnNotFound) {
        assert(!"Invalid vertex location");
    }
    return NULL;
}

static void constructRegions(Region* regions, DegreeType* generatedDegrees, DiceValue* diceValues) {
    // Set up regions by looping through array bottom to top, left to right
    // populating each region with the correct dice value and student type.
    // This is the order that generatedDegrees and diceValues happen to be in.
    size_t r = 0;
    size_t landRegionIndex = 0;
    RegionLocation location;
    location.x = -REGION_RADIUS;
    while (location.x <= REGION_RADIUS) {
        location.y = -REGION_RADIUS;
        while (location.y <= REGION_RADIUS) {
            // Make sure the region exists
            if (isExistentRegion(location)) {
                assert(r < NUM_ALL_REGIONS);

                if (isLandRegion(location)) {
                    assert(landRegionIndex < NUM_LAND_REGIONS);
                    constructRegion(&regions[r], location,
                        generatedDegrees[landRegionIndex],
                        diceValues[landRegionIndex]);
                    landRegionIndex++;
                } else {
                    constructRegion(&regions[r], location, 0, 0);
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
    anchorRegion.y = -REGION_RADIUS + 1;
    while (anchorRegion.y <= REGION_RADIUS) {
        anchorRegion.x = -REGION_RADIUS;
        while (anchorRegion.x <= REGION_RADIUS - 1) {
            EdgeLocation location;
            location.region0 = anchorRegion;
            location.region1 = getAdjacentRegion(anchorRegion, DOWN);

            // Make sure edge is adjacent to land
            if (isLandRegion(location.region0) || isLandRegion(location.region1)) {
                assert(e < NUM_EDGES);
                constructEdge(&edges[e], location);
                e++;
            }

            // Repeat two times for the other edges
            location.region1 = getAdjacentRegion(anchorRegion, DOWN_RIGHT);
            if (isLandRegion(location.region0) || isLandRegion(location.region1)) {
                assert(e < NUM_EDGES);
                constructEdge(&edges[e], location);
                e++;
            }

            location.region1 = getAdjacentRegion(anchorRegion, UP_RIGHT);
            if (isLandRegion(location.region0) || isLandRegion(location.region1)) {
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
    anchorRegion.y = -REGION_RADIUS + 1;
    while (anchorRegion.y <= REGION_RADIUS) {
        anchorRegion.x = -REGION_RADIUS;
        while (anchorRegion.x <= REGION_RADIUS - 1) {
            VertexLocation location;
            location.region0 = anchorRegion;
            location.region1 = getAdjacentRegion(anchorRegion, DOWN);
            location.region2 = getAdjacentRegion(anchorRegion, DOWN_RIGHT);

            // Make sure vertex is adjacent to land
            if (isLandRegion(location.region0) || isLandRegion(location.region1) || isLandRegion(location.region2)) {
                assert(v < NUM_VERTICES);
                constructVertex(&vertices[v], location);
                v++;
            }

            // Do it again for the other vertex
            location.region1 = getAdjacentRegion(anchorRegion, UP_RIGHT);
            if (isLandRegion(location.region0) || isLandRegion(location.region1) || isLandRegion(location.region2)) {
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
    region->isSea = !isLandRegion(location);

    if (!region->isSea) {
        region->generatedDegree = generatedDegree;
        region->diceValue = diceValue;
    }
}

static void destroyRegion(Region* region) {
    (void)region; // Do nothing
}

static void constructEdge(Edge* edge, EdgeLocation location) {
    edge->location = location;
    edge->isOwned = false;
}

static void destroyEdge(Edge* edge) {
    (void)edge; // Do nothing
}

static void constructVertex(Vertex* vertex, VertexLocation location) {
    vertex->location = location;
    vertex->isOwned = false;
}

static void destroyVertex(Vertex* vertex) {
    (void)vertex; // Do nothing
}
