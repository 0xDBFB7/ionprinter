### Why diffusion?

Diffusion pumps are simple and cheap, with no moving parts. The alternatives:

* Turbomolecular pumps provide fast pumping rates and low base pressures. My Mk3 test chamber was built around a 20 year old Varian Turbo-V200 turbopump. However, these pumps require intricate, many-stage 5-axis-machined rotors spinning at 50k+ RPM, requiring specialized bearings, high stiffness, and careful design. Presumably achievable, but it seemed like a lot more complexity. My attempts at machining rotors with an electronic indexer failed miserably. 
* Sprengel pumps could be interesting
* Cryotraps aren't practical for this application.

We have to design our own, of course, since even used COTS diffusion pumps cost upwards of $400.

### Nozzle

The design parameters of diffusion pump nozzles have some similarities with rocket engine bells. A series of de Laval nozzles are used to accelerate a gas jet to extreme velocities. The pump will need to operate over a significant range of pressures (naturally); startup will probably occur at around 66 Pa (500 microns), and the system should continue pumping efficiently well past 0.01 mPa.

A solenoid on the nozzle could adjust the profile based on the pressure to increase pumping speed. We'll see.

### Boiler

The working fluid (DC or ECO 702 silicone oil, in our case [3]) is heated to its boiling point in the lower chambers of the pump, pressurizing the nozzle tree.

I originally tried putting a small Kanthal heater inside my Mk1 diffusion pump's boiler, using high-temperature JB-Weld Epoxy/Steel [ coated bolts as power and thermistor feedthroughs. The Kanthal did not have sufficient surface area to transfer heat efficiently, and burned out well before operating temperature was attained. 

It became apparent that heating externally would be far simpler; 



https://www.capvac.com/files/downloads/MSDS/DC704702705_Specs.pdf diffusion pump oil has a vapor pressure equation - we could assume that the nozzle is infinitely small

### Sensors



### Seals

In the event that the device must be built in multiple sections, I have found McMaster-Carr's high-temperature silicone O-rings (#1283N91) to be perfectly serviceable in these conditions.

### Baffle

#### Foreline

A series of chilled baffles must be built into the foreline tube to prevent the silicone oil from being lost to the roughing pump. This should be quite simple once one prototype printer is functional, since we can build cavities and chambers at will.

#### Inlet

Diffusion pumps can make a mess. A process known as backstreaming can cause errant molecules of silicone oil to exit the inlet flange, depositing themselves on the immaculate inner surfaces of the chamber. 

A well-designed pump (in other words, not one of my design) should have relatively minimal backstreaming; however, if this becomes an issue, a chilled optically-dense baffle can be attached to the input, preventing this effect at the cost of a ~60% reduction in pumping speed.

### Chiller



### Other stuff

As with many vacuum systems, if the pump is exposed to air while it is operating, the oil will likely be destroyed or degraded. Most vacuum setups seem to have a butterfly valve on the pump's inlet, allowing the chamber to be cycled rapidly without cooling the pump. 

 * [1]: https://www.grc.nasa.gov/www/k-12/airplane/ienzl.html
 * [2]: http://www.fusor.net/board/viewtopic.php?t=11284
 * [3]: https://diffusionpumpoil.com/products/eco-702-silicone-diffusion-pump-oil/
 * [4]: Since the boiler is on the high-pressure foreline side of the system, outgassing is likely not a significant concern. JB-weld has a relatively low TML, anyhow.