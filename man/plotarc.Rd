\name{plotarc}
\alias{plotarc}

\title{Plots the data imported from an ARC file}
\description{

Taking as argument the list returned by the get.arcdata function,
this function plots all the arcs. With the argument new the user can decide
whether to plot on a new window/device or on the last window/device.
}

\usage{plotarc(arc, new=T, ...)}

\arguments{
\item{arc}{The data returned by a call to get.arcdata}
\item{new}{Do you want to plot on the last window/device or on a new one?}
\item{...}{Options to be passed to a call to the function plot when
creating the display (i. e., window or file).}
}

\value{
This function returns nothing, but plots a nice map :-D.
}

\keyword{hplot}
