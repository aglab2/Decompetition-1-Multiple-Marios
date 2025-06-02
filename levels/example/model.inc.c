Vtx example_dl_Plane_mesh_layer_1_vtx_cull[8] = {
	{{ {-100, 0, 100}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-100, 0, 100}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-100, 0, -100}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-100, 0, -100}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, 0, 100}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, 0, 100}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, 0, -100}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, 0, -100}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx example_dl_Plane_mesh_layer_1_vtx_0[4] = {
	{{ {-100, 0, 100}, 0, {-16, 1008}, {0, 127, 0, 255} }},
	{{ {100, 0, 100}, 0, {1008, 1008}, {0, 127, 0, 255} }},
	{{ {100, 0, -100}, 0, {1008, -16}, {0, 127, 0, 255} }},
	{{ {-100, 0, -100}, 0, {-16, -16}, {0, 127, 0, 255} }},
};

Gfx example_dl_Plane_mesh_layer_1_tri_0[] = {
	gsSPVertex(example_dl_Plane_mesh_layer_1_vtx_0 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Gfx mat_example_dl_f3dlite_material[] = {
	gsSPLightColor(LIGHT_1, 0xFFFFFFFF),
	gsSPLightColor(LIGHT_2, 0x7F7F7FFF),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPEndDisplayList(),
};

Gfx mat_revert_example_dl_f3dlite_material[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx example_dl_Plane_mesh_layer_1[] = {
	gsSPDisplayList(mat_example_dl_f3dlite_material),
	gsSPDisplayList(example_dl_Plane_mesh_layer_1_tri_0),
	gsSPDisplayList(mat_revert_example_dl_f3dlite_material),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

