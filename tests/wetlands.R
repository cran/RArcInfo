library(RArcInfo)

datadir<-system.file("exampleData",pkg="RArcInfo")
infodir<-system.file("exampleData","info",pkg="RArcInfo")
coveragedir<-system.file("exampleData","wetlands",pkg="RArcInfo")

#List all the tables
tablenames<-get.tablenames(infodir)


#Display the name of the table and its filds
for(i in 1:length(tablenames))
{
	print(c("Table: ",tablenames[[i]][1]))

	fields<-get.tablefields(infodir,tablenames[[i]][1])
	print("Fields")
	for(j in 1:length(fields))
		print(fields[[j]][1])

	#Get the data
	if(i==1)
		tabledata<-get.tabledata(infodir,"wetlands",tablenames[[i]][1])
	else
		tabledata<-c(tabledata, get.tabledata(infodir,"wetlands",tablenames[[i]][1]) )
}

#Import data fromsome tables
arc<-get.arcdata(datadir,"wetlands")
pal<-get.paldata(datadir,"wetlands")

print("Plotting all the arcs")
plotarc(arc)

print("Plotting the first ten polygons (in red) on the previous plot")
par(col="red")
plotpal(arc,pal[1:10],new=FALSE)

