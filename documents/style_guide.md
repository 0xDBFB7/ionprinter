As Feynman said, it's more important to be clear rather than precise:

> **The real problem in speech is not precise language. The problem is clear language.** The desire is to have the idea clearly communicated to the other person. It is only necessary to be precise when there is some doubt as to the meaning of a phrase, and then the precision should be put in the place where the doubt exists. **It is really quite impossible to say anything with absolute precision, unless that thing is so abstracted from the real world as to not represent any real thing.**

I find that I often drift into obtuse and jargon-filled terminology, which greatly hinders understanding. Acronyms are to be abhored. 

If a technique or technology (or even variable!) is named properly, one often finds that the name alone gives you some intuitive understanding of what it means, and how it operates. "Jacobi solver" is less descriptive than Jacobi Relaxation. "Standard Deviation Of The Mean". 

All variables in all equations should be labeled immediately below. 

Any magic constants or empirical fits should have units attached.

Everything in this project should be converted to SI + eV units, unless that'd be weird (like O-ring standards)



Please don't speculate. 

When faced with a bottleneck of information about their interests, certain online communities have devolved into an unfounded, unscientific sort of speculation that I don't think is particularly healthy. The engineering world moves quickly, and there are often countless internal analyses that observers are not privy to. If you have a hunch or a theory, try to contradict it.

I will try to release information on 



sources like:

[^description]: [Internal]() [External]() 

Still not sure whether first- or third-person 

Keep it whimsical. 



<hr>

Here's something interesting.

Plugging Feynman's caltech address into the 'Hemingway Editor' yields grade 7 prose. Some snippets from FLP Vol. III: Grade 5 to 8. experiments.md: grade 12.

Griffiths QM is between 10 and 14.



<hr>

Does the practice of naming techniques after their inventors hinder understanding? 

It is important to give credit where it's due, of course, but consider this: if a paper says "Successive Over-Relaxation", you can already get an intuitive feel for what that might entail. 

"Successive": "whatever it is, we're doing it a bunch of times."
"Relaxation": "it'll start off crazy, then get to the point."
"Over": "sounds like there'd be a multiplication term in there. We're doing something 'too much' - maybe overshooting?"

SOR actually means "standard relaxation times 1.7 or so." - so we've really inferred a very accurate understanding just from the name. It's still not perfect terminology, but it's pretty good.

On the other hand, a very similar concept is known as the "Gauss-Seidel method". If you haven't experience in the field, you have to dump your cache, go hit a lookup table like Wikipedia, and try to parse its explanation. Gauss-Seidel is a general technique for linear equations, but in numerical practice it actually means "average the neighbors and update the value in-place", and this is the notion to which the authors most often refer. 'the single-matrix method' could equally be used. Of course, experts are not hindered by this subterfuge, but it might help the neophyte. 

On the other hand, maybe an approximate understanding isn't good enough. Maybe it's useful to force people to read deeply about a new technique. I don't know.

Just something to think about in case we have to name something.

<hr>

<hr>

Programming Style Guide

The command line to compile and run a program and all the environment variables should be in a comment block at the beginning. 

