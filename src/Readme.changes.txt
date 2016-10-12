Last Edited: 2015-11-10
This file contains the changes made to the current version of GMV.

GMV 4.6.2 (changes since 4.5.8 listed):
* Added fflush call to ensure consistent print output before possible crashes in
  makewidgets function
* Added two checks in case realloc fails for plvarray to output error and exit
* Added more detail to each 'can't allocate any more memory' error messages to 
  tell them apart
* Fixed freeze issue with Cygwin/Windows due to uninitialized values when 
  checking Azim/Elev/Twist (rotation x/y/z) values
* Fixed Cygwin 32-bit memory artificial limitation by adding 
  '-Wl,--large-address-aware' linker flag (can now access 4GB)
* Fixed issue by defaulting material name from 'mat 1' to 'mat_1'
* Fixed field calculation issue by increasing max field calculations to 10 from 
  5
* Fixed valgrind issue by initializing character array, and initialized malloc 
  array
* Fixed crash when opening GMV file with too many output option fields 
  (MAXTRFIELDS increased to 500)
* Fixed tracer selection issue with attribute files, added array to track all 
  selected, updated attribute file read/write
* Fixed issue with tracer colormap using off scale colors of pink and purple 
  instead of cmap file colors
* Fixed issue caused by "display_list off" in gmvrc file, need "display_list on"
  to have legend/label in Animation -> Cut Plane -> Quick Look
* Fixed crash caused by too close of min/max in all data limits due to double 
  to float conversion
* Fixed various long time bugs caused by legacy programming mistakes
* Fixed various initialization issues with tracer selection
* Fixed bug with snapshots for Cygwin version
* Fixes required to rebuild GMV on Cygwin fixing dynamic linking of libraries
* Fixed 50 blank pixels at start
* Fixed issues with gcc4 compilation and files not including 'windows.h' mostly
* Fixed makefile issues to get cygwin compiling correctly and using Mesa 6.4.2 
  from util folder to avoid box redraw issue


GMV 4.5.8 (changes since 4.5 listed):
* Added GMV support for Windows 7
* Added writing of data variables to files
* Added command line support for write variables feature for usage with Batch 
  mode
* Added command line help output for usage
* Added width/height of viewport to be written and read from attributes files
* Added average on subset of domain feature
* Added tracer select on multiple variables feature
* Added gmvbatch with SIZE radius functionality
* Added tracer field calculations functionality
* Updated to use OpenMotif 2.3.3 and Mesa 7.8.2 (fixed 7.8.2 bug with GMV Batch 
  as well)
* Fixed bug with 8th digit round off error in legend by changing to 7
* Fixed bug with missing pixels at top of render
* Fixed bug with data range on Tracers with certain mesh data
* Fixed bug with "-fileinfo" flag giving wrong particle count
* Fixed bugs with aspect ratio when loading files
* Fixed bugs with attribute files not writing/reading correct Tracer selections
* Fixed issue with slowness of GMV to open a file (went from ~14 seconds to ~2 
  seconds)
* Fixed memory leaks associated with auto-reading of GMV files that eventually 
  caused crash
* Fixed compatibility on SUSE operating systems
* Fixed disappearing text problem


GMV V4.5:

The following changes were made to version 4.5 of GMV:
1.  Added "Spheres" option to the "Nodes" menu.  This option draws a lit 
    sphere at the node location.  The sphere size is determined by the 
    "Point Size" option in Ctl-2.
2.  Changed the "Big Points" option in "Tracers" to "Spheres.  This option 
    draws a lit sphere at the node location.  The sphere size is determined 
    by the "Point Size" option in Ctl-2.
3.  Corrected an error in "Animation" that caused the Z rotation to rotate 
    backwards.
4.  Corrected an error in the "Pause" option in "Animation" that caused the 
    image to revert to the starting position instead of the current position.
5.  Corrected an error in the parallel version that caused a segmentation 
    fault when reading a single file.
6.  Corrected an error in the parallel version that caused a segmentation 
    fault when reading vfaces from a single file.
7.  Corrected an error when drawing surface faces or edges with
    material colors and with texture mapping that caused the material
    colors to be either blue or red only.
8.  Corrected an error when reading an attributes file that has an
    isovolume for a variable with a blank in the name.
9.  Fixed bug when reading node ids from a binary file.
10. Fixed bug that may cause a crash when reading structured meshes with lots
    of materials.


GMV V4.4:

The following changes were made to version 4.4 of GMV:
1.  Modified the code to allow up to 10,000 vertices per face and up to 
    10,000 faces per cell.
2.  Modified Cell drawing for larger numbers of vertices and faces.
3.  Modified Cutplane generation and drawing for larger numbers of vertices 
    and faces.
4.  Modified Cutsphere generation and drawing for larger numbers of vertices 
    and faces.
5.  Modified Isosurface generation for larger numbers of vertices and faces.
6.  Modified Isovolume generation for larger numbers of vertices and faces.
7.  Modified Surface drawing for larger numbers of vertices and faces.
8.  Added text input for the Begin and End slider informatin in the Animation 
    Cutplane menu.
9.  Added a scrolling window for the Cell Node Ids in to the Query Data menu 
    so that a large number of nodes in a cell will not cause the menu to be 
    cut off.
10. Corrected an error that caused certain configurations of the command 
    line input to be read incorrectly.
11. Corrected an error that did not correctly display all segments of a ray.
12. Corrected an error in Cutline that printed errors refering to Cutplane.


GMV V4.3a:

The following changes were made to version 4.3a of GMV:
1.  Serial version only, added capability to read x3d files and related 
    Bdy and Reg files.
2.  Modified the Cell Material and Flags search to allow Cells with a 
    flag type value of 0 to be selected.
3.  Modified the usage writer to use a GMV version number defined in 
    main.h.
4.  Commented out all malloc.h include statements to prevent problems 
    with MacOSX system.


GMV V4.3:

The following changes were made to version 4.3 of GMV:
1.  Added a moveable orientation axis to the Axes menu in Ctl-1.  The 
    Orientation Axis is displayed with heavy black lines and is alwas 
    rendered over the image so that it is always visible.  This axis 
    can be placed anywhere on the main view window via mouse actions.
2.  Added Orientation Axis information to the gmvrc file.
3.  Added Orientation Axis information to the attributes file.
4.  Changed the Auto Read functions to read files with up to 5 digits 
    in the numeric sequence portion of the file names.
5.  Changed the History function of Tracers to read files with up to 
    5 digits in the numeric sequence portion of the file names.
6.  Modified the display of the problem time on the main viewer to allow 
    a variable format instead of always using the E format.


GMV V4.2:

The following changes were made to version 4.2 of GMV:
1.  Added the capability for the serial version to read mesh data from 
    ExodusII files.  GMV will read the mesh from Cubit ExodusII time zero 
    files and will generate a node boundary flag from node sets and side 
    sets.  Also Cell and Flag group data as well as surface data is 
    generated from side sets.
2.  Added a "Write to File" option to "Query Data".  Select the option 
    to write node or cell data generated by Query Data.  Data for 
    additional nodes or cells are concatenated onto the file until the 
    option is turned off, or until the Query Data menu is closed.
3.  Modified the transparency algorithm to z-sort transparency objects.  
    This eliminates invalid tranparency images caused by inteference.
4.  Modified the RGB option in Snapshot to flush the output buffer before 
    closing the file on the Mac X versions.
5.  Increased the PJEG quality variable from 85 to 90.
6.  Added code to explicity specify the OpenGL buffer to use when saving 
    images.
7.  Modified the "Color By" -> Cell Groups routine to set colors faster 
    when there are no Cell Ids.
8.  Modified the "Color By" -> Face Groups routine to set colors faster 
    when there are no Face Ids.
9.  Corrected some typos in the Trace Field selection menu.
10. Corrected an error that caused a Segmentation Fault when drawing 
    unselected nodes in black.
11. Corrected an error in Surface Selection menu that did not let the 
    Materials/Flags selection option grow when resizing the menu.
12. Corrected an error that prevented the axis notation in the axis window 
    to be drawn on some systems.


GMV V4.1:

The following changes were made to version 4.1 of GMV:
1.  Added a JPEG option to Snapshot that saves images in jpeg format.
2.  Increased the number of node, cell and surface flags from 10 to 50.
3.  Added checks that prevent the number of flags to exceed the maximum 
    allowed.
4.  Added a 1 pixel point size option.
5.  Added a Material/Flags colorbar option.  The new option pops up a 
    new menu that draws material, node flags or cell flags colors along 
    with their names.
6.  Modified the transparency algorithm to help reduce the apperance of 
    aritfacts in the tranparent object.
7.  Added a new command line option in the Batch version, -stereo, that 
    will generate left and right eye stereo image files.
8.  Increased the size of the file name in the "Current file" field in 
    the main window to 64 characters.
9.  Corrected an error in the Fields Colorbar annotation with the "nice" 
    option when displaying a field that has the same value for all the 
    elements.
10. Corrected an error that did not display the axis labels on the axis 
    window in certain situations.
11. Corrected and error in the Animation Menu that caused the Isosurface 
    Button, if any, to override the Subset button.


GMV V4.0:

The following changes were made to version 4.0 of GMV:
1.  Modified the Distance Scale function so that a user can place it 
    anywhere on the image.  A crosshair is used to place the left edge 
    of the scale on the image.
2.  Added a command line option -noprint to suppress the output, except 
    for printing the input file name, and any error or warning messages.
3.  Added a command line option -fileinfo that when combined with the -i 
    command line option will print information for the input file, then 
    quit.  Note, this option is not available on the parallel version.
4.  Modifed the GUI so that when a file is read with the -i command line 
    option, the next File Browser menu will have the directory and file 
    name displayed in the menu.
5.  Added a new Tracer selection options that can select tracers by tracer 
    field data range.
6.  Added a new Tracer selection option that can select tracers that are 
    either inside or outside a user defined sphere.
7.  Added a new Tracer selection option that can select tracers that are 
    either inside or outside a user defined box.
8.  Moved the Select Tracer by number from the Tracer menu to a new Tracer 
    Select menu.  The new menu contains the new tracer selection options.
9.  Added a new Tracer field menu.
10. Modified the field data display functions to color data that is smaller 
    than 1.0e-10.  The new data floor is 1.0e-20.


GMV V3.9:

The following changes were made to version 3.9 of GMV:
1.  Added a new attributes file format.  This format is more consice for 
    readability.  The biggest change is to the references for material, 
    variables and flags, these are now referenced by their name instead 
    of a number.  This change allows an attribute file to be used on 
    other simulations with similar data.  
2.  Added a new field calculation option "C * Round(X) + Y" to node, cell 
    and surface field calculation.
3.  Added a label option to the Color Bar.  This option allows a user to 
    specify the Color Bar label.
4.  Modified the "Refine Face" option in the Cells menu so that it can be 
    applied at any time, not just when node field data is colored.  This 
    change allows the user to see non-planar faces.
5.  Modified "Auto Snapshots" in Auto Read to place .rgb at the end of 
    the rgb files.
6.  Modified "Scale Axis" to recaluclate search parameters to reflect the 
    current scale.
7.  Corrected a long standing error that did not generate an image when 
    the problem size was very small, ie. when the x, y and z extents were 
    all much less than 1.
8.  Corrected an error in "Get Attributes" and in "Read same simulation" 
    that incorrectly applied scale axis factors.
9.  Corrected an error in Tracer Menu that could cause GMV to Seg. Fault 
    if an Attributes file is read that has the SIZE option on, and there 
    is no SIZE tracer variable.  In this case the option is changed to 
    set to NONE.
10. Corrected an error in Cutplane that did not add nodes or vectors to the 
    cutplane in Autoread or Read Same Simulation when the distance slider 
    was not moved.
11. Corrected and error in the Animation menu that caused the "Subset" 
    button to be missing when there were no cells or polygons in the 
    input file.
12. Modified the Makefile to strip the executables and to ensure that the 
    correct routines to generate RGB images on Macs are always in place.


GMV V3.8:

