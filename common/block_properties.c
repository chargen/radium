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








#include "nsmtracker.h"
#include "placement_proc.h"
#include "list_proc.h"
#include "vector_proc.h"
#include "windows_proc.h"
#include "notes_legalize_proc.h"
#include "fxlines_legalize_proc.h"
#include "reallines_proc.h"
#include "time_proc.h"
#include "tracks_proc.h"
#include "undo_blocks_proc.h"
#include "wtracks_proc.h"
#include "cursor_proc.h"
#include "visual_proc.h"
#include "player_proc.h"
#include "player_pause_proc.h"
#include "OS_Bs_edit_proc.h"
#include "wblocks_proc.h"
#include "Beats_proc.h"
#include "../embedded_scheme/scheme_proc.h"
#include "seqtrack_proc.h"

#include "block_properties_proc.h"


extern struct Root *root;

void Block_Set_num_lines(
	struct Blocks *block,
	int num_lines
){
	Place lastplace1,lastplace;
	struct Tracker_Windows *window=root->song->tracker_windows;
	struct WBlocks *wblock;
	struct LocalZooms *localzoom;
	struct Tracks *track=block->tracks;
	struct Notes *note;
	int org_num_lines=block->num_lines;
	int lokke;

	if(org_num_lines==num_lines || num_lines<2) return;

        PC_Pause();{
              
          PlaceSetLastPos(block,&lastplace1);

          block->num_lines=num_lines;

          PlaceSetLastPos(block,&lastplace);

          if(num_lines<org_num_lines){

            CutListAt_a(&block->signatures,&lastplace);
            CutListAt_a(&block->lpbs,&lastplace);
            CutListAt_a(&block->tempos,&lastplace);
            CutListAt_a(&block->temponodes,&lastplace);
            PlaceSetLastPos(block,&block->lasttemponode->l.p);
            ListAddElement3(&block->temponodes,&block->lasttemponode->l);

            while(track!=NULL){
              CutListAt_a(&track->notes,&lastplace);
              note=track->notes;
              while(note!=NULL){
                CutListAt(&note->velocities,&lastplace);
                CutListAt(&note->pitches,&lastplace);
                if(PlaceEqual(&note->end,&lastplace1) && note->noend==1){
                  PlaceCopy(&note->end,&lastplace);
                }
                note=NextNote(note);
              }
              LegalizeNotes(block,track);

              CutListAt_a(&track->stops,&lastplace);

              VECTOR_FOR_EACH(struct FXs *fxs, &track->fxs){
                CutListAt_a(&fxs->fxnodelines,&lastplace);
              }END_VECTOR_FOR_EACH;
              LegalizeFXlines(block,track);
              track=NextTrack(track);
            }
            while(window!=NULL){
              wblock=ListFindElement1(&window->wblocks->l,block->l.num);
              CutListAt_a(&wblock->localzooms,&lastplace);
              window=NextWindow(window);
            }
          }else{

            PlaceSetLastPos(block,&block->lasttemponode->l.p);

            while(track!=NULL){
              note=track->notes;
              while(note!=NULL){
                if(PlaceEqual(&note->end,&lastplace1) && note->noend==1){
                  PlaceSetLastPos(block,&note->end);
                }
                note=NextNote(note);
              }
              LegalizeNotes(block,track);
              track=NextTrack(track);
            }
            while(window!=NULL){
              wblock=ListFindElement1(&window->wblocks->l,block->l.num);
              for(lokke=org_num_lines;lokke<num_lines;lokke++){
                localzoom=talloc(sizeof(struct LocalZooms));
                localzoom->Tline=lokke;
                localzoom->Tdividor=1;
                localzoom->zoomline=lokke;
                ListAddElement3(&wblock->localzooms,&localzoom->l);
              }
              window=NextWindow(window);
            }

          }
          
          TIME_block_num_lines_have_changed(block);

        }PC_StopPause(NULL);
        
	window=root->song->tracker_windows;

	while(window!=NULL){
		wblock=ListFindElement1(&window->wblocks->l,block->l.num);
                UpdateWBlockWidths(window,wblock);
		UpdateRealLines(window,wblock);
		window=NextWindow(window);
	}

        
}



