\name{plotpoly}
\alias{plotpoly}

\title{Plots polygons defined by the coverages.}

\description{
This function is capable of plotting polygons, referenced by its id number.
These can be filled with colors acording to a given variate.
}

\usage{plotpoly(arc,bnd,pal,index=NULL,col, xratio=1, yratio=1,...)}

\arguments{
\item{arc}{The data returned by a call to get.arcdata}
\item{bnd}{The data returned by a call to get.bnddata}
\item{pal}{The data returned by a call to get.paldata}
\item{index}{IDs of the polygon to be plotted. If it is 'NULL' then all the polygons are plotted.}
\item{col}{Colors to be used when filling the polygons}
\item{xratio}{Controls x-axis ratio. It can take any value from 0 to 1.}
\item{yratio}{Controls x-axis ratio. It can take any value from 0 to 1.}
\item{...}{Options to be passed to a call to the function plot when
creating the display (i. e., window or file).}
}

\value{
Thins function returns nothing, but plots a nice map. :-D
}

\keyword{hplot}
