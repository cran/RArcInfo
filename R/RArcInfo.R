get.namesofcoverages <- function(directory) 
{
	directory<-as.character(directory)
#	.Call("get_names_of_coverages", as.character(dir))

	if(length(dir(path=directory, pattern="info"))==1)
	{
		fil<-file.info(dir(path=directory, full.names=T))

		covnames<-(dir(path=directory))[fil$isdir]
		covnames<-covnames[covnames!="info"]
	}
}
get.tablenames <-function(infodir) 
{
	data<-.Call("get_table_names", as.character(infodir))

	#A data frame with all the data
	data.frame(TableName=I(data[[1]]), InfoFile=I(data[[2]]), NFields=data[[3]], RecSize=data[[4]], NRecords=data[[5]], External=I(data[[6]]))
}
get.tablefields <- function(infodir, tablename) 
{
	data<-.Call("get_table_fields", as.character(infodir), as.character(tablename))

	#A data frame with all the data
	data.frame(FieldName=I(data[[1]]), FieldType=data[[2]])
}

get.arcdata <- function(infodir, coverage, filename="arc.adf") 
{
	data<-.Call("get_arc_data", as.character(infodir), as.character(coverage), as.character(filename))

	#a table (dataframe) with the first seven fields is built
	df<-data.frame(ArcId=data[[1]], ArcUserId=data[[2]], FromNode=data[[3]], ToNode=data[[4]], LeftPoly=data[[5]], RightPoly=data[[6]], NVertices=data[[7]])

	list(df, data[[8]])
}


get.bnddata <- function(infodir, tablename) 
	.Call("get_bnd_data", as.character(infodir), as.character(tablename))

get.paldata <- function(infodir, coverage, filename="pal.adf") 
{
	data<-.Call("get_pal_data", as.character(infodir), as.character(coverage), as.character(filename))

	#a table (dataframe) with the first six fields is built
	df<-data.frame(PolygonId=data[[1]], MinX=data[[2]], MinY=data[[3]], MaxX=data[[4]], MaxY=data[[5]], NArcs=data[[6]])

	list(df, data[[7]])
}

get.labdata <- function(infodir, coverage, filename="lab.adf") 
{
	data<-.Call("get_lab_data", as.character(infodir), as.character(coverage), as.character(filename))
	data.frame(LabelUserID=data[[1]], PolygonID=data[[2]], Coord1X=data[[3]], Coord1Y=data[[4]], Coord2X=data[[5]], Coord2Y=data[[6]], Coord3X=data[[7]], Coord3Y=data[[8]])
}

get.cntdata <- function(infodir, coverage, filename="cnt.adf") 
{
	data<-.Call("get_cnt_data", as.character(infodir), as.character(coverage), as.character(filename))

	df<-data.frame(PolygonID=data[[1]], CoordX=data[[2]], CoordY=data[[3]], NLabels=data[[4]])

	list(df, data[[5]])
}

get.toldata <- function(infodir, coverage, filename="tol.adf") 
{
	data<-.Call("get_tol_data", as.character(infodir), as.character(coverage), as.character(filename))
	data.frame(Type=data[[1]], Status=data[[2]], Value=data[[3]])
}

get.tabledata <- function(infodir, tablename) 
{
	data<-.Call("get_table_data", as.character(infodir), as.character(tablename))

	df<-data.frame(I(data[[1]]))
	l<-length(data)

	if(l>=2)
	{
		for (i in 2:l)
			df<-cbind(df,I(data[[i]]))

	}
}