The following changes were made to version 3.8 of GMV:
1.  Added the capability to specify the size of individual tracer points.  
    The tracer variable SIZE is used to input the radius of each tracer 
    in user units.  
2.  Added a "Point with SIZE radius to the Tracer menu.  When this option  
    is selected, each tracer is drawn with it's SIZE radius.  This option 
    is not available if there is no SIZE tracer variable.
3.  Added a new option to the Center menu, "Center on Tracer".  When this 
    option is selected, a "Center on Tracer" menu pops up.  Enter a tracer 
    number in this menu to center on that tracer.
4.  Added a new "Hidden line edges" drawing option to the "Cells" menu.  
    This option generates a hidden line wireframe of the exterior of the 
    mesh while still allowing other objects (tracers, polygons, surfaces, 
    cutplanes, etc.) to still be visible within the wireframe.  The edges 
    of the wireframe are colored by the the selected "Color By" option.
    Note that  the hidden line wireframe is not affected by any cell 
    selection options.
5.  Added a "Black hidden edges" option to the Cells "Color By:" menu.
    This option enables the hidden line wireframe to be colored by the 
    text color (black for light backgrounds, white for dark backgrounds).  
    This option is only available when if the "Hidden Line Edges" cell 
    option is selected.
6.  Modified the I/O routines to search for fromfiles in the directory 
    of the GMV input file, unless the fromfile name includes a full 
    directory path.
7.  Modified Auto Read to check for only the next 50 file sequences at a 
    time to prevent the illusion that GMV has hung.

GMV V3.7:

The following changes were made to version 3.7 of GMV:
1.  Modified the drawing algorithm so that when the Interactivity slider 
    is set at the far left (all elements are drawn), the orientation 
    bounding box is not drawn.
2.  Modified Grid Analysis so that selected Grid Analysis cells will 
    continue to be drawn when reading another file of the same simulation 
    and with Auto Read.
3.  Added a new option, Test Concave, that tests all cells in an 
    unstructured mesh for faces that are concave.  The cells with concave 
    faces are noted in the menu, and the vertices of the concave faces 
    are reordered so that they will be displayed correctly.  The parallel 
    version also lists the cells by processor number and cell number in 
    the processor.
4.  Modified Grid Analysis so that memory needed for selected cells and 
    the field data used in Grid Analysis is allocated and freed as needed.
5.  Modified Center, Attributes and Auto Read so that when specifying a 
    node or cell used to center the image, subsequent images will 
    center on the cell or node.  Note, to turn this off, use Center to 
    center the image with different criteria.
6.  Modified the code that extracts the image and generates an SGI RGB 
    file to prevent problems with Mac OS X.
7.  Modified the code in Auto Read that searches for the next file to 
    limit the search to 50 sequences to prevent what appears to be a 
    hang when the last file specified in "Last" is a large number.
8.  Corrected an error in Grid Analysis that incorrectly displays flag 
    data when face data exists.
9.  Corrected an inefficient memory usage when determining which cell 
    edges to display so that each edge is only drawn once.



GMV V3.6:

The following changes were made to version 3.6 of GMV:
1.  Added a new input keyword "vectors".  Vectors allows the input of 
    vectors that can have multiple components.  The components are 
    saved as separate variable data with the variable name entered 
    as optional component names, otherwise GMV will generate a each 
    component name by prefixing the vector name with the number of 
    each component.
2.  Added code to calculate the machine zero value to be used to determine 
    when a very small number is to be treated as zero.  This will make 
    it possible for GMV to process simulation data that has very small 
    lengths.
3.  Modified the Autoread process so that simulations that do not have 
    consecutive cycle numbers in the GMV files will still be processed 
    correctly with Autoread.
4.  For the parallel version, corrected an error in cutline that did 
    not get the correct data for the cutline.
5.  For the parallel version, corrected an error in cutline that 
    caused a segmentation fault.
6.  For the parallel version, corrected an error in cutline in the 
    Mesa version that did not draw the cutline plot.
7.  For the parallel version, corrected an error in cutline in the 
    Mesa version that did not draw the cutline in the drawing window.
8.  Corrected an error in the Cygwin version that did not generate 
    correct SGI RGB snapshots.
9.  Corrected an error in the Mac X version in Auto Read where the 
    Pause and Quit buttons were not working.


GMV V3.5:

The following changes were made to version 3.5 of GMV:
1.  Added a new "Show unselected nodes in black." button that is used to 
    display unselected nodes as black dots.
2.  Added a new "Show unselected cells in black wireframe." that is used 
    to display edges of unselected cells in black.
3.  Increased the length of floating point text input in the selection 
    menus of the Node Select menu.
4.  Increased the length of floating point text input in the selection 
    menus of the Cell Select menu.
5.  Increased the length of floating point text input in the selection 
    menus of the Surface Select menu.
6.  Increased the length of floating point text input in the selection 
    menus of the Tracer Select menu.
7.  Increased the length of the Minimum and Maximum text input in the 
    Node Field Data Limits menu.
8.  Increased the length of the Minimum and Maximum text input in the 
    Cell Field Data Limits menu.
9.  Increased the length of the Minimum and Maximum text input in the 
    Flag Field Data Limits menu.
10. Increased the length of the Minimum and Maximum text input in the 
    Surface Field Data Limits menu.
11. Increased the length of the Minimum and Maximum text input in the 
    Tracer Field Data Limits menu.
12. Increased the length of the Minimum and Maximum text input in the 
    Ray Field Data Limits menu.
13. Modified the Color Edit menu to build entries for Flags or Groups if 
    there are more Flags or Groups than Materials.
14. Added flags to the attributes file for the options to draw unselected 
    nodes or cells.
15. Added a "Clear Entries" button to the "Node Select - Node Number" menu 
    that clears all the text fields in the menu.
16. Added a "Clear Entries" button to the "Cell Select - Cell Number" menu 
    that clears all the text fields in the menu.
17. Modified the Face Group selection algorithm to make it faster for large 
    face groups.
18. Modified the algorithm that determines the faces to draw to make it 
    faster.
19. Modified the reader for Setup's M3 file to generate cell and face 
    groups from appropirate data.
20. Modified the Mesa MPI parallel version to generate an image in the 
    background when the -m command line option is used.
21. Corrected an error in the algorithm that selects group faces to draw.  
    The error did not select all faces when there were a large number of 
    faces to draw.
22. Corrected an error in phex20 cells that caused non-planar faces to be 
    drawin incorrectly.


GMV V3.4:

The following changes were made to version 3.4 of GMV:
1.  Added a new "ghosts" input keyword that is used to specify ghost 
    cells and ghost nodes.  Ghost cells are cells that are used in 
    parallel simulations at processor interfaces to minimize the amount 
    of cross processor messages.  They are ghost images of the cells 
    across the processor boundary.  Ghost nodes are nodes that exist 
    in ghost cells, but are not part of the regular cells.  GMV will 
    generate ghost nodes automatically from the ghost cell information 
    if ghost nodes are not specified.  Ghost cell information will allow 
    GMV to interpolate cell fields to node fields more accurately for 
    simulations that have multiple GMV files per time step.  Ghost cells 
    will also help minimize the number of faces that are drawn at the 
    processor boundaries within the outside shell of the selected cells.
    Note, enter ghost nodes and cells in the nodes and cells input as 
    any other ghost or cell, then use the "ghosts" keyword to list the 
    node numbers or cell numbers that are ghosts.
2.  Modified the GMV reader code to generate a GHOSTS node flag and a 
    GHOSTS cell flag if ghost nodes or cells exist.
3.  Added a "Show ghost nodes" button to the nodes menu that adds ghost 
    nodes to the display.  This button only appears if ghost nodes exist.
4.  Added a "Show ghost cells" button to the cell menu that adds ghost 
    cells to the display.  This button only appears if ghost cells exist.
5.  Added a "Show cell faces next to ghost cells" button to the cells 
    menu that displays the cell faces adjacent to ghost cells as if 
    ghost cells were not entered.  This button only appears if ghost 
    cells exist.
6.  Modified the window resize function so that the new window size can 
    not be larger than the maximum window size available.  This affects 
    the -w command line line option and when entering a window size in 
    the Window Size menu.


GMV V3.3:

The following changes were made to version 3.3 of GMV:
1.  Added a "Use Log" function to "Vector Controls".  This function 
    calculates vector lengths based on the log of the vector magnitude 
    plus an offset for vector magnitudes less than 1.
2.  Modified the algorithms that display vface2d faces so that face 
    data is displayed along the cell edges.  This feature is not 
    available in the parallel version of the code.
3.  Added the "vector log" flag to the attributes file.
4.  Modified the algorithms to create isosurface, isovolume and 
    cutplane to ignore faces with less than three vertices.
5.  Modified the arguments to glPolygonOffset to get better line 
    resolution when rendering with Mesa.
6.  Corrected an error in gmvBatch and the Mesa parallel versions 
    of GMV that did not activate glPolygonOffset correctly.  Now 
    lines should be more distinct.
7.  Corrected an error in the various vector build functions that 
    did not correctly update the vector magnitudes when a new vector 
    was built.
8.  Corrected an error in the various vector build functions that 
    did not correctly update the data limits for the vector magnitude 
    when a new vector was built.


GMV V3.2:

The following changes were made to version 3.2 of GMV:
1.  Modified Animation Cutplane to include cutplanes that revolve 
    around the x, y or z axis.
2.  Added new "Rays" input file and display.  a ray is a set of line 
    sgements that can contain up to twenty field value for the points 
    on the line or for each line segment.  All rays in a Ray input 
    file have the same fields.  The Ray input file is only read when 
    requested.  The rays are displayed as lines colored by a set of 
    standard colors (black, white, grey, red, green, yellow, blue) 
    or by the blue to red intensity of their field values.  A special 
    field called "Material" can be used to color line segments by 
    material color.
3.  Modified GMV to allow names up to 32 characters in length.  The 
    longer names are for materials, flags, all variable field names, 
    groups, subvars and units.  Input file keywords are still limited 
    to 8 characters.
4.  Added new ieee binary input file types to read 32 character names.  
    The new types are "iecxi4r4" and "iecxi4r8".
5.  Modified the Animation Fade menu to list all materials and 
    isosurface contour options one scrolled window to allow for 32 
    character material names.
6.  Changed the buttons in the Color Edit -> Materials, Isosurfaces, 
    Isovolumes menu to lighted buttons so that users can see which 
    material or isosurfaces was selected.
7.  Changed the behavior of the file menus to set the default directory 
    of subsequent file menus to the directory of the GMV input file.
8.  Corrected an error in the file menus that caused the incorrect 
    file menu to be displayed after an error.
9.  Corrected an error in Data Limits for Surface fields and Tracer 
    fields when selecting the log. option.
10. Corrected an error in Animation magnify that did not use the 
    correct magnification values when Perspective View is on.
11. Corrected an error in Data Limits for Cells, Tracers, Surfaces 
    and Faces that caused a segmentation fault.


GMV V3.1:

The following changes were made to version 3.1 of GMV:
1.  Modified GMV to accept up to 1000 materials and flags.  In order 
    to accommodate this many materials and flags, the colored buttons 
    in the menus for materials and flags had to be modified to either 
    lists or uncolored buttons.  Menus were modified for material and 
    flag selections in nodes, cells, and surfaces.  The polygon menu 
    and the material order pop-up menu where also changed.  As were 
    the color edit, animation fade, and material isosurface menus.
2.  Added new Patran numbered cell types.  The new cell types are:
    3line (3 node line), 6tri (6 node triangle), 8quad (8 node quad.),
    ptet4 (4 node tet.), ptet10 (10 node tet.), ppyrmd5 (5 node pyramid),
    ppyrmd13 (13 node pyramid), pprism6 (6 node prism), and pprism15 
    (15 node prism).
3.  Added a new function, Reset Original Angles and Mag. to the View 
    menu.  The function resets the view angles and magnification to 
    those that are set for the first image.
4.  Modified the face vector drawing function to allow face vectors on 
    cells with only the cell edges on.
5.  Corrected an error in tracers, that did not set selected tracers 
    when viewing Tracer History.
