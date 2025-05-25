void scroll_gfx_castle_grounds_dl_fs_plane_mesh_layer_1() {
	Gfx *mat = segmented_to_virtual(castle_grounds_dl_fs_plane_mesh_layer_1);


	shift_t(mat, 13, PACK_TILESIZE(0, 1));
	shift_s(mat, 18, PACK_TILESIZE(0, 1));

};

void scroll_castle_grounds() {
	scroll_gfx_castle_grounds_dl_fs_plane_mesh_layer_1();
};
