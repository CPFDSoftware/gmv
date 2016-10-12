0a1,281
> netcdf testcp_nl { // format variant: 64bit 
> dimensions:
> 	len_string = 33 ;
> 	len_line = 81 ;
> 	four = 4 ;
> 	time_step = UNLIMITED ; // (0 currently)
> 	num_dim = 3 ;
> 	num_nodes = 33 ;
> 	num_elem = 7 ;
> 	num_el_blk = 7 ;
> 	num_node_sets = 2 ;
> 	num_side_sets = 5 ;
> 	num_el_in_blk1 = 1 ;
> 	num_nod_per_el1 = 4 ;
> 	num_att_in_blk1 = 1 ;
> 	num_el_in_blk2 = 1 ;
> 	num_nod_per_el2 = 4 ;
> 	num_att_in_blk2 = 1 ;
> 	num_el_in_blk3 = 1 ;
> 	num_nod_per_el3 = 8 ;
> 	num_att_in_blk3 = 1 ;
> 	num_el_in_blk4 = 1 ;
> 	num_nod_per_el4 = 4 ;
> 	num_att_in_blk4 = 1 ;
> 	num_el_in_blk5 = 1 ;
> 	num_nod_per_el5 = 6 ;
> 	num_att_in_blk5 = 1 ;
> 	num_el_in_blk6 = 1 ;
> 	num_nod_per_el6 = 8 ;
> 	num_att_in_blk6 = 1 ;
> 	num_el_in_blk7 = 1 ;
> 	num_nod_per_el7 = 3 ;
> 	num_att_in_blk7 = 1 ;
> 	num_nod_ns1 = 5 ;
> 	num_nod_ns2 = 3 ;
> 	num_side_ss1 = 2 ;
> 	num_df_ss1 = 4 ;
> 	num_side_ss2 = 2 ;
> 	num_df_ss2 = 4 ;
> 	num_side_ss3 = 7 ;
> 	num_side_ss4 = 8 ;
> 	num_side_ss5 = 10 ;
> variables:
> 	float time_whole(time_step) ;
> 	int eb_status(num_el_blk) ;
> 	int eb_prop1(num_el_blk) ;
> 		eb_prop1:name = "ID" ;
> 	int ns_status(num_node_sets) ;
> 	int ns_prop1(num_node_sets) ;
> 		ns_prop1:name = "ID" ;
> 	int ss_status(num_side_sets) ;
> 	int ss_prop1(num_side_sets) ;
> 		ss_prop1:name = "ID" ;
> 	float coordx(num_nodes) ;
> 	float coordy(num_nodes) ;
> 	float coordz(num_nodes) ;
> 	char coor_names(num_dim, len_string) ;
> 	char eb_names(num_el_blk, len_string) ;
> 	char ns_names(num_node_sets, len_string) ;
> 	char ss_names(num_side_sets, len_string) ;
> 	int elem_map(num_elem) ;
> 	float attrib1(num_el_in_blk1, num_att_in_blk1) ;
> 	char attrib_name1(num_att_in_blk1, len_string) ;
> 	int connect1(num_el_in_blk1, num_nod_per_el1) ;
> 		connect1:elem_type = "quad" ;
> 	float attrib2(num_el_in_blk2, num_att_in_blk2) ;
> 	char attrib_name2(num_att_in_blk2, len_string) ;
> 	int connect2(num_el_in_blk2, num_nod_per_el2) ;
> 		connect2:elem_type = "quad" ;
> 	float attrib3(num_el_in_blk3, num_att_in_blk3) ;
> 	char attrib_name3(num_att_in_blk3, len_string) ;
> 	int connect3(num_el_in_blk3, num_nod_per_el3) ;
> 		connect3:elem_type = "hex" ;
> 	float attrib4(num_el_in_blk4, num_att_in_blk4) ;
> 	char attrib_name4(num_att_in_blk4, len_string) ;
> 	int connect4(num_el_in_blk4, num_nod_per_el4) ;
> 		connect4:elem_type = "tetra" ;
> 	float attrib5(num_el_in_blk5, num_att_in_blk5) ;
> 	char attrib_name5(num_att_in_blk5, len_string) ;
> 	int connect5(num_el_in_blk5, num_nod_per_el5) ;
> 		connect5:elem_type = "wedge" ;
> 	float attrib6(num_el_in_blk6, num_att_in_blk6) ;
> 	char attrib_name6(num_att_in_blk6, len_string) ;
> 	int connect6(num_el_in_blk6, num_nod_per_el6) ;
> 		connect6:elem_type = "tetra" ;
> 	float attrib7(num_el_in_blk7, num_att_in_blk7) ;
> 	char attrib_name7(num_att_in_blk7, len_string) ;
> 	int connect7(num_el_in_blk7, num_nod_per_el7) ;
> 		connect7:elem_type = "tri" ;
> 	int eb_prop2(num_el_blk) ;
> 		eb_prop2:_FillValue = 0 ;
> 		eb_prop2:name = "MATERIAL_PROPERTY_LONG_NAME_32CH" ;
> 	int eb_prop3(num_el_blk) ;
> 		eb_prop3:_FillValue = 0 ;
> 		eb_prop3:name = "DENSITY" ;
> 	int node_ns1(num_nod_ns1) ;
> 	float dist_fact_ns1(num_nod_ns1) ;
> 	int node_ns2(num_nod_ns2) ;
> 	float dist_fact_ns2(num_nod_ns2) ;
> 	int ns_prop2(num_node_sets) ;
> 		ns_prop2:_FillValue = 0 ;
> 		ns_prop2:name = "FACE" ;
> 	int ns_prop3(num_node_sets) ;
> 		ns_prop3:name = "VELOCITY" ;
> 	int elem_ss1(num_side_ss1) ;
> 	int side_ss1(num_side_ss1) ;
> 	float dist_fact_ss1(num_df_ss1) ;
> 	int elem_ss2(num_side_ss2) ;
> 	int side_ss2(num_side_ss2) ;
> 	float dist_fact_ss2(num_df_ss2) ;
> 	int elem_ss3(num_side_ss3) ;
> 	int side_ss3(num_side_ss3) ;
> 	int elem_ss4(num_side_ss4) ;
> 	int side_ss4(num_side_ss4) ;
> 	int elem_ss5(num_side_ss5) ;
> 	int side_ss5(num_side_ss5) ;
> 	int ss_prop2(num_side_sets) ;
> 		ss_prop2:_FillValue = 0 ;
> 		ss_prop2:name = "COLOR" ;
> 
> // global attributes:
> 		:api_version = 4.46f ;
> 		:version = 3.05f ;
> 		:floating_point_word_size = 4 ;
> 		:file_size = 1 ;
> 		:title = "This is a test" ;
> data:
> 
>  eb_status = 1, 1, 1, 1, 1, 1, 1 ;
> 
>  eb_prop1 = 10, 11, 12, 13, 14, 15, 16 ;
> 
>  ns_status = 1, 1 ;
> 
>  ns_prop1 = 20, 21 ;
> 
>  ss_status = 1, 1, 1, 1, 1 ;
> 
>  ss_prop1 = 30, 31, 32, 33, 34 ;
> 
>  coordx = 0, 1, 1, 0, 1, 2, 2, 1, 0, 10, 10, 1, 1, 10, 10, 1, 0, 1, 10, 7, 3, 
>     6, 0, 3, 6, 0, 2.7, 6, 5.7, 3.7, 0, 10, 10 ;
> 
>  coordy = 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 10, 10, 10, 10, 0, 0, 0, 5, 0, 
>     0, 0, 2, 2, 2, 1.7, 1.7, 1.7, 0, 0, 0, 10 ;
> 
>  coordz = 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10, -10, 0, 0, -10, -10, 0, 5, 2, 3, 
>     6, 0, 0, 6, 2, 0, 2.7, 3.3, 1.7, 2.3, 0, 0, 10 ;
> 
>  coor_names =
>   "xcoor",
>   "ycoor",
>   "zcoor" ;
> 
>  eb_names =
>   "block_1",
>   "block_2",
>   "block_3",
>   "block_4",
>   "block_5",
>   "block_6",
>   "block_7" ;
> 
>  ns_names =
>   "nset_1",
>   "nset_2" ;
> 
>  ss_names =
>   "sset_1",
>   "sset_2",
>   "sset_3",
>   "sset_4",
>   "sset_5" ;
> 
>  elem_map = 1, 2, 3, 4, 5, 6, 7 ;
> 
>  attrib1 =
>   3.1416 ;
> 
>  attrib_name1 =
>   "THICKNESS" ;
> 
>  connect1 =
>   1, 2, 3, 4 ;
> 
>  attrib2 =
>   6.1416 ;
> 
>  attrib_name2 =
>   "THICKNESS" ;
> 
>  connect2 =
>   5, 6, 7, 8 ;
> 
>  attrib3 =
>   6.1416 ;
> 
>  attrib_name3 =
>   "THICKNESS" ;
> 
>  connect3 =
>   9, 10, 11, 12, 13, 14, 15, 16 ;
> 
>  attrib4 =
>   6.1416 ;
> 
>  attrib_name4 =
>   "THICKNESS" ;
> 
>  connect4 =
>   17, 18, 19, 20 ;
> 
>  attrib5 =
>   6.1416 ;
> 
>  attrib_name5 =
>   "THICKNESS" ;
> 
>  connect5 =
>   21, 22, 23, 24, 25, 26 ;
> 
>  attrib6 =
>   6.1416 ;
> 
>  attrib_name6 =
>   "THICKNESS" ;
> 
>  connect6 =
>   17, 18, 19, 20, 27, 28, 30, 29 ;
> 
>  attrib7 =
>   6.1416 ;
> 
>  attrib_name7 =
>   "THICKNESS" ;
> 
>  connect7 =
>   31, 32, 33 ;
> 
>  eb_prop2 = 10, 20, 30, 40, 50, 60, 70 ;
> 
>  eb_prop3 = _, _, _, _, _, _, _ ;
> 
>  node_ns1 = 100, 101, 102, 103, 104 ;
> 
>  dist_fact_ns1 = 1, 2, 3, 4, 5 ;
> 
>  node_ns2 = 200, 201, 202 ;
> 
>  dist_fact_ns2 = 1.1, 2.1, 3.1 ;
> 
>  ns_prop2 = 4, 5 ;
> 
>  ns_prop3 = 1000, 2000 ;
> 
>  elem_ss1 = 2, 2 ;
> 
>  side_ss1 = 4, 2 ;
> 
>  dist_fact_ss1 = 30, 30.1, 30.2, 30.3 ;
> 
>  elem_ss2 = 1, 2 ;
> 
>  side_ss2 = 2, 3 ;
> 
>  dist_fact_ss2 = 31, 31.1, 31.2, 31.3 ;
> 
>  elem_ss3 = 3, 3, 3, 3, 3, 3, 3 ;
> 
>  side_ss3 = 5, 3, 3, 2, 4, 1, 6 ;
> 
>  elem_ss4 = 4, 4, 4, 4, 6, 6, 6, 6 ;
> 
>  side_ss4 = 1, 2, 3, 4, 1, 2, 3, 4 ;
> 
>  elem_ss5 = 5, 5, 5, 5, 5, 7, 7, 7, 7, 7 ;
> 
>  side_ss5 = 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 ;
> 
>  ss_prop2 = 100, 101, _, _, _ ;
> }
