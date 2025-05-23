const Collision p84_collision[] = {
	COL_INIT(),
	COL_VERTEX_INIT(10),
	COL_VERTEX(828, 0, -1172),
	COL_VERTEX(-172, 0, -1172),
	COL_VERTEX(0, 0, 0),
	COL_VERTEX(1828, 0, -1172),
	COL_VERTEX(1000, 0, 0),
	COL_VERTEX(1848, 150, -1172),
	COL_VERTEX(1020, 150, 0),
	COL_VERTEX(-192, 150, -1172),
	COL_VERTEX(-1020, 150, 0),
	COL_VERTEX(-1000, 0, 0),
	COL_TRI_INIT(SURFACE_DEFAULT, 8),
	COL_TRI(0, 1, 2),
	COL_TRI(0, 2, 3),
	COL_TRI(3, 2, 4),
	COL_TRI(3, 4, 5),
	COL_TRI(5, 4, 6),
	COL_TRI(7, 8, 9),
	COL_TRI(7, 9, 1),
	COL_TRI(1, 9, 2),
	COL_TRI_STOP(),
	COL_END()
};