6.  Corrected an error in Animation Subset, that did not reset the 
    default box values when Scale Axes is used.
7.  Corrected an error that caused a Segmentation Fault when reading 
    a GMV file with vfaces where there a vface pair did not point to 
    each other.
8.  Corrected an error in Cut Sphere's height function that did not 
    properly push the cut sphere's faces relative to the data.
9.  Corrected and error in Put Attributes where the filename can have 
    spaces in the Windows versions.  The spaces will cause Get Attributes 
    to fail with an invalid keyword error.  Put Attributes now replaces 
    the space with at dash.
10. Corrected an error in Color Edit that did not color the main color 
    window or colored buttons correctly on TrueColor visuals.
11. Added an undocumented keyword "xfaces" for testing by the Setup  
    Team.



GMV V3.0.1:

The following changes were made to version 3.0.1 of GMV:
1.  Modified the routine that checks for duplicate faces to be faster 
    and to use memory more effectively. 
2.  Modified the routine that checks for duplicate edges to be faster 
    an to use memory more effectively.
3.  Modified the read routines to use less memory when filling in 
    the mesh structure.
4.  Modified the read routines to differentiate between nodes and 
    nodev keywords in the data structure.
5.  Modified the read routines to stop reading the input file when 
    an error is encountered and to continue with GMV.  An input error
    would cause older versions of GMV to quit.
6.  Modified the read routines to print out information while reading 
    the mesh data, instead of after all mesh data was read.
7.  Stopped the generation of Stereo options when remote displayin GMV 
    on non-SGI workstations while GMV is running on a SGI workstation.
8.  Added a Reverse Greyscale option to the Color Edit -> Field Color 
    Map function.
9.  Corrected a memory error when mixing general and regular cell types.
10. Corrected an error where GMV did not recognize the surfids keyword.
11. Corrected an error that did not eliminate all duplicate nodes in a 
    cell for vfaces or faces input when the first face of a cell has 
    duplicate nodes.
12. Corrected an error in the parallel version that did not renumber 
    opposite face data correctly when distributing vfaces data after 
    reading from a single GMV input file.
13. Corrected an error in the parallel version that did not allow the 
    animation features.


GMV V3.0:

The following changes were made to version 3.0 of GMV:
1.  Added Subset to the Animation Menu.  Use the Animation Subset menu 
    to set the begining and ending subset boxes.  The Animation Subset 
    function works on cells, nodes, tracers and polygons.
2.  Added Node Material and Cell Material selection buttons to the 
    Animation Cutplane menu. 
3.  Added Node Material and Cell Material selection buttons to the 
    Animation Cutsphere menu.
4.  Modified the duplicate face search and the duplicate cell edge 
    search to help improve the speed.
5.  Corrected an error that did not generate correct vectors when 
    using vector component variables that were entered as subvars.
6.  Corrected an error that did not set the line size and point size 
    as specified in a gmvrc file on some systems when using the -i 
    command line option.
7.  Corrected an error caused ver large cell and node numbers to be 
    displayed in scientific notation.
8.  Corrected an error when drawing Median Mesh edges when the input 
    mesh was a structured mesh.
9.  Corrected an error that caused a segmentation fault when drawing 
    Median Mesh faces when the input mesh was a structured mesh.
10. Corrected a color error when drawing cell faces with field data 
    and the face is a 2 point line.
11. Corrected an error in Color Edit that caused a segmentation fault 
    on some systems when setting Isovolume colors.
12. Corrected an error that caused a bus error when reading multiple 
    phex20 cell types in a row.
13. Corrected an error that did not allow node field selection of 
    the y-coord and z-coord when no variables were entered.
14. Corrected a memory error in the parallel version when interactively 
    picking cells.
15. Corrected various memory errors in the parallel version when reading 
    a single file.


GMV V2.9.2:

The following changes were made to version 2.9.2 of GMV:
1.  Added the node coordinates to the node fields list (after the 
    node field calc. list) with the names X-Coords, Y_Coords and 
    Z-Coords.  This allows the coordinates to be used in selections, 
    to be displayed and to be available for node field calculations.
2.  Changed the Node Field menu from selection on buttons to a selection 
    list.  The Cell Field, Face Field and Surface Field menus were also 
    changed to a selection list.
3.  Modified the display of Cutplanes, when the Height function is used, 
    to use a lighting model to show a more realistic shaded relief image.
4.  Modified the display of Cutspheres, when the Height function is used, 
    to use a lighting model to show a more realistic shaded relief image.
5.  Modified the use of the Height slider in the Cutplane and Cutsphere 
    Options menu to redraw the cutplane only after the slider is released.  
    This will eliminate the lag time experienced when moving the slider on 
    a large problem.
6.  Modified the use of the Dist. slider in the Cutplane and Cutsphere 
    Options menu to redraw the cutplane and selected elements within the 
    Dist. distance only after the slider is released.  This will eliminate 
    the lag time experienced when moving the slider on a large problem.
7.  Changed the default setting of "Texture Smoothing" in Ctl-3 to on.  
    Using texture smoothing when drawing node data on cell faces, faces, 
    or cutplanes creates a more accurate color interpolation.
8.  Changed the default setting of "Use Display List" in Ctl-3 to off.  
    There seems to be some problems with some graphics boards when the 
    display list is on and there are a large number of OpenGL primitives 
    to draw.  Also, on the SGI Impact boards, texture smoothing does not 
    work correctly with display list on.
9.  Corrected an error in Get Attributes, when calculating a field minimum 
    and maximum on Field Calc. fields.
10. Corrected an memory error when creating scrolled lists.
11. Corrected an error in Plot Box in Ctl-2 that did not allow a change 
    in the plot box limits with a 2D problem.


GMV V2.9.1:

The following changes were made to version 2.9.1 of GMV:
1.  Modified Query Data to list all nodes, as entered, for the "regular" 
    cell set, and not eliminate duplicates.
2.  Corrected an error in Files -> Read GMV when an invalid gmv input 
    file is encountered.  After the error menu was dismissed, the Read 
    Attributes file menu, instead of the Read GMV file menu was displayed.
Note: The following changes are code changes and do not affect GMV 
      functionality.
3.  Added the arrays cellnnode and cellnodes to the gmv_meshdata structure 
    in gmvread.h to store the as entered nodes for regular cells.
4.  Changed long long variables to long variables to conform to C, C++ 
    standards.
5.  Changed long long variables in the gmvread utility to conform to C, 
    C++ standards.
6.  Changed long long variables in the gmvwrite utility to conform to C, 
    C++ standards.
7.  Added an opposite face element to the cellfaces structure for future 
    use.


GMV V2.9:

The following changes were made to version 2.9 of GMV:
1.  Added "Select Material From Screen" and "Remove Material From Screen" 
    toggle buttons to "Cell Select - Materials and Flags" menu.  The new 
    buttons allow users to select or remove materials interactively.  
    When "Select Material From Screen" is pressed, all the material 
    buttons are turned off and a crosshair cursor appears on the display.  
    Right click the mouse over a cell to select it's material, the selected 
    material button will be turned on.  As before, then click on the "Apply" 
    button to display cells with the selected material.  The "Remove 
    Material From Screen" button is similarly used to deselect a material.
2.  Added "Select Material From Screen" and "Remove Material From Screen" 
    toggle buttons to "Surface Select - Materials and Flags" menu.  These 
    buttons function the same as the ones in Cell Select.
3.  Added "Select Material From Screen" and "Remove Material From Screen" 
    toggle buttons to the "Polygon" menu.  These button function the same 
    as the ones in Cell Select.
4.  Added data type information (nodes, cells, faces) to the output of 
    variable field names, group names and subvar variable names.
5.  For the parallel versions, in Query Data, changed the nodes in a 
    cell list to show the global node number and the processor id and 
    node number in processor.
6.  Corrected a couple of Motif parent widget warning messages.


GMV V2.8:

The following changes were made to version 2.8 of GMV:
1.  Changed the maximum number of variable fields from 100 to 250.
2.  Added a new input keyword - subvars.  The subvars keyword allows 
    input of variable data for a subset of the mesh.  The data for 
    a subvars variable includes a list of elements for which field 
    data is defined.  The elements can be cells, nodes or faces.   
    The field minimum and maximum are calculated and set for the data 
    that is entered, and a value that is smaller than the calculated 
    minimum is set for the rest of the elements.  Fields that are 
    defined in the subvars keyword are added to the list of fields 
    from the variable keyword.
3.  Modified the regular version of GMV to read in a filename with 
    the -s command line option (as in the gmvBatch versions).  If a 
    filename is not entered, the snapshot will be saved to AzsnapgmvAz.
4.  Corrected an error in Center on Node and Center on Cell that did 
    not center on the the element specified after a pan operation.
5.  Corrected an error in Color Edit that did not provide all the 
    buttons necessary for all flag elements.  (Also used in Animation).
6.  Corrected an error when using the -s command line option that did 
    not create the AzsnapgmvAz file.
7.  Corrected an error when reading in an attributes file and all cells 
    are selected by default and there is no cell field data.  This caused 
    no cells to be selected.
8.  Corrected an error when coloring cell faces by face data.  Only the 
    first face field variable was used no matter which face field was 
    selected.
9.  Corrected an error in the 64-bit SGI versions that caused an incorrect 
    background color after reading an attributes file.


GMV V2.7.1:

The following changes were made to version 2.7.1 of GMV:
1.  Modified the Center function to allow user input of the x, y, and z
    center coordinates.
2.  Modified the Magnifier function to allow input of the magnification 
    value.
3.  Modified Cell Explode to recalculate the explode direction vectors 
    only when changing the "Explode On" option;
4.  Modified Surface Explode to recalculate the explode direction vectors 
    only when changing the "Explode On" option;
6.  Added a new file selection widget used only by Read GMV File to  
    increase the speed of any subsequent display of the widget when 
    there are many thousands of files in the directory.
7.  (Parallel versions only) Modified the Read GMV function to prevent 
    display of the Machine Name List widget and the Number of Files 
    widget when any of the Same Simulation options are selected.
8.  Enhanced the parallel versions to read a single GMV file and 
    distribute the data across the available processors, this includes 
    structured data.  Structured data is converted internally to the 
    "faces" format for distribution.
9.  Corrected an error when reading the "tracer" keyword and the number 
    of tracers is zero.
10. Corrected a segmentation error that may occur after reading certain 
    GMV files with the "faces" format.



GMV V2.7:

The following changes were made to version 2.7 of GMV:
1.  Added three new keywords to the GMV file format.  The keywords are:
      codename followed by the name of the code that generated the file 
               (up to 8 characters).
      codever followed by the version of the code that generated the file 
              (up to 8 characters).
      simdate followed by the date the file was generated (mm/dd/yy).
    These keywords are for file information only, they are not used by 
    GMV.  These keywords can be placed before the nodes keyword.
2.  Added the ability to specify the an attributes file on the command 
    line.  The "-a attribute.filename" option will read the attributes 
    file and generate the appropriate image.
3.  Modified the cutline drawing routine to correct errors with the 
    parallel version.
4.  Added the ability to allow multiple material and velocity keywords.  
    The input file can now have up to 2 material keywords, one for nodes  
    and another for cells.  Note, the material names must be the same 
    on both two keywords.  There can be up to 3 velocity keywords for
    cells, node and faces velocities.
5.  Corrected an error that could cause a segmentation fault when 
    selecting face groups in cell select.
6.  Fixed memory leaks when face fields exist.
7.  Fixed memory leaks when groups exist.
8.  Corrected an error when reading a subsequent same simulation file 
    that contains face velocity data.
9.  Took the time to clean up unused variables in the code.
10. Took the time to consolidate the two parallel versions into a 
    source source path.
11. Generated a new version of GMV, gmvBatch, that uses Mesa's OSMesa  
    buffers for off screen rendering of the image.  Now the -s command 
    line option is used to specify the RGB output file name.



GMV V2.6:

The following changes were made to version 2.6 of GMV:
1.  Added phex8 phex20 cell types to GMV input.  These are 8 point and 
    20 point Patran numbered hexahedral cells.
