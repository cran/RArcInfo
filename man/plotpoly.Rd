\name{plotpoly}
\alias{plotpoly}

\title{Plots polygons defined by the coverages.}

\description{
This function is capable of plotting polygons, referenced by its id number.
These can be filled with colors acording to a given variate.
}

\usage{plotpoly(arc,bnd,pal,index,col, ...)}

\arguments{
\item{arc}{The data returned by a call to get.arcdata}
\item{bnd}{The data returned by a call to get.bnddata}
\item{pal}{The data returned by a call to get.paldata}
\item{index}{IDs of the polygon to be plotted}
\item{col}{Colors to be used when filling the polygons}
\item{...}{Options to be passed to a call to the function plot when
creating the display (i. e., window or file).}
}

\value{
Thins function returns nothing, but plots a nice map. :-D
}

\keyword{hplot}
