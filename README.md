# G_TES_touch-synth-lib
The librairy used to program the G_TES_touch-synth


## Architecture

+-----------+				  +--------------+
|  INPUTS   |	    +--------------+	  |   DISPLAY    |
| midi      |------>| PARAMETERS   |----->| vPot         |
| sensors   |	    +--------------+      | Menus        |
+-----------+		    |		  +--------------+
			    v  
		    +--------------+
		    |   MOZZI	   |
		    | sound    	   |
		    |	  engine   |
		    +--------------+





## Todo:
 
 - finish GT_MenuParameter -> mostly done
 - implement midi_learn in param
 - implement main gui:
   - simple click switches between main gui and normal mode
   - in gui, rotating change the pre-selected item
   - in gui, clicking select an item
   - if selected, rotated changes the prospective input
   - long press opoen the parameter menu.