2.  Added more memory alloc checks to the input routine (gmvread).
3.  Modified the Stereo functions to work with any graphics board that 
    supports OpenGL stereo-in-a-window.  Note: the X server must be 
    started in stereo mode for this to work on other than SGI.
4.  Added Inside/Outside option to node, cell, and surface group selection.
5.  Added the option to draw face numbers to Cell Display and Grid Analysis.
6.  Added color by group to Node Display, Cell Display, and Surface Display.
    Notes: 1) When coloring by group, only those elements that are in a 
    group are displayed.  2). If an element belongs to more than one group, 
    it will have the color of the last group it is in.
7.  Modified Query Data probes to select only nodes or cells that have a  
    group color, if colored by group.
8.  Modified Auto Center to calculate only nodes, cells and/or surfaces 
    that have a group color, if colored by group.
9.  Added surfids, surface ids, to GMV input.
10. Modified Query Data to probe nodes or cells that are displayed with 
    cutplane or cutsphere.
11. Add "Note: Display List must be off for PS Lines." message to Snapshot.
12. Corrected an error that may cause bad colors to appear when log data 
    limits is set and negative numbers close to 0 are calculated.
13. Corrected an error in Linux versions that caused a segmentation fault 
    when a cutline is requested.
14. Corrected an error when generating material isosurfaces.


GMV V2.5:

The following changes were made to version 2.5 of GMV:
1.  Added a "Texture Smoothing" button to Ctl-3.  This button allows the
    use of a 1D texture colormap to more accurately represent the smooth 
    color gradients when displaying node field data on cell faces, surface 
    faces, cell edges, surface edges, cutplanes, isosurfaces, isovolumes, 
    and cutlines.  The first and last colors of the 256 color field colormap 
    are used to represent data below or above the data limits.  Note, on
    on graphics boards without texture hardware, texture smoothing may be
    slow, but the image will look better.
2.  Modified the Cell Face Refine and Surface Face Refine algorithms to 
    reduce the level of refinement.  Texture smoothing is a better option.
3.  Added "Center on Node" and "Center on Cell" options to Center.  An 
    interactive probe to pick the cell/node from the screen is supplied.
4.  Added the window size options windowwidth and windowheight to gmvrc.
5.  Modified the selection boxes for cells, nodes, surfaces, and polygons 
    to prevent the callbacks from being activated when setting the sliders 
    for these boxes.
6.  Added the capability to specify surface type to the groups input 
    keyword (data_type = 3).
7.  Modified GMV to allow input of tracers keywords when ntracers = 0.
8.  Corrected an error that caused a segmentation fault when reading a 
    file with only 1 cell.
9.  Corrected an error that caused a segmentation fault when resizing the 
    drawing window on certain OSs.  The code was modified to allocate a 
    large drawing buffer once instead of reallocating the buffer for each 
    window size.
10. Corrected an error in the SGI 64 bit versions that caused GMV to crash 
    when reading nodeids.
11. Corrected errors in Get Attributes and in gmvrc that did not reset the 
    3D plot box after a scale axis change.
12. Corrected errors in Get Attributes and in gmvrc that caused some of 
    the selection boxes to be drawn under certain conditions.
13. Corrected an error in gmvrc that caused GMV to hang when polygons are 
    selected for display.


GMV V2.4:

The following changes were made to version 2.4 of GMV:
1.  Added the capability for velocity and variable keywords to read face 
    data.  Use 2 for the face data type.  Face data is only available if 
    faces are entered or the cells -> vface3d and vfaces combination is 
    entered.
2.  Modified the "Color By:" buttons in the Cells menu.  Now the "Color 
    By:" is a button that pops up a menu that contains the color selection 
    buttons.  A new "Face Data" button colors cell faces by face data.
3.  Modified the vector button in the Cells menu to allow selection or 
    building of cell vectors or face vectors.  If face vectors are drawn, 
    they are drawn in a dark grey color.
4.  Added a new Face Field menu to select face fields.
5.  Added a new Face Data Limits menu.
6.  Added a new Face Vector Build menu.
7.  Added the new input keyword "groups".  Groups allows the specification 
    of named groups of nodes, cells or faces.  Up to 1000 group names for 
    each type can be specified.
8.  Added "Node Groups" selection menu to node select.
9.  Added "Cell Groups"  and "Face Groups" selection menus to cell select.
    If face groups are selected, only those faces that have been selected 
    are drawn.
10. Added the new input keyword "nodev" as an alternate to the "nodes" 
    keyword.  The nodev keyword allows the x, y, and z values of the 
    nodes to be input on one line.  There will be nnodes lines of x, y, 
    and z values.  The "nodev" keyword does not work with block structured 
    mesh.
11. Modified the "Auto Read" function so that the menu is not removed 
    after the last file specified is read.  When the last file is processed, 
    Auto Read will automaically go in to "Pause" mode for further processing.  
    Press the "Quit" button to finish Auto Read. 
12. Added White, Grey, and Black color buttons to the Background Color 
    Edit menu.
13. Modified the grey color for cell edges and face vectors to a darker 
    grey.
14. Modified the printing of material or flag numbers that are greater 
    than the maximum allowed to the first encounter to minimize print 
    lines on the screen.
15. Added an "Auto Menu Popup" toggle button to the Data Limits menu items.  
    This button determines whether the Data Limits menu is automatically 
    displayed when selecting a "New Field" or coloring by a field.  The 
    new default is set to off.
16. Corrected an error in Cell Selection Box that did not select the 
    correct set of cells when a structured mesh is input.
17. Corrected a drawing error in the Vector Flow portion of Animation.
18. Corrected an error in Window Size that reset the size two pixels 
    larger than requested.
19. Corrected an error in Window Size that caused Auto Read to stop  
    working after the window was resized.
20. Corrected an error in Auto Read that did not reset the Auto Snapshot 
    button after Auto Read completed.
21. Corrected an error in Cells -> Faces -> Test Normals that did not 
    color the cell faces when colored by material, flags or cell data.
22. Corrected an error in Cutplanes and Cutspheres that did not color
    plane when colored by node materials or cell materials.


GMV V2.3:

The following changes were made to version 2.3 of GMV:
1.  Changed the default line type from Smooth to Regular.
2.  Modified the Cell probe in Query Data find cells that are displayed 
    in Grid Analysis.
3.  Modified Query Data to clear the data listing when an error occurs.
4.  Modiified the main window, it can now be resized and has fewer controls.
    The clip, interacitvity and light controls have been moved under the
    new controls menus in the menu bar.  There are now 3 control buttons 
    in the menu bar: Ctl-1, Ctl-2 and Ctl-3.
5.  Added a Window Size function to Ctl-3.  This function allows precise 
    pixel size setting of the drawing window widht and height.
6.  Removed the "Movie Size" options from all snapshot functions.  Now 
    all snapshots will have the pixel size of the current window.  If 
    you need to make snapshots that fit a particular resolution (eg. 
    760x486 for video), set the resolution with Window Size.
7.  Modified the execution of some slider callback routines to prevent 
    it from crashing on Solaris with the latest X11 patches.
8.  Corrected an error when drawing tracer numbers.
9.  Corrected an erro on AutoRead menu that did not allow the Auto 
    Snapshot option to be turned off.


GMV V2.2:

The following changes were made to version 2.2 of GMV:
1.  Added an Average function to the Calculate Menu.  Average is used 
    to calculate a weighted average of two fields for each material in 
    the problem.  The weighting field can also be the value 1 to get 
    the average for a field.  Averages can be generated for node, cell 
    or surface variable field data.
2.  Modified the Cell probe in Query Data to use ray tracing techniques 
    to accurately determine the cell that is selected.
3.  Added a Beep Sound option to turn the beep sound in GMV on/off.
4.  Changed the color of cell edges, surface edges and cutplane edges 
    to black (from grey) when faces are drawn.  If contours are also 
    requested, the contour lines are drawn in black and the edges are
    drawn in grey.
5.  Added nice linear scaling and log scaling options to Data Limits 
    for node fields, cell fields, surface fields and tracer fields.
6.  Added a bright pink color to represent data greater than the maximum 
    limit specified in Data Limits and a bright purple color for data 
    below the minimum limit.
7.  Modified Node menu, Cell menu and Surface menu to display the 
    appropriate Data Limits menu when selecting a field to color.
8.  Added a Distance Scale option to Controls-1.  This option turns 
    on a distance scale at the bottom of the image.  Distance Scale 
    only works in Othographic view.
9.  Rewrote the drawing routines to select data and generate a set 
    of primitives (triangles, lines, points, vectors) to draw.  The 
    set of primitives are generated only when the graphics state 
    changes (i.e. when an apply button is pressed).  The main drawing 
    routine then renders the primitives.
    Advantages:
     - Reduce drawing times when Display List is off, especially for 
       large problems.
     - Allows transparency in cell faces with materials colors and 
       shading on.
     - Allows transparency in surface faces with materials colors and 
       shading on.
     - Can use the same drawing routines for Sun Ultras.
    Disadvantages:
     - Uses more memory.
10. Along with 9 above, changed to a more efficient color model when 
    shading to speed up drawing.  Also, shaded surfaces are now brighter.


GMV V2.1:

The following changes were made to version 2.1 of GMV:
1.  Changed an error to a warning when reading materials, flags and/or 
    polygons that have a value > 128.  Now all values > 128 are set 
    to the mod(128) with 0 set to 128.
2.  Added traceids keyword to the input format.  The traceids keyword 
    allows the user to input a set of alternate numbers for the tracers.  
    The traceids keyword must not appear before the tracer keyword.
3.  Used unlink on the temporary files used for field data and polygon 
    data so that they don't exist after GMV is completed, even if GMV 
    crashes.
4.  Added a Virtual Trackball option to Controls-2.  Thanks to John 
    Fowler, the Virtual Trackball option allows the angular controls 
    (left mouse button) to be used in the same manner as many CAD 
    programs.  Where holding down the mouse button in the middle of 
    the drawing window grabs and rotates the image.  Holding down the 
    mouse button at the perimeter of the drawing window rotates the 
    image about an axis normal to the screen.
5.  Changed the Bounding Box menu (Controls-1) to allow direct input
    of the minimum or maximum box boundaries.
6.  Added the capability to enter the GMV input filename on the command 
    line with the -i option.
7.  Modified the cell probe in Query Data to return the cell id of the 
    cell closest to the viewer.
8.  Added a Clear Entry button to the Grid Analysis menu to blank out 
    all node and cell entries.
9.  Added the capability to generate Voronoi Mesh faces and edges in 
    Grid Analysis.  Note, this option is only available for tetrahedral 
    meshes.
10. Added comments keyword to the input format.  The comments keyword 
    allows ASCII comments to be added to a GMV input file.  Comments 
    are ended with the endcomm keyword.  Note, since comments is a 
    keyword, it can not be placed inside another keyword.  However,
    comments can be placed before the node keyword.  Be sure that the 
    endcomm keyword has a blank before and after the keyword.
11. Corrected an error in Reflections menu when read from a gmvrc file.
12. Corrected an error in Use Display List button when read from a gmvrc 
    file.
13. Corrected Segmentation Faults in the following:
    a. Auto Read - Same Simulation when only displaying velocities with 
       field colors.
    b. Auto Read - Same Simulation when only displaying node numbers, cell 
       numbers, and/or surface numbers with field colors.
    c. When displaying only cell or surface numbers with field colors.
14. Corrected displaying the Color Bar when only displaying node, cell or 
    surface numbers with field colors.


GMV V2.0:

The following changes were made to version 2.0 of GMV:
1.  Created a new read package (gmvread) that is easy to use and will
    provide more flexibility when new keywords are added.  GMV was 
    modified to use the package.
2.  Added two new cell types, vface2d and vface3d and a new keyword 
    vface.  The new cell types define a cell by a set of face numbers 
    entered with the vface keyword.  Face data under the vface keyword 
    contains the vertex nodes, the processor number for the node, the 
    opposite face, the opposite face processor number and the cell 
    that the node belongs to.
