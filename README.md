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
 - GT_PhysicalButton: a small class handling a physical momentary button
 - finish GT_MenuParameter                                 
