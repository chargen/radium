/* Copyright 2000 Kjetil S. Matheussen

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


#ifndef RADIUM_COMMON_TIME_PROC_H
#define RADIUM_COMMON_TIME_PROC_H



extern LANGSPEC STime Place2STime_from_times(
                                             const struct STimes *times,
                                             const Place *p
                                             );

extern LANGSPEC STime Place2STime(
                                  const struct Blocks *block,
                                  const Place *p
                                  );

extern LANGSPEC double STime2Place_f(
                                    const struct Blocks *block,
                                    double time
                                    );

extern LANGSPEC Place STime2Place(
                                  const struct Blocks *block,
                                  STime time
                                  );

extern LANGSPEC bool isSTimeInBlock(const struct Blocks *block,STime time);
extern LANGSPEC STime getBlockSTimeLength(const struct Blocks *block);

extern LANGSPEC void TIME_block_tempos_have_changed(struct Blocks *block);
extern LANGSPEC void TIME_block_LPBs_have_changed(struct Blocks *block);
extern LANGSPEC void TIME_block_signatures_have_changed(struct Blocks *block);
extern LANGSPEC void TIME_block_num_lines_have_changed(struct Blocks *block);
extern LANGSPEC void TIME_block_swings_have_changed(struct Blocks *block);
extern LANGSPEC void TIME_global_tempos_have_changed(void);
extern LANGSPEC void TIME_global_LPB_has_changed(void);
extern LANGSPEC void TIME_global_signature_has_changed(void);
extern LANGSPEC void TIME_everything_has_changed2(int default_bpm, int default_lpb, Ratio default_signature);
extern LANGSPEC void TIME_everything_has_changed(void);
extern LANGSPEC void TIME_everything_in_block_has_changed(struct Blocks *block, int default_bpm, int default_lpb, Ratio default_signature);

  
#endif

