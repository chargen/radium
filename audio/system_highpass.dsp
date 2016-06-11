/* Copyright 2016 Kjetil S. Matheussen

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. */


import("filter_smoothing.lib");

// Using a 2nd order Butterworth filter. The 'lowpass' function is implemented by Julius O. Smith III.

process = highpass(2,fc) with{
  fc = vslider("[1] Hp. Freq [unit:Hz] [style:knob]",
               315, 40, 20000, 1);
};