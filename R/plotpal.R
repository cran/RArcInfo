#Plots the polygons in pal imported by get.paldata according
#to the arcs in arc (value returned by get.arcdata)
plotpal<-function(arc,pal, new=T)
{
	larc<-length(arc)
	arcs<-vector(mode="logical", length=larc)

	lpal<-length(pal)


	for(i in 1:lpal)
	{
		l<-length(pal[[i]][[5]])
		for(j in 3*1:(l/3)-2)
		{
			arcs[abs(pal[[i]][[5]][j])]<-T	
		}
	}

	plotarc(arc[arcs], new)

}