3.  Modified the algorithm that eliminates duplicate faces to keep 
    all faces and identify the primary cell and the cell opposite 
    the duplicate face.  This is so that the Test Normal function  
    for cell faces works correctly for interior cells while still 
    allowing for faster drawing by not drawing faces inside the outer 
    boundary. 
4.  Added an information line in Query Data that displays the distance 
    between the last two selected nodes.
5.  Modified the Vector Scale in Vector Control so that the vector scale 
    is increased/decreased by a factor of 10.
6.  Added an Arrow Head Size slider control to increase or decrease the 
    size of the arrowhead.
7.  Modified the gmvrc process to print whether the gmvrc file is read 
    from the local directory or from the home directory.
8.  Corrected an error in the gmvrc process that caused a segmentation 
    fault when using display options for data that did not exist.
9.  Corrected an error in Animation that caused vectors not to animate 
    when the Display List option is on.
10. Corrected an error in Query Data that can cause a Segmentation Fault 
    on some systems while in Get Node From Field Value or Get Cell From 
    Field Value menus.


GMV V1.9:

The following changes were made to version 1.9 of GMV:
1.  Modified the expose routine to copy and redisplay the last image 
    without rendering the image again.  This will prevent the long 
    wait (on large problems) after a menu clears after covering part 
    of the main drawing area.
2.  Added a Rubberband Zoom capability under Controls-2.  Use this feature
    to draw a box around an area of the display to zoom into.
3.  Modified the interactivity slider to get faster response with large
    problems.  When the slider is pushed all the way to the top, the 
    following occurs:
    a). When the mouse buttons are pressed, a solid, bounding box appears 
        for orientation purposes.
    b). The boxes drawn when dragging the sliders in Cell Node and Tracer 
        Subset, Polygon Subset, Cell Selection Box, Node Selection Box, 
        and Surface Selection Box will overlay the latest image.
    c). The Rubberband Zoom box is drawn as an overlay on the latest image.
4.  Set the Use Display List option default to "on".
5.  Changed the Auto Read Search Time default to .001 to speed up reading 
    the next file.
6.  Changed the averaging of cell centered field data to node field data  
    algorithm so that averaging to node field data will not occur if a 
    node field with the same name already exists.
7.  Extended the Scale Axis range to (1.0e-10 to 1.e10). 
8.  Added a Test Normals button to the Cells menu.  When on, cell faces  
    with inward (toward the screen) facing normals are drawn in black.  
    Normals are generated from the face vertex order using the right-
    hand rule.
9.  Added an Apply button to the Subset menus.  Click on the Apply button 
    to select data after setting the subset box.
10. Modified the include files for better efficiency.
11. Modified the initialization routines.
12. Corrected an error in Surface Materials the did not create and use 
    material numbers greater than those input under the Material keyword 
    for cells.
13. Corrected an error in Grid Analysis that allocated a new data field 
    every time the menu was opened an a field selected.
14. Corrected an error in the SGI Mesa version that caused a Bus error 
    on certain X servers.
15. Corrected errors in Search Box and Search Sphere in Nodes, Cells and 
    Surface Select functions.  Data was not reset after a Scale Axis  
    change was applied.
16. Corrected and error that caused GMV to byte swap data when the number 
    of nodes is large and faces are input instead of cells.
17. Corrected an error in Query Data when getting node or cell numbers by 
    field value when the field data mins. and maxs. are very large numbers.


GMV V1.8:

The following changes were made to version 1.8 of GMV:
1.  Added an option to Tracer History plots that toggles the 
    intermediate points on/off.
2.  Changed the Polygon subset menu to look and act like Nodes, Cells, 
    and Tracers subset menu, except to draw a purple bounding box while 
    using the sliders.
3.  Created a version that uses stereo on Infinite Reality boards and 
    on the Imersadesks attached to the ASCI computers.
4.  Added a new object - surface.  Surfaces are defined by facets whose 
    vertices are mesh nodes.  Surfaces can have the following data 
    defined for each facet: material numbers, flags, velocity and variable 
    fields.  The display and selection options for surface facets are very 
    similar to Cell options.  See the user's manual for details on surface 
    input and drawing optons.
5.  Corrected an error in Query Data where the y and z cell centers were 
    the same.
6.  Corrected an error in  Build Cell Vectors that did not close the Cell 
    Field menu.
7.  Corrected and error that prevented reading of ascii files if there 
    was 2 or 3 spaces between the gmvinput and ascii keywords.  Be sure 
    that there are no more than 3 spaces between gmvinput and ascii.


GMV V1.7:

The following changes were made to version 1.7 of GMV:
1.  Added a line cell type to the input format.
2.  Modified the Tracer History option to add a "Stop at" and "Stride" 
    read file options.
3.  Added a Vector Control menu item to Controls-2.  The Vector Scale 
    slider has been moved from the main window to the Vector Control
    menu.  There is a "Arrow Heads On" toggle button on the menu that
    controls the drawing of arrow heads.  If this option is off, the
    old style text color line segment is drawn at the vector end.
4.  Modified the Auto Read menu to add "Step Back" and "Step Next" buttons 
    under the Pause function.  These options will read the previous or 
    next file determined by "Stride".
5.  Modified Grid Analysis to draw cell numbers in grey.
6.  Added Display List to the gmvrc option list.  The keyword is 
    display_list followed by on or off.
7.  Modified Cutline to add "2 Nodes" and "2 Cells" buttons.  Use these 
    options to get the coordinates of a line between two nodes or two 
    cell centers.
8.  Corrected an error in Scale Axis that generated a polygon read 
    warning message when no polygons were input.


GMV V1.6:

The following changes were made to version 1.6 of GMV:
1.  Changed the default of the Materials and Flag selection menus in 
    Nodes and Cells from "Or" to "And" for the material/flag combinations.
2.  Added points on Trace History lines at time steps.  The point size 
    and shape is controlled by Point Size in Controls-2.
3.  Added a Reset button to the Plot Box menu that will reset the 3D 
    plot box back to the original size of the problem.
4.  Added arrowheads to vectors.  The arrowheads are tall pyramids with 
    the base colored in the textcolor to help determine direction.
5.  Modified Vector Scale so that vectors can be scaled from .001 to 10.
6.  Added the capability to color vectors by node/cell field and flags.  
    The Color By option in the Nodes or Cells menu will now also color 
    vectors.  You must now also click Apply to activate vector drawing.
7.  Modified the File Selection browser to maintain a seperate directory 
    path for each read/write file function that uses File Select.
8.  Added the capability to read a GMV resource file.  GMV will look  
    for gmvrc in the directory where GMV is started, then in the home 
    directory and will use the drawing options in gmvrc to display the 
    initial image.
9.  Added a "Write gmvrc" to the Files menu.  This will write a gmvrc 
    file with the current drawing options.
10. Corrected an error in the Attributes file if cells are selected by 
    node field range.
11. Corrected errors in Query Data that caused invalid cursor functions 
    if the Query Data menu is open when reading a new simulation.
12. Corrected an error in Auto Center when only nodes are drawn.


GMV V1.5:

The following changes were made to version 1.5 of GMV:
1.  Modified GMV to read 64 bit binary reals with the new ieeei4r8 file 
    type.  With ieeei4r8, all floating point input data must be 64 bit 
    long (double precision), including problem time.
2.  Modified GMV to store all input floating point data as 64-bit floats 
    (double precision).
3.  Moved the background slider bars to a submenu under the Color Edit 
    (in Controls-1) options.
4,  Modified the Node and Cell Select Box menus to include sliders that 
    interactively adjust the select box.  In the Node option, a green 
    box appears that is controlled by the sliders.  In the Cell option, 
    a red box appears.
5.  Modified Query Data to display the material name along with the 
    material number and the flag type name along with the flag number.
6.  Modified the Motif GUI to use a TrueColor Visual and Colormap, if 
    one is available, for SGI's only.
7.  Corrected an error in Cutplanes that left "holes" in the cutplane 
    under certain conditions.
8.  Corrected a Segmentation Fault error when quitting GMV with the 
    Display List option on.
9.  Corrected an error that generated possibly incorrect cutlines when 
    reading or auto reading same simulations.
10. Corrected an error that did not display polygons as the first image 
    when there are no nodes or cells in the input file. 


GMV V1.4:

The following changes were made to version 1.4 of GMV:
1.  Modified GMV to allow 2D structured grids.  If nzv is set to 1 for a  
    2D structured mesh, GMV will generate quadrilateral cells instead of
    hexahedral cells.
2.  Modified the initial view angles after reading the first input file and 
    after reading a new simulation file.  The new view angles provide the 
    best view of the largest length along the axes.  For 2D problems the 
    view will be from the top with the Y axis pointing right and the X axis 
    pointing up.
3.  Added the option of displaying stipple patterns when drawing polygons.  
    This is an undocumented feature.
4.  Modified the initial view to display cell faces, instead of polygons, 
    after reading the first input file, or after reading a new simulation 
    file.  If there are no cells, then nodes are displayed.  If there are 
    no cells and no nodes, then polygons are displayed.
5.  Added a temporary disk file to store polygon data so that memory can be 
    freed when polygons are not drawn.
6.  Corrected errors in the attributes file.  Omitted setting node and cell 
    sphere and box selection data.  Also omitted setting the node number 
    option from the Cell Menu.
7.  Corrected an error when using Auto Read.  Omitted setting node and cell 
    sphere and box selection data.
8.  Corrected an error in cutplanes when calculating cell data if the cell 
    face lies on the cutplane.
9.  Corrected an error that did not calculate new fields in Field Calc. when 
    reading or autoreading a same simulation file.
10. Corrected an error that drew the axes incorrectly when the problem was 
    located in a negative quadrant.


GMV V1.3:

The following changes were made to version 1.3 of GMV:
1.  Eliminated uneeded ifdefs from the source.
2.  Defined better structures for polygon data.
3.  Modified the cell display menu to add node numbers to the menu.
    When the option is on, the node locations are drawn as points colored 
    by the node's material color and the node numbers are drawn in the 
    text color.  With this option, node numbers are drawn in the correct 
    location when cell explode is on.
4.  Modified the face and edge list algorithm speed up the elimination of 
    duplicate faces/edges especially for spherical and cylindrical grids.  
    The algorithm now uses statistics so that bins with too many elements 
    are re-binned.
5.  Modified Cutline to automatically update the 2D plot when reading 
    another GMV file with Same Simulation or Auto Read options.
6.  Corrected an error that cased vector magnitude to be reset after an 
    attributes file was read or after reading a same simulation file.
7.  Corrected an error in Cutline that caused GMV to crash under certain 
    conditions. 
8.  Corrected an error that caused the Ultra3d version of GMV to crash 
    when processing a structured grid (nnodes -1).
9.  Modified the code to prevent a GLX Bad Render error when processing 
    GMV remotely on a workstation that has OpenGL Version 1.0 and displayed 
    on a workstation that has OpenGL Version 1.1.  The Polygon Offset 
    extension is not supported on newer versions of OpenGL X extensions. 
10. Corrected a get attributes error that caused the cutplane field to be
    reset to the first field.


GMV V1.2:

The following changes were made to version 1.2 of GMV:
1.  Modified Animation Cutplane menu to provide better beginning and 
    ending numbers.
2.  Extended the significant digits when displaying floating point 
    numbers on the Subset, Plot Box, and Data Limits menus.
3.  Added new options Reset All, Auto Reset and Auto Reset All to all 
    Data Limits menus.
4.  Added the Median Mesh Edges option to the cells menu.  This option 
    calculates and draws the median mesh.  Medium mesh edges are colored 
    by the field value while the regular cell edges are colored black.  
    When cell faces are drawn, the median mesh edges are colored white.
5.  Added Median Mesh Faces and Median Mesh Edges to the Grid Analysis 
    menu.  When selected, these options calculate and draw the median 
    mesh faces and/or edges for the selected cells.
6.  Modified the Node and Cell Search Sphere select options to add an 
    inside/outside option to determine the cells to select relative to 
    the search sphere.
