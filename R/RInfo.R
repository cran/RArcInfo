get.namesofcoverages <- function(dir) 
	.Call("get_names_of_coverages", as.character(dir))

get.tablenames <-function(dir) 
	.Call("get_table_names", as.character(dir))

get.tablefields <- function(infodir, tablename) 
	.Call("get_table_fields", as.character(infodir), as.character(tablename))

get.arcdata <- function(directory, coverage, filename) 
	.Call("get_arc_data", as.character(directory), as.character(coverage), as.character(filename))

get.bnddata <- function(directory, coverage, filename) 
	.Call("get_bnd_data", as.character(directory), as.character(coverage), as.character(filename))

get.paldata <- function(directory, coverage, filename) 
	.Call("get_pal_data", as.character(directory), as.character(coverage), as.character(filename))

get.labdata <- function(directory, coverage, filename) 
	.Call("get_lab_data", as.character(directory), as.character(coverage), as.character(filename))

get.cntdata <- function(directory, coverage, filename) 
	.Call("get_cnt_data", as.character(directory), as.character(coverage), as.character(filename))

get.toldata <- function(directory, coverage, filename) 
	.Call("get_tol_data", as.character(directory), as.character(coverage), as.character(filename))

get.tabledata <- function(infodir, coverage, tablename) 
	.Call("get_table_data", as.character(infodir), as.character(coverage), as.character(tablename))

