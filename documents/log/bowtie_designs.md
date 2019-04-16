

```BowtieML
x = 4-40 threaded rod
(o) = alumina ionization column
o = RF coil
I = z pinch magnet
= or - or _ or O = graphite
! = copper electrode

|x|=========( )=========|x| - top graphite heater to keep ionization column warm \
    II    o (o) o    II |x|				also acts as acceleration anode?
    II    o (o) o    II |x|
    II    o (o) o    II |x|
    II    o (o) o    II |x|
      x   o (o) o   x   |x|
    ||x|| o (o) o ||x|| |x|	- nested insulators and nuts
  !!!|x|! o (o) o !|x|! |x| - copper bowtie connectors
     |x|===-_O_-===|x| 	|x|	- bowtie and injector and threaded insulators
    ||x||		  ||x|| |x|	- nested ceramic insulators 
    -----------------------	- aluminium base with blind 4-40 tapped holes
    
    Injector could be placed on the top. The top and bottom graphite heaters can be perpendicular.
    
Advantages:


Problems:
Collision with alumina wall is likely to cause high losses at high pressures.
No charge discrimination.

    
```



```BowtieML
Hybrid Pierce-type extractor/ICP

= - graphite
\/ - conductive but sufficiently RF transparent material - graphite?
|- - alumina insulator

			   - unspecified recombination wizardry
=====    =====
o|/        \|o - multi-segment deflection/astigmatism plates (optional)
o||		   ||o - alumina insulator ring - probably required at HV
o|\        /|o
o| \      / |o
o|  \	 /  |o - 50+kv electrostatic cone
-----    ----- - 
			   - standard graphite injection nozzle
			   
Electrostatic cone profile can be precisely adjusted to cancel out beam dispersion à la microwave dishes.

The beam must be recombined before exiting the focus chamber. 

Z-pinch and helical B-fields can be added to increase ionization efficiency; these won't affect the ions but will direct electrons. Care must be taken to ensure that these fields don't interfere with recombination.

Cone must be carefully positioned to prevent axial acceleration and the resultant X-rays. 

A phosphor+pmt interlock could be added for safety.

Why would the particles be compelled to move in a specific direction? With AC B input, would the entire arrangement not be perfectly symmetrical? 
```




```BowtieML
Hybrid Goertzel-type extractor

= - graphite
\/ - conductive but sufficiently RF transparent material - graphite?
|- - alumina insulator

			   - unspecified recombination wizardry
=====    =====
o|/        \|o - multi-segment deflection/astigmatism plates (optional)
o||		   ||o - alumina insulator ring - probably required at HV
o|-        -|o
o|--      --|o
o|- 	   -|o - 50+kv electrostatic focus structure
-----    ----- - 
			   - standard graphite injection nozzle

A reference structure is on p148 of Goerzel
```
```

```

```
      ||
      ||
      \
        \  |  |
      \   \|  |
      ||\	  |
      ||  \---|
      
          

The region where wire is injected must be colder than the melting point of the wire, else gas will escape. A large radiator can be attached to the nozzle.

```

``` 
()  = BaO coating or hot cathode
_   = high-resistance gradient deflector
--  = ceramic insulator
O   = ICP coil
[] 	= iron and mu-metal/pyrolytic graphite core and shield to prevent icp-recomb. interference
| | = graphite bowtie nozzle

   ||(     )||
   ||(_   _)|| 	    - recombination another fixed distance away
O] ----- ----- [O   - very small ~250um gap to stay within Langmuir limits
O] ----- ----- [O   - precise C-L-defined gap for ionization, +100v or so
	   | |		    - bowtie nozzle extension
 ======_O_======
 
 Entire structure can be heated to prevent deposition and avoid 
```

```
:  = graphite "drawing die" injector
#  = injected wire
- 
--------------------||
-----------\		||
########## :========||
-----------/
----------------------------
 ^   	   ^	    ^ - transition to ideal vapor temperature occurs here
 |         | - transition to melting temperature occurs around here.
 |				gas-tight seal must be formed.
 | -This side radiatively cooled by graphite fins to below melting point

This may allow us to accept different gauges of wire.
```

<hr>

There are some difficulties in using ICP for this application - namely,

- Ambiguity in output particle energy spectrum
- Time-varying components leading to excessively complex simulations. Full analysis requires both small-scale timesteps to model collisions, and large-scale timesteps to model "butterfly" ion trajectories.
- Uncontrolled magnetic field - shielding from recombination current will likely be required

The main reason why I considered it in the first place was to obviate the need for a heavy and dangerous high-power, high-voltage transformer. 



However, the precise ionization cross section at 

However, the electron impact cross section for most of the elements we would choose peaks at below 

Lotz, Wolfgang. "Electron-impact ionization cross-sections for atoms up to Z= 108." *Zeitschrift für Physik A Hadrons and nuclei* 232.2 (1970): 101-107, https://link.springer.com/content/pdf/10.1007/BF01393132.pdf

```

, simple transverse-electron impact might be a simpler route.


```

```
The ion and recombination beams must be precisely matched for recombination to occur - see relativistic argument. 

The e-beam can be bent into alignment by a small magnetic field:

| / - wasted recomb. electrons
|(  - recombination point
| \ - e-beam gun

This would allow precise control over the recombination position - essential for accurate deposition.

This also allows the recombination beam energy to be varied independently, as the B-field will not affect the heavy beam.

\ | /
 )|(
/ | \

An axial field may also aid in recombination.
```



