var order = {
    game: [
        "Game-redefines.h",
        "Map.h",
        "University.h",
        "Game-wrapper.h",
        "RegionLocation-utils.h",
        "EdgeLocation-utils.h",
        "VertexLocation-utils.h",
        "Game.c",
        "Map.c",
        "University.c",
        "RegionLocation-utils.c",
        "EdgeLocation-utils.c",
        "VertexLocation-utils.c"
    ],
    ai: [
        "Game-redefines.h",
        "RegionLocation-utils.h",
        "EdgeLocation-utils.h",
        "VertexLocation-utils.h",
        "mechanicalTurk-Map.h",
        "mechanicalTurk-Resources.h",
        "mechanicalTurk-wrapper.h",
        "mechanicalTurk.c",
        "mechanicalTurk-Map.c",
        "mechanicalTurk-Resources.c",
        "RegionLocation-utils.c",
        "EdgeLocation-utils.c",
        "VertexLocation-utils.c"
    ]
};

var header =
"// ====================================================\n" +
"// File: {{filename}}\n" +
"// ====================================================\n"

if (!order[process.argv[2]]) {
    console.log("Please specify one of: " + Object.keys(order).join(", "));
    process.exit(1);
}

var fs = require("fs");
var out = fs.createWriteStream("_" + process.argv[2] + ".c");
for (var f = 0; f < order[process.argv[2]].length; ++f) {
    if (f != 0)
        out.write("\n");
    out.write(header.replace("{{filename}}", order[process.argv[2]][f]));
    out.write(fs.readFileSync(order[process.argv[2]][f]).toString().replace(/(#include "[^"]+")/g, "//$1"));
}
out.end();
