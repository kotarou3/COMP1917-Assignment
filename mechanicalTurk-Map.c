#include <stdlib.h> // For NULL
#include <math.h> // For INFINITY
#include <assert.h>

#include "mechanicalTurk-Map.h"
#include "RegionLocation-utils.h"

static void constructMechanicalTurkRegions(MechanicalTurkRegion* regions, Game* game);
static void destroyMechanicalTurkRegions(MechanicalTurkRegion* regions);
static void constructMechanicalTurkEdges(MechanicalTurkEdge* edges, Game* game);
static void destroyMechanicalTurkEdges(MechanicalTurkEdge* edges);
static void constructMechanicalTurkVertices(MechanicalTurkVertex* vertices, Game* game);
static void destroyMechanicalTurkVertices(MechanicalTurkVertex* vertices);

static void constructMechanicalTurkRegion(MechanicalTurkRegion* region, Game* game, RegionLocation location);
static void destroyMechanicalTurkRegion(MechanicalTurkRegion* region);
static void constructMechanicalTurkEdge(MechanicalTurkEdge* edge, Game* game, EdgeLocation location);
static void destroyMechanicalTurkEdge(MechanicalTurkEdge* edge);
static void constructMechanicalTurkVertex(MechanicalTurkVertex* vertex, Game* game, VertexLocation location);
static void destroyMechanicalTurkVertex(MechanicalTurkVertex* vertex);

static void connectMechanicalTurkEdgesAndVertices(MechanicalTurkMap* map);

void constructMechanicalTurkMap(MechanicalTurkMap* map, Game* game) {
    constructMechanicalTurkRegions(map->regions, game);
    constructMechanicalTurkEdges(map->edges, game);
    constructMechanicalTurkVertices(map->vertices, game);
    connectMechanicalTurkEdgesAndVertices(map);
}

void destroyMechanicalTurkMap(MechanicalTurkMap* map) {
    destroyMechanicalTurkRegions(map->regions);
    destroyMechanicalTurkEdges(map->edges);
    destroyMechanicalTurkVertices(map->vertices);
}

MechanicalTurkRegion* getMechanicalTurkRegion(MechanicalTurkMap* map, RegionLocation location, bool isFatalOnNotFound) {
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

MechanicalTurkEdge* getMechanicalTurkEdge(MechanicalTurkMap* map, EdgeLocation location, bool isFatalOnNotFound) {
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

MechanicalTurkVertex* getMechanicalTurkVertex(MechanicalTurkMap* map, VertexLocation location, bool isFatalOnNotFound) {
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

static void constructMechanicalTurkRegions(MechanicalTurkRegion* regions, Game* game) {
    // Set up regions by looping through array bottom to top, left to right
    size_t r = 0;
    RegionLocation location;
    location.x = -REGION_RADIUS;
    while (location.x <= REGION_RADIUS) {
        location.y = -REGION_RADIUS;
        while (location.y <= REGION_RADIUS) {
            // Make sure the region exists
            if (isExistentRegion(location)) {
                assert(r < NUM_ALL_REGIONS);
                constructMechanicalTurkRegion(&regions[r], game, location);
                r++;
            }
            location.y++;
        }
        location.x++;
    }
    assert(r == NUM_ALL_REGIONS);
}

static void destroyMechanicalTurkRegions(MechanicalTurkRegion* regions) {
    size_t r = 0;
    while (r < NUM_ALL_REGIONS) {
        destroyMechanicalTurkRegion(&regions[r]);
        r++;
    }
}

static void constructMechanicalTurkEdges(MechanicalTurkEdge* edges, Game* game) {
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
                constructMechanicalTurkEdge(&edges[e], game, location);
                e++;
            }

            // Repeat two times for the other edges
            location.region1 = getAdjacentRegion(anchorRegion, DOWN_RIGHT);
            if (isLandRegion(location.region0) || isLandRegion(location.region1)) {
                assert(e < NUM_EDGES);
                constructMechanicalTurkEdge(&edges[e], game, location);
                e++;
            }

            location.region1 = getAdjacentRegion(anchorRegion, UP_RIGHT);
            if (isLandRegion(location.region0) || isLandRegion(location.region1)) {
                assert(e < NUM_EDGES);
                constructMechanicalTurkEdge(&edges[e], game, location);
                e++;
            }

            anchorRegion.x++;
        }
        anchorRegion.y++;
    }
    assert(e == NUM_EDGES);
}

static void destroyMechanicalTurkEdges(MechanicalTurkEdge* edges) {
    size_t e = 0;
    while (e < NUM_EDGES) {
        destroyMechanicalTurkEdge(&edges[e]);
        e++;
    }
}

