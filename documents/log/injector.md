```
"mmworm" injector

| - 12L14 or equivalent, probably passivated?
: - bearing material (optional)
o - copper winding

 |: ooo| *|:|
o|: ==-O>*|:|o
o|: ooo||*|:|o 
o|:____||*|:|o
o|:|      |:|o
 |:|      |:|
 |:|_____ |:|
 
Blade > energized by clamp solenoid on plunger clamps down on wire *
Plunger turned to size then milled radially on CNC in 1 op.

Wire hole should probably be centered so that it can be drilled by a lathe operation.

There could be two or more blades and solenoids per plunger.
Return pressure on both plungers could be effected by spring, or by a second coil.
Wire is fed by sequentially pulsing each solenoid. 
Hard stops should be integrated so that a known length is injected per stroke, and so the clamp plunger sees maximum force.

The clamp plunger could be actuated by the advance solenoid somehow. More complex geometry, probably. Could also be coupled inductively! 

Solenoid wire fatigue is a concern.

It remains to be seen whether the tribology of 12L14 requires a bearing insert - delrin AF, perhaps alumina-alumina, or even a thin graphite layer.

If reversing wire is not required, the two solenoids could simply be connected together and driven simultaneously as long as the inner plunger is faster than the outer.

The two solenoids will have different inductances and resistances etc, perhaps that could be leveraged to control them seperately. 

Clamp could even be passive, just a piece of spring steel or something akin to a strand-jack collet.
```



```
Inductive mmworm

 |: ooo| *|:|
o|: ==-O>*|:|o
o|: ooo||*|:|o
o|:____||*|:|o
o|:oooooo |:|o
 |:|      |:|
 |:oooooo |:|
 
Plunger coils are connected together.
 
The advance field is activated, and plunger travels to stop. Advance field is driven at resonance, plunger remains at stop, clamp activates. Advance field is turned off (or perhaps frequency/duty cycle changed), plunger returns to rest position, clamp slowly deactivates through remnant field.
 
Advantage: no wire fatigue, only 1 mosfet required
```

