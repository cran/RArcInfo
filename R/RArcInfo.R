get.namesofcoverages <- function(dir) 
	.Call("get_names_of_coverages", as.character(dir))

get.tablenames <-function(infodir) 
	.Call("get_table_names", as.character(infodir))

get.tablefields <- function(infodir, tablename) 
	.Call("get_table_fields", as.character(infodir), as.character(tablename))

get.arcdata <- function(infodir, coverage, filename="arc.adf") 
	.Call("get_arc_data", as.character(infodir), as.character(coverage), as.character(filename))

get.bnddata <- function(directory, coverage, filename="bnd.adf") 
	.Call("get_bnd_data", as.character(directory), as.character(coverage), as.character(filename))

get.paldata <- function(infodir, coverage, filename="pal.adf") 
	.Call("get_pal_data", as.character(infodir), as.character(coverage), as.character(filename))

get.labdata <- function(infodir, coverage, filename="lab.adf") 
	.Call("get_lab_data", as.character(infodir), as.character(coverage), as.character(filename))

get.cntdata <- function(directory, coverage, filename="cnt.adf") 
	.Call("get_cnt_data", as.character(directory), as.character(coverage), as.character(filename))

get.toldata <- function(directory, coverage, filename="tol.adf") 
	.Call("get_tol_data", as.character(directory), as.character(coverage), as.character(filename))

get.tabledata <- function(infodir, coverage, tablename) 
	.Call("get_table_data", as.character(infodir), as.character(coverage), as.character(tablename))

