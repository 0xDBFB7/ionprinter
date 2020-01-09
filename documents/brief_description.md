### Project Ionolith

##### Brief description



Wait, why not publish it? If the technique is unfounded, then I will waste a great deal of scientists time, and  



> Reality must take precedence over public relations, for nature cannot be fooled. - *Feynman*

Behold, the central thesis of the *Ionolith*:

Less facetiously, this technique is known as ion beam deposition (or lithography). It is currently limited to producing objects at a scale of angstroms per hour, and the machines cost many millions. 

Very unfortunately for my studies, parents, and mental health, I've been unable to conclusively demonstrate that it is entirely impossible to simply scale this up to $\text{cm}^3$/hour with a $1000 consumer machine, despite 3 years and 1,600 papers.

This is almost certainly impossible, of course, since nobody is doing it; but it's such an absurd and fantastic problem that I've been unable to stop working on it.

I am buoyed by the fact that a very closely related thin-film technique called Physical Vapor Deposition was once demonstrated to produce millimeter-scale layers.

Even when it ends up being impossible, I have learned a great deal from the process; so I'm not altogether concerned about when that day comes - though I must admit to hoping that it won't.



### Problems in order of difficulty

#### Space-charge neutralization

Feynman mentioned ion beam lithography offhand in his 1969 lecture *Plenty of Room at the Bottom*, remarking that it was probably impossible to scale up to anything macro because of space-charge effects.

To see why, let's compare with a CRT screen again.

The ions we're trying to print are about $10^5$ times heavier than electrons, and so they move commensurately slower. Current theory predicts 250x slower.

The electric field created by the space charge is proportional to the velocity; so the gradient is that much higher. 



Compounded with these difficulties are the limited power consumption and target heating, both of which demand a low per-particle energy of ~100 eV.

Physics does not demand space charge, however. There are many 

Largely due to progress in accelerator science, the prevalence (ha!) of industrial ion-beam implantation systems for semiconductor doping, and  a great deal has been learned about implementing space charge neutralization, compensation, and beam cooling since Feynman's talk.

The simulation detail required to resolve these effects did not exist at the time. 

The current design somewhat mitigates this issue with a massively-parallel many-beamlet design; but this may be completely wishful thinking.

*It is entirely possible that this is unreasonable.* There are many regions of the beamline that are still opaque to the horribly crude zeroth-order analysis I've been able to produce so far.

#### High-performance multiscale multiphysics simulation on consumer hardware

##### Status: almost certainly solved. Demo implementation in-progress, but with very recent developments in half-precision computing and improvements in memory bandwidth.

'common-beam' effects. 

DSMC



#### An inexpensive, 'demountable', user-friendly high vacuum system

##### Status: not an issue.

This turned out to be the easiest part of the whole system; in fact, I have a system built from scrap for \$250 that easily attains $10^{-5}$ mBar. 

Together with ceramic techniques from below, 

Attempts to build diffusion pumps have revealed a ludicrously simple construction belied by their high prices.

Because of the temperatures involved, the contamination that  the bane of all are not expected to cause difficulty.







#### Dirt-cheap high-temperature ceramics with arbitrary geometries capable of continuously withstanding superheated metal.

##### Status: Partially solved. A process for sintering gelcast aluminum oxide / graphite composite parts has been demonstrated with very acceptable economics. Candidate linings (aluminum nitride), have been found; 

The typical gaseous vapor source for PVD is a tungsten boat. These are far too expensive for a consumer product; tungsten is neurotoxic (necessitating expensive industrial hygiene) and stupendously difficult to work with. 

Moreover, W's low resistance coupled with the required geometries mandates heavy, copper-filled transformers and high-current vacuum feedthroughs. 

To add even more difficulty, current tungsten boats are only designed to produce the faintest wisps of vapor. The vapor pressures required to produce macro-scale objects require much higher. 



Simple tests with micro-machined graphite piping 

Even more concerning is the fact that superheated metals dissolve almost everything; only precious few materials can withstand prolonged exposure. Aluminum nitride is particularly convenient. 

Still more concerning is the tremendous power consumption. The enthalpy of vaporization of the metals is only contributes a small fraction of the. 

#### Surface effects: crystalline structure, ad-atom reflection, sputter yields, skitter

It remains to be seen if so-called depressed collector systems will work.

#### Inexpensive, high frequency, high voltage precision electronics for beam control

The current design calls for 1024 to 2048 individually-controlled beamlet emitters, with approximately 8 electrodes that must switch 5Kv at a few Mhz within a tolerance of a few millivolts.

That number of high-voltage MOSFETs would already break the budget.  

Feedback looks to be achievable via beam modulation, coupling into electrostatic beam position monitoring buttons.

