#Plots the polygons in pal imported by get.paldata according
#to the arcs in arc (value returned by get.arcdata)
plotpal<-function(arc,pal, new=TRUE)
{

	#We only need the lists of arcs
	arc<-arc[[2]]
	pal<-pal[[2]]

	larc<-length(arc)
	arcs<-vector(mode="logical", length=larc)

	lpal<-length(pal)


	for(i in 1:lpal)
	{
		l<-length(pal[[i]][[1]])
		for(j in 1:l)
		{
			arcs[abs(pal[[i]][[1]][j])]<-TRUE	
		}
	}

	plotarc(list(c(0),arc[arcs]), new=new)

}
