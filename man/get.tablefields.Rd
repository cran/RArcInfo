\name{get.tablefields}
\alias{get.tablefields}

\title{Read names of the table fields in the coverages}
\description{
Returns the names of the fields (and its type) in the table whose name is 
provided by the user. 
}


\usage{get.tablefields(infodir,tablename)}

\arguments{
\item{infodir}{info directory where there is a file called arc.dat}
\item{tablename}{The name of the table from which we want to get the fields}
}

\value{
Basically, it is a list in which every element is
also a list with two items:

\item{Field Name}{The name of the field.}

\item{Field Type}{
This is an integer from 1 to 6 that explain the kind of data:

	\itemize{
		
	\item{1}{Date}

	\item{2}{Character String}

	\item{3}{Integer (stored as a character string)}

	\item{4}{Numeric (stored as a character string)}

	\item{5}{Binary integer}

	\item{6}{Binary float}
	}
}
More information about this kind of data can be found at
\url{http://pages.infinit.net/danmo/e00/docs/v7_bin_cover.html}.
}

\keyword{file}
