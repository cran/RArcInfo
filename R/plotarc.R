#Plots all the arcs imported from an ARC file by get.arcdata

#New: T for new plots
plotarc<-function(arc, new=T)
{

ll<-length(arc)

if(new==T)
{
#Calculate the boundary

bnd<-c(0,0,0,0)
nbnd<-c(0,0,0,0)

x<-arc[[1]][[8]]
l<-as.integer(length(x)/2)

bnd[1]<-min(x[2*1:l-1])
bnd[2]<-max(x[2*1:l-1])
bnd[3]<-min(x[2*1:l])
bnd[4]<-max(x[2*1:l])



for(i in 2:ll)
{

x<-arc[[i]][[8]]
l<-as.integer(length(x)/2)

nbnd[1]<-min(x[2*1:l-1])
nbnd[2]<-max(x[2*1:l-1])
nbnd[3]<-min(x[2*1:l])
nbnd[4]<-max(x[2*1:l])

	for(j in 1:2)
	{
		if( nbnd[2*j-1] < bnd[2*j-1] ) bnd[2*j-1]<-nbnd[2*j-1]
		
		if(nbnd[2*j]>bnd[2*j]) bnd[2*j]<-nbnd[2*j]

	}
}


range<-max( c(bnd[2]-bnd[1], bnd[4]-bnd[3]) )/2

xmedian<-(bnd[1]+bnd[2])/2
ymedian<-(bnd[3]+bnd[4])/2

plot(arc[[1]][[8]][1], arc[[1]][[8]][2], xlim=c(xmedian-range,xmedian+range), ylim=c(ymedian-range, ymedian+range), type="n")

}##if(new)

for (i in 1:ll)
{
	l<-length(arc[[i]][[8]])/2
	lines(arc[[i]][[8]][2*1:l-1], arc[[i]][[8]][2*1:l])
}


}
