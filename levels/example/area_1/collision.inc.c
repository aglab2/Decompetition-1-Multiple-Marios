const Collision example_area_1_collision[] = {
	COL_INIT(),
	COL_VERTEX_INIT(4),
	COL_VERTEX(-100, 0, 100),
	COL_VERTEX(100, 0, 100),
	COL_VERTEX(100, 0, -100),
	COL_VERTEX(-100, 0, -100),
	COL_TRI_INIT(SURFACE_DEFAULT, 0),
	COL_TRI_STOP(),
	COL_END()
};