7.  Added a Search Box option to Node and Cell selections.  A box is 
    defined by two opposite vertices.  The inside/outside option is used 
    to select nodes or cells that are inside or outside the box.
8.  Modified the cutplane height option drawing so that the minimum value 
    of the data on the cutplane will clamp to the plane.
9.  Changed the creation of Cutline, Cutplane and Isovolume widgets  
    so that they are created once, then modified after reading a new 
    simulation.  Currently, they are destroyed and recreated for each 
    new simulation.
10. Added a Cutsphere option to the Calculate menu.  Data is interpolated 
    to the intersection of the cutsphere and cell edges then used to
    color the sphere surface.  Up to 5 cutspheres are available and each 
    cutsphere has the same drawing options as do cutplanes.
11. Added a 2D plotting package to Cutlines using the SciPlot widget set.  
    The 2D plot appears when a cutline is created.  All the cutlines are 
    displayed on the plot.
12. Modified the algorithm that generated cutplanes to add a search for 
    faces that lie on the cutplane.  This allows cutplanes on 2D grids 
    which can provide a relief (height) map of 2D grids.  Also relaxed 
    the epsilon test to determine faces on the plane.
13. Added an animated Cutsphere option to the Animation menu.  Cutsphere 
    animate from a begining radius to an ending radius.
14. Added Sin, Cos, Tan and Atan functions to the node and cell Field 
    Calculator.  Data for these functions must be in radians.
15. Corrected an error that did not enable the cutplane options for the 
    animation cutplane.
16. Modified the algorithm that calculates the vectors for the "Explode" 
    options so that regions that have the same vectors will have their 
    vectors modified.  This will ensure that all regions will be separated.


GMV V1.1:

The following changes were made to version 1.1 of GMV:
1.  Developed a new face list based input format.  This format uses 
    a new keyword "faces" that replaces the "cells" keyword.  This 
    new format allowed a single set of faces to be defined, along 
    with the associated cells that each face is a part of.
2.  Added an option to change the field data colormap.  Three new 
    colormaps are added - Red to Blue, Greyscale and Blue to Red by 
    contour levels.  Also, a user defined color map can be read in.  
3.  Added an option to change the number of contour levels.
4.  Added an option to refine cell face drawing when coloring the face 
    with node data.  This option generates and interpolates a face 
    center point and edge centered points to provide a smoother color 
    change across the face.
5.  Added a new input keyword, "cellids" that allows an alternate 
    numbering for the cells for display and reference purposes.
6.  Added a new input keyword, "nodeids" that allows an alternate 
    numbering for the nodes for display and reference purposes.
7.  Modified the drawing routine for isosurfaces and isovolumes so  
    that drawing is 1.8 times faster.
8.  Corrected an error in the draw angle text input function that 
    required a return (enter) for each angle change.  Now one return 
    will now read and activate all angle changes at one time.
9.  Corrected and error that caused GMV to crash when changing the 
    scale axes on a problem that has no cells.


GMV V1.0:

The following changes were made to version 1.0 of GMV:
1.  Added a "Use Display List" option to the Controls-2 menu.  This 
    option causes GMV to use OpenGL display lists to contain most of 
    the drawing primitives.  This will greatly improve drawing speed for 
    rotation, pan or zoom functions when  GMV is run remotely.  The lists 
    exist on the local workstation and will not be sent through the 
    network for each image.  Drawing speed is better with display list 
    when the local workstation has good graphics hardware.  Since Mesa 
    versions use software rendering, there are no significant differences 
    in drawing speed.  Note that when the display list option is on, the 
    "PS Lines" Snapshot option is not available.
2.  Added an option to provide a title at the bottom of the image.
3.  Developed a new polygon normal algorithm that ensures a correct normal
    even for degenerate polygons and polygons with three or more vertices
    that lie on a line.
4.  Modified the display window expose event routine to disable animation 
    and snapshot functions while drawing for the expose event.
5.  Modified the callback routines for animation and snapshot functions to 
    wait until the file selection menu is popped down before continuing.  
    This change along with item 4 corrects some Snapshot problems where 
    a menu shadow appeared on the snapshot rgb file and counting errors 
    with animation.
6.  Corrected an error that caused an overflow when a rotation angle 
    greater than the slider limits was entered in the angle text field.  
    Now an input rotation angle will be transformed to lie between -180 
    and 180 degrees.
7.  Moved the "Data Limits" menu item from Controls-2 to Controls-1.
8.  Corrected an error in the Linux Mesa version that did not draw 
    tracers.



GMV V0.9.9:

The following changes were made to version 0.9.9 of GMV:
1.  For SGI only, added two stereoscopic views (Stereo Perspective and
    Stereo Flight) to the view menu.  These new view modes place the 
    monitor in full screen mode and require Crystal Eyes LCD glasses 
    and emitter (or equivalent).  The mouse controls operate the same 
    as in the non-stereo mode (Perspecive or Flight).  Three new key-
    board keys control stereo, the x key is used to exit Stereo mode, 
    the m key is used to increase eye separation and the n key is used 
    to decrease eye separation.  Modify eye separation to decrease eye 
    fatigue.  Note, the Stereo options only work on local SGI workstations,
    the option will not work from a remote SGI.  If GMV crashes while in 
    Stereo mode, issue the following command to return the monitor back 
    to normal mode:  /usr/gfx/setmon -n 72HZ
2.  Modified all input/output functions to pop up the file selection menu 
    before reading or writing a file.
3.  Added a line width option to Controls-2.  Lines can now be drawn in 
    1, 2, or 3 pixel widths and can be aliased or antialiased.
4.  Modified the tracer menu to allow tracer points to be drawn as points 
    whose size and shape is determined by the Point Size control.
5.  Modified the Subset menu for nodes, cells and tracers to use larger 
    increments when clicking in the slider area.  Also, changed the text 
    are to allow user input of subset limits.
6.  Modified the View Angle text area to allow user input of view angles.  
    Also changed the slider definitions so that they always line up.
7.  Changed the node x, y, and z arrays to node structure arrays to insure 
    better caching of a node's location values.
8.  Changed movie size SGI RGB images to 720x486 pixels  to better fit our 
    new video recorder controller.
9.  Modified the code to compile a 64 bit version of GMV.
10. Corrected an animation error.  When paused, the regular mouse functions 
    were not working correctly, and when the animation resumed, it did not 
    resume from the animation settings that existed when paused.  Also, the 
    frame count for snapshot files was incorrect.
11. Corrected an error that caused GMV to quit when an error in closing the 
    snapshot file was encountered.


GMV V0.9.8:

The following changes were made to version 0.9.8 of GMV:
1.  Added "Select On" and "Apply" buttons to all submenus in Node 
    Select and Cell Select.  These buttons have the same function 
    as those on the Node Select menu and the Cell Select menu.
2.  Added the capability to color the surface of field isosurfaces,
    material isosurfaces and isovolume with a blue-to-red intensity 
    color of a node field.
3.  Cutplanes can now be colored with node material or cell material 
    colors.
4.  Added the ability to read more than one set of material data.  
    Materials can now be read for both node data and cell data.  Note, 
    Use two "material" keyword sets of data.  The material numbers 
    should be consistent for a particular material.  Material names 
    should be repeated for equivalent material numbers.
5.  Added an auto scale feature to data limits for use when reading 
    another input file for the same simulation, or in the attributes 
    file.  Enter 0 as the minimum and 0 as the maximum data range 
    for a field, then when a the next file is read, or the attributes 
    file applied, the data limits will scale to the data entered for 
    the field.
6.  Changed the maximum number of faces per cell in the "general" cell 
    from 200 to 1000.  The error message when the maximum number of 
    faces is reached has been modified to list the number of faces.
7.  Modified the algorithm that calculates average node field data and 
    node velocities from cell data to exclude cells with 0 material.
8.  Corrected an error that always turned on cycle number and problem 
    time when reading another file using the "Same Simulation" options.
9.  Corrected an error in allocating trace selection memory.
10. Corrected an array bounds error in the trace selection function.
11. Corrected an error that changed the watch cursor while reading in 
    another input file.
12. Corrected an error that caused a segmentation error after closing 
    the Query Data menu when no cells exist.


GMV V0.9.7:

The following changes were made to version 0.9.7 of GMV:
1.  Added two new GMV input file read options, "Same Simulation, 
    Same Cells" and "Auto Read - Same Simulation, Same Cells".  
    The new options are similar to "Same Simulation" and "Auto 
    Read - Same Simulation" except that they expect the cell 
    configuration to be the same as the current file.  This means 
    that the cells are defined by the same node numbers.  The nodes 
    can have a different x,y,z coordinate.
2.  Added a "Shade" option to Cell Faces drawing.  This turns on a 
    lighting model and shades the cell faces.
3.  Changed the Cell Edges drawing to color the edges by a cell or 
    a node field value.  For node field values, the cell edges are 
    smoothly colored according to the node values.
4.  Added new options to node select and added new menus to process 
    the new options.  The new node selection options are "Search 
    Sphere" where nodes that lie within a user defined sphere are 
    selected and "Node Numbers" where nodes can be selected by 
    number, a range of numbers or a range of numbers with a stride.  
    The node select by "Node Field Data Range" now has it's own menu.
5.  Added new options to cell select and added new menus to process 
    the new options.  One new cell selection option is "Node Field 
    Data Range" where cells whose nodes lie within the specified data 
    range are selected.  The selection can be further restricted by 
    specifying that only cells that have all their nodes lie within 
    the data range be selected.  The other options are "Search Sphere" 
    where cells whose cell centers lie within a user defined sphere 
    and "Cell Numbers" where cells can be selected by number, a range 
    of numbers or a range of numbers with a stride.  The cell select by 
    "Cell Field Data Range" now has it's own menu.
6.  Added a tracer selection option where tracers can be selected by 
    a number, a range of numbers, or a range of numbers with a stride.  
    This option, as well as the node number and cell number selection 
    options allow up to 50 selection sets.
7.  Increased the number of bins used in generating unique cell faces 
    and cell edges to speed up the search in larger problems.
8.  Corrected an error that did not turn on the watch cursor when 
    reading the next file under the Same Simulation option.
9.  Corrected an Auto Center error when drawing cell numbers and/or 
    node numbers only.


GMV V0.9.6:

The following changes were made to version 0.9.6 of GMV:
1.  Added new GMV input file read options.  The new options are:
    a.  "New Simulation" - this is the same as the old read.  Use 
        this option to read a GMV file from a simulation different 
        than the one that created the current file.  Custom menus 
        are remade after reading the file and only the view angles,
        magnification and material colors remain from the previous
        image.
    b.  "Same Simulation" - use this option to read a GMV file that 
        is a different time step of the same simulation that created 
        the current file.  Custom menus are not remade, so the image 
        is displayed faster than with the New Simulation option.   
        The new image will have the same attributes as the previous 
        image.
    c.  "Auto Read - Same Simulation" - use this option to have GMV 
        automatically read a family of GMV files created by a single 
        simulation run.  The files must all have the same file name 
        prefix and must all have a 3 or 4 digit numeric suffix (eg.:
        gmvin.001, gmvin.002, gmvin.003, gmvin.004, .....).  All the 
        files must reside in the same directory as the current file.  
        GMV can read and display the files either forwards, forwards  
        to the latest, and backwards.  Once Auto Read is started, the  
        sequence can be paused and resumed.  An Auto Snapshots function  
        is also available to save rgb images of each file.  Each of the  
        new images will have the same attributes as the image for the 
        file that was current when auto read is started.  So be sure 
        that data limits, plot box, etc. are set to cover the span 
        of files, just as in making a GMV time sequence movie.
2.  Added a "History"option to the Tracers menu.  When this option 
    is applied, GMV will automatically read up to 250 previous GMV 
    input files and extract tracer data.  Then the each tracer is 
    drawn with colored lines connecting the history locations.  The 
    lines are colored according to the field value at the history 
    points.  As with Auto Read, all the input files must have the 
    same file name prefix and a 3 or 4 digit numeric suffix.  The 
    files must also reside in the directory of the current file.
