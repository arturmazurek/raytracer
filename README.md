This is a simple ray tracer written in C++.

After reading "Ringworld" - a great, hard sci-fi book by Larry Niven I started thinking - "How would it actually look like to live on the Ring?". The covers of books I have are very beautiful and also Internet has quite a few images inspired by the Ring. However I was not sure if they are actually "to scale". 

So I decided to check that myself. I could of course use e.g. Blender and make such a model in an hour (I'd have to learn it first). But since I'll never be a 3D modeller I decided to write a raytracer myself. That way I'll learn stuff that actually may be useful to me in the future.

-----

You'll need a compiler which understands C++11 features such as (but not only): threads, lambdas, initialiser lists etc. I used XCode and was working on OS X so it'll currently run only on that OS. The Renderer class is platform-independent but something has to actually ask it for a bitmap - and that think currently exists only for OS X.

-----

Version 0.0.1 - you can render a simple non-coloured image from perspective os someone standing on the ground of the Ring.
