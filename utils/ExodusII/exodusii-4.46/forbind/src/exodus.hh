/*
 * Copyright (c) 2005 Sandia Corporation. Under the terms of Contract
 * DE-AC04-94AL85000 with Sandia Corporation, the U.S. Governement
 * retains certain rights in this software.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 * 
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.  
 * 
 *     * Neither the name of Sandia Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
/*
 * This file contains defined constants that are used in the EXODUS II API.
 *
 * The first group of constants refer to netCDF variables, attributes, or 
 * dimensions in which the EXODUS II data are stored.  Using the defined 
 * constants will allow the names of the netCDF entities to be changed easily 
 * in the future if needed.  The first three letters of the constant identify 
 * the netCDF entity as a variable (VAR), dimension (DIM), or attribute (ATT).
 *
 *      DEFINED CONSTANT	ENTITY NAME	DATA STORED IN ENTITY
 */
#define ATT_FILE_TYPE		"type"		/* the EXODUS II file type   */
						/*   ("r" for regular or "h" */
						/*   for history)            */
#define ATT_TITLE		"title"		/* the database title        */
#define ATT_VERSION		"version"	/* the EXODUS II version #   */
#define DIM_NUM_NODES		"num_nodes"	/* # of nodes                */
#define DIM_NUM_DIM		"num_dim"	/* # of dimensions; 2- or 3-d*/
#define DIM_NUM_ELEM		"num_elem"	/* # of elements             */
#define DIM_NUM_EL_BLK		"num_el_blk"	/* # of element blocks       */
#define VAR_COORD		"coord"		/* nodal coordinates         */
#define VAR_NAME_COOR		"coor_names"	/* names of coordinates      */
#define VAR_ID_EL_BLK		"el_blk_id"	/* element block ids         */
#define ATT_NAME_ELB		"elem_type"	/* element type names for    */
						/*   each element block      */
#define DIM_NUM_EL_IN_BLK(num)	catstr("num_el_in_blk",num)
						/* # of elements in element  */
 						/*   block num               */
#define DIM_NUM_NOD_PER_EL(num)	catstr("num_nod_per_el",num)
						/* # of nodes per element in */
						/*   element block num       */
#define DIM_NUM_ATT_IN_BLK(num)	catstr("num_att_in_blk",num)
						/* # of attributes in element*/
						/*   block num               */
#define VAR_CONN(num)		catstr("connect",num)
						/* element connectivity for  */
						/*   element block num       */
#define VAR_ATTRIB(num)		catstr("attrib",num)
						/* list of attributes for    */
						/*   element block num       */
#define VAR_MAP			"elem_map"	/* element order map         */
#define DIM_NUM_SS		"num_side_sets"	/* # of side sets            */
#define VAR_SS_IDS		"side_set_ids"	/* side set id's             */
#define DIM_NUM_EL_SS(num)	catstr("num_el_ss",num)
						/* # of elements in side set */
						/*   num                     */
#define DIM_NUM_NOD_SS(num)	catstr("num_nod_ss",num)
						/* # of nodes in side set num*/
#define VAR_FACT_SS(num)	catstr("dist_fact_ss",num)
						/* the distribution factors  */
						/*   for each node in side   */
						/*   set num                 */
#define VAR_ELEM_SS(num)	catstr("elem_ss",num)
						/* list of elements in side  */
						/*   set num                 */
#define VAR_NODE_SS(num)	catstr("node_ss",num)
						/* list of nodes in side set */
						/*   num                     */
#define DIM_NUM_NS		"num_node_sets"	/* # of node sets            */
#define DIM_NUM_NOD_NS(num)	catstr("num_nod_ns",num)
						/* # of nodes in node set    */
						/*   num                     */
#define VAR_NS_IDS		"node_set_ids"	/* node set ids              */
#define VAR_NODE_NS(num)	catstr("node_ns",num)
						/* list of nodes in node set */
						/*   num                     */
#define VAR_FACT_NS(num)	catstr("dist_fact_ns",num)
						/* list of distribution      */
						/*   factors in node set num */