3.  Modified the read routines to check for "endgmv" in a GMV input 
    file.  The endgmv keyword must be within the last 20 characters 
    of the input file before gmv will continue.  This helps prevent 
    GMV from reading an incomplete input file then crashing.
4.  Added verification that Put Attributes, Snapshot, Cutlines, and 
    Fly-Through Animation can open an output file.  If not, an error 
    box appears, and GMV can continue.
5.  Added the ability to display nodes or cells with a 0 material 
    number.  Now unconnected nodes or nodes or cells that have been 
    tagged with a 0 material number to prevent them from being shown 
    can be displayed on demand.  These nodes or cells will always 
    pass material selection.
6.  Added an error check for cells that contain a node number that 
    is out of range (< 0 or > nnodes).
7.  For OpenGL implementations, added a polygon offset when both 
    lines and polygons (or faces) are requested.  The lines are
    now more distinct. 
8.  Developed a new, more robust duplicate face list algorithm.
9.  Modified the front and back clip plane algorithms.
10. Change Query Data to interactively get node or cell numbers from 
    the screen when only node or cell numbers are displayed.
11. Corrected an error in get attributes that did not set scale axis 
    and polygon subset correctly.
12. Corrected an error that made the full size snapshot image smaller 
    than the requested window size.
13. Corrected an error when displaying transparent objects in OpenGL 
    on some SGI workstations.
        


GMV V0.9.5:

The following changes were made to version 0.9.5 of GMV:
1.  Added Isovolume function to the Calculate menu.
2.  Added Multiple Cutplanes, where the cutplanes all interpolate 
one node field or cell field.  Each cutplane has its own set of 
options.  Additionally, existing cutplanes will remain while the 
Animation cutplane is processing.
3.  Added the ability to change the size and shape of the node 
points.  The shape of the points can be square or round.  Round 
points are anti-aliased and display much slower than square points.
This function is available for OpenGL version only in Controls-2.
4.  Changed the attributes file.  The new file is an ASCII file that 
is keyword driven.  The file contains control data only for those 
functions that exist when the file was created.  In addition, there 
is a line at the beginning of the attributes file that lists the 
GMV input file that was opened at the time that the attributes file 
was created.  This is so that users can check that the GMV file name 
is compatible before getting the attributes file.
5.  Modified Grid Analysis so that cells and nodes with material 
numbers less than 1 are displayed in black.
6.  Modified Grid Analysis to show node points in their material 
colors when Node Numbers is selected.  This feature in conjunction 
with a larger point size can be used to compare node and cell materials.
7.  Added tri (triangle) and quad (quadrilateral) cell types to the 
input format.
8.  Added a fromfile keyword to the nodes, cells, material, flags and 
polygons input types.  This keyword enables those input types that do 
not change over the course of the simulation to reside on only one GMV 
input file.  The fromfile keyword tells GMV to retrieve that input data 
type from the specified file.  Note: the specified file must be enclosed 
in quotes.  Eg.  nodes fromfile "gmvinput.001".
9.  Modified the Motif GUI to work with TrueColor visuals.
10.  Corrected a Snapshot Menu error that did not create PS Lines for  
subsequent snapshots.
11.  Corrected and error that prevented the interactive Subset bounding 
box from displaying when only node or cell numbers are selected.
12.  Greyed out the Options button in the Cutplane menu until a Cutplane 
has been created.
13.  Corrected an error that caused the Solaris version to crash under 
certain conditions when reading a subsequent GMV input file.
14.  Corrected a potential error when selecting a node field or a 
cell field.
15.  Corrected an error when specifying the Animation isosurface range.


GMV V0.9.4:

The following changes were made to version 0.9.4 of GMV:
1.  Changed the internal cell structures to point to a common face 
list and a common edge list for cells.  This change allows faster 
cell drawing and more flexibility when generating isosurfaces.
2.  Changed cell drawing to draw cell edges once and to draw only 
the outside shell for faces.  This change provides significant 
drawing speed especially for Mesa version and on workstations 
without advanced graphics hardware.
3.  Change isosurface calculations to include outside shell faces.  
For field isosurfaces, all face nodes must match to be included.
For material isosurfaces, any face node that matches will be 
included and interpolated if necessary.
3.  Changed the Mesa versions to allocate a private colormap.  
This change ensures that GMV will always run and provide a good 
set of colors for the images.
4.  Modified the Node, Cell, and Polygon menus to better define 
the controls affected by the Apply button.
5.  Modified the Cell and Grid Analysis controls to draw cell 
numbers even if Cell Edges is not selected.
6.  Added an Apply button to the Animation Explode Menu.
7.  Changed the algorithm that generated material numbers to the 
nodes from cell data.  The node is assigned the material number 
that has the largest representation from the nodes common cells.
8.  Modified Subset, Bounding Box, Center, Scale Axis, and Plot 
Box menus so they are created once at initialization time and 
not for each input file read.  Also modified the slider values 
to use the maximum - minimum value as the slider range.
9.  Added cutplane contours to the PS Lines snapshot option.
10.  Added code to delete the temporary field database file when 
quitting GMV from the window manager decoration quit or with Ctl-c.
11.  Corrected an error that prevented the Bounding Box from 
expanding with Reflections on.
12.  Modified the pop-up widgets so they always rise to the top 
when selected.
13.  Added more messages to the information line.



GMV V0.9.3:

The following changes were made to version 0.9.3 of GMV:
1.  Added the capability to keep cell field data for display and 
processing.  The cell field data is still averaged to the nodes 
so that processing and display of the averaged fields as node data
is still available.  The cell field data impacts the following:
   Display:
      Cell - Added Cell Field option for Faces display, the face 
             will be displayed as a single color.
           - Added Vector button, cell centered vectors can be 
             built and displayed.
           - Added cell field selection criteria to Cell Select
   Calculate:
      Cutplane - Added Cell Field option, the area intersected 
                 with the cutplane for a cell will be displayed
                 as a single color.
               - Added ability to display Cell Vectors, Cells and 
                 Cell Numbers within the Distance (Dist.) function.
      Grid Analysis - Added Cell Field option for Faces display.
      Cut Lines - Added Cell Field option, cell data selected from 
                  cell centers will be translated to the cutline.
      Field Calc - Option to select from node data or cell data. 
      Query Data - Added cell fields to Cell Values.
   Controls-1:
      Animation - Added Cell Field option to Cutplane animation.
   Controls-2:
      Data Limits - Option to select from node data or cell data.
2.  Added an Options button to Cutplane.  The button pops up a new 
menu that contains the Clip options, Height option, Distance option,
Edges option, and Contour Lines option.  These options were previously 
on the Cutplane menu.
3.  Added an Options button to the Animation Cutplane menu to pop up 
the Cutplane Options menu described above.
4.  Added a Show Coordinates button to the Bounding Box menu.  This 
option toggles the display of the bounding box coordinates.  Also, 
the bounding box lines are now thicker.
5.  Modified the Cell Explode and Polygon Explode maximums from 150% 
to 1000%.
6.  Corrected an Auto Center error that occurs after a pan.
7.  Corrected some widget building errors when no cell data exists.


GMV V0.9.2:

The following changes were made to version 0.9.2 of GMV:
1.  Added Query Data function to the main to the Calculate menu.
Query Data replaces the value function in the Nodes and Grid 
Analysis menus.  Query Data can probe the image to retrieve a 
node number or cell number.  Query Data can also return a node 
number based on a node field value.
2.  Changed the maximum number of materials and flag values to 128.
3.  Changed the Mesa (X) versions to provide a better 8-bit colormap 
for screen images.  Menu button colors are approximated.
4.  Created a Node Field Selection menu and changed all menus that 
used a scrolling node field window to display the current field and 
added a button to pop up the Node Field Selection menu.
5.  Added an Apply button to all the Display menus for better drawing 
control.  The Apply button must be pressed before changes are 
implemented.
6.  Added field range selection to Node Selection Menu.  This option 
can select for display those nodes that lie within a user defined 
data range for a selected node field.
7.  Changed cutplane menu to display node numbers of nodes selected 
with the distance function.
8.  Changed cutline menu to display node numbers of selected nodes.
9.  Added calculation of a default cutline search radius.
10. Changed the animation cutplane so that it can display selected 
nodes, velocities, or node numbers with the cutplane distance function.
11. Added Reset buttons to the Subset menus.
12. Corrected distance value error when writing cutlines file.
13. Corrected a drawing error when moving bounding box edges.
14. Corrected an error that omitted tracers from plot box and auto 
center calculations.
15. Corrected various errors when reading the attributes file in 
interactive mode.


GMV V0.9.1:

The following changes were made to version 0.9.1 of GMV:
1.  Added a value function to the nodes menu that allows display 
of data values for a selected node.
2.  Added more unary operators and some binary operators to the 
Field Calc. function.
3.  Changed the input routines to allow polygons with material 
numbers that have not been defined in the material input list.
4.  Changed the cell edge drawing routine to draw 40% faster.
5.  Corrected an error when drawing transparent objects with GL.
6.  Corrected an error when calculating isosurfaces at cell 
vertices.
7.  Corrected an error that caused the code to crash when no field
data exists.
8.  Corrected an error when searching for nodes within a cutplane 
distance after reading an attributes file.
9.  Corrected an error in setting polygon menu material buttons 
after reading an attributes file.
10. Corrected an error when drawing the small axis window before 
reading gmv data.
11. Converted all the OpenGL byte color calls to floating point 
color calls for future Cray version.


GMV V0.9:

The following changes were made to version 0.9 of GMV:
1.  A new feature has been added to the Cells display form.  Under 
the Faces button is a new Contour button that controls drawing of 
contour lines on the cell faces.
2.  A new Cutline option has been added to the Calculate menu.  
This option allows the user to define a line through the problem 
along with a search radius to create a cylinder.  The selected 
field value for any nodes that lie within the cylinder are trans-
fered to the line.  The result is a color intensity line of the 
node data along the line.  A wave option allows a wave that is
centered on the cutline to be displayed.  A nodes option displays 
the nodes that lie within the cylinder.  The line can be defined 
by entering 2 points, by interactively selecting one point on the
screen (the line will be normal to the screen), or by interactively 
selecting two points on an existing cutplane.  A search radius can 
be determined by using the Distance option in the Calculate menu.
Up to twenty cutlines can be generated, and a Write to File option 
writes the line data to a file for further processing by the user.
3.  A new Field Calc. option has been added to the Calculate menu.
This option provides 5 new fields whose values can be generated with
a set of unary calculations (eg. log, ln, sqrt, etc.).
4.  All popup menus are unrealized when reading an attributes file 
so that the information on the menus are updated correctly.
5.  Changed the cursor icon to a watch when calculating isosurfaces, 
cutplanes and cutlines.
6.  Pop down the Flight Animation menu when reading a new gmv 
file.
7.  Modified attributes routines to save the three cutplane points 
and fill the Cutplane menu with the cutplane points when reading 
an attributes file.
8.  Corrected an error when setting the polygon material buttons in 
the Polygon display menu after reading a new gmv file.
9.  Corrected an error when deleting an isosurface in the Isosurface 
Field menu after reading an attributes file.
10. Changed calculation for field maximum when all data for the field
is the same.  The new calculation adds 1/100000 of the minimum value.


GMV V0.8:

The following changes were made to version 0.8 of GMV:
1.  GMV has been modified to more easily accommodate a structured 
regular brick mesh.  This change allows for smaller input files 
and less memory used for regular brick meshes, since each cell is 
is generated on the fly.  There are some small changes to the GMV 
input file to read structured regular brick meshes.  First, on the 
"nodes" keyword, change the number of nodes value to -1, then add 
the number of vertices along the x, y, and z axes.  Second, enter 
the x, y, and z arrays for the number of vertex components along 
each of the axes.  Third, on the "cells" keyword, enter 0 as the 
number of cells.  The symbolic input lines are as follows:
     nodes -1 nvx nvy nvz
        x(nvx)
        y(nvy)
        z(nvz)
     cells 0 
