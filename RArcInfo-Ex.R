pkgname <- "RArcInfo"
source(file.path(R.home("share"), "R", "examples-header.R"))
options(warn = 1)
library('RArcInfo')

assign(".oldSearch", search(), pos = 'CheckExEnv')
cleanEx()
nameEx("RArcInfo")
### * RArcInfo

flush(stderr()); flush(stdout())

### Name: RArcInfo
### Title: RArcInfo
### Aliases: RArcInfo
### Keywords: file

### ** Examples


#See example(plotpoly) for another example

library(RArcInfo)

datadir<-system.file("exampleData",package="RArcInfo")
infodir<-system.file("exampleData","info",package="RArcInfo")
coveragedir<-system.file("exampleData","wetlands",package="RArcInfo")

#get.bnddata needs the last slash...
infodir<-paste(c(infodir,"/"), collapse="")

#List all the tables

covnames<-get.namesofcoverages(datadir)
tablenames<-get.tablenames(infodir)


#Display the name of the table and its fields
for(i in 1:length(tablenames[[1]]))
{
	print(c("Table: ",tablenames$TableName[i]))

	fields<-get.tablefields(infodir,tablenames$TableName[i])
	print("Fields")
	for(j in 1:length(fields))
		print(fields[[j]][1])

	#Get the data
	if(i==1)
		tabledata<-get.tabledata(infodir,tablenames$TableName[i])
	else
		tabledata<-c(tabledata, get.tabledata(infodir,tablenames$TableName[i]) )
}

#Import data from some tables
arc<-get.arcdata(datadir,"wetlands")
pal<-get.paldata(datadir,"wetlands")
lab<-get.labdata(datadir,"wetlands")
cnt<-get.cntdata(datadir,"wetlands")

bnd<-get.bnddata(infodir,"WETLANDS.BND")

print("Plotting all the arcs")
plotarc(arc)

print("Plotting the first ten polygons (in red) on the previous plot")
par(col="red")
plotpal(arc,pal,new=FALSE, index=1:10)



graphics::par(get("par.postscript", pos = 'CheckExEnv'))
cleanEx()
nameEx("plotpoly")
### * plotpoly

flush(stderr()); flush(stdout())

### Name: plotpoly
### Title: Plots polygons defined by the coverages.
### Aliases: plotpoly
### Keywords: hplot

### ** Examples

#This example is the same as the one provided under the test directory

#1.- Create temporary directory (if needed)
#1.- Extract the E00 file form a ZIP file
#2.- Create an Arc/Info binary coverage
#3.- Create the map

#get current working directory
cwd<-getwd()
#Create tmp directory.
tmpdir<-tempdir()


datadir<-system.file("exampleData",package="RArcInfo")
setwd(datadir)
file.copy(c("valencia.zip", "data_valencia.csv"), tmpdir, overwrite = TRUE)

setwd(tmpdir)

#Convert E00 file to a binary covertage to be imported into R

#Comment this line if the file valencia.e00 already exists
zip.file.extract(file="valencia.e00", zipname="valencia.zip")

#Comments this lines if the binary coverage already exists
library(RArcInfo)
e00toavc("valencia.e00", "valencia")


library(RColorBrewer)
library(RArcInfo)

#Read map data
arcsmuni<-get.arcdata(".", "valencia")
palmuni<-get.paldata(".", "valencia")
bnd.muni<-get.bnddata("info/", "VALENCIA.BND")
patmuni<-get.tabledata("./info", "VALENCIA.PAT")

#Number of polygons
nmuni<-length(palmuni[[1]][[1]])

municipios<-data.frame(1:nmuni, patmuni$"VALENCIA-ID")
names(municipios)<-c("INDEX", "CODMUNICI")


#Datafiles to be used

unemp<-read.table(file="data_valencia.csv", sep=";", 
	dec = ",",skip=1)

unemp<-unemp[,c(1,3)]
names(unemp)<-c("CODMUNICI", "UNEMP")

breaks<-quantile(unemp[,2], c(0,  .025,.2, .8, .975, 1))

unemp<-cbind(unemp, CAT=as.ordered(cut(unemp[,2], breaks, include.lowest = TRUE) ))


#Colors to be used in maps
#colors<-brewer.pal(5, "Oranges")
colors<-brewer.pal(5, "Greens")

ldata<-merge(unemp, municipios, by.x="CODMUNICI", by.y="CODMUNICI")


#Valencia
idx<-(ldata$"CODMUNICI">=46000)
bnd.muni<-c(626679.9, 4250000, 760000, 4460000)


p<-par()
pin<-1.5*p$pin
main<-"Rate of unemployment"

plotpoly(arc=arcsmuni, pal=palmuni, bnd=bnd.muni,
	index=ldata$INDEX[idx], col=colors[ldata$CAT][idx],
	xlab="", ylab="", main=main,
	xaxt="n", yaxt="n", bty="n")

#Set legend
l<-levels(unemp$CAT)
l[1]<-"[0.00,1.26]"
legend(700000, 4460000, fill=colors, 
legend=l, bty="n", cex=1)

setwd(cwd)



graphics::par(get("par.postscript", pos = 'CheckExEnv'))
### * <FOOTER>
###
cat("Time elapsed: ", proc.time() - get("ptime", pos = 'CheckExEnv'),"\n")
grDevices::dev.off()
###
### Local variables: ***
### mode: outline-minor ***
### outline-regexp: "\\(> \\)?### [*]+" ***
### End: ***
quit('no')
