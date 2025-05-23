const Collision p2_collision[] = {
	COL_INIT(),
	COL_VERTEX_INIT(25),
	COL_VERTEX(1000, 0, 0),
	COL_VERTEX(1020, 150, 0),
	COL_VERTEX(1000, 0, -1000),
	COL_VERTEX(1020, 150, -1000),
	COL_VERTEX(1000, 0, -2000),
	COL_VERTEX(1020, 150, -2000),
	COL_VERTEX(1000, 0, -3000),
	COL_VERTEX(1020, 150, -3000),
	COL_VERTEX(1000, 0, -4000),
	COL_VERTEX(1020, 150, -4000),
	COL_VERTEX(-1000, 0, -4000),
	COL_VERTEX(-1020, 150, -4000),
	COL_VERTEX(-1000, 0, -3000),
	COL_VERTEX(-1020, 150, -3000),
	COL_VERTEX(-1000, 0, -2000),
	COL_VERTEX(-1020, 150, -2000),
	COL_VERTEX(-1000, 0, -1000),
	COL_VERTEX(-1020, 150, -1000),
	COL_VERTEX(-1000, 0, 0),
	COL_VERTEX(-1020, 150, 0),
	COL_VERTEX(0, 0, 0),
	COL_VERTEX(0, 0, -1000),
	COL_VERTEX(0, 0, -2000),
	COL_VERTEX(0, 0, -3000),
	COL_VERTEX(0, 0, -4000),
	COL_TRI_INIT(SURFACE_DEFAULT, 32),
	COL_TRI(0, 1, 2),
	COL_TRI(2, 1, 3),
	COL_TRI(2, 3, 4),
	COL_TRI(4, 3, 5),
	COL_TRI(4, 5, 6),
	COL_TRI(6, 5, 7),
	COL_TRI(6, 7, 8),
	COL_TRI(8, 7, 9),
	COL_TRI(10, 11, 12),
	COL_TRI(12, 11, 13),
	COL_TRI(12, 13, 14),
	COL_TRI(14, 13, 15),
	COL_TRI(14, 15, 16),
	COL_TRI(16, 15, 17),
	COL_TRI(16, 17, 18),
	COL_TRI(18, 17, 19),
	COL_TRI(18, 20, 16),
	COL_TRI(16, 20, 21),
	COL_TRI(16, 21, 14),
	COL_TRI(14, 21, 22),
	COL_TRI(14, 22, 12),
	COL_TRI(12, 22, 23),
	COL_TRI(12, 23, 10),
	COL_TRI(10, 23, 24),
	COL_TRI(20, 0, 21),
	COL_TRI(21, 0, 2),
	COL_TRI(21, 2, 22),
	COL_TRI(22, 2, 4),
	COL_TRI(22, 4, 23),
	COL_TRI(23, 4, 6),
	COL_TRI(23, 6, 24),
	COL_TRI(24, 6, 8),
	COL_TRI_STOP(),
	COL_END()
};