static void constructMechanicalTurkVertices(MechanicalTurkVertex* vertices, Game* game) {
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
                constructMechanicalTurkVertex(&vertices[v], game, location);
                v++;
            }

            // Do it again for the other vertex
            location.region1 = getAdjacentRegion(anchorRegion, UP_RIGHT);
            if (isLandRegion(location.region0) || isLandRegion(location.region1) || isLandRegion(location.region2)) {
                assert(v < NUM_VERTICES);
                constructMechanicalTurkVertex(&vertices[v], game, location);
                v++;
            }

            anchorRegion.x++;
        }
        anchorRegion.y++;
    }
    assert(v == NUM_VERTICES);
}

static void destroyMechanicalTurkVertices(MechanicalTurkVertex* vertices) {
    size_t v = 0;
    while (v < NUM_VERTICES) {
        destroyMechanicalTurkVertex(&vertices[v]);
        v++;
    }
}

static void constructMechanicalTurkRegion(MechanicalTurkRegion* region, Game* game, RegionLocation location) {
    region->location = location;
    region->isSea = isSea(game, location);
    if (!region->isSea) {
        region->generatedDegree = getDegree(game, location);
        region->diceValue = getDiceValue(game, location);
    }
}

static void destroyMechanicalTurkRegion(MechanicalTurkRegion* region) {
    (void)region; // Do nothing
}

static void constructMechanicalTurkEdge(MechanicalTurkEdge* edge, Game* game, EdgeLocation location) {
    edge->location = location;
    edge->adjacentVertexCount = 0;

    ArcType arcType = getARC(game, location);
    if (arcType == VACANT_ARC) {
        edge->isOwned = false;
    } else {
        edge->isOwned = true;
        if (arcType == ARC_A) {
            edge->owner = UNI_A;
        } else if (arcType == ARC_B) {
            edge->owner = UNI_B;
        } else if (arcType == ARC_C) {
            edge->owner = UNI_C;
        }
    }
}

static void destroyMechanicalTurkEdge(MechanicalTurkEdge* edge) {
    (void)edge; // Do nothing
}

static void constructMechanicalTurkVertex(MechanicalTurkVertex* vertex, Game* game, VertexLocation location) {
    vertex->location = location;
    vertex->adjacentEdgeCount = 0;
    vertex->desirability = -INFINITY;

    CampusType campusType = getCampus(game, location);
    if (campusType == VACANT_VERTEX) {
        vertex->isOwned = false;
    } else {
        vertex->isOwned = true;
        if (campusType == CAMPUS_A || campusType == GO8_CAMPUS_A) {
            vertex->owner = UNI_A;
            vertex->isGo8Campus = campusType == GO8_CAMPUS_A;
        } else if (campusType == CAMPUS_B || campusType == GO8_CAMPUS_B) {
            vertex->owner = UNI_B;
            vertex->isGo8Campus = campusType == GO8_CAMPUS_B;
        } else if (campusType == CAMPUS_C || campusType == GO8_CAMPUS_C) {
            vertex->owner = UNI_C;
            vertex->isGo8Campus = campusType == GO8_CAMPUS_C;
        }
    }
}

static void destroyMechanicalTurkVertex(MechanicalTurkVertex* vertex) {
    (void)vertex; // Do nothing
}

static void connectMechanicalTurkEdgesAndVertices(MechanicalTurkMap* map) {
    size_t e = 0;
    while (e < NUM_EDGES) {
        map->edges[e].adjacentRegions[0] = getMechanicalTurkRegion(map, map->edges[e].location.region0, true);
        map->edges[e].adjacentRegions[1] = getMechanicalTurkRegion(map, map->edges[e].location.region1, true);

        SurroundingVerticesFromEdge vertices = getSurroundingVerticesFromEdge(map->edges[e].location);
        size_t v = 0;
        while (v < NUM_SURROUNDING_VERTICES_FROM_EDGE) {
            MechanicalTurkVertex* foundVertex = getMechanicalTurkVertex(map, vertices.locations[v], false);
            if (foundVertex != NULL) {
                map->edges[e].adjacentVertices[map->edges[e].adjacentVertexCount] = foundVertex;
                map->edges[e].adjacentVertexCount++;
            }
            v++;
        }
        e++;
    }

    size_t v = 0;
    while (v < NUM_VERTICES) {
        map->vertices[v].adjacentRegions[0] = getMechanicalTurkRegion(map, map->vertices[v].location.region0, true);
        map->vertices[v].adjacentRegions[1] = getMechanicalTurkRegion(map, map->vertices[v].location.region1, true);
        map->vertices[v].adjacentRegions[2] = getMechanicalTurkRegion(map, map->vertices[v].location.region2, true);

        SurroundingEdgesFromVertex edges = getSurroundingEdgesFromVertex(map->vertices[v].location);
        size_t e = 0;
        while (e < NUM_SURROUNDING_EDGES_FROM_VERTEX) {
            MechanicalTurkEdge* foundEdge = getMechanicalTurkEdge(map, edges.locations[e], false);
            if (foundEdge != NULL) {
                map->vertices[v].adjacentEdges[map->vertices[v].adjacentEdgeCount] = foundEdge;
                map->vertices[v].adjacentEdgeCount++;
            }
            e++;
        }
        v++;
    }
}
