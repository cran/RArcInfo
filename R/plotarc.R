#Plots all the arcs imported from an ARC file by get.arcdata

#New: T for new plots
plotarc<-function(arc, new=T)
{

#We only need the list of arcs, not the dataframe with the other data
arc<-arc[[2]]

ll<-length(arc)

if(new==T)
{
#Calculate the boundary

bnd<-c(0,0,0,0)
nbnd<-c(0,0,0,0)

x<-arc[[1]][[1]]
y<-arc[[1]][[2]]
l<-as.integer(length(x))

bnd[1]<-min(x)
bnd[2]<-max(x)
bnd[3]<-min(y)
bnd[4]<-max(y)



for(i in 2:ll)
{

x<-arc[[i]][[1]]
y<-arc[[i]][[2]]
l<-as.integer(length(x))

nbnd[1]<-min(x)
nbnd[2]<-max(x)
nbnd[3]<-min(y)
nbnd[4]<-max(y)

	for(j in 1:2)
	{
		if( nbnd[2*j-1] < bnd[2*j-1] ) bnd[2*j-1]<-nbnd[2*j-1]
		if(nbnd[2*j]>bnd[2*j]) bnd[2*j]<-nbnd[2*j]
	}
}


range<-max( c(bnd[2]-bnd[1], bnd[4]-bnd[3]) )/2

xmedian<-(bnd[1]+bnd[2])/2
ymedian<-(bnd[3]+bnd[4])/2

plot(arc[[1]][[1]], arc[[1]][[2]], xlim=c(xmedian-range,xmedian+range), ylim=c(ymedian-range, ymedian+range), type="n")

}##if(new)

for (i in 1:ll)
{
	lines(arc[[i]][[1]], arc[[i]][[2]])
}


}
