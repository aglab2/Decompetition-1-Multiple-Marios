const Collision p4_collision[] = {
	COL_INIT(),
	COL_VERTEX_INIT(8),
	COL_VERTEX(500, 0, -4000),
	COL_VERTEX(-500, 0, -4000),
	COL_VERTEX(-500, 0, 0),
	COL_VERTEX(500, 0, 0),
	COL_VERTEX(-520, 150, 0),
	COL_VERTEX(-520, 150, -4000),
	COL_VERTEX(520, 150, 0),
	COL_VERTEX(520, 150, -4000),
	COL_TRI_INIT(SURFACE_DEFAULT, 6),
	COL_TRI(0, 1, 2),
	COL_TRI(0, 2, 3),
	COL_TRI(4, 2, 1),
	COL_TRI(4, 1, 5),
	COL_TRI(0, 3, 6),
	COL_TRI(0, 6, 7),
	COL_TRI_STOP(),
	COL_END()
};
