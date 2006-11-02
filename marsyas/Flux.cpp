/*
** Copyright (C) 1998-2006 George Tzanetakis <gtzan@cs.uvic.ca>
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/** 
    \class Flux
    \brief Flux calculate the flux between the current and prev. vector

    The flux is defined as the norm of the difference vector between 
two succesive spectra. 
*/

#include "Flux.h"

using namespace std;
using namespace Marsyas;

Flux::Flux(string name):MarSystem("Flux",name)
{
  //type_ = "Flux";
  //name_ = name;
}


Flux::~Flux()
{
}


MarSystem* 
Flux::clone() const 
{
  return new Flux(*this);
}

void
Flux::myUpdate()
{
  MRSDIAG("Flux.cpp - Flux:localUpdate");
  setctrl("mrs_natural/onSamples", getctrl("mrs_natural/inSamples"));
  setctrl("mrs_natural/onObservations", (mrs_natural)1);
  setctrl("mrs_real/osrate", getctrl("mrs_real/israte")->toReal());
  setctrl("mrs_string/onObsNames", "Flux,");
  
  prevWindow_.create(getctrl("mrs_natural/inObservations")->toNatural(),
		     getctrl("mrs_natural/inSamples")->toNatural());
}

void 
Flux::myProcess(realvec& in, realvec& out)
{
  checkFlow(in,out);
  
  // computer flux of observations for each time sample 
  prevWindow_.setval(0.0);

  for (t = 0; t < inSamples_; t++)
    {
      flux_ = 0.0;
      max_ = 0.0;
      for (o=0; o < inObservations_; o++)
	{
	  diff_ = in(o,t) - prevWindow_(o,t);
	  if (in(o,t) > max_) 
	    max_ = in(o,t);
	  flux_ += (diff_ * diff_);
	  prevWindow_(o,t) = in(o,t);
	}

      if (flux_ != 0.0)
		flux_ = flux_ / (max_ * max_ * inObservations_);
      out(0,t) = flux_;
    }

}

      
      







	
	
	
	
