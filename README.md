


Hardware, firmware, software, and documentation for ion printers.

This was a fun pie in the sky, blue-skies, almost physically implausible but nonthesless fun

At the time the project was suspended, it looked like this:


Goal was ultra high current (>100 A of ionized aluminum accelerated to 100 to 1000 eV) heavy ion beam deposition, aiming to 3D print large parts at cubic centimeters per hour and with arbitrary combinations of alloys and crystalline structure. Ideally would cost <$2000 and run from a single 1500w outlet.

  - Acceleration voltages over ~5000 V would pose huge X-ray and general power consumption (dozens of kilowatts in the beam!) and deposition power density (build platform overheating) issues.
  - However, heavy ions like Al move slowly, meaning space charge effects are huge and, so-called "beam rigidity", magnetic focusing is useless.
  - However, it seemed like it could be possible to use space-charge neutralization, actively injecting and confining electrons into metal-ion "packets" to neutralize the beam space charge,
  - Particularly h spreading the beam current over 512 or 1024 individually-controlled emitters, this seemed possible to alleviate deposition surface roughness issues because of the wide range of angles.
  - Feedstock would be vaporized in graphite-loaded magnesium oxide coated in aluminum nitride and with layers of ceramic multi-layer insulation to prevent heat loss.

Things that are here:

  - Because electrons are so fast compared to ions, to resolve both over long durations without resorting to slow-electron simulations, Particle in Cell DSMC sims must solve grids very rapidly and efficiently. A test implementation of a very fast (19 ms / update cycle) block structured mesh (probably now superseded by WarpX / AMReX) is here. This ran entirely on the GPU memory via native CUDA.
  Ultimately, a little more knowledge of Vlasov equations or even basic matrix algebra would probably have been better.
  * This specific form of the block-structured mesh data structure seems perfect for geometric multigrid electrostatic solvers because the hierarchical geometry of the mesh corresponds well to multigrid requirements; depth- and breadth- traversals are easy to code; ghost cells can be handled separately; and updates "at the edges" of a block are handled seamlessly.
