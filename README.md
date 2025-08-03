# ActiWheel
Complex activity wheel for mice behavioural research

The wheel is very simple in its implementation, as it has an arduino MEGA 2560 with a [logger shield](https://www.az-delivery.uk/products/datenlogger-modul) at its core.  The wheel axel is the shaft of a continuous potentiometer so that the exact position of the wheel in the radial axis can be logged, at a frequency of 100 Hz. 

The potentiometer it was designed to work with can be found [here](https://uk.rs-online.com/web/p/potentiometers/6930746), although any continuous potentiometer would do. A possible improvement on the design would come from the implementation of a rotary encoder instead of a potentiometer.  
  
## Assembly instructions:  
1) Print all the parts - the rungs have a bigger circular base for printing stability that should be snapped away with wire cutters or other tool.   
  
2) Assemble the Base and the Box with M3 countersunk screws of appropriate length.  

3) Attach the Arduino onto the Lid with M3 countersunk screws and M3 nuts. Mount the logger shield. Insert the SD card.  
  
4) Use dupont wires or soldering to connect the potentiometer to 5V, ground and A8 (middle potentiometer pin). Mount the potentiometer onto the Box with its hex nut. Slot the Connector onto the potentiometer shaft, and mount the Wheel onto the Connector with an M3 screw (center of Wheel).  
  
5) To power the arduino, externalise 2 wires from 5V and ground through the small slit on top of the Box, ideally soldered to a female 5V jack to be connected to a wall 9V power supply. Make sure to use plastic or metal shielding for the part that is still inside the mouse cage to avoid them being chewed.  
  
6) Mount the Lid onto the Box with countersank M3 screws.  
