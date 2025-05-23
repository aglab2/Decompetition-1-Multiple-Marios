const Collision p10_collision[] = {
	COL_INIT(),
	COL_VERTEX_INIT(10),
	COL_VERTEX(0, 0, 0),
	COL_VERTEX(1000, 0, 0),
	COL_VERTEX(0, 0, -1000),
	COL_VERTEX(-1000, 0, 0),
	COL_VERTEX(-1000, 0, -1000),
	COL_VERTEX(-1020, 150, 0),
	COL_VERTEX(-1020, 150, -1000),
	COL_VERTEX(1000, 0, -1000),
	COL_VERTEX(1020, 150, 0),
	COL_VERTEX(1020, 150, -1000),
	COL_TRI_INIT(SURFACE_DEFAULT, 8),
	COL_TRI(0, 1, 2),
	COL_TRI(0, 2, 3),
	COL_TRI(3, 2, 4),
	COL_TRI(3, 4, 5),
	COL_TRI(5, 4, 6),
	COL_TRI(2, 1, 7),
	COL_TRI(7, 1, 8),
	COL_TRI(7, 8, 9),
	COL_TRI_STOP(),
	COL_END()
};