// Must be called when inserting or deleting tracks.
void Block_Set_num_tracks(
	struct Blocks *block,
	NInt num_tracks
){
	struct Tracker_Windows *window=root->song->tracker_windows;
	struct WBlocks *wblock;
	NInt org_num_tracks=block->num_tracks;
	NInt lokke;

	if(num_tracks==0){
		RError("Error in function 'Block_Set_num_tracks' in file 'block_properties.c'. num_tracks=0.\n");
		return;
	}

	if(num_tracks==org_num_tracks) return;

	block->num_tracks=num_tracks;

	if(num_tracks<org_num_tracks){

          if (window->curr_track >= num_tracks){
            //RError("window->curr_track >= num_tracks: %d >= %d",window->curr_track, num_tracks);
            //ATOMIC_WRITE(window->curr_track, num_tracks - 1);
            wblock=(struct WBlocks *)ListFindElement1(&window->wblocks->l,block->l.num);

            SetCursorPosConcrete(
                                 window,
                                 wblock,
                                 num_tracks - 1,
                                 -1);
          }
          CutListAt1(&block->tracks,num_tracks);
          while(window!=NULL){
            wblock=(struct WBlocks *)ListFindElement1(&window->wblocks->l,block->l.num);
            CutListAt1(&wblock->wtracks,num_tracks);
            window=NextWindow(window);
          }
	}else{
		for(lokke=org_num_tracks;lokke<num_tracks;lokke++){
			AppendTrack(block);
		}
		window=root->song->tracker_windows;
		while(window!=NULL){
			wblock=(struct WBlocks *)ListFindElement1(&window->wblocks->l,block->l.num);
			UpdateWTracks(window,wblock);
			window=NextWindow(window);
		}
	}

        TIME_block_num_tracks_have_changed(block);
}


void Block_Properties(
	struct Blocks *block,
	NInt num_tracks,
	int num_lines
){
    PC_Pause();{
      
	if(num_tracks!=block->num_tracks){
		struct Tracker_Windows *window=root->song->tracker_windows;
		while(window!=NULL){
			struct WBlocks *wblock=(struct WBlocks *)ListFindElement1(&window->wblocks->l,block->l.num);
			SetCursorPosConcrete(window,wblock,0,-1);
			window=NextWindow(window);
		}
		Block_Set_num_tracks(block,num_tracks);
	}

	if(num_lines!=block->num_lines){
		Block_Set_num_lines(block,num_lines);
	}

	struct Tracker_Windows *window=root->song->tracker_windows;
	while(window!=NULL){
		struct WBlocks *wblock=ListFindElement1(&window->wblocks->l,block->l.num);
		if(wblock->curr_realline>=wblock->num_reallines){
			wblock->curr_realline = wblock->num_reallines-1;
		}

		UpdateReallinesDependens(window,wblock);
		window=NextWindow(window);
	}

    }PC_StopPause(NULL);
}

void Block_set_name(struct Blocks *block, const char *new_name){
  block->name=talloc_strdup(new_name);
  BS_UpdateBlockList();
  BS_UpdatePlayList();
}

void Block_Properties_CurrPos(
	struct Tracker_Windows *window
){
	NInt num_tracks;
	int num_lines;
	char seltext[500];
	ReqType reqtype;

	struct WBlocks *wblock=window->wblock;
	struct Blocks *block=wblock->block;

	reqtype=GFX_OpenReq(window,33,5,"Edit block properties. Press return to keep old value.");

	sprintf(seltext,"Number of tracks (now %d): ",wblock->block->num_tracks);
	num_tracks=GFX_GetInteger(window,reqtype,seltext,2,200,true);
	if(num_tracks==1) num_tracks=wblock->block->num_tracks;

	sprintf(seltext,"Number of lines (now %d): ",wblock->block->num_lines);
	num_lines=GFX_GetInteger(window,reqtype,seltext,2,2000,true);
	if(num_lines==1) num_lines=wblock->block->num_lines;

	sprintf(seltext,"Name (now: '%s'): ",wblock->block->name);
	const char *blockname=GFX_GetString(window,reqtype,seltext,true);

        bool has_made_undo = false;

	if(blockname!=NULL){
          ADD_UNDO(Block_CurrPos(window));
          has_made_undo = true;
          Block_set_name(wblock->block, blockname);
        }

	GFX_CloseReq(window,reqtype);

	if(num_tracks==wblock->block->num_tracks && num_lines==wblock->block->num_lines){
		return;
	}

        if (has_made_undo==false)
          ADD_UNDO(Block_CurrPos(window));

	Block_Properties(block,num_tracks,num_lines);

	window=root->song->tracker_windows;
	while(window!=NULL){
		window->must_redraw = true;
		window=NextWindow(window);
	}

}





