#define DIM_NUM_QA		"num_qa_rec"	/* # of QA records           */
#define VAR_QA_TITLE		"qa_records"	/* QA records                */
#define DIM_NUM_INFO		"num_info"	/* # of information records  */
#define VAR_INFO		"info_records"	/* information records       */
#define VAR_HIS_TIME		"time_hist"	/* simulation times for      */
						/*   history time steps      */
#define VAR_WHOLE_TIME		"time_whole"	/* simulation times for whole*/
						/*   time steps              */
#define VAR_ELEM_TAB		"elem_var_tab"	/* element variable truth    */
						/*   table                   */
#define DIM_NUM_GLO_VAR		"num_glo_var"	/* # of global variables     */
#define VAR_NAME_GLO_VAR	"name_glo_var"	/* names of global variables */
#define VAR_GLO_VAR		"vals_glo_var"	/* values of global variables*/
#define DIM_NUM_NOD_VAR		"num_nod_var"	/* # of nodal variables      */
#define VAR_NAME_NOD_VAR	"name_nod_var"	/* names of nodal variables  */
#define VAR_NOD_VAR		"vals_nod_var"	/* values of nodal variables */
#define DIM_NUM_ELE_VAR		"num_elem_var"	/* # of element variables    */
#define VAR_NAME_ELE_VAR	"name_elem_var"	/* names of element variables*/
#define VAR_ELEM_VAR(num1,num2)	catstr2("vals_elem_var",num1,"eb",num2)
						/* values of element variable*/
						/*   num1 in element block   */
						/*   num2                    */
#define DIM_NUM_HIS_VAR		"num_his_var"	/* # of history variables    */
#define VAR_NAME_HIS_VAR	"name_his_var"	/* names of history variables*/
#define VAR_HIS_VAR		"vals_his_var"	/* values of history         */
						/*   variables               */
#define DIM_STR			"len_string"	/* general dimension of      */
						/*   length MAX_STR_LENGTH   */
						/*   used for name lengths   */
#define DIM_LIN			"len_line"	/* general dimension of      */
 						/*   length MAX_LINE_LENGTH  */
						/*   used for long strings   */
#define DIM_N4			"four"		/* general dimension of      */
						/*   length 4                */
#define DIM_TIME		"time_step"	/* unlimited (expandable)    */
						/*   dimension for time steps*/
#define DIM_HTIME		"hist_time_step"/* unlimited (expandable)    */
						/*   dimension for history   */
						/*   variable time steps     */

/*
 * The following are miscellaneous constants used in the EXODUS II API.
 */

#define EX_NOCLOBBER	0
#define EX_CLOBBER	1

#define EX_READ		0
#define EX_WRITE	1

#define EX_VERS		1.01

#define EX_VERBOSE	1

#define EX_INQ_FILE_TYPE	1		/* inquire EXODUS II file type*/
#define EX_INQ_VERS		2		/* inquire version number     */
#define EX_INQ_TITLE		3		/* inquire database title     */
#define EX_INQ_DIM		4		/* inquire number of          */
						/*   dimensions               */
#define EX_INQ_NODES		5		/* inquire number of nodes    */
#define EX_INQ_ELEM		6               /* inquire number of elements */
#define EX_INQ_ELEM_BLK		7		/* inquire number of element  */
						/*   blocks                   */
#define EX_INQ_NODE_SETS	8		/* inquire number of node sets*/
#define EX_INQ_NS_NODE_LEN	9		/* inquire length of node set */
						/*   node list                */
#define EX_INQ_SIDE_SETS	10		/* inquire number of side sets*/
#define EX_INQ_SS_NODE_LEN	11		/* inquire length of side set */
						/*   node list                */
#define EX_INQ_SS_ELEM_LEN	12		/* inquire length of side set */
						/*   element list             */
#define EX_INQ_QA		13		/* inquire number of QA       */
						/*   records                  */
#define EX_INQ_INFO		14		/* inquire number of info     */
						/*   records                  */
#define EX_INQ_TIME		15		/* inquire number of time     */
						/*   steps in the database    */

#define MAX_STR_LENGTH		8
#define MAX_VAR_NAME_LENGTH	20
#define MAX_LINE_LENGTH		80

#define verbose			1