For example, the node and cell input lines for a 4x5x3 regular 
brick mesh follows:
     nodes -1 4 5 3
        0. 1. 2. 3. 
        0. 1. 2. 3. 4.
        0. 1. 2.
     cells 0
2.  The regular Animation Menu now has pause control buttons.  
These buttons allow the user to pause, step back or forward, or 
go to a particular frame number.  The pause control buttons are 
not active when saving snapshots of the animation.  Also, the 
Animation Isosurface Menu displays the current isosurface value, 
and the Animation Cutplane Menu displays the current location of 
the cutplane along the selected axis.
3.  A new "Lines" button has been added to the "Cutplane" Menu.  
This button allows contour lines to be drawn on the cutplane.  
The lines are drawn in the same color as the text color and are 
at the intervals shown on the cutplane colorbar (except for the 
minimum and maximum values).  Also, in order to speed drawing 
during interactive manipulations only portions of the cutplane 
is drawn.
4.  There are now two Isosurface Menus, one for Field Isosurfaces 
another for Material Isosurfaces.  The Isosurface button on the 
Main Menu Bar now has a submenu with "Field" and "Material" buttons.
5.  Flag data is no longer combined for both nodes and cells.  There 
are now separate lists for node flag data and for cell flag data.
6.  In order to speed interactive drawing while manipulating with 
the mouse, the complete image will not be drawn as long as a mouse 
button is held down.  Previously, the complete image was drawn 
whenever mouse movement stopped, even if a mouse button was held 
down.
7.  Main menu bar items that are not relevant to the current data 
are now unavailable.  In fact, these menus will not be created.
For example, if there is no cell data, then the Cell display menu 
and the Isosurface, Cutplane and Grid Analysis menus will not be 
generated and their buttons on the Main menu bar will be grey.
8.  The Speed field (generated when two or more fields exist) has 
been renamed Vect Mag for Vector Magnitude.  This field contains
the vector magnitude for the current vector.  The K. E. (kinetic 
energy) field will not be generated.
 


GMV V0.7:

The following changes were made to version 0.7 of GMV:
1.  The field (variable) data base is now placed on a temporary 
disk file that begins with "GMVT".  This temporary file will be
in the /var/tmp, /usr/tmp or /tmp directory, depending on the
permissions for the directories.  If GMV quits with any error 
messages relating to the field data, try setting the TMPDIR 
environmental variable to a directory that is writable and  
has larger disk space.  eg: setenv TMPDIR /mydisk/mydirectory
If GMV aborts, look for and delete files with a GMVT prefix.
Arrays are allocated for displaying the current node, cell and 
grid analysis field data so these objects are drawn just as 
fast as the previous versions.  The disk database should save
significant memory for large problems with more than three 
variable data fields.  There are now up to 100 data fields 
available.
2.   Added "Clip on Cell Selection" buttons to Isosurface and
Cutplane menus.  This option restricts isosurface and cutplane
calculations to cells selected by the "Selection" submenu of
the "Cells" display menu.
3.  Added the ability to build vectors from variable fields.
The "Velocity" button in the "Nodes" display menu has been 
changed to "Vector", this button has a submenu with an "On" 
toggle button to draw the current vectors and a "Build Vector"
button.  The "Build Vector" button pops up a new menu that 
enables the user to build a vector from the variable fields.
The Speed and K. E. fields are recalculated for the new vector.
If velocities are input, the fields U, V and W are created and 
velocity is the initial vector.
4.  Added a "Isosurface" button to the Animation menu.  This
button pops up a new menu that allows isosurface animation of 
a selected field over a user defined range at nframe increments.
5.  Added a "Title" button to the "Controls-2" pull down menu.
This option allows a tile of up to 40 characters to be placed 
at the top center of the image.
6.  Placed all the material, field and flag buttons in scrolling 
windows.
7.  Placed the node selection and cell selection elements in 
scrolling windows and added "On" and "Off" buttons to the menus.
The "On" and "Off" buttons are used to select or unselect all of 
the materials or flags in that section.
8.  Automatically pops up the appropriate display menu (Polygons,
Cells or Nodes) for the image drawn after reading a GMV file.


GMV V0.6:

The following changes were made to version 0.6 of GMV:
1.  A new menu item called View has been added.  View allows the 
user to select one of three viewing functions: Orthographic (the
default), Perspective and Flight.  Mouse and slider controls are
the same for Orthographic and Perspective views, although the 
Distance, Cutplane 2-points, and Polygon Subset controls are not 
available under Perspective view.  The mouse controls for Flight 
view are only slightly different.  In Flight view, the right mouse 
button is used to move into and out from the image, while the left 
mouse button controls heading and pitch.  The middle mouse button 
still controls left-right, up-down panning.  In Flight mode, the 
Elev. slider is the pitch angle, where 90 is level flight and 0 is
straight down.  The Azim. slider is the heading, where 0 is parallel 
to the X-axis heading in the positive X direction, and 90 is 
parallel to the Y-axis heading in the positive Y direction.  While 
in Flight view, a large multi-colored crosshair appears to help 
the user line up the direction of travel.  This crosshair will not 
be seen while generating snapshots. 
2.  A new Animation menu used during Flight view has been added.
The Flight Animation menu allows users to define a flight path to 
animate over.  The flight path is defined by moving to a new pos- 
ition and entering the number of frames to animate from the previous 
position and setting a control point at that position.  A button 
allows the user to return to the current control point.  Users can 
also delete the current control point an move to the previous control 
point with a delete button.  When an animation is completed and 
saved to a set of snapfiles, the control point list is cleared.
3.  An Explode feature has been added to the regular Animation menu.
This enables animation of polygon or cell explode functions.
4.  The view information was added to the Attributes file, and an
error in the Colorbar menu settings when reading an Attributes file
was corrected.
5.  An error in creating the Trace Data Limits menu was corrected.


GMV V0.5:

The following changes were made to version 0.5 of GMV:
1.  The middle mouse button is now used to pan the image.  The pan
box has been removed and the light and axis boxes have been moved.
2.  Modified the Tracer menus so that tracers can be drawn with an 
icosahedron (Big Point), with the tracer number or with the tracer
value.  The color of the tracer element is still determined by the 
blue to red color interpolation of the tracer values.
3.  Added a default color button to the Color Edit menu.  When this 
option is on, a mouse click on a material or isosurface button will 
restore the default color for that material or isosurface.
4.  Added a Stop button to the Animation menu so that users can stop 
an unwanted animation sequence.  Changed the Do It to Start and added
a frame counter.  Also changed the Animation menu to keep Cutplane 
and Fade settings after closing the Animation menu.  Toggle buttons 
indicate when Cutplane or Fade settings are on.
5.  Changed Polygon, Matorder, Color Edit, Node Select and Cell 
Select menus to display two columns of material/flag buttons when 
there are more than 32 materials/flags.
6.  Changed the Grid Analysis Menu so that it contains it's own set
of cell display controls.  These controls are independent of the Cell
display from the Display menu.  Also corrected an error that caused 
some cell edges to be drawn in black when displaying node numbers in 
the Grid Analysis display.
7.  Corrected an error that caused the program to crash when reading 
a new file that did not have flag data.
8.  Corrected an error that caused the program to crash when opening 
the Animation form if the problem contained only node data.
9.  Corrected an error that turned off the bounding box button after 
reading a new file.
10.  Changed generation of main menu sliders so they don't run into
each other when specifying the size of the drawing window.  eg.
  gmv -w 0 0 640 512 
tells gmv to create a drawing window 640 pixels wide by 512 pixels
high.
11.  Modified the program to clear the main drawing window as soon
as the Read Gmv file selection menu pops down.  This also prevents 
a second drawing command to be issued because of the expose event.
12.  Cycle no. and the problem time will not be automatically dis-
played unless data was entered.
13.  Changed the Color Bar default to On.
14.  Corrected an error when generating cutplanes and isosurfaces 
from an attributes file.



GMV V0.4:

The following changes were made to version 0.4 of GMV:
1.  The Grid Analysis function has been enhanced.  The menu now has a 
Start New Display and Add To Display buttons.  The Start New Display 
button displays only the cells selected, while the Add To Display button 
adds the selected cells to those previously selected and displays all 
selected cells.
2.  A Color Bar display function has been added to the image control 
menu.  When turned on, this function displays an annotated color bar 
of field values for nodes, tracers, cell field display, or cutplanes.  
Only one colorbar can be displayed, the field data on the color bar is
determined by the order listed above.  The user can use the Preference 
submenu to select a color bar display preference.
3.  A Data Limits function has been added to the image control menu.
This function enables the user to input the minimum and maximum values 
to clamp to the blue-to-red color intensity images of field or tracer 
data values.
4.  A Bounding Box display function has been added to the image control
menu.  When pressed, a Bounding Box menu pops up.  This is used to turn
the bounding box on, and contains sliders that controls the size of the
bounding box.  The bounding box size is also affected by the reflection,
scale axis, and plot box functions.  The bounding box has it's main x, 
y, and z axis colored the same as the axis colors and are annotated by 
the current x, y, and z minimum and maximum sizes.
5.  A Cutplane feature has been added to the Animate control function.
This feature allows animation of a cutplane along the x, y, or z axis.
The user specifies the field to interpolate on as well as the beginning
and ending locations along the selected axis.  The clip on subset and
height features from the regular Cutplane menu are in effect when the
animation cutplane is processed and displayed.
6.  The attributes file has been updated to version 2 to accommodate
the new functions.
7.  Because of the new control functions, the main menu now has two 
Controls pulldown items, Controls-1 and Controls-2.  The control 
functions are found in alphabetical order within the two items.
8.  The Matedit control function has been renamed Color Edit.  The 
Contour calculate function has been renamed Isosurface.
9.  All material, flag and isosurface colored buttons now have their 
text annotated in black.
10.  The time and cycle no. annotations and the color bar are drawn in
2-D mode. 


GMV V0.3:

The following changes were made to version 0.3 of GMV:
1.  GMV has been ported to OpenGL, ifdefs define which graphics library
(GL or OpenGL) to compile and link, see Readme.makfiles.  The OpenGL
version works better than the GL version on SGI Indys.
2.  A number of memory errors caused by reading a new GMV file have been
fixed.  New checks have been added and error messages printed if there 
is not enough memory to run a GMV problem.
3.  Nodes and cells that have a material number less than 1 will not be 
drawn.
4.  Color cells for material and contour buttons are allocated from the
default colormap to reduce color flashing between menus.  If there aren't 
enough cells to allocate, a new motif colormap is created.  
5.  In order to speed point and line drawing, antialiasing is disabled on 
workstations that do not have graphics hardware designed for antialiasing.
6.  The small axis window has been moved so that the GMV window can fit 
on workstations with smaller screens.  A parent widget has been created 
so that the popup menus (except for File Menus) pop up on the right side 
of the GMV window and not in the center of the drawing window.
7.  Calls to the drawing routine when poping down a menu have been 
removed to reduce unnecessary drawing.
8.  All snapshots generated by GMV are saved in SGI rgb format no matter 
what workstation GMV is running.


GMV V0.2:

The following changes were made to version 0.2 of GMV: 
1. The ability to read other GMV files without the need to quit the 
program.  The viewing angles, magnification and material colors are 
the only attributes that are kept from the previous file.  If exact 
attributes are needed from the previous file, use the put and get 
attribute buttons.
2. The use of a menu bar.  All the buttons that existed on the main 
window are now under a pull-down menus.
3. Generating a 3D plotting box around the problem, not including 
the origin.
4. Because of 3, adding a Plot Box function that enables the user 
to specify a 3D plotting box.  The specified plot box dimensions 
are placed in the attributes file.  This is useful when a problem 
moves through space or shrinks and expands over time.  The Plot Box 
function allows the user to specify a plotting box that provides a 
constant perspective.
5. The Matedit menu was modified to display 2 columns of material 
buttons when there are more than 32 materials in a problem.
6. The magnification limit has been raised to 1000X.
7. The cell numbers are now drawn at the centroid of the cell.
8. Some button overlap errors have been corrected.


GMV V0.1:

Original version of GMV (General Mesh Viewer).
