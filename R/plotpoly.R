plotpoly <-function(arc,bnd,pal,index,col)
{
	lindex<-length(index)
	col<-rep(col,length.out=lindex)	

#First, we just select the arcs we will need. This will speed up
#this function

	plot((bnd[1]+bnd[3])/2,(bnd[2]+bnd[4])/2,xlim=c(bnd[1],bnd[3]),ylim=c(bnd[2],bnd[4]),type="n" )	

	for(i in 1:lindex)
	{
	palindex<-which(pal[[1]]$PolygonId==index[i])
	arcindex<-as.vector(sapply(pal[[2]][palindex],function(X){X[[1]]}))

	arcindex<-as.vector(arcindex)
	palindex<-as.vector(palindex)
	

	print(arcindex)
	arcindex<-sort(unique(abs(arcindex)))


	arc1<-list(arc[[1]][1:7][arcindex,],arc[[2]][arcindex])
	pal1<-list(pal[[1]][1:6][palindex,],pal[[2]][palindex])

#Now, we plot the polygons

		#This gives us the list of arcs 
		p<-which(pal1[[1]]$PolygonId==index[i])
		p<-as.vector(pal1[[2]][[p]][[1]])


		#And here we get the "rows" of them in the variable arc
		absp<-abs(p)
		a<-which(arc1[[1]]$ArcId==absp[1])

		if(length(absp)>1)
		{
			for(j in 2:length(absp))
				a<-c(a,which(arc1[[1]]$ArcId==absp[j]))
		}

#	p<-absp
#When p[[j]]==0 it means that the previous arcs build a closed polygon
#and it can be plotted

		#This counts the number of zeroes found in the Arc List.
		#It is used to correct the arc to plot.
		zeroes<-0
		if(p[[1]]>0)
		{
			x<-arc1[[2]][[ a[1] ]][[1]]
			y<-arc1[[2]][[ a[1] ]][[2]]
		}
		else if(p[[1]]<0)
		{
			x<-arc1[[2]][[ a[1] ]][[1]]
			y<-arc1[[2]][[ a[1] ]][[2]]
			x<-x[length(x):1]
			y<-y[length(y):1]
		}
		else
		{
			if(length(x)>0 && length(y)>0)
			{
				if( (x[1]!=x[length(x)]) || (y[1]!=y[length(y)]) )
				{
					print("The polygon isn't closed")
					print(c(1,p[[1]]))
					x<-c(x,x[1])
					y<-c(y,y[1])
					#print(x)
					#print(y)

					polygon(x,y,col=col[which(index==index[i])])
					lines(c(x[length(x)],x[1]),c(y[length(y)],y[1]),col[which(index==index[i])])
				}
				else
				{
					polygon(x,y,col=col[which(index==index[i])])
				}
				x<-c()
				y<-c()
			}

		}

		lp<-length(p)
		if(lp>1)
		{
			for (j in 2:lp)
			{
				if(p[[j]]>0)
	                	{
	        	                x<-c(x, arc1[[2]][[ a[j-zeroes] ]][[1]])
        	        	        y<-c(y, arc1[[2]][[ a[j-zeroes] ]][[2]])
	        	        }
        	        	else if(p[[j]]<0)
	                	{
        	                	xaux<-arc1[[2]][[ a[j-zeroes] ]][[1]]
	        	                yaux<-arc1[[2]][[ a[j-zeroes] ]][[2]]
					x<-c(x,xaux[length(xaux):1])
					y<-c(y,yaux[length(yaux):1])
        	        	}
				else
				{

					zeroes<-zeroes+1
					if(length(x)>0 && length(y)>0)
					{
						if( (x[1]!=x[length(x)]) || (y[1]!=y[length(y)]) )
						{
							print("The polygon isn't closed")
							print(c(j,p[[j]]))
							print(c(x[1],y[1]))
							print(c(x[length(x)],y[length(y)]))
						
#					x<-c(x,x[1])
#					y<-c(y,y[1])
					polygon(x,y,col=col[which(index==index[i])])
					lines(c(x[length(x)],x[1]),c(y[length(y)],y[1]),col=col[which(index==index[i])])
#print(x)
#print(y)
						}
						else
						{
							polygon(x,y,col=col[which(index==index[i])])
						}

					x<-c()
					y<-c()
					}
				}
			}
		}


		if(length(x)>0 && length(y)>0)
		{
			if(x[1]!=x[length(x)] || y[1]!=y[length(y)] )
{
				print("The polygon isn't closed")
							print(c(j,p[[j]]))
					x<-c(x,x[1])
					y<-c(y,y[1])
#print(x)
#print(y)
					polygon(x,y,col=col[which(index==index[i])])
					lines(c(x[length(x)],x[1]),c(y[length(y)],y[1]),col=col[which(index==index[i])])
}
else
{
			polygon(x,y,col=col[which(index==index[i])])
}
		}

		x<-c()
		y<-c()
	}
}
